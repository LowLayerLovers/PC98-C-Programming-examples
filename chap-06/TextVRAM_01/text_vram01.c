#include <stdio.h>
#include <i86.h>

int main(void) {
  wchar_t __far *a;
  a = (wchar_t __far *)MK_FP(0xa000, 0x0000);  // 0xa0000000L;

  printf("addr = 0x%lx\n", a);

  *a = (wchar_t)'@';

  return 0;
}
