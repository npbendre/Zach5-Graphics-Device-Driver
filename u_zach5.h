struct u_zach5_device {
	unsigned int *control_base;
} zach5;

#define ZACH5_CONTROL_SIZE (1024)
#define THE_BIG_BUCKET (128*1024)
#define ONE_PAGE (4096)
#define MAX_DMA (THE_BIG_BUCKET - ONE_PAGE)

#define VMODE 0
#define BIND_DMA 1
#define START_DMA 7

#define V_OFF 0
#define V_800x600_75Hz 1


#define FIFOSYNC while(U_READ_REG(FIFOSpaceFree) != 0x00008000)
#define EX0 (127)

typedef struct {
	float x, y, z, r, g, b;
} vertex;

	vertex vertices[] = {
		//Chekered Background

		//T1
		{ 200.0,0.0,0.0,0.0,0.0,0.0 },
		{ 0.0,0.0,0.0,0.0,0.0,0.0 },
		{ 200.0,200.0,0.0,0.0,0.0,0.0}, 

		//T2
		{ 0.0,0.0,0.0,0.0,0.0,0.0 },
		{ 200.0,200.0,0.0,0.0,0.0,0.0 },
		{ 0.0,200.0,0.0,0.0,0.0,0.0}, 

		//T3
		{ 400.0,0.0,0.0,255.0,255.0,255.0 },
		{ 200.0,0.0,0.0,255.0,255.0,255.0 },
		{ 400.0,200.0,0.0,255.0,255.0,255.0}, 

		//T4
		{ 200.0,0.0,0.0,255.0,255.0,255.0 },
		{ 400.0,200.0,0.0,255.0,255.0,255.0 },
		{ 200.0,200.0,0.0,255.0,255.0,255.0}, 

		//T5
		{ 600.0,0.0,0.0,0.0,0.0,0.0 },
		{ 400.0,0.0,0.0,0.0,0.0,0.0 },
		{ 600.0,200.0,0.0,0.0,0.0,0.0},

		//T6
		{ 400.0,0.0,0.0,0.0,0.0,0.0 },
		{ 600.0,200.0,0.0,0.0,0.0,0.0 },
		{ 400.0,200.0,0.0,0.0,0.0,0.0}, 

		//T7
		{ 800.0,0.0,0.0,255.0,255.0,255.0 },
		{ 600.0,0.0,0.0,255.0,255.0,255.0 },
		{ 800.0,200.0,0.0,255.0,255.0,255.0},

		//T8
		{ 600.0,0.0,0.0,255.0,255.0,255.0 },
		{ 800.0,200.0,0.0,255.0,255.0,255.0 },
		{ 600.0,200.0,0.0,255.0,255.0,255.0},  
 
		//T21
		{ 200.0,200.0,0.0,255.0,255.0,255.0 },
		{ 0.0,200.0,0.0,255.0,255.0,255.0 },
		{ 200.0,400.0,0.0,255.0,255.0,255.0}, 

		//T22
		{ 0.0,200.0,0.0,255.0,255.0,255.0 },
		{ 200.0,400.0,0.0,255.0,255.0,255.0 },
		{ 0.0,400.0,0.0,255.0,255.0,255.0}, 

		//T23
		{ 400.0,200.0,0.0,0.0,0.0,0.0 },
		{ 200.0,200.0,0.0,0.0,0.0,0.0 },
		{ 400.0,400.0,0.0,0.0,0.0,0.0}, 

		//T24
		{ 200.0,200.0,0.0,0.0,0.0,0.0 },
		{ 400.0,400.0,0.0,0.0,0.0,0.0 },
		{ 200.0,400.0,0.0,0.0,0.0,0.0}, 

		//T25
		{ 600.0,200.0,0.0,255.0,255.0,255.0 },
		{ 400.0,200.0,0.0,255.0,255.0,255.0 },
		{ 600.0,400.0,0.0,255.0,255.0,255.0},

		//T26
		{ 400.0,200.0,0.0,255.0,255.0,255.0 },
		{ 600.0,400.0,0.0,255.0,255.0,255.0 },
		{ 400.0,400.0,0.0,255.0,255.0,255.0}, 

		//T27
		{ 800.0,200.0,0.0,0.0,0.0,0.0 },
		{ 600.0,200.0,0.0,0.0,0.0,0.0 },
		{ 800.0,400.0,0.0,0.0,0.0,0.0},

		//T28
		{ 600.0,200.0,0.0,0.0,0.0,0.0 },
		{ 800.0,400.0,0.0,0.0,0.0,0.0 },
		{ 600.0,400.0,0.0,0.0,0.0,0.0},  
 
		//T31
		{ 200.0,400.0,0.0,0.0,0.0,0.0 },
		{ 0.0,400.0,0.0,0.0,0.0,0.0 },
		{ 200.0,600.0,0.0,0.0,0.0,0.0}, 

		//T32
		{ 0.0,400.0,0.0,0.0,0.0,0.0 },
		{ 200.0,600.0,0.0,0.0,0.0,0.0 },
		{ 0.0,600.0,0.0,0.0,0.0,0.0}, 

		//T33
		{ 400.0,400.0,0.0,255.0,255.0,255.0 },
		{ 200.0,400.0,0.0,255.0,255.0,255.0 },
		{ 400.0,600.0,0.0,255.0,255.0,255.0}, 

		//T34
		{ 200.0,400.0,0.0,255.0,255.0,255.0 },
		{ 400.0,600.0,0.0,255.0,255.0,255.0 },
		{ 200.0,600.0,0.0,255.0,255.0,255.0}, 

		//T35
		{ 600.0,400.0,0.0,0.0,0.0,0.0 },
		{ 400.0,400.0,0.0,0.0,0.0,0.0 },
		{ 600.0,600.0,0.0,0.0,0.0,0.0},

		//T36
		{ 400.0,400.0,0.0,0.0,0.0,0.0 },
		{ 600.0,600.0,0.0,0.0,0.0,0.0 },
		{ 400.0,600.0,0.0,0.0,0.0,0.0}, 

		//T37
		{ 800.0,400.0,0.0,255.0,255.0,255.0 },
		{ 600.0,400.0,0.0,255.0,255.0,255.0 },
		{ 800.0,600.0,0.0,255.0,255.0,255.0},

		//T38
		{ 600.0,400.0,0.0,255.0,255.0,255.0 },
		{ 800.0,600.0,0.0,255.0,255.0,255.0 },
		{ 600.0,600.0,0.0,255.0,255.0,255.0},  

		//T41
		{ 200.0,600.0,0.0,255.0,255.0,255.0 },
		{ 0.0,600.0,0.0,255.0,255.0,255.0 },
		{ 200.0,800.0,0.0,255.0,255.0,255.0}, 

		//T42
		{ 0.0,600.0,0.0,255.0,255.0,255.0 },
		{ 200.0,800.0,0.0,255.0,255.0,255.0 },
		{ 0.0,800.0,0.0,255.0,255.0,255.0}, 

		//T43
		{ 400.0,600.0,0.0,0.0,0.0,0.0 },
		{ 200.0,600.0,0.0,0.0,0.0,0.0 },
		{ 400.0,800.0,0.0,0.0,0.0,0.0}, 

		//T44
		{ 200.0,600.0,0.0,0.0,0.0,0.0 },
		{ 400.0,800.0,0.0,0.0,0.0,0.0 },
		{ 200.0,800.0,0.0,0.0,0.0,0.0}, 

		//T45
		{ 600.0,600.0,0.0,255.0,255.0,255.0 },
		{ 400.0,600.0,0.0,255.0,255.0,255.0 },
		{ 600.0,800.0,0.0,255.0,255.0,255.0},

		//T46
		{ 400.0,600.0,0.0,255.0,255.0,255.0 },
		{ 600.0,800.0,0.0,255.0,255.0,255.0 },
		{ 400.0,800.0,0.0,255.0,255.0,255.0}, 

		//T47
		{ 800.0,600.0,0.0,0.0,0.0,0.0 },
		{ 600.0,600.0,0.0,0.0,0.0,0.0 },
		{ 800.0,800.0,0.0,0.0,0.0,0.0},

		//T48
		{ 600.0,600.0,0.0,0.0,0.0,0.0 },
		{ 800.0,600.0,0.0,0.0,0.0,0.0 },
		{ 600.0,800.0,0.0,0.0,0.0,0.0},  

		// Triangle 1
		{ 100.0,200.0,0.0,255.0,255.0,0.0 },
		{ 150.0,250.0,0.0,255.0,255.0,0.0 },
		{ 50.0,250.0,0.0,255.0,100.0,0.0 },
		// Triangle 2
		{ 50.0,250.0,0.0,255.0,255.0,0.0 },
		{ 50.0,450.0,0.0,255.0,255.0,0.0 },
		{ 100.0,500.0,0.0,255.0,100.0,0.0 },
		// Triangle 3
		{ 120.0,420.0,0.0,255.0,255.0,0.0 },
		{ 200.0,420.0,0.0,255.0,255.0,0.0 },
		{ 100.0,500.0,0.0,255.0,100.0,0.0 },
		// Triangle 4
		{ 350.0,200.0,0.0,255.0,255.0,0.0 },
		{ 300.0,250.0,0.0,255.0,255.0,0.0 },
		{ 400.0,250.0,0.0,255.0,100.0,0.0 },
		// Triangle 5
		{ 300.0,250.0,0.0,255.0,255.0,0.0 },
		{ 250.0,400.0,0.0,255.0,255.0,0.0 },
		{ 300.0,450.0,0.0,255.0,100.0,0.0 },
		// Triangle 6
		{ 300.0,450.0,0.0,255.0,255.0,0.0 },
		{ 400.0,450.0,0.0,255.0,255.0,0.0 },
		{ 350.0,500.0,0.0,255.0,100.0,0.0 },
		// Triangle 7
		{ 400.0,250.0,0.0,255.0,255.0,0.0 },
		{ 450.0,400.0,0.0,255.0,255.0,0.0 },
		{ 400.0,450.0,0.0,255.0,100.0,0.0 },
		// Triangle 8
		{ 525.0,200.0,0.0,255.0,255.0,0.0 },
		{ 500.0,400.0,0.0,255.0,255.0,0.0 },
		{ 525.0,470.0,0.0,255.0,100.0,0.0 },
		// Triangle 9
		{ 525.0,470.0,0.0,255.0,255.0,0.0 },
		{ 625.0,470.0,0.0,255.0,255.0,0.0 },
		{ 575.0,500.0,0.0,255.0,100.0,0.0 },
		//Triangle 10
		{ 675.0,200.0,0.0,255.0,255.0,0.0 },
		{ 650.0,400.0,0.0,255.0,255.0,0.0 },
		{ 675.0,500.0,0.0,255.0,100.0,0.0 },
		//Triangle 11
		{ 675.0,200.0,0.0,255.0,255.0,0.0 }, 
		{ 725.0,275.0,0.0,255.0,255.0,0.0 }, 
		{ 725.0,350.0,0.0,255.0,100.0,0.0 }, 
		//Triangle 12
		{ 725.0,350.0,0.0,255.0,255.0,0.0 }, 
		{ 725.0,425.0,0.0,255.0,255.0,0.0 }, 
		{ 675.0,500.0,0.0,255.0,100.0,0.0 }

		// null terminator
	//	0.0
	};


