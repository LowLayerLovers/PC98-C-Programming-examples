#include <stdio.h>
#include <i86.h>

int main(void) {
  wchar_t __far *a;
  a = (wchar_t __far *)MK_FP(0xa000, 0x0000);  // 0xa0000000L;

  printf("addr = 0x%lx\n", a);

  a += (80 * 5);  // 5行目から

  // 1面=80x25
  for(int i=0; i<80; i++){
    *a = (wchar_t)'A';
    a++;
  }

  return 0;
}
