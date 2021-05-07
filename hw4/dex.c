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
   printf("needs arguments: pubfile hashfile sigfile hashfile2 sigfile2\n");
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
/*****************************************************/

FILE  *hashfile_2, *sigfile_2;
 DSA_SIG *dsig_2;
 const BIGNUM *r_2, *s_2;
 BIGNUM *m_2 = BN_new();
 

 unsigned char hashbuf_2[BLEN];
 unsigned char sigbuf_2[BLEN];
 int hashlen_2, siglen_2;
 int rc_2;
 const unsigned char *cp_2,**cpp_2;

 hashfile_2 = fopen(argv[4],"r");
 sigfile_2 = fopen(argv[5],"r"); 

 hashlen_2 = fread(hashbuf_2,1,BLEN,hashfile_2);
 siglen_2 = fread(sigbuf_2,1,BLEN,sigfile_2);

 if (hashlen_2 != 20)
   printf("warning: hash_2 is not a sha-1 hash?\n");
 
 BN_bin2bn(hashbuf_2, hashlen_2, m_2);
 printf("hash_2 as a bignum  %s\n",BN_bn2hex(m_2));

 cp_2 = &sigbuf_2[0];
 cpp_2 = &cp_2;
 
 dsig_2 = d2i_DSA_SIG(NULL,cpp_2,siglen_2);

 if (NULL == dsig_2) {
   printf("warning: signature_2 file was not properly formatted\n");
   return -1;
 }
 
 DSA_SIG_get0(dsig_2,&r_2,&s_2);
 printf("r_2 %s\n",BN_bn2hex(r_2));
 printf("s_2 %s\n",BN_bn2hex(s_2));
/*****************************************************/

  BIGNUM  *sasb = BN_new();     //SA-SB
  BIGNUM  *hahb = BN_new();     //HA-HB
  BIGNUM  *k = BN_new();
  BIGNUM  *sasbInv = BN_new();
  BN_CTX *ctx = BN_CTX_new();

  BN_mod_sub(sasb, s, s_2, q, ctx);
  BN_mod_sub(hahb, m, m_2,q,ctx);

  //BN_sub(sasb, s, s_2);
  //BN_sub(hahb,m,m_2);
  BN_mod_inverse(sasbInv, sasb, q, ctx);

  BN_mod_mul(k, hahb,sasbInv,q,ctx);
    
  BIGNUM  *sk = BN_new();
  BIGNUM  *sk_h = BN_new();
  BIGNUM  *rInv = BN_new();
  BN_mod_mul(sk, s, k,q,ctx);
  BN_mod_sub(sk_h, sk, m,q,ctx);
  BN_mod_inverse(rInv, r, q, ctx);
   
 
 {
   DSA *d2 = DSA_new();
   BIGNUM *a= BN_new();
   BIGNUM *p2,*q2,*g2,*y2;
   FILE *privfile;

   //get a
   //BN_div(a, NULL, hahb, sasb, ctx);
   BN_mod_mul(a, sk_h,rInv,q,ctx);
   //BN_mul(a, hahb, sasbInv, ctx);
   
   //BN_one(a);  // the guess
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



