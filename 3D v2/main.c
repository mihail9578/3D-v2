#include <stdlib.h>
#include <stdio.h>
#include <math.h>

#include "main.h"
#include "xlib.h"

#include "m2.h"

static int n = 10;
static int m = 20;
static double a = -5;
static double b = +5;
static double c = -5;
static double d = +5;
static double xl, xr;
static double yu, yd;
static double max, min;

static int cond = 1;
static int nstp = 100;
static double phi = 30.0 * 3.1415926/180.0;
static double psi = 0.5;

double f(double x, double y)
{
	return x * x + y * y;
}

static double ferr(double x, double y)
{
	return f(x, y) - Pfres(x, y);
}		

void drFunc(int width, int height, double (*func)(double x, double y), int cv)
{
	Point* cline = (Point*)malloc(nstp * sizeof(Point));
	Point* nline = (Point*)malloc(nstp * sizeof(Point));
	Point* axline = (Point*)malloc(nstp * sizeof(Point));
	Point* ayline = (Point*)malloc(nstp * sizeof(Point));
	Point* azline = (Point*)malloc(nstp * sizeof(Point));
	
	double sphi = sin(phi);
	double cphi = cos(phi);
	double spsi = sin(psi);
	double cpsi = cos(psi);

	double e1[3] = {cphi, sphi, 0.0};
	double e2[3] = {sphi * spsi, -spsi * cphi, cpsi};

	double x;
	double y;
	double z;

	double hx = (xr - xl)/(nstp - 1);
	double hy = (yu - yd)/(nstp - 1);

	double xMin = (e1[0] >= 0.0 ? xl : xr) * e1[0] + (e1[1] >= 0.0 ? yd : yu) * e1[1];

	double xMax = (e1[0] >= 0.0 ? xr : xl) * e1[0] + (e1[1] >= 0.0 ? yu : yd) * e1[1];

	double yMin = (e2[0] >= 0.0 ? xl : xr) * e2[0] + (e2[1] >= 0.0 ? yd : yu) * e2[1];

	double yMax = (e2[0] >= 0.0 ? xr : xl) * e2[0] + (e2[1] >= 0.0 ? yu : yd) * e2[1];

	double ax;
	double bx;
	double ay;
	double by;
	int i, j;
	double axx, ayy, azz;
	double aaxx, aayy, aazz;
	double aaaxx, aaayy, aaazz;
	double lX, lY, lZ;
	double llX, llY, llZ;
	double lllX, lllY, lllZ;
	int cLX, cLY;
	int ccLX, ccLY;
	int cccLX, cccLY;
	
	width = width; height = height;

	if (e2[2] >= 0.0)
	{
		yMin += min * e2[2];
		yMax += max * e2[2];
	}
	else
	{
		yMin += max * e2[2];
		yMax += min * e2[2];
	}

	ax = -750 * xMin/(xMax - xMin);
	bx = 750/(xMax - xMin);
	ay = -550 * yMin/(yMax - yMin);
	by = 550/(yMax - yMin);

	for (i = 0; i < nstp; i++)
	{
		x = xl + i * hx;
		y = yd + (nstp - 1) * hy;
		z = func(x, y);
		
		lX = xl + (i) * hx;
		lY = 0;
		lZ = 0;
		
		llX = 0;
		llY = xl + (i) * hx;
		llZ = 0;
		
		lllX = 0;
		lllY = 0;
		lllZ = xl + (i) * hx;
		
		axx = xl + (i-nstp/2) * hx * 100;
		ayy = 0;
		azz = 0;
		
		aaxx = 0;
		aayy = (xl + (i-nstp/2) * hx * 100);
		aazz = 0;

		aaaxx = 0;
		aaayy = 0;
		aaazz = (xl + (i-nstp/2) * hx * 100);
		
		cline[i].x = (int)(ax + bx * (x * e1[0] + y * e1[1]));
		cline[i].y = height - (int)(ay + by * (x * e2[0] + y * e2[1] + z * e2[2]));
		
		axline[i].x = (int)(ax + bx * (axx * e1[0] + ayy * e1[1]));
		axline[i].y =  height - (int)(ay + by * (axx * e2[0] + ayy * e2[1] + azz * e2[2]));
		
		ayline[i].x = (int)(ax + bx * (aaxx * e1[0] + aayy * e1[1]));
		ayline[i].y =  height - (int)(ay + by * (aaxx * e2[0] + aayy * e2[1] + aazz * e2[2]));
		
		azline[i].x = (int)(ax + bx * (aaaxx * e1[0] + aaayy * e1[1]));
		azline[i].y =  height - (int)(ay + by * (aaaxx * e2[0] + aaayy * e2[1] + aaazz * e2[2]));
		
		if(i == 0){
		  cLX = (int)(ax + bx * (lX * e1[0] + lY * e1[1]));
		  cLY = height - (int)(ay + by * (lX * e2[0] + lY * e2[1] + lZ * e2[2]));
		}
		
		if(i == 0){
		  ccLX = (int)(ax + bx * (llX * e1[0] + llY * e1[1]));
		  ccLY = height - (int)(ay + by * (llX * e2[0] + llY * e2[1] + llZ * e2[2]));
		}
		
		if(i == 0){
		  cccLX = (int)(ax + bx * (lllX * e1[0] + lllY * e1[1]));
		  cccLY = height - (int)(ay + by * (lllX * e2[0] + lllY * e2[1] + lllZ * e2[2]));
		}
	}
	for (i = nstp - 1; i > -1; i--)
	{
		//printf("%f\n", axline[1].x);
		//printf("%f\n", axline[1].y);
		if(cv == 1){
		  xlibSetColor(255, 0, 0);
		} else {
		  xlibSetColor(0, 0, 255);
		}
		for (j = 0; j < nstp - 1; j++) xlibDrawLine(cline[j].x, cline[j].y, cline[j + 1].x, cline[j + 1].y);	
		xlibSetColor(0, 255, 0);
		printf("%d\n", cLX);
		printf("%d\n", cLY);
		
		for (j = 0; j < nstp - 1; j++) xlibDrawLine(axline[j].x, axline[j].y, axline[j + 1].x, axline[j + 1].y); xlibDrawString(cLX, cLY, "X");
		for (j = 0; j < nstp - 1; j++) xlibDrawLine(ayline[j].x, ayline[j].y, ayline[j + 1].x, ayline[j + 1].y); xlibDrawString(ccLX, ccLY, "Y");
		for (j = 0; j < nstp - 1; j++) xlibDrawLine(azline[j].x, azline[j].y, azline[j + 1].x, azline[j + 1].y); xlibDrawString(cccLX, cccLY, "Z");
		
		if (i > 0)
			for (j = 0; j < nstp; j++)
			{
				x = xl + j * hx;
				y = yd + (i - 1) * hy;
				z = func(x, y);
				if(cv == 1){
				  xlibSetColor(255, 0, 0);
				} else {
				  xlibSetColor(0, 0, 255);
				}
				
				nline[j].x = (int)(ax + bx * (x * e1[0] + y * e1[1]));
				nline[j].y = height - (int)(ay + by * (x * e2[0] + y * e2[1] + z * e2[2]));
				xlibDrawLine(cline[j].x, cline[j].y, nline[j].x, nline[j].y);
				cline[j] = nline[j];
			}
	}

	free(cline);
	free(nline);
	free(axline);
}