int makecolor(float x)
{
	int ix;
	int sx;
	unsigned char ex;
	
	ix = *((int *)&x);
	sx = ix>>31;
	ex = (unsigned char)(ix>>23);
	
	if (ex < (EX0 - 15)) return 0;
	if (ex >= (EX0 + 8)) return (0x007fffff ^ sx);
	
	ix = ((unsigned int)((ix|0x00800000) << 8) >> (EX0 + 16 - ex));
	if (sx < 0) ix = -ix;
	return (0xfffff0 & ix);
}

int makecoord(float x)
{
	int ix;
	int sx;
	unsigned char ex;
	
	ix = *((int *)&x);
	sx = ix>>31;
	ex = (unsigned char)(ix>>23);
	
	if (ex < (EX0 - 16)) return 0;
	if (ex >= (EX0 + 15)) return (0x007fffff ^ sx);
	
	ix = ((unsigned int)((ix|0x00800000) << 8) >> (EX0 + 15 - ex));
	if (sx < 0) ix = -ix;
	return ix;
}

int maketag(int reg)
{
	int tag;
	
	tag = (reg)/4;
	return tag;
}	

unsigned int U_READ_REG(unsigned int reg)
{
	return *(zach5.control_base+(reg>>2));
}

unsigned int U_WRITE_REG(unsigned int reg, unsigned int value)
{
	*(zach5.control_base+(reg>>2)) = value;
}
