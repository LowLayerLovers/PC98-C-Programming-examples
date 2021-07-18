#ifndef FPU_H
#define FPU_H

void finit();
void zeroset();
void zeroset_4elm();
// FLOAT TYPE
float fpop();
float fadd(float a, float b);
float fmul(float a, float b);
int   fma_r(float a, float b);
int   fadd_r(float a);
int   wset(float a);
int set_mul_4elm(float a,float b, float c, float d);
int fma_4elm(float a,float b, float c, float d);
float fadd_4elm(float a);

// DOUBLE TYPE
double faddl(double a, double b);
double fmull(double a, double b);

#endif