void CalcMaxAndMin(int width, int height, double (*func)(double x, double y), double* maxim, double* minim)
{
	int i, j;
	double x, y, z;
	double hx, hy;
	
	width = width;
	height = height;

	hx = (xr - xl)/nstp;
	hy = (yu - yd)/nstp;

	*maxim = *minim = func(xl, yd);
	for (i = 0; i < nstp; i++)
		for (j = 0; j < nstp; j++)
		{
			x = xl + i * hx;
			y = yd + j * hy;
			z = func(x, y);
			if (z > *maxim) *maxim = z;
			if (z < *minim) *minim = z;
		}
}

void render(int width, int height)
{
	double r1, r2;
	char s01[256];
	char s02[256];
	char s03[256];
	
	xlibSetColor(1, 1, 1);
	xlibFillRectangle(0, 0, width, height);
        
    if(cond == 1) sprintf(s01, "f, a = %.2f, b = %.2f, n = %d, c = %.2f, d = %.2f, m = %d", a, b, n, c, d, m);
	if(cond == 2) sprintf(s01, "Pf2, a = %.2f, b = %.2f, n = %d, c = %.2f, d = %.2f, m = %d", a, b, n, c, d, m);
	if(cond == 3) sprintf(s01, "||f-Pf2||, a = %.2f, b = %.2f, n = %d, c = %.2f, d = %.2f, m = %d", a, b, n, c, d, m);
	
	sprintf(s02, "xl = %.2f  xr = %.2f, yu = %.2f, yd = %.2f", xl, xr, yu, yd);
        
	xlibSetColor(0, 0, 0);
	xlibDrawString(50, 40, s01);
	xlibDrawString(50, 60, s02);

	if (cond != 3)
	{
		CalcMaxAndMin(width, height, f, &max, &min);
		sprintf(s03, "max f = %.2f  min f = %.2f", max, min);
		xlibDrawString(50, 80, s03);
	}
	else if (cond == 3)
	{
		CalcMaxAndMin(width, height, ferr, &r1, &r2);
		if (fabs(r1) < fabs(r2)) r1 = r2;
		r1 = fabs(r1);
		sprintf(s03, "err = %e", r1);
		xlibDrawString(50, 80, s03);
	}

	switch (cond)
	{
	case 1:
		xlibSetColor(1.0, 0.0, 0.0);
		drFunc(width, height, f, 1);
		break;
	case 2:
		xlibSetColor(0.0, 0.0, 1.0);
		drFunc(width, height, Pfres, 2);
		break;
	case 3:
		xlibSetColor(0.0, 0.0, 1.0);
		drFunc(width, height, ferr, 2);
		break;
	}
}

