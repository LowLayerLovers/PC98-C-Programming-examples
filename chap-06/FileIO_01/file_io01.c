#include <stdio.h>

long long int getFileSize(const char* fileName){
    FILE* fp = fopen(fileName, "rb");
    if (fp == NULL) {
        return -1LL;
    }

    if (fseek(fp, 0L, SEEK_END) == 0) {
        fpos_t pos;

        if (fgetpos(fp, &pos) == 0) {
            fclose(fp);
            return (long long int)pos;
        }
    }

    fclose(fp);
    return -1LL;
}


int main(void) {
  FILE *fp; // FILE型構造体
  long long int file_size;
  char fname[] = "hello.txt";

  file_size = getFileSize(fname);
  printf("file_size=%lld\n", file_size);
 
  fp = fopen(fname, "r"); // ファイルを開く。失敗するとNULLを返す。
  if(fp == NULL) {
    printf("%s file not open!\n", fname);
    return -1;
  }

  printf("===== TEXT is =====\n");
  char ch;
  while( ( ch = fgetc(fp) ) != EOF ) {
    printf("%c", ch);
  }
 
  fclose(fp); // ファイルを閉じる

  return 0;
}
