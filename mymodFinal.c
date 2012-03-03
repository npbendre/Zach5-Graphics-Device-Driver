#include <linux/mm.h>
#include <asm/mman.h>
#include <linux/init.h>
#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/kernel_stat.h>
#include <linux/fs.h>
#include <linux/pci.h>
#include <linux/cdev.h>
#include <linux/delay.h>
#include <linux/locked_ioctl.h>
#include <linux/pci_ids.h>
#include <linux/wait.h>
#include <linux/sched.h>
#include <linux/signal.h>
#include <asm/irq.h>
#include <linux/interrupt.h>
#include <asm/uaccess.h>
#include "z5registers.h"
#include <linux/spinlock.h>
#define BUFFCOUNT 8


// use license "GPL" to avoid "tainted" kernels
MODULE_LICENSE("GPL");
MODULE_AUTHOR("822 Team Gold");
int save_reg[19];
int major = 127;
int minor = 500;
struct cdev whatever;
struct pci_dev *pci_dev1; 

dma_addr_t dma_handle[BUFFCOUNT];

//defining the spinlock 
DEFINE_SPINLOCK(gold_lock); 

// ZACH5 device memory space info  
struct zach5_sdev {
	unsigned int p_control_base;
	unsigned int *k_control_base;
	unsigned int *u_control_base;
	unsigned int pci_irq;
	unsigned int fill;
	unsigned int drain;
}zach5;

//structure for DMA buffers
struct dbuff {
	unsigned long p_base;
	unsigned int *k_virtual_base;
	unsigned int *user_base;
	int count;
}DMA_buffs[BUFFCOUNT];

//get the current buffer in use 
unsigned int current_buffer;

// ZACH5 device ID
const struct pci_device_id zach5_dev_ids[] = 
{
	{PCI_DEVICE(PCI_VENDOR_ID_ZJONES,PCI_DEVICE_ID_ZACH5)},
	{0}
};

// remove the ZACH5 device
void zach5_remove(struct pci_dev *dev)
{
	printk(KERN_ALERT "in zach5 remove\n");
}
 

// probe the ZACH5 device
int zach5_probe(struct pci_dev *pci_dev,const struct pci_device_id *pci_id)
{
	int z;
 	zach5.p_control_base = pci_resource_start(pci_dev,0);
 	zach5.pci_irq = pci_dev->irq;
  	z=pci_enable_device(pci_dev);
        pci_set_master(pci_dev);
	printk(KERN_ALERT "In Zach5 Probe\n");
 	return(0);
}

// open the ZACH5 device for I/O 
int zach5_open(struct inode *inode, struct file *fp) 
{
        zach5.k_control_base = ioremap(zach5.p_control_base,1024);
	printk(KERN_ALERT "Opened zach5\n");
	return 0;
}

// release the ZACH5 device and free allocated memory
int zach5_release(struct inode *inode, struct file *fp) 
{
	int i;
	struct mm_struct *mm = current->mm;
	
	// releasing buffers allocated using pci_alloc_consistent (BIND_DMA) 
	for(i=0;i<BUFFCOUNT;i++)
	{
		pci_free_consistent(NULL,(124*1024),DMA_buffs[i].k_virtual_base,(dma_addr_t)(DMA_buffs[i].p_base));
	}
	
	// for physical base address that has been ioremapped 
	iounmap(zach5.k_control_base);

	// for any DMA register that have been memory mapped using do_mmap()		
	for(i=0;i<BUFFCOUNT;i++)
	{
		do_munmap(mm,(unsigned long) DMA_buffs[i].p_base,(124*1024));	
	}
	
	// freeing an interrupt
	free_irq(zach5.pci_irq,&whatever);
	printk(KERN_ALERT "Released zach5\n");
	return 0;
}


// PCI driver info
struct pci_driver zach5_pci_drv = 
{
	.name = "whatever",
	.id_table = zach5_dev_ids,
	.probe = zach5_probe,	
	.remove = zach5_remove
};


// map the ZACH5 device into the memory 
int zach5_mmap(struct file *fp,struct vm_area_struct *vma)
{
	printk(KERN_ALERT "Inside zach5 mmap");
   	io_remap_pfn_range(vma,vma->vm_start,(zach5.p_control_base)>>PAGE_SHIFT,vma->vm_end - vma->vm_start,vma->vm_page_prot);
   	zach5.u_control_base = (unsigned int*)vma -> vm_start;
	return 0;
}

