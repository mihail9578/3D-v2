#ifndef __MAIN_H__
#define __MAIN_H__

/* Function */
typedef struct _Point
{
	int x;
	int y;
} Point;

double f(double x, double y);

int Init(void);
//void drawLine(Point* p1, Point*p2);
void drFunc(int width, int height, double (*func)(double x, double y), int cv);
void CalcMaxAndMin(int width, int height, double (*func)(double x, double y), double* maxim, double *minim);
void render(int width, int height);
void keyboard(int key);
//void Finalize(void);

#endif /* __MAIN_H__ */
