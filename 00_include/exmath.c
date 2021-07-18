#include "exmath.h"
#include <math.h>
#include <stdlib.h>
#include <string.h>

#include <stdio.h>

/* Basic Function */
int copy_vec_f32(float* vec_in, float* vec_dst, int vec_len){
    memcpy(vec_dst, vec_in, sizeof(float) * vec_len);
    return 0;
}

//not used
int zeros_vec_f32(float* vec_in, int vec_len){
    memset(vec_in, 0, sizeof(float) * vec_len);
    return 0;
}

int mask_vec_f32(float* vec_in, int vec_len,char* vec_mask,float mask_value){
    for(int i = 0; i < vec_len; i++){
        vec_in[i] = vec_mask[i] ? mask_value : vec_in[i];
    }
}

//not used
int transpose_mat_f32(float* mat_in, float* mat_out, int mat_in_row_len, int mat_in_column_len){
    for(int i = 0; i < mat_in_row_len; i++){
        for(int j = 0; j < mat_in_column_len; j++){
            mat_out[j* mat_in_row_len +i] = mat_in[i * mat_in_column_len + j];
        }
    }
}


//not used
int transpose_square_mat_f32(float* mat_in, float* mat_out, int mat_in_len){
    float tmp;
    for(int i = 0; i < mat_in_len + 1 >> 1; i++){
        for(int j = i + 1; j < mat_in_len; j++){
            tmp = mat_in[j* mat_in_len + i];
            mat_out[j* mat_in_len +i ] = mat_in[i * mat_in_len + j];
            mat_out[i * mat_in_len + j] = tmp;
        }
    }
}

//from c99, math.h has same function as fmax()
float max_f32(float a, float b){
    return a > b ? a : b;
}

int max_vec_i32(int* vec_in, int vec_len) {
    int max = vec_in[0];
    for(int i = 1; i < vec_len; i++){
        if (vec_in[i] > max){
            max = vec_in[i];
        }
    }
    return max;
}

int max_vec_f32(float* vec_in, int vec_len) {
    float max = vec_in[0];
    for(int i = 1; i < vec_len; i++){
        if (vec_in[i] > max){
            max = vec_in[i];
        }
    }
    return max;
}

float sum_vec_f32(float* vec_in, int vec_len) {
    float sum = 0;
    for(int i = 0; i < vec_len; i++){
        sum += vec_in[i];
    }
    return sum;
}

int div_vec_scalar_f32(float* vec_a, float scalar_b, float* vec_out, int vec_len){
    for(int i = 0; i < vec_len; i++){
        vec_out[i] = vec_a[i] / scalar_b;
    }
    return 0;
}


int add_vec_f32(float* vec_a, float* vec_b, float* vec_out, int vec_len){
    for(int i = 0; i < vec_len; i++){
        vec_out[i] = vec_a[i] + vec_b[i];
    }
    return 0;
}

int sub_vec_f32(float* vec_a, float* vec_b, float* vec_out, int vec_len){
    for(int i = 0; i < vec_len; i++){
        vec_out[i] = vec_a[i] - vec_b[i];
    }
    return 0;
}

int mul_vec_f32(float* vec_a, float* vec_b, float* vec_out, int vec_len){
    for(int i = 0; i < vec_len; i++){
        vec_out[i] = vec_a[i] * vec_b[i];
    }
    return 0;
}

float sum_mul_vec_f32(float* vec_a, float* vec_b, int vec_len){
    float sum = 0.0f;
    for(int i = 0; i < vec_len; i++){
        sum += vec_a[i] * vec_b[i];
    }
    return sum;
}

int mul_mat_f32(float* mat_a ,int mat_a_row_len, int mat_a_column_len ,float* mat_b, int mat_b_row_len, int mat_b_column_len, float* mat_out){
    //mat_a_column_len == mat_b_row_len
    for(int i = 0; i < mat_a_row_len; i++){
        for(int k = 0; k < mat_a_column_len; k++){
            for(int j = 0; j < mat_b_column_len; j++){
                mat_out[i * mat_b_column_len + j] += mat_a[i * mat_a_column_len + k] * mat_b[k * mat_b_column_len + j];
            }
        }
    }
    return 0;
}

int calc_conv_out_len(int in_len, int filter_len, int stride, int padding){
    if (stride == 0) return -1;
    return (in_len + 2*padding - filter_len) / stride + 1;
}

int calc_pool_out_len(int in_len, int pool_len, int stride){
    if (stride == 0) return -1;
    return (in_len - pool_len) / stride + 1;
}

