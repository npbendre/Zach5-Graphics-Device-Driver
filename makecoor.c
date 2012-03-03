#include <stdio.h>
#include <fcntl.h>
#include <stdlib.h>
#include <math.h>

#define EX0 (127)
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

struct vertex {
	float x, y, r, g, b;
};

struct vertex V[] = {
	10.0, 10.0, 128.0, 128.0, 0.0,
	790.0, 590.0, 200.0, 200.0, 0.0,
	0.0
};

void main()
{

	float drdy, dgdy, dbdy;
	FILE *fd;

	drdy = (V[1].r - V[0].r)/(V[1].y - V[0].y);
	dgdy = (V[1].g - V[0].g)/(V[1].y - V[0].y);
	dbdy = (V[1].b - V[0].b)/(V[1].y - V[0].y);

        
	fd = fopen("fltoin.txt","w");
	fprintf(fd,"%d \n",makecolor(V[0].r));
	fprintf(fd,"%d \n",makecolor(V[0].g));
	fprintf(fd,"%d \n",makecolor(V[0].b));

	fprintf(fd,"%d \n",makecolor(drdy));
	fprintf(fd,"%d \n",makecolor(dgdy));
	fprintf(fd,"%d \n",makecolor(dbdy));

	fprintf(fd,"%d \n",makecoord(V[0].x));
	fprintf(fd,"%d \n",makecoord(0.0));

	fprintf(fd,"%d \n",makecoord(V[1].x));
	fprintf(fd,"%d \n",makecoord(0.0));
	fprintf(fd,"%d \n",makecoord(V[0].y));
	fprintf(fd,"%d \n",makecoord(1.0));

	fclose(fd);	
}
