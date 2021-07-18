#include <stdio.h>
#include <dos.h>
#include <fcntl.h>

#define IMG_PATH "./02_img_data/6.bmp"
#define OUT_PATH "./dst.bmp"
#define IMG_HEADER 1078
#define IMG_X 28
#define IMG_Y 28
#define ALLOC_SIZE(X) ((X+16-1)/16)

typedef unsigned char uchar;
typedef unsigned int  uint;

int main(void) {
  int handle;
  unsigned int bytes;
  uint header_seg, img_seg;
  uchar __far *header, *img;

  _dos_allocmem(0xFFFF, &header_seg);
  printf("free_mem = %uKB\n", (long long)header_seg*16/1024);

  _dos_allocmem((long)sizeof(uchar) * IMG_HEADER/16 + 1, &header_seg);
  _dos_allocmem((long)sizeof(uchar) * IMG_Y*IMG_X/16 + 1, &img_seg);

  header = (uchar __far *)MK_FP(header_seg, 0x0000);
  img    = (uchar __far *)MK_FP(img_seg, 0x0000);

  // ファイルオープン
  if( _dos_open(IMG_PATH, O_RDONLY, &handle) != 0 ){
    printf("Failed to open file : %s\n", IMG_PATH);
    return -1;
  }

  // 読み込み位置を画像データ位置まで移動 (dos.hにfseek的なものはない)
  _dos_read(handle, header, IMG_HEADER, &bytes);

  // 画像データを逆さに読み込み
  uchar bufLine[IMG_X];
  for(int y = 0; y < IMG_Y; y++){
    _dos_read(handle, bufLine, IMG_X, &bytes);
    for(int x = 0; x < IMG_X; x++){
      img[(IMG_Y-y-1)*IMG_X + x] = bufLine[x];
    }
  }

  _dos_close(handle);


  // ファイル生成・書き込み
  _dos_creat(OUT_PATH, _A_NORMAL, &handle);
  _dos_write(handle, header, IMG_HEADER, &bytes);
  _dos_write(handle, img, IMG_X*IMG_Y, &bytes);
  _dos_close(handle);

  puts("MNIST image copy!");
  printf("%s -> %s\n", IMG_PATH, OUT_PATH);

  _dos_freemem(header_seg);
  _dos_freemem(img_seg);

  return 0;
}