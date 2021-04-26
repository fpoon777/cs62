#include <stdio.h>
#include <openssl/rsa.h>
#include <openssl/pem.h>

int examine_key() {

        RSA *r = NULL;
        FILE  *pemfile;

        pemfile = fopen("key.pem", "r+");
        r = PEM_read_RSAPrivateKey(pemfile,NULL, NULL, NULL);

        //https://www.openssl.org/docs/man1.1.1/man3/RSA_get0_key.html                                                                
        //https://www.openssl.org/docs/man1.0.2/man3/BN_bn2bin.html
}