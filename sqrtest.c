#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <errno.h>
#include <math.h>
#include <sys/mman.h>
#include "u_zach5.h"
#include "z5registers.h"
#define BUFFCOUNT 8

struct dbuff {
	unsigned int *k_virtual_base;
	unsigned int p_base;
	unsigned int *user_base;
	int count;
} DMA_buffs[BUFFCOUNT];

int drawTriangleDMA(int cu_buff,vertex v1,vertex v2,vertex v3)
{
	//printf("in BIND DMA triangle\n");
	float drdy, dgdy, dbdy, drdx,dgdx,dbdx,dx02,dx01,dx12;
	FILE *fd;
	
	int a = fabs(((v1.x-v3.x)*(v2.y-v3.y))-((v2.x-v3.x)*(v1.y-v3.y)));
	
	//y - gradient to get colors
	drdy = (v3.r - v1.r)/(v3.y - v1.y);
	dgdy = (v3.g - v1.g)/(v3.y - v1.y);
	dbdy = (v3.b - v1.b)/(v3.y - v1.y);
	
	//x-gradient to get colors
	drdx = ((v1.r-v3.r)*((v2.y-v3.y)/a))-((v2.r-v3.r)*((v1.y-v3.y)/a));
	dgdx = ((v1.g-v3.g)*((v2.y-v3.y)/a))-((v2.g-v3.g)*((v1.y-v3.y)/a));
	dbdx = ((v1.b-v3.b)*((v2.y-v3.y)/a))-((v2.b-v3.b)*((v1.y-v3.y)/a));
	
	// slopes of all three sides of all triangle
	dx02 = ((v3.x-v1.x)/(v3.y-v1.y));
	dx01 = ((v2.x-v1.x)/(v2.y-v1.y));
	dx12 = ((v3.x-v2.x)/(v3.y-v2.y));

	// 	unsigned int *pbuf = cu_buff;
	unsigned int *pbuf= DMA_buffs[cu_buff].user_base;
	//printf("pbuff:%p\n",pbuf);

	*pbuf++ = R0>>2;
	*pbuf++ = makecolor(v1.r);
	*pbuf++ = G0>>2; 
	*pbuf++ = makecolor(v1.g);
	*pbuf++ = B0>>2; 
	*pbuf++ = makecolor(v1.b);
	
	//color of the dominant edge
	*pbuf++ = dRdyDom>>2;
	*pbuf++ = makecolor(drdy);
	*pbuf++ = dGdyDom>>2;
	*pbuf++ = makecolor(dgdy);
	*pbuf++ = dBdyDom>>2;
	*pbuf++ = makecolor(dbdy);
	
	*pbuf++ = dRdx>>2; 
	*pbuf++ = makecolor(drdx);
	*pbuf++ = dGdx>>2;
	*pbuf++ = makecolor(dgdx);
	*pbuf++ = dBdx>>2; 
	*pbuf++ = makecolor(dbdx);

	*pbuf++ = RasterizerCfg>>2;
	*pbuf++ = 0x40000000;
	
	*pbuf++ = XDom0>>2;
	*pbuf++ = makecoord(v1.x);
	*pbuf++ = dXDom>>2;
	*pbuf++ = makecoord(dx02);
	
	*pbuf++ = XSub0>>2;
	*pbuf++ = makecoord(v1.x);
	*pbuf++ = dXSub>>2;
	*pbuf++ = makecoord(dx01);

	*pbuf++ = Y0>>2;
	*pbuf++ = makecoord(v1.y);
	
	*pbuf++ = dY>>2;
	*pbuf++ = makecoord(1.0);
	*pbuf++ = Count>>2;
	*pbuf++ = (int)(fabs(v1.y-v2.y))<<18;
	
	*pbuf++ = Render>>2;
	*pbuf++ = RENDER_TRAPEZOID_SPC;
	
	
	*pbuf++ = XSub0>>2;
	*pbuf++ = makecoord(v2.x);
	*pbuf++ = dXSub>>2;
	*pbuf++ = makecoord(dx12);
	
	*pbuf++ = ContinueNewSub>>2;
	*pbuf++ = (int)(fabs(v2.y-v3.y))<<18;
	
				
	return ((int)(pbuf - (DMA_buffs[cu_buff].user_base)));
}

int main()
{
	int fd, i = 0, current_buffer = 0;
	long result;
	
		
	// open the graphics device
	fd = open("/dev/zach5", O_RDWR);
	zach5.control_base = mmap(0, ZACH5_CONTROL_SIZE, PROT_READ |PROT_WRITE, MAP_SHARED, fd, 0);
	
	// start graphics mode
	ioctl(fd, VMODE, V_800x600_75Hz);
	result = ioctl(fd, BIND_DMA, 1);
	
	// checking if the user is root
	if(result==-1) 
	{
		printf("\n Please obtain root access to continue to DMA\n"); 
	}
	else
	{
	// display the triangles
	while(i<sizeof(vertices)/24) {
		DMA_buffs[current_buffer].user_base = (unsigned int*)result;		
		DMA_buffs[current_buffer].count = drawTriangleDMA(current_buffer, vertices[i], vertices[i + 1], vertices[i + 2]);
		result = ioctl(fd, START_DMA, DMA_buffs[current_buffer].count);
		current_buffer = (current_buffer + 1) % 8;
		i += 3;
	}
	
	} //end of else

	sleep(10);
	
	// disable graphics mode
	ioctl(fd, VMODE, V_OFF);	
	close(fd);
	return 0;
}
