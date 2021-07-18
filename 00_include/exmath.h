#ifndef _EXMATH_H_
#define _EXMATH_H_

int copy_vec_f32(float* , float* , int );
int zeros_vec_f32(float*, int);

int mask_vec_f32(float*, int,char* ,float);

float max_f32(float, float);

int max_vec_i32(int*, int);
int max_vec_f32(float* , int);
float sum_vec_f32(float*, int);

int div_vec_scalar_f32(float*, float, float* , int );

int add_vec_f32(float* , float* , float* , int);
int sub_vec_f32(float* , float* , float* , int);
int mul_vec_f32(float* , float* , float* , int);
int mul_mat_f32(float* ,int ,int ,float* ,int ,int ,float* );
int mul_mat_mat_vt_f32(float* ,int, int ,float*, int, int , float* );
int mul_mat_vt_mat_f32(float* ,int, int ,float*, int, int , float* );

int calc_pool_out_len(int, int, int);
int calc_conv_out_len(int, int, int, int);
int conv_mat_f32(float* , int ,int , float* ,int , int , int ,int , float* , int , int );



int log_vec_f32(float* , float* , int, float);

int relu_vec_f32(float*, int);
int relu_with_mask_vec_f32(float* , int, char*);

float sigmoid_f32(float, float);
float std_sigmoid_f32(float);
int std_sigmoid_vec_f32(float*, int);

int softmax_vec_f32(float*, int);

float cross_entropy_error_vec_f32(float* , float* , int );

#endif