// map the ZACH5 device into the memory 
int zach5_do_mmap(struct file *fp,struct vm_area_struct *vma)
{
	//check whether the user is root 
	if(current_fsuid()==0)
	{
		printk(KERN_ALERT "Inside zach5 mmap");
		io_remap_pfn_range(vma,vma->vm_start,(DMA_buffs[current_buffer].p_base)>>PAGE_SHIFT,vma->vm_end - vma->vm_start,vma->vm_page_prot);
		return 0;
	}
	else
	{
		return -1; 
	}
}

// read from graphic registers in kernel mode
unsigned int K_READ_REG(unsigned int reg)
{
	unsigned int value;
	udelay(1);
	rmb();
	value =  *(zach5.k_control_base+(reg>>2));	
	udelay(1);
	return (value);
}

// write to graphics registers in kernel mode
unsigned int K_WRITE_REG(unsigned int reg, unsigned int value)
{
	udelay(1);
	*(zach5.k_control_base+(reg>>2)) = value;
  	udelay(1);
  	return (0);
}

// save the registers
void saveRegisters(void)
{
	printk(KERN_ALERT "Inside save regs\n");
	save_reg[0] = K_READ_REG(Aperture1);
	save_reg[1]  = K_READ_REG(Aperture2);
	save_reg[2] = K_READ_REG(FrameBufferWriteMask); 
	save_reg[3] = K_READ_REG(DisconDFIFO); 
	save_reg[4] = K_READ_REG(DisconFIFO); 
	save_reg[5] = K_READ_REG(BootAddress); 
	save_reg[6] = K_READ_REG(MemoryCfg); 
	save_reg[7] = K_READ_REG(CtrlGraphics); 
	save_reg[8] = K_READ_REG(HgEnd); 
	save_reg[9] = K_READ_REG(ScreenBase); 
	save_reg[10] = K_READ_REG(HTotal); 
	save_reg[11] = K_READ_REG(HbEnd);
	save_reg[12] = K_READ_REG(HsStart);
	save_reg[13] = K_READ_REG(HsEnd);
	save_reg[14] = K_READ_REG(VTotal);
	save_reg[15] = K_READ_REG(VbEnd);
	save_reg[16] = K_READ_REG(VsStart);
	save_reg[17] = K_READ_REG(VsEnd);
	save_reg[18] = K_READ_REG(CfgGraphics);
}


// load grpahics registers to define the screen resolution
void loadResolutionDependentRegisters(void)
{
	printk(KERN_ALERT "inside load resol Dependent regs");
	K_WRITE_REG(Aperture1,0x0);
	K_WRITE_REG(Aperture2,0x0);
	K_WRITE_REG(HTotal,0x041F0000); 
	K_WRITE_REG(HbEnd,0x100);
	K_WRITE_REG(HsStart,0x1000);
	K_WRITE_REG(HsEnd,0x60000);
	K_WRITE_REG(HgEnd,0x1000); 
	K_WRITE_REG(VTotal,0x2700000);
	K_WRITE_REG(VsStart,0x0);
	K_WRITE_REG(VsEnd,0x3000);
	K_WRITE_REG(VbEnd,0x19);
}

// load register data for graphics control 
void loadControlRegisters(void)
{
	printk(KERN_ALERT "Inside load control regs\n");
	K_WRITE_REG(InterruptLine,0x0);
	K_WRITE_REG(DisplayData,0x0);
	K_WRITE_REG(FIFOCtrl,0x8010); 
	K_WRITE_REG(LineCount,0x0); 
	K_WRITE_REG(ScreenBaseRight,0x0); 
	K_WRITE_REG(ScreenBase,0x0); 
	K_WRITE_REG(FlagsInt,0x0); 
	K_WRITE_REG(VClkCtl,0x40000); 
	K_WRITE_REG(DMACtrl,0x0); 
	K_WRITE_REG(DisconFIFO,0x10); 
	K_WRITE_REG(MemoryCtrl,0x0); 
	K_WRITE_REG(WriteMaskBypass,0xffffffff); 
	K_WRITE_REG(BootAddress,0x18C000);
	K_WRITE_REG(MemoryCfg,0xe6002021); 
	K_WRITE_REG(ChipConfig, 0x80020006);	
}

