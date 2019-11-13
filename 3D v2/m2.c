#include <stdlib.h>
#include "main.h"
#include "m2.h"

static int n;
static double a;
static double b;

static int qk;
static int ql;

static int m;
static double c1;
static double d;

static double *xpoints = NULL;
static double *ypoints = NULL;
static double *f_ij = NULL;
static double *c = NULL;

/*void Coeff_01(int n, int m, double *xpoints, double *ypoints, double *f_ij, double *c, int qk, int ql);
double Value_01(double x, double y, int n, int m, double *c, double *xpoints, double *ypoints, int qk, int ql);*/

int in2(int n_, double a_, double b_, int m_, double c_, double d_)
{
	n = n_;
	a = a_;
	b = b_;

	m = m_;
	c1 = c_;
	d = d_;

	qk = 2;
	ql = 2;

	xpoints = (double*)malloc(n * sizeof(double));
	ypoints = (double*)malloc(m * sizeof(double));
	f_ij = (double*)malloc(n * m * sizeof(double));
	c = (double*)malloc((n - qk) * (qk + 1) * (m - ql) * (ql + 1) * sizeof(double));

	if (!(xpoints && ypoints && f_ij && c)) return 0;


	return 1;
}

void fr(void)
{
	if (xpoints) free(xpoints); xpoints = NULL;
	if (ypoints) free(ypoints); ypoints = NULL;
	if (f_ij) free(f_ij); f_ij = NULL;
	if (c) free(c); c = NULL;
}

void ip2(void)
{
	int i, j;
	double hx, hy;

	hx = (b - a)/(n - 1);
	hy = (d - c1)/(m - 1);

	for (i = 0; i < n; i++)
	{
		xpoints[i] = a + i * hx;
		for (j = 0; j < m; j++)
		{
			ypoints[j] = c1 + j * hy;
			f_ij[i * m + j] = f(xpoints[i], ypoints[j]);
		}
	}
}

void Delta_01(int number1, int number2, double delta)
{
	f_ij[number1 * m + number2] += delta;
}

void res2(void)
{
	met2(n, m, xpoints, ypoints, f_ij, c, qk, ql);
}

double Pfres(double x, double y)
{
	return v2(x, y, n, m, c, xpoints, ypoints, qk, ql);
}

static void INTERP_X(int i, int n, int m, double *c, double *ypoints, int qk, int ql, int px)
{
	int k, j;
	n = n;
	for (k = 1; k < ql + 1; k++)
		for (j = ql; j > k - 1; j--)
			c[px * (m - ql) * (qk + 1) * (ql + 1) + (m - ql -1) * (qk + 1) * (ql + 1) + i * (ql + 1) + j] = (c[px * (m - ql) * (qk + 1) * (ql + 1) + (m - ql - 1) * (qk + 1) * (ql + 1) + i * (ql + 1) + j] - c[px * (m - ql) * (qk + 1) * (ql + 1) + (m - ql -1) * (qk + 1) * (ql + 1) + i * (ql + 1) + j - 1])/(ypoints[m - ql - 1 + j] - ypoints[m - ql - 1 + j - k]);

	for(k = 1; k < ql + 1; k++)
		for(j = 0; j < m - ql - 1; j++)
			 c[px * (m - ql) * (qk + 1) * (ql + 1) + j * (qk + 1) * (ql + 1) + i * (ql + 1) + k] = (c[px * (m - ql) * (qk + 1) * (ql + 1) + (j + 1) * (qk + 1) * (ql + 1) + i * (ql + 1) + k - 1] - c[px * (m - ql) * (qk + 1) * (ql + 1) + j * (qk + 1) * (ql + 1) + i * (ql + 1) + k - 1]) / (ypoints[j + k] - ypoints[j]);
}

static void INTERP_Y(int j, int n, int m, double *c, double *xpoints, int qk, int ql, int py)
{
	int i, k;

	for (i = 1; i < qk + 1; i++)
		for (k = qk; k > i - 1; k--)
			c[(n - qk - 1) * (m - ql) * (qk + 1) * (ql + 1) + py * (qk + 1) * (ql + 1) + k * (ql + 1) + j] = (c[(n - qk - 1) * (m - ql) * (qk + 1) * (ql + 1) + py * (qk + 1) * (ql + 1) + k * (ql + 1) + j] - c[(n - qk - 1) * (m - ql) * (qk + 1) * (ql + 1) + py * (qk + 1) * (ql + 1) + (k - 1) * (ql + 1) + j])/(xpoints[n - qk - 1 + k] - xpoints[n - qk - 1 + k - i]);

	for (i = 1; i < qk + 1; i++)
		for (k = 0; k < n - qk - 1; k++)
			c[k * (m - ql) * (qk + 1) * (ql + 1) + py * (qk + 1) * (ql + 1) + i * (ql + 1) + j] = (c[(k + 1) * (m - ql) * (qk + 1) * (ql + 1) + py * (qk + 1) * (ql + 1) + (i - 1) * (ql + 1) + j] - c[k * (m - ql) * (qk + 1) * (ql + 1) + py * (qk + 1) * (ql + 1) + (i - 1) * (ql + 1) + j])/(xpoints[k+ i] - xpoints[k]);

}

void met2(int n, int m, double *xpoints, double *ypoints, double *f_ij, double *c, int ql, int qk)
{
	int px, py;
	int i, j;

	for (px = 0; px < n - qk; px++){
		for (py = 0; py < m - ql; py++){
			for(i = 0; i < qk + 1; i++){
				for(j = 0; j < ql + 1; j++){
					c[px * (m - ql) * (qk + 1) * (ql + 1) + py * (qk + 1) * (ql + 1) + i * (ql + 1) + j] = f_ij[(px + i) * m + (py + j)];
				}
			}
		}
	}

    for(px = 0; px < n - qk; px++){
		for (i = 0; i < qk + 1; i++){
			INTERP_X(i, n, m, c, ypoints, qk, ql, px);
			}
	}

	for(py = 0; py < m - ql; py ++){
		for (j = 0; j < ql + 1; j++){
			INTERP_Y(j, n, m, c, xpoints, qk, ql, py);
		}
	}
		
	
}

double v2(double x, double y, int n, int m, double *c, double *xpoints, double *ypoints, int qk, int ql)
{
	int i, j, px,py;
	double f_xy;
	double t;

	f_xy = 0;

	for(px = 0; px < n - qk - 1; px++)
		if(x <= xpoints[px + 1]) break;

	for(py = 0; py < m - ql - 1; py++)
		if(y <= ypoints[py + 1]) break; 

	for (i = qk; i >= 0; i--)
	{


		f_xy *= (x - xpoints[px + i]);
        t = c[px * (m - ql) * (qk + 1) * (ql + 1) + py * (qk + 1) * (ql + 1) + i * (ql + 1) + ql];
		for (j = ql - 1; j >= 0; j--){
			t = t * (y - ypoints[py + j]) + c[px * (m - ql) * (qk + 1) * (ql + 1) + py * (qk + 1) * (ql + 1) + i * (ql + 1) + j];
		}
		f_xy += t;
	}

	return f_xy;
}