void keyboard(int key)
{
	double t;
	int n1;
	int n2;
	double delta;

	switch (key)
	{
	case 'q':
		xlibPostExit();
		return;
	case 'd':
		if (++cond > 3) cond = 1;
		if (!cond) break;
		if (cond == 2 || cond == 3)
		{
			ip2();
			res2();
		}
		break;
	case 'a':
		if (--cond < 1) cond = 3 ;
		if (!cond) break;
		if (cond == 2 || cond == 3)
		{
			ip2();
			res2();
		}
		break;
	case 'z':
	case 'u':
		t = (xr - xl);
		if (key == 'z') t /= 4.0;
		else t /= -2.0;
		xl += t;
		xr -= t;
		t = (yu - yd);
		if (key == 'z') t /= 4.0;
		else t /= -2.0;
		yd += t;
		yu -= t;
		break;
	case 'N':
	case 'n':
		fr();

		if (key == 'N')
		{
			n *= 2;
		}
		else if (n > 5 && m > 5)
		{
			n /= 2;
		}

		if (!in2(n, a, b, m, c, d))
		{
			printf("Not enough memory.\n");
			fr();
			xlibPostExit();
			return;
		}

		if (cond == 2 || cond == 3)
		{
			ip2();
			res2();
		}
		break;
	case 'M':
	case 'm':
		fr();

		if (key == 'M')
		{
			m *= 2;
		}
		else if (n > 5 && m > 5)
		{
			m /= 2;
		}

		if (!in2(n, a, b, m, c, d))
		{
			printf("Not enough memory.\n");
			fr();
			xlibPostExit();
			return;
		}

		if (cond == 2 || cond == 3)
		{
			ip2();
			res2();
		}
		break;
	case 'e':
	case 'r':
		n1 = n/3;
		n2 = m/3;
		if (key == 'e') delta = +3.0;
		else delta = -3.0;
		if (cond == 2 || cond == 3)
		{
			Delta_01(n1, n2, delta);
			res2();
		}
		break;
	case 'w':
		phi += (3.1415926/180.0)/4.0;
		break;
	case 'W':
		phi += (3.1415926/180.0)*8.0;
		break;
	case 's':
		phi -= (3.1415926/180.0)/4.0;
		break;
	case 'S':
		phi -= (3.1415926/180.0)*8.0;
		break;
	case '+':
		nstp *= 2;
		break;
	case '-':
		if (nstp > 5) nstp /= 2;
		break;
	case KEY_LEFT:
		xl -= 1.0;
		xr -= 1.0;
		break;
	case KEY_RIGHT:
		xl += 1.0;
		xr += 1.0;
		break;
	case KEY_DOWN:
		yd -= 1.0;
		yu -= 1.0;
		break;
	case KEY_UP:
		yd += 1.0;
		yu += 1.0;
		break;
	default:
		return;
	}

	xlibPostRedisplay();
}

int Init(void)
{
/*#ifdef CONSOLE_INPUT
	printf("a, b: ");
	scanf("%lg %lg", &a, &b);
	printf("n: ");
	scanf("%d", &n);
	printf("c, d: ");
	scanf("%lg %lg", &c, &d);
	printf("m: ");
	scanf("%d", &m);
#endif*/

	if (b <= a || n < 4 || d <= c || m < 4)
	{
		printf("Input err.\n");
		return 0;
	}

	xl = a;
	xr = b;
	yd = c;
	yu = d;

	if (!in2(n, a, b, m, c, d))
	{
		printf("Memory err.\n");
		fr();
		return 0;
	}

	return 1;
}

int main(void)
{
	int width = 750;
	int height = 550;

	xlibInitPosition(0, 0);
	xlibInitWindowSize(width, height);

	xlibRenderFunc(render);
	xlibKeyboardFunc(keyboard);

	if (!Init()) return -1;

	xlibMainLoop("Interpolation");

	fr();

	return 0;
}
