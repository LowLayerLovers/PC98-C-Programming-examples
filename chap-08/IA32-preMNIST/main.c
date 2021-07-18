#include "../../00_include/exmath.h"
#include "../../00_include/nn_data_io.h"
#include <math.h>
#include <stdio.h>
#include <stdlib.h>

#define  WB_DIR "../01_CNN_WB"
#define IMG_DIR "../02_img_data"

int argmax(float *array, int size){
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

int main(void){
    unsigned char *img;
    IMG_INFO *img_info;
    float *W1, *W2, *W3, *b1, *b2, *b3, *dst1, *dst2, *dst3, *dst4, *input;
    
    W1 = (float *)malloc(  30 * (5*5) * sizeof(float));
    W2 = (float *)malloc(4320 *   100 * sizeof(float));
    W3 = (float *)malloc( 100 *    10 * sizeof(float));
    b1 = (float *)malloc( 30 * sizeof(float));
    b2 = (float *)malloc(100 * sizeof(float));
    b3 = (float *)malloc( 10 * sizeof(float));
    input=(float *)malloc( 30 * 28 * 28 * sizeof(float));
    dst1=(float *)malloc( 30 * 24 * 24 * sizeof(float));
    dst2=(float *)malloc( 30 * 12 * 12 * sizeof(float));
    dst3=(float *)malloc( 100 * sizeof(float));
    dst4=(float *)malloc( 10 * sizeof(float));
    
    read_bin(WB_DIR"/W1.bin", &W1, 30*(5*5));
    read_bin(WB_DIR"/W2.bin", &W2, 4320*100);
    read_bin(WB_DIR"/W3.bin", &W3, 100*10);
    read_bin(WB_DIR"/b1.bin", &b1, 30);
    read_bin(WB_DIR"/b2.bin", &b2, 100);
    read_bin(WB_DIR"/b3.bin", &b3, 10);

    // 入力があっているか不安な場合，
    // 配列をダンプした[.txt]とWeightBiasフォルダ内の[.txt]のdiffを取ればよい
    // dump_matrix(W1, 30, (5*5));

    puts("===== Read MNIST image =====");
    read_mnist_bmp(IMG_DIR"/8.bmp", &img, &img_info);
    printf("Size  : %d px\n", img_info->size);
    printf("Width : %d px\n", img_info->width);
    printf("Height: %d px\n", img_info->height);

    // MNISTimg配列のCUI表示
    for(int y = 0; y < img_info->height; y++){
        for(int x = 0; x < img_info->width; x++){
            printf("%2X ", img[y * img_info->width + x]);
        }
        puts("");
    }
    /* for(int n=0;n<5;n++){ */
    /*   for(int m=0;m<5;m++){ */
    /*     printf("%f,",W1[n*5+m]); */
    /*   } */
    /* } */
    /* printf("\n"); */
    
    int kh=5;
    int kw=5;
    int in_h=28;
    int in_w=28;
    int out_ch=30;
    int padx=0;
    int pady=0;
    int out_h=in_h-((kh/2)-padx)*2;
    int out_w=in_w-((kw/2)-padx)*2;
    for(int i=0;i<28*28;i++){
      input[i]=img[i]/255.0;
    }

    //use exmath
    /* for(int och=0;och<30;och++){ */
    /*   conv_mat_f32(input,28,28,W1+och*25,5,5,0,1,dst1+och*24*24,24,24); */
    /* } */
    
    // use my convolution
    for(int och=0;och<out_ch;och++){
      for(int i=0;i<out_h;i++){
        for(int j=0;j<out_w;j++){
          float tmp=0;
          for(int ki=0;ki<kh;ki++){
            for(int kj=0;kj<kw;kj++){
              int inidx=(i-pady+ki)*in_w+(j-padx+kj);
              int widx=och*kh*kw+ki*kw+kj;
              //if(i-pady>=0 && j-padx>=0){
            tmp+=input[inidx]*W1[widx];
            //}
            }
          }
          tmp+=b1[och]; //add bias
          tmp=fmax(tmp,0); // relu
          dst1[och*out_h*out_w+i*out_w+j]=tmp;
        }
      }
    }


    /* float tmp=0; */
    /* for(int j=5;j<5+5;j++){ */
    /*   for(int i=0;i<5;i++){ */
    /*     tmp+=input[i*in_w+j]*W1[i*kw+(j-5)]; */
    /*     printf("%f,",input[i*in_w+j]*W1[i*kw*(j-5)]); */
    /*   } */
    /* } */
    /* printf("sum:%f\n",tmp+b1[0]); */

    //pool
    int ph=2;
    int pw=2;
    in_h=24;
    in_w=24;
    int in_ch=30;
    int stride=2;
    out_w=in_h/stride;
    out_h=in_w/stride;

    //use exmath.h
    /* for(int ich=0;ich<30;ich++){ */
    /*   pool_max_mat_f32(dst1+ich*in_h*in_w,24,24,2,2,2,dst2+ich*12*12,12,12); */
    /* } */

    //use my pooling
    for(int ich=0;ich<in_ch;ich++){
      for(int i=0;i<out_h;i++){
        for(int j=0;j<out_w;j++){

          float tmp=-1000;
          for(int pi=0;pi<ph;pi++){
            for(int pj=0;pj<pw;pj++){
              int inidx=ich*in_h*in_w+(i*stride+pi)*in_w+(j*stride+pj);
              tmp=fmax(dst1[inidx],tmp);
            }
          }
          dst2[ich*out_w*out_h+i*out_w+j]=tmp;
        }
      }
    }


    //affine
    int matw=4320;
    int math=100;
    for(int i=0;i<math;i++){
      float tmp=0;
      for(int j=0;j<matw;j++){
        tmp+=W2[j*math+i]*dst2[j];
      }
      dst3[i]=tmp+b2[i];
      dst3[i]=fmax(dst3[i],0); //relu
    }

    
    /* for(int i=0;i<10;i++){ */
    /*   printf("%f,",dst3[i]); */
    /* } */
    /* printf("\n");     */


    //affine
    matw=100;
    math=10;
    for(int i=0;i<math;i++){
      float tmp=0;
      for(int j=0;j<matw;j++){
        tmp+=W3[j*math+i]*dst3[j];
      }
      dst4[i]=tmp+b3[i];
      //printf("%f,",dst4[i]);
    }

    softmax_vec_f32(dst4,10);
    for(int i=0;i<10;i++){
      printf("%d,%f\n", i, dst4[i]);
    }

    puts("====================");
    puts("Classification:");
    printf("Number is %d\n", argmax(dst4, 10));
    puts("====================");

    return 0;
}
