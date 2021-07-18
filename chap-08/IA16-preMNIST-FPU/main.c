#include "../../00_include/nn_data_io.h"
#include "../../00_include/dos_timer.h"
#include "../../00_include/dos_fileio.h"
#include "../../00_include/fpu.h"
#include <math.h>
#include <stdio.h>
#include <stdlib.h>

#include <bios.h>
#include <i86.h>
#include <dos.h>
#include <conio.h>
#include <libi86/string.h>

#define  WB_DIR "01_CNN_WB"
#define IMG_DIR "02_img_data/"
#define IMG_PATH "./02_img_data/8.bmp"

typedef unsigned char uchar;
typedef unsigned int  uint;
typedef unsigned long ulong;

long get_freemem_size();
int  argmax(float __far *array, int size);
int  clear_screen();
int  print_img(uchar *img, int width, int height);

int main(void){
  printf("simpleCNN IA16\n");
  unsigned char *img;
  IMG_INFO *img_info;
  TIME start_time, end_time;
  int  err;
  
  uint W1_seg, W2_seg, W3_seg, b1_seg, b2_seg, b3_seg;
  uint dst1_seg, dst2_seg, dst3_seg, dst4_seg, input_seg;
  float __far *W1, *W2, *W3, *b1, *b2, *b3, *dst1, *dst2, *dst3, *dst4, *input;
  
  long available_mem1 = get_freemem_size();
  printf("available %lu bytes (%lu KB)\n", available_mem1, available_mem1/1024);

  puts("\n==> MALLOC");
  if(err = _dos_allocmem((long)sizeof(float) *   30 * (5*5) /16 + 1, &W1_seg) != 0) return -11;
  if(err = _dos_allocmem((long)sizeof(float) * 4320         /16 + 1, &W2_seg) != 0) return -12; // 4320 * 100 の1行分
  if(err = _dos_allocmem((long)sizeof(float) *  100 *   10  /16 + 1, &W3_seg) != 0) return -13;
  if(err = _dos_allocmem((long)sizeof(float) *  30 /16 + 1, &b1_seg) != 0)          return -14;
  if(err = _dos_allocmem((long)sizeof(float) * 100 /16 + 1, &b2_seg) != 0)          return -15;
  if(err = _dos_allocmem((long)sizeof(float) *  10 /16 + 1, &b3_seg) != 0)          return -16;

  W1 = (float __far *)MK_FP(W1_seg, 0x0000);
  W2 = (float __far *)MK_FP(W2_seg, 0x0000);
  W3 = (float __far *)MK_FP(W3_seg, 0x0000);
  b1 = (float __far *)MK_FP(b1_seg, 0x0000);
  b2 = (float __far *)MK_FP(b2_seg, 0x0000);
  b3 = (float __far *)MK_FP(b3_seg, 0x0000);


  float input_array[28*28];

  float __far *dst1_array[30];
  for (int i=0; i<30; i++) {
    uint seg, err;
    if(err = _dos_allocmem((long)sizeof(float)*(24 * 24)/16+1, &seg) != 0){
      printf("** error ----  _dos_allocmem out of memory  **\n");
      exit(1);
    }

    dst1_array[i] = (float __far *)MK_FP(seg, 0x0000);
    if (dst1_array[i] == NULL) {
      printf("** error ----  out of memory  **\n");
      exit(1);
    }
  }

  float dst2_array[30 * 12 * 12];
  float dst3_array[100];
  float dst4_array[10];


  puts("==> Read Weight & Bias");
  err = 0;
  err += read_bin(WB_DIR"/W1.bin", &W1, (long)30*(5*5));
  err += read_bin(WB_DIR"/W3.bin", &W3, (long)100*10);
  err += read_bin(WB_DIR"/b1.bin", &b1, (long)30);
  err += read_bin(WB_DIR"/b2.bin", &b2, (long)100);
  err += read_bin(WB_DIR"/b3.bin", &b3, (long)10);
  if(err != 0){
    printf("Failed to read .BIN file. \n");
    return -1;
  }

  puts("==> Read MNIST image");
  read_mnist_bmp(IMG_DIR"8.bmp", &img, &img_info);
  printf("Size  : %d px\n", img_info->size);
  printf("Width : %d px\n", img_info->width);
  printf("Height: %d px\n", img_info->height);
  print_img((uchar *)img, 28, 28);

  puts("\n==> Result of malloc");
  long available_mem2 = get_freemem_size();
  printf("reserved  %lu bytes (%lu KB)\n", (available_mem1 - available_mem2), (available_mem1 - available_mem2)/1024);
  printf("available %lu bytes (%lu KB)\n", available_mem2, available_mem2/1024);

  puts("\n===== All ready !! =====");
  printf("Press [Enter] to start the PREDICT!\n");
  while(getchar() != '\n');

  /* //////////////////////////////////////////// */
  /* CNN                                          */
  /* //////////////////////////////////////////// */
  get_time(&start_time);
  int kh     = 5;
  int kw     = 5;
  int in_h   = 28;
  int in_w   = 28;
  int out_ch = 30;
  int padx   = 0;
  int pady   = 0;
  int out_h  = in_h-((kh/2)-padx)*2;
  int out_w  = in_w-((kw/2)-padx)*2;

  for(int y=0;y<28;y++){
    for(int x=0;x<28;x++){
      input_array[y*28+x]=img[y*28+x]/255.0;
    }
  }

  // use my convolution
  puts("Layer1: Convolution ...");
  for(int och=0;och<out_ch;och++){
    for(int i=0;i<out_h;i++){
      for(int j=0;j<out_w;j++){
        float tmp=0;
	finit();
        for(int ki=0;ki<kh;ki++){
          for(int kj=0;kj<kw;kj++){
            int inidx = (i-pady+ki)*in_w+(j-padx+kj);
            int widx  = och*kh*kw+ki*kw+kj;
	    tmp=fadd(fmul(input_array[inidx],W1[widx]),tmp);
          }
        }
	tmp=fadd(b1[och],tmp);//add bias
        tmp  = fmax(tmp, 0); // relu

        dst1_array[och][i*out_w+j]=tmp;
      }
    }
  }

  // for(int och=0;och<out_ch;och++){
  //   for(int i=0;i<out_h;i++){
  //     for(int j=0;j<out_w;j++){
  //       printf("%.10f ", dst1_array[och][i*out_w+j]);
  //     }
  //   }
  //   puts("");
  // }

  //pool
  puts("Layer2: Max pooling ...");
  int ph = 2;
  int pw = 2;
  in_h   = 24;
  in_w   = 24;
  int in_ch  = 30;
  int stride = 2;
  out_w = in_h/stride;
  out_h = in_w/stride;

  //use my pooling
  for(int ich=0;ich<in_ch;ich++){
    for(int i=0;i<out_h;i++){
      for(int j=0;j<out_w;j++){

        float tmp=-1000;
        for(int pi=0;pi<ph;pi++){
          for(int pj=0;pj<pw;pj++){
            // int inidx = ich*in_h*in_w+(i*stride+pi)*in_w+(j*stride+pj);
            // tmp = fmax(dst1[inidx], tmp);
            tmp = fmax(dst1_array[ich][(i*stride+pi)*in_w+(j*stride+pj)], tmp);
          }
        }

        dst2_array[ich*out_w*out_h+i*out_w+j]=tmp;
      }
    }
  }

  // for(int y=0; y<30; y++){
  //   for(int x=0; x<(12*12); x++){
  //     printf("%.10f ", dst2_array[y*(12*12)+x]);
  //   }
  //   puts("");
  // }
  // puts("");


  //affine
  puts("Layer3: Affine1 ...");
  int matw=4320;
  int math=100;
  uint W2bin_handle = 0;

  W2bin_handle = dos_fopen(WB_DIR"/W2T.bin");
  
  for(int i=0;i<math;i++){
    float tmp=0;
    dos_fread((void __far *)W2, sizeof(float), 4320, W2bin_handle); // 値を4320個(1行分)読み込む
    finit();
    for(int j=0;j<matw;j++){
      tmp=fadd(fmul(W2[j],dst2_array[j]),tmp);
    }
    dst3_array[i] = fadd(tmp,b2[i]);
    dst3_array[i] = fmax(dst3_array[i], 0); //relu
  }

  dos_fclose(W2bin_handle);
  

  // for (int i = 0; i < 100; i++){
  //   printf("% .10f\n", dst3_array[i]);
  // }
  

  //affine
  puts("Layer4: Affine2 ...");
  matw=100;
  math=10;
  for(int i=0;i<math;i++){
    float tmp=0;
    finit();
    for(int j=0;j<matw;j++){
      tmp=fadd(fmul(W3[j*math+i],dst3_array[j]),tmp);
      //tmp += W3[j*math+i] * dst3_array[j];
    }
    dst4_array[i] = fadd(tmp,b3[i]);
  }

  get_time(&end_time);

  puts("\n===== result =====");
  // softmax_vec_f32(dst4,10);
  for(int i=0;i<10;i++){
    printf("%d, % .5f\n", i, dst4_array[i]);
  }

  printf("\x1b[33m");
  puts("\n====================");
  puts("Classification:");
  printf("Number is %d\n", argmax(dst4_array, 10));
  puts("====================");
  printf("\x1b[39m");

  printf("start_");
  print_time(&start_time);
  printf("  end_");
  print_time(&end_time);
  print_difftime(&start_time, &end_time);

  printf("\nPress [ENTER] to FINISH!\n");
  while(getchar() != '\n');
  clear_screen();
  
  return 0;
}


