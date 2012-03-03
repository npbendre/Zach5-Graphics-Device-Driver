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
	float x, y, z, r, g, b;
};

struct vertex V[] = {
	50.0, 400.0, 0.0, 255.0, 0.0, 0.0,
	200.0, 650.0, 0.0, 255.0, 0.0, 0.0,
	500.0, 200.0, 0.0, 0.0, 0.0, 128.0,
	0.0
};

void main()
{

	float drdy, dgdy, dbdy, drdx,dgdx,dbdx,dx02,dx01,dx12;
	FILE *fd;
	int a;
	
        a = fabs(((V[0].x-V[2].x)*(V[1].y-V[2].y))-((V[1].x-V[2].x)*(V[0].y-V[2].y)));
	printf ("a %d \n",a);
	drdy = (V[2].r - V[1].r)/(V[2].y - V[1].y);
	dgdy = (V[2].g - V[1].g)/(V[2].y - V[1].y);
	dbdy = (V[2].b - V[1].b)/(V[2].y - V[1].y);

        drdx = (((V[0].r-V[2].r)*((V[1].y-V[2].y)/a))-((V[1].r-V[2].r)*((V[0].y-V[2].y)/a)));
	dgdx = (((V[0].g-V[2].g)*((V[1].y-V[2].y)/a))-((V[1].g-V[2].g)*((V[0].y-V[2].y)/a)));
	dbdx = (((V[0].b-V[2].b)*((V[1].y-V[2].y)/a))-((V[1].b-V[2].b)*((V[0].y-V[2].y)/a)));

	printf("drdx dgdx dbdx, %f %f %f\n", drdx,dgdx,dbdx);
	dx02 = ((V[2].x-V[0].x)/(V[2].y-V[0].y));
	dx01 = ((V[1].x-V[0].x)/(V[1].y-V[0].y));
	dx12 = ((V[2].x-V[1].x)/(V[2].y-V[1].y));
	
	fd = fopen("fltointri.txt","w");
//colors
	fprintf(fd,"r0 %d \n",makecolor(V[0].r));
	fprintf(fd,"g0 %d \n",makecolor(V[0].g));
	fprintf(fd,"b0 %d \n",makecolor(V[0].b));

	fprintf(fd,"%d \n",makecolor(drdy));
	fprintf(fd,"%d \n",makecolor(dgdy));
	fprintf(fd,"%d \n",makecolor(dbdy));

	fprintf(fd,"%d \n",makecolor(drdx));
	fprintf(fd,"%d \n",makecolor(dgdx));
	fprintf(fd,"%d \n",makecolor(dbdx));

//coordinates

	fprintf(fd,"0 %d \n",makecoord(0.0));
	fprintf(fd,"0 %d \n",makecoord(0.0));
	fprintf(fd,"0 %d \n",makecoord(0.0));
	fprintf(fd,"0 %d \n",makecoord(0.0));
	fprintf(fd,"0 %d \n",makecoord(0.0));
	fprintf(fd,"0 %d \n",makecoord(0.0));

	//top part
	fprintf(fd,"StartXDom %d \n",makecoord(V[0].x));
	fprintf(fd,"dXDom %d \n",makecoord(dx02));
	fprintf(fd,"XSub0 %d \n",makecoord(V[0].x));
	fprintf(fd,"dXSub %d \n",makecoord(dx01));
	
	fprintf(fd,"y0 %d \n",makecoord(V[0].y));
	fprintf(fd,"dy %d \n",makecoord(1.0));
	
	//lower part
	fprintf(fd,"StartXDom %d \n",makecoord(V[2].x));
	fprintf(fd,"dXDom %d \n",makecoord(dx02));
	fprintf(fd,"XSub0 %d \n",makecoord(V[2].x));
	fprintf(fd,"dXSub %d \n",makecoord(dx12));

	fprintf(fd,"y0 %d \n",makecoord(V[2].y));
	fprintf(fd,"dy %d \n",makecoord(1.0));
	
	fprintf(fd,"XSub0 %d \n",makecoord(V[1].x));

	fclose(fd);	
}
