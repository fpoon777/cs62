#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <string.h>
#include <openssl/md5.h>
#include <time.h>

int md5bhash(char *data, int len, int buckets){
    unsigned char hval[128];
    unsigned int *ip = (unsigned int*) &hval[0], rval;

    MD5(data, len, &hval[0]);

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
        rval = md5bhash(data, sizeof(data), count);
    }    
    clock_t end = clock();

    double times = (double) (end -begin)/CLOCKS_PER_SEC;

    printf("time spent running hashing is: %lf\n",times);

    return 0;
}