int conv_mat_f32(float* mat_in, int mat_in_row_len,int mat_in_column_len, float* mat_filter,int mat_filter_row_len, int mat_filter_column_len, int padding,int stride, float* mat_out, int mat_out_row_len, int mat_out_column_len){
    for(int i = 0; i < mat_out_row_len; i++){
        for(int k = i*stride < padding? padding - i*stride: 0,
         kmax = i*stride > mat_in_row_len - mat_filter_row_len + padding? mat_in_row_len - i*stride + padding: mat_filter_row_len;
         k < kmax; k++){
            for(int j = 0; j < mat_out_column_len; j++){
                for(int l = j*stride < padding? padding - j*stride : 0,
                 lmax = j*stride > mat_in_column_len - mat_filter_column_len + padding? mat_in_column_len - j*stride + padding: mat_filter_column_len ;
                 l < lmax; l++){
                    //if((i*stride+k-padding) < 0 || (i*stride+k-padding) >= mat_in_row_len || (j*stride+l - padding) < 0 || (j*stride+l - padding) >= mat_in_column_len) printf("o(%d,%d,%d,%d),",i,j,k,l);     
                    mat_out[i*mat_out_column_len+j] += mat_in[(i*stride+k-padding)*mat_in_column_len+(j*stride+l - padding)] * mat_filter[k*mat_filter_column_len + l];
                }
            }
        }
    }
}

int conv_mat_f32_memsafe(float* mat_in, int mat_in_row_len,int mat_in_column_len, float* mat_filter,int mat_filter_row_len, int mat_filter_column_len, int padding,int stride, float* mat_out, int mat_out_row_len, int mat_out_column_len){
    for(int i = 0; i < mat_out_row_len; i++){
        //printf("\n");
        if (i*stride + mat_filter_row_len - padding <= 0 || i*stride - padding >= mat_in_row_len) {
            //for lower and over i => will process only init 0! 
            for(int j = 0; j < mat_out_column_len; j++) {
                //printf("-");
                mat_out[i*mat_out_column_len+j] = 0.0f;
            }
            continue;
        }
        //for lower k
        int k_start =  i*stride < padding ? padding - i*stride : 0;
        for(int j = 0; j < mat_out_column_len; j++){
            
            // need lower j
            if (j*stride + mat_filter_row_len - padding <= 0 || j*stride - padding >= mat_in_row_len) {
                mat_out[i*mat_out_column_len+j] = 0.0f;
                continue;
            }
            int l_start = 0;
            int len = len = j*stride + mat_filter_column_len > mat_in_row_len + padding? mat_in_column_len + padding - j*stride : mat_filter_column_len;
            if(j*stride < padding){
                l_start = padding - j*stride;
                len -= l_start;
            }
            if(len <= 0) {
                //for over j => will process only init 0!
                do{
                    mat_out[i*mat_out_column_len+j] = 0.0f;
                    j++;
                } while(j < mat_out_column_len);
                break;
            }
            //printf("*");
            //printf("(%d~%d)",j*stride - padding + l_start,j*stride - padding + l_start+len);
            mat_out[i*mat_out_column_len+j] = sum_mul_vec_f32(&mat_in[(i*stride+k_start-padding)*mat_in_column_len+(j*stride - padding + l_start)], &mat_filter[k_start*mat_filter_column_len+l_start], len);
        }
        for(int k = k_start + 1 ,
         //for over k
         kmax = i*stride + mat_filter_row_len > mat_in_row_len + padding? mat_in_row_len + padding - i*stride: mat_filter_row_len;
         k < kmax; k++){
            for(int j = 0; j < mat_out_column_len; j++){
                // need lower j
                if (j*stride + mat_filter_row_len - padding <= 0 || j*stride - padding >= mat_in_row_len) {
                    mat_out[i*mat_out_column_len+j] = 0.0f;
                    continue;
                }
                int l_start = 0;
                int len = len = j*stride + mat_filter_column_len > mat_in_row_len + padding? mat_in_column_len + padding - j*stride : mat_filter_column_len;
                if(j*stride < padding){
                    l_start = padding - j*stride;
                    len -= l_start;
                }
                if(len <= 0) {
                    //for over j => will process only init 0!
                    do{
                        mat_out[i*mat_out_column_len+j] = 0.0f;
                        j++;
                    } while(j < mat_out_column_len);
                    break;
                }
                //printf("/");
                //printf("(%d~%d)",j*stride - padding + l_start,j*stride - padding + l_start+len);
                mat_out[i*mat_out_column_len+j] += sum_mul_vec_f32(&mat_in[(i*stride+k-padding)*mat_in_column_len+(j*stride - padding + l_start)], &mat_filter[k*mat_filter_column_len+l_start], len);
            }
        }
    }
    //printf("\n");
}

