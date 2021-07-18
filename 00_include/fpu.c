#include "fpu.h"

#ifdef __MSDOS__
void finit(){
  __asm volatile(
		 ".arch pentium;         "
		 "finit;                 "
		 "fwait;                 "
		 ".arch i286             "
		 );
}

void zeroset(){
  __asm volatile(
		 ".arch pentium; "
		 "fldz;          " // zero -> st0
		 "fwait;         "
		 ".arch i286     "
		 );
}

void zeroset_4elm(){
  __asm volatile(
		 ".arch pentium; "
		 "fldz;          "
		 "fldz;          "
		 "fldz;          "
		 "fldz;          "
		 "fwait;         "
		 ".arch i286     "
		 );
}

// FLOAT TYPE
float fpop(){
  float out;

  __asm volatile(
		 ".arch pentium;         "
		 "fstp  %0;              " // st0 -> %0 & pop st0
		 "fwait;                 "
		 ".arch i286             "
		 : "=m" (out) 
		 );

  return out;
}

float fadd_4elm(float a){
  __asm volatile(
		 ".arch pentium;         "
		 "fld   %0;              "
		 "fxch  %%st(4);          "
		 "fadd  %%st(4), %%st(0);"
		 
		 "fxch  %%st(3);         "
		 "fadd  %%st(4), %%st(0);"
		 
		 "fxch  %%st(2);          "
		 "fadd  %%st(4), %%st(0);"
		 
		 "fxch  %%st(1);          "
		 "fadd  %%st(4), %%st(0);"
		 "fwait;                 "
		 ".arch i286             "
		 : : "m" (a)
		 );
  return 0;  
}

float fadd(float a, float b){
  float out;

  __asm volatile(
		 ".arch pentium;         "
		 "fld   %1;              " // %1 -> st0
		 "fld   %2;              " // %2 -> st0, %1 -> st1
		 "faddp %%st(0), %%st(1);" // st0 + st1 -> st1 & pop
		 "fstp  %0;              " // st0 -> %0 & pop st0
		 "fwait;                 "
		 ".arch i286             "
		 : "=m" (out) : "m" (a), "m" (b)
		 );

  return out;
}

int fadd_r(float a){
  __asm volatile(
		 ".arch pentium;         "
		 "fld   %0;              " // %0 -> st0
		 "fadd  %%st(1), %%st(0);" // fma 
		 "ffree %%st(1);         "
		 "fwait;                 "
		 ".arch i286             "
		 : : "m" (a)
		 );
  return 0;
}

int wset(float a){
  __asm volatile(
		 ".arch pentium;         "
		 "fld   %0;              " // %0 -> st0
		 "fwait;                 "
		 ".arch i286             "
		 : : "m" (a)
		 );
  return 0;
}

int fma_4elm(float a,float b, float c, float d){
  __asm volatile(
		 ".arch pentium;         "
		 "fld   %0;              "
		 "fmul  %%st(1), %%st(0);"
		 "fadd  %%st(5), %%st(0);"
		 "ffree %%st(5);         "
    
		 "fld   %1;              "
		 "fmul  %%st(2), %%st(0);"
		 "fadd  %%st(5), %%st(0);"
		 "ffree %%st(5);         "
    
		 "fld   %2;              "
		 "fmul  %%st(3), %%st(0);"
		 "fadd  %%st(5), %%st(0);"
		 "ffree %%st(5);         "
    
		 "fld   %3;              "
		 "fmul  %%st(4), %%st(0);"
		 "fadd  %%st(5), %%st(0);"
		 "ffree %%st(5);         "
		 "ffree %%st(4);         "
		 "fwait;                 "
		 ".arch i286             "
		 : : "m" (a), "m" (b), "m" (c), "m"(d)
		 );
  return 0;
}

int fma_r(float a, float b){
  __asm volatile(
		 ".arch pentium;         "
		 "fld   %0;              " // %0 -> st0
		 "fld   %1;              " // %1 -> st0, %0 -> st1
		 "fmul  %%st(1), %%st(0);" // st1 * st0 -> st0
		 "fadd  %%st(2), %%st(0);" // fma 
		 "ffree %%st(1);         "
		 "ffree %%st(2);         "
		 "fwait;                 "
		 ".arch i286             "
		 : : "m" (a), "m" (b)
		 );
  return 0;
}

float fmul(float a, float b){
  float out;

  __asm volatile(
		 ".arch pentium;         "
		 "fld   %1;              " // %1 -> st0
		 "fld   %2;              " // %2 -> st0, %1 -> st1
		 "fmulp %%st(0), %%st(1);" // st0 * st1 -> st1 & pop
		 "fstp  %0;              " // st0 -> %0 & pop st0
		 "fwait;                 "
		 ".arch i286             "
		 : "=m" (out) : "m" (a), "m" (b)
		 );

  return out;
}

// DOUBLE TYPE
double faddl(double a, double b){
  double out;

  __asm volatile(
		 ".arch pentium;         "
		 "fldl  %1;              " // %1 -> st0
		 "fldl  %2;              " // %2 -> st0, %1 -> st1
		 "faddp %%st(0), %%st(1);" // st0 + st1 -> st1 & pop
		 "fstpl %0;              " // st0 -> %0 & pop st0
		 "fwait;                 "
		 ".arch i286             "
		 : "=m" (out) : "m" (a), "m" (b)
		 );

  return out;
}

double fmull(double a, double b){
  double out;

  __asm volatile(
		 ".arch pentium;         "
		 "fldl  %1;              " // %1 -> st0
		 "fldl  %2;              " // %2 -> st0, %1 -> st1
		 "fmulp %%st(0), %%st(1);" // st0 * st1 -> st1 & pop
		 "fstpl %0;              " // st0 -> %0 & pop st0
		 "fwait;                 "
		 ".arch i286             "
		 : "=m" (out) : "m" (a), "m" (b)
		 );

  return out;
}

#endif
