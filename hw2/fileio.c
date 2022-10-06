#include<stdio.h>

int main(int argc, char *argv[])
{
    FILE *fp;
    int len = 0;

    printf("argv[0] = %s\n", argv[1]);
    for(int i = 0; argv[1][i] != '\0'; i++)
        len++;
    printf("argv[0] len = %d\n", len);

    fp = fopen("test.txt", "w");

    if(fp == NULL){
        printf("파일열기 실패\n");
    } 
    else{
        printf("파일열기 성공\n");
    }
  
    fclose(fp);

    return 0;
}