/////////////////////////////
// Local functions
/////////////////////////////
long get_freemem_size(){
  uint seg;
  _dos_allocmem(0xFFFF, &seg);
  long available_mem = (long)seg * 16;  // Byte

  return available_mem;
}

int argmax(float __far *array, int size){
    int i, res;
    float max, min;

    max = array[0];
    min = array[0];
    for(i=1; i<size; i++){
        if(array[i] > max){
            max = array[i];
            res = i;
        }
        if(array[i] < min){
            min = array[i];
        }        
    }

    return res;
}

int clear_screen(){
  char __far *vram_P0_addr;
  char __far *vram_P1_addr;
  char __far *vram_P2_addr;
  char __far *vram_P3_addr;
  vram_P0_addr = (char __far *)MK_FP(0xa800, 0x0000);
  vram_P1_addr = (char __far *)MK_FP(0xb000, 0x0000);
  vram_P2_addr = (char __far *)MK_FP(0xb800, 0x0000);
  vram_P3_addr = (char __far *)MK_FP(0xe000, 0x0000);

  for(int y=0; y<400; y++){
    for(int x=0; x<(640/8); x++){
      vram_P0_addr[y*80+x] = 0x00;
      vram_P1_addr[y*80+x] = 0x00;
      vram_P2_addr[y*80+x] = 0x00;
      vram_P3_addr[y*80+x] = 0x00;
    }
  }

  return 0;
}

