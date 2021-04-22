#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <string.h>
#include <openssl/md5.h>
#include <time.h>

unsigned int cs10_bhash(char *data, int len, int buckets){
    unsigned int sum,j,rval;

    if ((0==data)|| (0==len)){
        printf("illegal input\n");
        exit(-1);
    }

    sum=data[0];

    for (j=1; j<len; j++){
        sum = 37*sum + ((unsigned char)(data[j]));
    }

    rval = sum % buckets;

    return rval;
}

int main(){

    FILE *fp;
    int count = 0;
    char filename[20];
    char c;
    ssize_t read;
    size_t len;

    fp=fopen("/usr/share/dict/words", "r");

    if(fp == NULL){
        fprintf(stderr, "invalid filename\n");
    }

    for(c = getc(fp); c!=EOF; c=getc(fp)){
        if (c == '\n'){
            count ++;
        }
    }
    
    //printf("count is %d\n", count);



    char *data = malloc(sizeof(char)*128);
    unsigned int rval;

    rewind(fp);
    
    clock_t begin = clock();
    while((read=getline(&data, &len, fp))!=-1){
        rval = cs10_bhash(data, sizeof(data), count);
    }    
    clock_t end = clock();
    
    double times = (double) (end - begin)/CLOCKS_PER_SEC;
    printf("time spent hashing is :%lf\n", times);

    return 0;
}
