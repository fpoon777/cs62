
// sws, CS62/162, Dartmouth

#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <string.h>

#define MAXPRIMECOUNT 4000
#define MAXPRIME 32000
#define MAXMODBITS 30
// so MAXPRIME * MAXPRIME fits

long int table[MAXPRIMECOUNT];



int primelist(void) {

  int i, j, c, count;

  FILE *file;

  file = fopen ("primelist.txt", "r");

  for (count = 0; count < MAXPRIMECOUNT; count++) {
    if (1 != fscanf(file,"%ld",&table[count]))
      break;
    if (table[count] >= MAXPRIME)
      break;
  }

  printf("read %d primes \n",count);

  return count;

}


// straight from HAC
long int eea(long int phi, long int e) {
  long int q,r,a,b,x,y,x1,x2,y1,y2,d;

  a = phi;
  b = e;

  if (0==b) {
    return 0;
  }

  x2 = 1;
  x1 = 0;
  y2 = 0;
  y1 = 1;

  while (b > 0) {
    q = a / b;
    r = a - q*b;
    x = x2 - q*x1;
    y = y2 - q*y1;
    a = b;
    b = r;
    x2 = x1;
    x1 = x;
    y2 = y1;
    y1 = y;
  }

  return y2;

}




void bprint(char *str, long int x) {

  int i;
  long int val;

  val = x;

  printf("%s = ", str);

  for (i = MAXMODBITS-1; i >=0; i--) {
    
    if (val >= (1 << i)) {
      printf("1 ");
      val = val - (1<<i);
    }
    else printf("0 ");
  }

  printf("\n");
  
}


long int modexp(long int x, long int y, long int n) {

  int i;
  long int val;
  long int ret = 1;

  val = y;

  for (i = MAXMODBITS-1; i >=0; i--) {

    ret = (ret * ret) % n;
    
    if (val >= (1 << i)) {
      // bit i is 1
      ret = (ret * x) % n;
      val = val - (1<<i);
    }

  }

  return ret;
  
}

// long int encrypt(long int P, long int y, long int g, long int k, long int p){
//   long int c1, Pp, ykp, c2;
//   c1 = modexp(g,k,p);
//   //ab mod m = (a mod m)(b mod m) mod m
//     //Py^k mod p = (P mod p)(y^k mod p) mod p
//   Pp = P % p;
//   ykp=modexp(y,k,p);
//   c2 = (Pp*ykp)%p;
// }

long int decrypt(long int c1, long int c2, long int x, long int p){
  //first part = c1^(p-x-1) mod p, secondpart=c2 mod p.
  long int firstPart, secondPart, am;
  firstPart = modexp(c1,p-x-1,p);
  secondPart = c2 % p;
  am = (firstPart * secondPart) % p; 
  return am;
}


int main(void) {

  long int p, g, k, x, y, P, am, Pp, ykp,c1, c2;
  long int ykpD, firstPart, secondPart;

  g = 558927443;    //generated from openssl dhparam command
  p = 801482723;

  printf("p = %ld, g = %ld\n", p,g);

  printf("enter a private key x:\n");
  scanf("%ld", &x);

  y = modexp(g,x,p);
  printf("y= %ld\n", y);

  while(1) {

    printf("Enter message: \n");
    scanf("%ld",&P);

    printf("enter random number k:\n");
    scanf("%ld", &k);
    
    c1 = modexp(g,k,p);
    
    //ab mod m = (a mod m)(b mod m) mod m
    //Py^k mod p = (P mod p)(y^k mod p) mod p
    Pp = P % p;
    ykp=modexp(y,k,p);
    c2 = (Pp*ykp)%p;
    printf("c1 = g^k mod p = %ld\n",c1);
    printf("c2 = Py^k mod p = %ld\n",c2);
    
    //decrypt m=c1^(p-x-1)c2 mod p
    ////first part = c1^(p-x-1) mod p, secondpart=c2 mod p.
    // ykpD = modexp(g, x*k, p);
    // printf("original y^k mod p is %ld, decrypted y^k mod p is %ld\n", ykp, ykpD);
    
    // firstPart = modexp(c1,p-x-1,p);
    // secondPart = c2 % p;
    am = decrypt(c1,c2,x,p); 

    printf("decrypted text is %ld\n", am);
  }

  printf("-------------------------------------\n");

  //part 2, mental poker
  long int cards[16] = {0x82, 0xaa, 0xa3, 0xee, 0x33, 0xd4, 0x7a, 0x4c, 0x72, 0xbb, 0xb8, 0x32, 0x61, 0xe2, 0x2f,0x4b};
  long int keyAlice = 145;
  long int keyBob = 77;
  long int randAlice = 44;
  long int randBob = 123;
  long int c1Alice, c2Alice, c1Bob, c2Bob;
  long int yAlice, yBob;

  printf("Alice's key is %ld, and Bob's key is %ld\n", keyAlice, keyBob);

  //calculate y for Alice and bob
  yAlice = modexp(g,keyAlice,p);
  yBob = modexp(g,keyBob,p);

  //choose g^k mod p for alice and bob;
  c1Alice = modexp(g,randAlice,p);
  c1Bob = modexp(g,randBob,p);


  long int encryptedBothc2[16];
  long int yakp, ybkp;    //y alice ^ x, and y bob ^x
  long int encryptedBoth;
  for (int i = 0; i < 16; i++)
  {
    //Alice encrypts everything with her key
    Pp = P % p;
    yakp=modexp(yAlice,randAlice,p);
    c2Alice = (Pp*yakp)%p;

    //Bob encrypts everything with his key
    //ab mod m = (a mod m)(b mod m) mod m
    //(PyA^k)yB^kmod m = ((PyA^k) mod p)(yB^k mod p) mod p
    ybkp=modexp(yBob,randBob,p);
    c2Alice = (Pp*ykp)%p;

    encryptedBoth = (c2Alice * ybkp) % p;
    encryptedBothc2[i] = encryptedBoth;
  }

  //Now Alice chooses one for Bob
  int alicePick = 6;
  long int aliceDecrypted;
  long int bobRes;
  //Alice decrypt for Bob
  aliceDecrypted = decrypt(c1Alice,encryptedBoth[alicePick],keyAlice,p); 

  //Bob receives the result and decrypt it using his key.
  bobRes = decrypt(c1Bob,aliceDecrypted,keyBob, p);


  //Now Bob chooses one for Alice
  int bobPick = 13;
  long int bobDecrypted;
  long int aliceRes;
  //Bob decrypt for Alice
  bobDecrypted = decrypt(c1Bob,encryptedBoth[bobPick],keyBob,p); 

  //Alice receives the result and decrypt it using his key.
  aliceRes = decrypt(c1Alice,bobDecrypted,keyAlice, p);

  printf("Alice chose %ld, and Bob chose %ld\n", aliceRes, bobRes);
}
