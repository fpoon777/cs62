#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <stdbool.h>

unsigned long find_e(unsigned long phi){
    //find a small e number that's the coprime of phi
    unsigned long e = 0;
    unsigned long gcd = 0;

    for (unsigned long i = 2; i < 100; i++)
    {   
        for (unsigned long j = 1; j < 100; j++)
        {
            if (phi % j == 0 && i % j ==0)
            {
                gcd=j;
            }
        }

        if (gcd == 1)
        {
            e = i;
            break;
        }
    }
    
    if (e == 0)
    {
        fprintf(stdout, "no coprime e found\n");
        return -1;
    }
    else{
        return e;
    }
}

//extendedEuclidean distance 
//Referred the source: https://crypto.stackexchange.com/questions/12150/how-do-i-calculate-the-private-key-in-rsa
//Referred the source: https://en.wikipedia.org/wiki/Extended_Euclidean_algorithm#Recursive_method_2 Pseudocode section
long extendedEuclidean(unsigned long e, unsigned long phi, long* a, long* b){
    //ex1+mx2 = 1
    if (e==0){
        *a=0;
        *b=1;
        return phi;
    }
    else{
        long x, y;
        //run recursive calls
        long gcd;
        gcd=extendedEuclidean(phi % e, e, &x, &y);
        
        //update the coefficients of e and phi:
        //calculate floor of phi/e
        *a = y - (phi/e)*x;
        *b = x;
        return gcd;
    }

}

long find_d(unsigned long e, unsigned long phi){
    long a;
    long b;

    long gcd = extendedEuclidean(e, phi, &a, &b);
    if (gcd == 1)
    {
        long d = ((a % phi) + phi) % phi;
        return d;
    }
    else{
        return -1;
    }
}


void print_dec_bin(unsigned long decimalNum) {
    int bit;

    for (int i = 30; i >=0; i--)
    {   
        //shift i bits to the right
        bit = decimalNum >> i;
        //bitwise opeartor & to see if the bit is a 1
        if((bit & 1) == 1){
            fprintf(stdout, "1 ");
        }
        else{
            fprintf(stdout, "0 ");
        }
    }

    fprintf(stdout, "\n");
}

unsigned long modular_ex(unsigned long plaintext, unsigned long e, unsigned long n){
    bool inNumber=false;    //see if the first 1 is encountered
    int bit;
    int nextBit;
    unsigned long res;
    res = plaintext;

    for (int i = 30; i >=0; i--)
    {   
        //shift i bits to the right
        bit = e >> i;

        //identify the first binary 1
        if (inNumber == false && (bit&1)){
            inNumber = true;
        }

        if(inNumber==true){
            if (i>=1)
            {
                nextBit = bit >> 1;
                //when next bit is 1
                if((nextBit & 1) == 1){
                    unsigned long temp = (res * res) % n;
                    res = (temp * plaintext) % n;
                }
                //when next bit is 0
                else{
                    unsigned long temp = res*res;
                    res = temp % n;
                }
            }
        }
    }

    return res;
}

unsigned long modular_ex_v1(unsigned long plaintext, unsigned long e, unsigned long n){
    unsigned long res=1;
    for (int i = 0; i < e; i++)
    {
        res = (res*plaintext) % n;
    }
    return res;
}

int main() {

    unsigned long primeList[3433];
    FILE* fp;
    fp = fopen("./primelist.txt","r");
    if (fp==NULL)
    {
        fprintf(stderr,"invalid file location");
        exit(1);
    }

    int index=0;
    unsigned long num;
    while (fscanf(fp,"%lu",&num) > 0)
    {   
        primeList[index]=num;
        index ++;
    }

    fclose(fp);

    int p_index;
    int q_index;

    fprintf(stdout,"read 3432 primes\n");
    scanf("%d", &p_index);
    scanf("%d", &q_index);

    unsigned long p,q;
    p = primeList[p_index];
    q = primeList[q_index];

    //calculate n, phi
    unsigned long n = p*q;
    unsigned long phi = (p-1)*(q-1);

    fprintf(stdout,"p = %d, q = %d\n", ((unsigned int) p), (unsigned int) q);
    fprintf(stdout,"n = %lu, phi = %lu\n", n, phi);
    //find e:
    unsigned long e = find_e(phi);

    fprintf(stdout,"e = %d\n", ((unsigned int) e));

    //find d:
    long d = find_d(e, phi);
    // unsigned long d = find_d(3,11);
    fprintf(stdout,"d = %d\n", ((unsigned int) d)); 

    //print e, d, and n in binary
    fprintf(stdout,"e = ");    
    print_dec_bin(e);
    fprintf(stdout,"d = ");  
    print_dec_bin(d);
    fprintf(stdout,"n = ");  
    print_dec_bin(n);
    
    while(true){
        fprintf(stdout,"\n");
        //while loop for user to enter message
        unsigned long userInput;
        fprintf(stdout,"Enter message: \n");
        scanf("%lu", &userInput);

        unsigned long ciphertext;
        ciphertext = modular_ex(userInput, e, n);
        
        fprintf(stdout,"c = m^e mod n = %lu\n", ciphertext);  

        unsigned long decrypttext;
        decrypttext = modular_ex_v1(ciphertext, d, n);

        fprintf(stdout,"c^d mod n = %lu\n", decrypttext);  
    }
    

    return 0;
}
