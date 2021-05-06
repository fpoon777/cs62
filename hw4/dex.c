#include <stdio.h>
#include <openssl/dsa.h>
#include <openssl/pem.h>


#define BLEN 100



int main(int argc, char *argv[]) {

 FILE  *pubfile,  *hashfile, *sigfile;
 const BIGNUM *p,*q,*g,*y;
 DSA *d;
 DSA_SIG *dsig;
 const BIGNUM *r, *s;
 BIGNUM *m = BN_new();
 

 unsigned char hashbuf[BLEN];
 unsigned char sigbuf[BLEN];
 int hashlen, siglen;
 int rc;
 const unsigned char *cp,**cpp;

 if( argc < 4) {
   printf("needs arguments: pubfile hashfile sigfile\n");
   return -1;
 }

 pubfile = fopen(argv[1], "r");
 
 d = PEM_read_DSA_PUBKEY(pubfile,NULL,NULL,NULL);
 DSA_get0_pqg(d, &p, &q, &g);
 DSA_get0_key(d,&y,NULL);
 
 printf("p %s\n",BN_bn2hex(p));
 printf("q %s\n",BN_bn2hex(q));
 printf("g %s\n",BN_bn2hex(g));
 printf("y %s\n",BN_bn2hex(y)); 


 hashfile = fopen(argv[2],"r");
 sigfile = fopen(argv[3],"r"); 

 hashlen = fread(hashbuf,1,BLEN,hashfile);
 siglen = fread(sigbuf,1,BLEN,sigfile);

 if (hashlen != 20)
   printf("warning: hash is not a sha-1 hash?\n");
 
 BN_bin2bn(hashbuf, hashlen, m);
 printf("hash as a bignum  %s\n",BN_bn2hex(m));

 cp = &sigbuf[0];
 cpp = &cp;
 
 dsig = d2i_DSA_SIG(NULL,cpp,siglen);

 if (NULL == dsig) {



   printf("warning: signature file was not properly formatted\n");
   return -1;
 }
 
 DSA_SIG_get0(dsig,&r,&s);
 printf("r %s\n",BN_bn2hex(r));
 printf("s %s\n",BN_bn2hex(s));
 

 // example code to create and save new keypair
 // that's just like this public one...
 // except with a guessed private key

 
 {
   DSA *d2 = DSA_new();
   BIGNUM *a= BN_new();
   BIGNUM *p2,*q2,*g2,*y2;
   FILE *privfile;

   
   BN_one(a);  // the guess
   p2 = BN_dup(p);
   q2 = BN_dup(q);
   g2 = BN_dup(g);
   y2 = BN_dup(y);

   DSA_set0_pqg(d2,p2,q2,g2);
   DSA_set0_key(d2,y2,a);

   privfile = fopen("fake_private.pem","w+");
   PEM_write_DSAPrivateKey(privfile,d2,NULL,NULL,0,NULL,NULL);

 }
 
}



