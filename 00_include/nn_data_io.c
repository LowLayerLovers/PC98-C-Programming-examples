#include <stdio.h>
#include <stdlib.h>
#include "nn_data_io.h"

#ifdef __MSDOS__
#include "dos_fileio.h"
#endif

typedef unsigned char uchar;

int read_mnist_bmp(const char *file_path, uchar **img, IMG_INFO **img_info){
  FILE * fp = NULL;
  BITMAPFILEHEADER pFH;
  BITMAPINFOHEADER pIH;

  *img_info =(IMG_INFO*)malloc(sizeof(IMG_INFO));

  if((fp=fopen(file_path, "rb")) == NULL){
    printf("%sファイルのオープンに失敗\n", file_path);
    return -1;
  }

#ifdef __MSDOS__
  // ファイルポインタを画像データ位置まで移動
  // fseek(fp, 1078, SEEK_SET); // IA16_GCC で fseek はなぜかうまく動作しない．
  uchar img_tmp[1078];
  fread(img_tmp, sizeof(uchar), 1078, fp);
  
  int img_fp = dos_fopen(file_path);
  dos_fread(&pFH, sizeof(BITMAPFILEHEADER), 1, img_fp);
  dos_fread(&pIH, sizeof(BITMAPINFOHEADER), 1, img_fp);
  dos_fclose(img_fp);
#else
  fread(&pFH, sizeof(BITMAPFILEHEADER), 1, fp);
  fread(&pIH, sizeof(BITMAPINFOHEADER), 1, fp);
#endif

  // BMPであることを確認
  if(pFH.bfType != 0x4d42){
    printf("This file is not BMP!\n");
    return -2;
  }

  (*img_info)->size   = pIH.biWidth * pIH.biHeight;
  (*img_info)->width  = pIH.biWidth;
  (*img_info)->height = pIH.biHeight;

  *img = (uchar *)malloc(pIH.biWidth * pIH.biHeight * sizeof(uchar));

  // ファイルポインタを画像データ位置まで移動
  fseek(fp, pFH.bfOffBits, SEEK_SET);

  // 画像データを逆さに読み込み
  uchar bufLine[pIH.biWidth];
  for(int y = 0; y < pIH.biHeight; y++){
    fread(bufLine, 1, pIH.biWidth, fp);
    for(int x = 0; x < pIH.biWidth; x++){
      (*img)[(pIH.biHeight-y-1)*pIH.biWidth + x] = bufLine[x];
    }
  }

  fclose(fp);

  return 0;
}

#ifdef __MSDOS__
int read_bin(char* file_name, float __far **array, long size){
  int handle = 0;
  uint err_code = 0;
  
  handle = dos_fopen(file_name);
  if(handle < 0){
    printf("Failed to open file : %04X\n", (handle * -1));
    // return -1;
  }

  dos_fread((void __far *)*array, sizeof(float), size, handle);

  err_code = dos_fclose(handle);
  if(err_code != 0){
    printf("Failed to close file : %04X\n", (err_code * -1));
    return -1;
  }
  
  return 0;
}
#else
int read_bin(const char* file_name, float **array, int size){
    FILE * fp = NULL;

    if((fp = fopen(file_name, "rb")) == NULL){
        printf("ファイルオープン失敗 %s\n", file_name);
        return -1;
    };

    fread(*array, sizeof(float), size, fp);
    fclose(fp);

    return 0;
}
#endif