int  print_img(uchar *img, int width, int height){
  char __far *vram_P0_addr;
  char __far *vram_P1_addr;
  char __far *vram_P2_addr;
  char __far *vram_P3_addr;
  vram_P0_addr = (char __far *)MK_FP(0xa800, 0x0000);
  vram_P1_addr = (char __far *)MK_FP(0xb000, 0x0000);
  vram_P2_addr = (char __far *)MK_FP(0xb800, 0x0000);
  vram_P3_addr = (char __far *)MK_FP(0xe000, 0x0000);

  // グラフィック設定
  asm ("mov $0x42, %ah");
  asm ("mov $0b11000000, %ch");
  asm ("int $0x18");

  // アナログパレット設定
  // 0x0: デジタル8色モード
  // 0x1: アナログ16色モード
  asm ("mov $0x1, %al");
  asm ("out %al, $0x6a");

  // グラフィック使用開始
  asm ("mov $0x40, %ah");
  asm ("int $0x18");

  int off_y = 10-8;
  int off_x = 200-1;
  for(int y=0; y<height*8+(8*2); y++){
    for(int x=0; x<width+2; x++){
      vram_P3_addr[(off_y + y) * 80 + (off_x + x)] = 0xff;
    }
  }

  off_y = 10;
  off_x = 200;
  for(int y=0; y<height*8; y++){
    for(int x=0; x<width; x++){
      char dot = ((img[(y/8)*width+x] > 0) ? 0xff : 0x00);
      vram_P0_addr[(off_y + y) * 80 + (off_x + x)] = dot;
      vram_P1_addr[(off_y + y) * 80 + (off_x + x)] = dot;
      vram_P2_addr[(off_y + y) * 80 + (off_x + x)] = dot;
      vram_P3_addr[(off_y + y) * 80 + (off_x + x)] = dot;
    }
  }

  return 0;
}
