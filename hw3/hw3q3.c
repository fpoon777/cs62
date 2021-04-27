#include <stdio.h>
#include <openssl/rsa.h>
#include <openssl/pem.h>
#include <openssl/bn.h>

int examine_key() {

        RSA *r = RSA_new();
        FILE  *pemfile;

        pemfile = fopen("./key.pem", "r+");
        r = PEM_read_RSAPrivateKey(pemfile,NULL, NULL, NULL);

        if(r == NULL){
            fprintf(stderr, "r is NULL\n");
        }
        
        const BIGNUM* n = NULL;
        const BIGNUM* e = NULL;
        const BIGNUM* d = NULL;

        RSA_get0_key(r,&n,&e,&d);

        const BIGNUM* p = NULL;
        const BIGNUM* q = NULL;

        RSA_get0_factors(r,&p,&q);
        
        if(n == NULL){
            fprintf(stderr, "n is empty\n");
        }
        char* nStr = BN_bn2hex(n);
        printf("n is: %s\n", nStr);

        char* eStr = BN_bn2hex(e);
        printf("e is: %s\n", eStr);

        char* dStr = BN_bn2hex(d);
        printf("d is: %s\n", dStr);
        char* pStr = BN_bn2hex(p);
        printf("p is: %s\n", pStr);
        char* qStr = BN_bn2hex(q);
        printf("q is: %s\n", qStr);

        fclose(pemfile);


        /*******************Q4***********************/
        BIGNUM* res = BN_new();    //the result of pq goes here

        BN_CTX *c;
        c = BN_CTX_new();


        //pq
        BN_mul(res, p, q, c);
        printf("n is: %s\n", nStr);

        char* pqStr = BN_bn2hex(res);
        printf("pq is: %s\n", pqStr);



        if(BN_cmp(res,n)==0){
            printf("n=pq\n");
        }
        else{
            printf("n not equal to pq\n");
        }

        BIGNUM* phi = BN_new();
        BIGNUM* p_1 = BN_new();
        BIGNUM* q_1 = BN_new();
        
        BIGNUM* one=BN_new();
        //set one as 1
        BN_one(one);

        BN_sub(p_1, p, one);
        BN_sub(q_1,q,one);
        
        BN_mul(phi, p_1,q_1,c);

        BIGNUM* de = BN_new();
        BN_mul(de,d,e,c);

        BN_mod(res,de,phi,c);
        if(BN_cmp(res,one)==0){
            printf("ed = 1 mod (p-1)(q-1)\n");
        }
        else{
            printf("ed doesn't equal to 1 mod (p-1)(q-1)\n");
        }

        //BN_SUB


        BN_CTX_free(c); //free the pointer
        


        return 0;
        //https://www.openssl.org/docs/man1.1.1/man3/RSA_get0_key.html                                                                
        //https://www.openssl.org/docs/man1.0.2/man3/BN_bn2bin.html
}

int main(){
    
    examine_key();

    return 0;
}
