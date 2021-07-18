#include <stdio.h>
#include <stdlib.h>
#include <i86.h>
#include <dos.h>

#define IMG_PATH "./02_img_data/8.bmp"
#define IMG_X 28
#define IMG_Y 28

typedef unsigned char uchar;

int print_img(uchar *img){
  printf("print img...\n");

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
  for(int y=0; y<IMG_Y*8+(8*2); y++){
    for(int x=0; x<IMG_X+2; x++){
      vram_P3_addr[(off_y + y) * 80 + (off_x + x)] = 0xff;
    }
  }

  off_y = 10;
  off_x = 200;
  for(int y=0; y<IMG_Y*8; y++){
    for(int x=0; x<IMG_X; x++){
      char dot = ((img[(y/8)*IMG_X+x] > 0) ? 0xff : 0x00);
      vram_P0_addr[(off_y + y) * 80 + (off_x + x)] = dot;
      vram_P1_addr[(off_y + y) * 80 + (off_x + x)] = dot;
      vram_P2_addr[(off_y + y) * 80 + (off_x + x)] = dot;
      vram_P3_addr[(off_y + y) * 80 + (off_x + x)] = dot;
    }
  }

  printf("Press [ENTER] key!\n");
  while(getchar() != '\n');

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

int main(void) {
  FILE * fp = NULL;
  uchar img[28][28];

  puts("MNIST image show !");

  if((fp=fopen(IMG_PATH, "rb")) == NULL){
    printf("Failed to open file : %s\n", IMG_PATH);
    return -1;
  }

  // ファイルポインタを画像データ位置まで移動
  // fseek(fp, 1078, SEEK_SET); // IA16_GCC で fseek はなぜかうまく動作しない．
  uchar tmp[1078];
  fread(tmp, sizeof(uchar), 1078, fp);

  // 画像データをそのまま読み込み
  // fread(img, sizeof(uchar), IMG_X*IMG_Y, fp);

  // 画像データを逆さに読み込み
  uchar bufLine[IMG_X];
  for(int y = 0; y < IMG_Y; y++){
    fread(bufLine, 1, IMG_X, fp);
    for(int x = 0; x < IMG_X; x++){
      img[(IMG_Y-y-1)][x] = bufLine[x];
    }
  }

  // // HEX表示
  // for(int y = 0; y < IMG_Y; y++){
  //   for(int x = 0; x < IMG_X; x++){
  //     printf("%2X ", img[y][x]);
  //   }
  //   puts("");
  // }

  // // 2値表示
  // for(int y = 0; y < IMG_Y; y++){
  //   for(int x = 0; x < IMG_X; x++){
  //     printf("%c", (img[y][x] > 0) ? '@' : ' ');
  //   }
  //   puts("");
  // }

  print_img((uchar *)img);

  fclose(fp);

  return 0;
}