// restore the consol registers
void restoreRegisters(void)
{
	printk(KERN_ALERT "Restoring registers\n");
	K_WRITE_REG(Aperture1,0x0);
	K_WRITE_REG(Aperture2,0x0);
	K_WRITE_REG(WriteMaskBypass,0xffffffff); 
	K_WRITE_REG(DisconDFIFO,0x0); 
	K_WRITE_REG(DisconFIFO,0x0); 
	K_WRITE_REG(BootAddress,0x20); 
	K_WRITE_REG(MemoryCfg,0xe6002021); 
	K_WRITE_REG(CtrlGraphics,save_reg[7]); 
	K_WRITE_REG(HgEnd,0x1); 
	K_WRITE_REG(ScreenBase,0x7fa92);
	K_WRITE_REG(VClkCtl,0x80); 
	K_WRITE_REG(HTotal,0xd0); 
	K_WRITE_REG(HbEnd,0x1);
	K_WRITE_REG(HsStart,0x1ca);
	K_WRITE_REG(HsEnd,0x8);
	K_WRITE_REG(VTotal,0x300);
	K_WRITE_REG(VbEnd,0x30);
	K_WRITE_REG(VsStart,0x200);
	K_WRITE_REG(VsEnd, 0xa);
	K_WRITE_REG(VsEnd,save_reg[17]);
	K_WRITE_REG(CfgGraphics,save_reg[18]);
}

// draw the screen background 
void drawSquare(void)
{
	printk(KERN_ALERT "Inside draw square");	
	K_WRITE_REG(R0, 4194304);
	K_WRITE_REG(G0, 0);
	K_WRITE_REG(B0, 2097152);
	
	K_WRITE_REG(dRdyDom, 4064);
	K_WRITE_REG(dGdyDom, 0);
	K_WRITE_REG(dBdyDom, 2032);
	
	K_WRITE_REG(RasterizerCfg, 0x40000000);
	K_WRITE_REG(XDom0, 655360);
	K_WRITE_REG(dXDom, 0);
	
	K_WRITE_REG(XSub0, 51773440);
	K_WRITE_REG(dXSub, 0);
	K_WRITE_REG(Y0, 655360);
	
	
	K_WRITE_REG(dY,65536);
 	K_WRITE_REG(Count,580<<18);

	K_WRITE_REG(Render,RENDER_TRAPEZOID_SPC);
}

DECLARE_WAIT_QUEUE_HEAD(dma_snooze);

// begin DMA transfer
void initiate_transfer(unsigned long arg)
{
	spin_lock(&gold_lock);

	//queue is empty
	if(zach5.fill == zach5.drain)
	{	
		DMA_buffs[zach5.fill].count = arg;
		printk(KERN_ALERT "queue is empty");
		
		//increment the fill, points to the next buffer 
		zach5.fill=(zach5.fill+1)%BUFFCOUNT; 
    
		K_WRITE_REG(DMAAddress, DMA_buffs[zach5.drain].p_base);
		K_WRITE_REG(DMACount,(DMA_buffs[zach5.drain].count)<<12);
		
		spin_unlock(&gold_lock);
		return;
	}
	
	//if the channel is busy, queue the buffer
	else
	{
		DMA_buffs[zach5.fill].count = arg;
		zach5.fill = (zach5.fill+1)%BUFFCOUNT;
		
		//if queue is full
		if(zach5.fill == zach5.drain)
		{
			unlock_BDL();
			spin_unlock(&gold_lock);
			wait_event_interruptible(dma_snooze,zach5.fill!=zach5.drain);
			lock_BDL();
			return;
		}		
		spin_unlock(&gold_lock);
		return;
	}
	return;
}


