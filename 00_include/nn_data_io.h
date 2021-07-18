#ifndef _NN_DATA_IO_H_
#define _NN_DATA_IO_H_

#pragma pack (push, 1)   //パディングを１byteに変更

#ifdef __MSDOS__
struct tagBITMAPFILEHEADER {
    unsigned short bfType;           /* ファイルタイプ */
    unsigned long  bfSize;           /* ファイルサイズ */
    unsigned short bfReserved1;      /* 予約領域１ */
    unsigned short bfReserved2;      /* 予約領域２ */
    unsigned long  bfOffBits;        /* 先頭から画像データまでのオフセット */
};

struct tagBITMAPINFOHEADER {
    unsigned long  biSize;           /* ヘッダサイズ */
    long           biWidth;          /* 画像の幅(px) */
    long           biHeight;         /* 画像の高さ(px) */
    unsigned short biPlanes;         /* プレーン数 常に１ */
    unsigned short biBitCount;       /* １画素あたりのデータサイズ */
    unsigned long  biCompression;    /* 圧縮形式 */
    unsigned long  biSizeImage;      /* 画像データ部のサイズ */
    long           biXPixPerMeter;   /* 横方向解像度(dot/m) */
    long           biYPixPerMeter;   /* 縦方向解像度(dot/m) */
    unsigned long  biClrUsed;        /* 格納されているパレット数(使用色数) */
    unsigned long  biClrImportant;   /* 重要なパレットのインデックス */
};
#else
struct tagBITMAPFILEHEADER {
    unsigned short bfType;           /* ファイルタイプ */
    unsigned int   bfSize;           /* ファイルサイズ */
    unsigned short bfReserved1;      /* 予約領域１ */
    unsigned short bfReserved2;      /* 予約領域２ */
    unsigned int   bfOffBits;        /* 先頭から画像データまでのオフセット */
};

struct tagBITMAPINFOHEADER {
    unsigned int   biSize;           /* ヘッダサイズ */
    int            biWidth;          /* 画像の幅(px) */
    int            biHeight;         /* 画像の高さ(px) */
    unsigned short biPlanes;         /* プレーン数 常に１ */
    unsigned short biBitCount;       /* １画素あたりのデータサイズ */
    unsigned int   biCompression;    /* 圧縮形式 */
    unsigned int   biSizeImage;      /* 画像データ部のサイズ */
    int            biXPixPerMeter;   /* 横方向解像度(dot/m) */
    int            biYPixPerMeter;   /* 縦方向解像度(dot/m) */
    unsigned int   biClrUsed;        /* 格納されているパレット数(使用色数) */
    unsigned int   biClrImportant;   /* 重要なパレットのインデックス */
};
#endif

struct tagRGBQUAD {
    unsigned char rgbBlue;            /* 青０～２５５ */
    unsigned char rgbGreen;           /* 緑０～２５５ */
    unsigned char rgbRed;             /* 赤０～２５５ */
    unsigned char rgbReserved;        /* 予約領域０ */
};
#pragma pack (pop)

struct tagIMG_INFO {
    unsigned int size;
    unsigned int width;
    unsigned int height;
};

typedef struct tagBITMAPFILEHEADER BITMAPFILEHEADER;
typedef struct tagBITMAPINFOHEADER BITMAPINFOHEADER;
typedef struct tagRGBQUAD RGBQUAD;
typedef struct tagIMG_INFO IMG_INFO;


int read_mnist_bmp(const char *file_path, unsigned char **img, IMG_INFO **img_info);

#ifdef __MSDOS__
int read_bin(char* file_name, float __far **array, long size);
#else
int read_bin(const char* file_name, float **array, int size);
#endif

#endif