int pool_max_mat_f32(float* mat_in, int mat_in_row_len,int mat_in_column_len,int pool_size_row_len,int pool_size_column_len,int stride, float* mat_out, int mat_out_row_len, int mat_out_column_len){
    for(int i = 0; i < mat_out_row_len; i++){
        for(int j = 0; j < mat_out_column_len; j++){
            int len = j * stride + pool_size_column_len <= mat_in_column_len ? pool_size_column_len : mat_in_column_len - j * stride; 
            //if use calc_out_size => will not happen 0 < len !
            mat_out[i*mat_out_column_len+j] = max_vec_f32(&mat_in[(i*stride) * mat_in_column_len + (j*stride)], len);
        }
        for(int k = 1, kmax = i * stride + pool_size_row_len <= mat_in_row_len ? pool_size_row_len : mat_in_row_len - i * stride; k < kmax; k++){
            for(int j = 0; j < mat_out_column_len; j++){
                int len = j * stride + pool_size_column_len <= mat_in_column_len ? pool_size_column_len : mat_in_column_len - j * stride; 
                mat_out[i*mat_out_column_len+j] = max_f32(mat_out[i*mat_out_column_len+j] , max_vec_f32(&mat_in[(i*stride + k) * mat_in_column_len + (j*stride)], len));
            }
        }
    }
}




// vt_mat == mat, but use t_mat in mul_mat fuction.
int mul_mat_mat_vt_f32(float* mat_a ,int mat_a_row_len, int mat_a_column_len ,float* mat_vt_b, int mat_vt_b_row_len, int mat_vt_b_column_len, float* mat_out){
    //mat_a_column_len == mat_vt_b_column_len
    for(int i = 0; i < mat_a_row_len; i++){
        for(int j = 0; j < mat_vt_b_column_len; j++){
            for(int k = 0; k < mat_vt_b_row_len; k++){
                mat_out[i * mat_vt_b_row_len + j] += mat_a[i * mat_a_column_len + k] * mat_vt_b[j * mat_vt_b_row_len + k];
            }
        }
    }
    return 0;
}

int mul_mat_vt_mat_f32(float* mat_vt_a ,int mat_vt_a_row_len, int mat_vt_a_column_len ,float* mat_b, int mat_b_row_len, int mat_b_column_len, float* mat_out){
    //mat_vt_a_row_len == mat_b_row_len
    for(int k = 0; k < mat_vt_a_row_len; k++){
        for(int i = 0; i < mat_vt_a_column_len; i++){    
            for(int j = 0; j < mat_b_column_len; j++){    
                mat_out[i * mat_b_column_len + j] += mat_vt_a[k * mat_vt_a_row_len + i] * mat_b[k * mat_b_column_len + j];
            }
        }
    }
    return 0;
}



int exp_vec_f32(float* vec_in, float* vec_out, int vec_len, float offset){
    for(int i = 0; i < vec_len; i++){
        vec_out[i] = (float)exp((double)vec_in[i] - offset);
    }
    return 0;
}

int log_vec_f32(float* vec_in, float* vec_out, int vec_len, float offset){
    for(int i = 0; i < vec_len; i++){
        vec_out[i] = (float)log((double)vec_in[i] - offset);
    }
    return 0;
}


/* Activation Function */

float relu_f32(float x){
    return x > 0 ? x : 0;
}

char relu_with_mask_f32(float* x){
    if (x <= 0) {
        *x = 0;
        return 0x01;
    }else{
        return 0x00;
    }
}

int relu_vec_f32(float* vec_in, int vec_len){
    for(int i = 0; i < vec_len; i++){
        vec_in[i] = relu_f32(vec_in[i]);
    }
    return 0;
}

int relu_with_mask_vec_f32(float* vec_in, int vec_len, char* vec_mask){
    for(int i = 0; i < vec_len; i++){
        vec_mask[i] = relu_with_mask_f32(&vec_in[i]);
    }
    return 0;
}

float sigmoid_f32(float x, float gain){
    return 1.0f / (1.0f + (float)exp((double)-gain * x));
}

float std_sigmoid_f32(float x){
    return sigmoid_f32(x, 1);
}

int std_sigmoid_vec_f32(float* vec_in, int vec_len){
    for(int i = 0; i < vec_len; i++){
        vec_in[i] = std_sigmoid_f32(vec_in[i]);
    }
    return 0;
}

int softmax_vec_f32(float* vec_in, int vec_len){
    float max = max_vec_f32(vec_in, vec_len);
    exp_vec_f32(vec_in, vec_in ,vec_len , max);
    float sum = sum_vec_f32(vec_in, vec_len);
    div_vec_scalar_f32(vec_in, sum, vec_in, vec_len);
    return 0;
}

/* Loss [Error] Function */
//not used
float mean_squared_error_vec_f32(float* vec_correct, float* vec_predict , int vec_len) {
    float* cache = (float*)malloc(sizeof(float) * vec_len);
    sub_vec_f32(vec_predict , vec_correct, cache, vec_len);
    mul_vec_f32(cache,cache,cache,vec_len);
    return 0.5f * sum_vec_f32(cache, vec_len);
}

float cross_entropy_error_vec_f32(float* vec_correct, float* vec_predict , int vec_len) {
    float* cache = (float*)malloc(sizeof(float) * vec_len);
    log_vec_f32(vec_predict, cache, vec_len, -1e-7);
    mul_vec_f32(vec_correct,cache,cache,vec_len);
    return -sum_vec_f32(cache,vec_len);
}