//Interrupt handler
irqreturn_t dma_intr(int irq,void *dev_id,struct pt_regs *regs)
{
	unsigned int iflags;
	// Inside the handler	
	
	iflags = K_READ_REG(IntFlags);

	// reset to get next interrupt
	K_WRITE_REG(IntFlags,0x0);  
		
	if(iflags &&0x40 == 0){
		return(IRQ_NONE);
	}

	zach5.drain = (zach5.drain+1)%BUFFCOUNT;
	wake_up_interruptible(&dma_snooze);
	
	//interrupt handler advances "drain"
	if(zach5.fill != zach5.drain)
	{
		K_WRITE_REG(DMAAddress, DMA_buffs[zach5.drain].p_base);
		K_WRITE_REG(DMACount,(DMA_buffs[zach5.drain].count)<<12);
 	}
	return(IRQ_HANDLED);
}

struct file_operations zach5_fops; 


long zach5_ioctl(struct file *fp, unsigned int cmd, unsigned long arg)
{
	int i,result;

	
	printk(KERN_ALERT "IN ZACH5 IOCTL\n");
	
	lock_BDL();

	switch(cmd)
	{
		case VMODE:
			if(arg == 1)
                       {
				printk(KERN_ALERT "In graphics ON");
				saveRegisters();
				loadResolutionDependentRegisters();
				loadControlRegisters();
				drawSquare();
				K_WRITE_REG(CfgGraphics,0x80020006);
				K_WRITE_REG(CtrlGraphics,0x82132000);
                        }
				
 			if(arg == 0)
			{
				printk(KERN_ALERT "In graphics OFF");
				if(zach5.fill!=zach5.drain) 
				{
					unlock_BDL();
					wait_event_interruptible(dma_snooze,zach5.fill==zach5.drain);
					lock_BDL();
				}
				restoreRegisters();
				K_WRITE_REG(CtrlGraphics,0x0);
				K_WRITE_REG(CfgGraphics,0x80020004);
				K_WRITE_REG(Reboot, 0x01);
			}
		break;

		 case BIND_DMA:
				unlock_BDL();
				zach5.fill=0;
				zach5.drain=0;
				
				// enabling the zach5 do_mmap
				zach5_fops.mmap = zach5_do_mmap; 
				

				for(i=0;i<BUFFCOUNT;i++)
				{
					// buffers being allocated using PCI_ALLOC_CONSISTENT
					DMA_buffs[i].k_virtual_base = pci_alloc_consistent(NULL,(124*1024),&dma_handle[i]);
					DMA_buffs[i].p_base = (unsigned long) dma_handle[i];

					current_buffer = i;
					
					// passing the physical address to do_mmap()
					DMA_buffs[i].user_base = (unsigned int *)do_mmap(fp,(unsigned long) dma_handle[i],(124*1024),PROT_READ|PROT_WRITE,MAP_SHARED,0);

				}

				//enabling the mmap 	
				zach5_fops.mmap =zach5_mmap;
			
				// for interrupt handling
				result = request_irq(zach5.pci_irq,(irq_handler_t)dma_intr,IRQF_DISABLED|IRQF_SHARED, "dma_intr", &whatever);
				if(result==0)
				K_WRITE_REG(IntEnable,0x40);
				K_WRITE_REG(IntFlags,0x00); 	
				
		
				return (unsigned long)(DMA_buffs[0].user_base);
				break;

		case START_DMA:
				//call to intiate transfer 
				initiate_transfer(arg);
				unlock_BDL();
				return (unsigned long)(DMA_buffs[zach5.fill].user_base);
				break;
	}
	unlock_BDL(); 
	return 1; 
}


// Zach5 file operation function pointers
struct file_operations zach5_fops = {
	.open=			zach5_open,
	.release=		zach5_release,
	.owner=			THIS_MODULE,
	.mmap =         zach5_mmap, 
	.unlocked_ioctl = zach5_ioctl
};

// Intialize the Zach5 device module 
static int zach_init(void) {
	int results;
	cdev_init(&whatever, &zach5_fops);
        zach5_fops.owner = THIS_MODULE;     
	cdev_add(&whatever, MKDEV(major,minor),1);
        printk(KERN_ALERT "Inside zach init");   
	results = pci_register_driver(&zach5_pci_drv);
	return(0);
}

// Unregister the Zach5 device module
static void zach_exit(void) 
{
	pci_unregister_driver(&zach5_pci_drv);
	cdev_del(&whatever);
	printk(KERN_ALERT "Successfully removed device\n");
}

module_init(zach_init);
module_exit(zach_exit);
