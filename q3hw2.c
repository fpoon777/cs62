
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


int main(void) {

  int primecount;
  long int p, q, n, phi, e, d, m, c,am;
  int i,j;

  primecount = primelist();

  printf("Enter indices for two primes:\n");
  scanf("%d %d", &i, &j);

  if ( (i==j) || (i >= primecount) || (j >= primecount)) {
    printf("out of range\n");
    exit(-1);
  }

  p = table[i];
  q = table[j];

  printf("p = %ld, q = %ld\n", p,q);
  n = p*q;
  phi = (p-1)*(q-1);
  printf("n = %ld, phi = %ld\n", n,phi);
  

  // find e
  e = 0;
  for (i = 0; i < phi; i++) {
    if (phi % table[i]) {
      e = table[i];
      break;
    }
  }
  if (e==0) {
    printf("didn't find an e!\n");
    exit(-1);
  }

  printf("e = %ld\n", e);

  d = eea(phi,e);
  while (d < 0) d = d +phi;

  printf("d = %ld\n", d);

  bprint("e",e);
  bprint("d",d);
  bprint("n",n);
  

  while(1) {

    printf("Enter message: \n");
    scanf("%ld",&m);

    if ((m < 0)| (m >= n) ) {
      printf("out of range\n");
      exit(-1);
    }

    c = modexp(m,e,n);
    printf("c = m^e mod n = %ld\n", c);
    am = modexp(c,d,n);
    printf("c^d mod n = %ld\n", am);
  }


}
