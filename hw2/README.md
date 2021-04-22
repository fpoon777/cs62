---
README file for building Q3 and Q6 for HW2 

---

There is four c files: q3hw2.c is for Q3 HW2, q6hw2.c and cs10q6.c is for Q6 HW2, q6hw2.c uses the MD5 hash and cs10q6.c uses the cs10 hash. cs10q6p2.c is used for part2 of Q6.

### Make
To make q3hw2, simply run `make`
To make q6hw2.c, run `gcc -I -o q6hw2 q6hw2.c -lcrypto`
To make cs10q6.c, run `gcc -I -o cs10q6 cs10q6.c -lcrypto`
To make cs10q6p2.c, run `gcc -I -o cs10q6p2 cs10q6p2.c -lcrypto`



### run
To run q3hw2, use `./q3hw2`
To run q6hw2, use `./q6hw2`
To run cs10q6, use `./cs10q6`
To run cs10q6p2, use `./cs10q6p2`

### note
cs10q6.c and q6hw2.c test the time that it takes to hash all values in the word list.
cs10q6p2.c test how many collisions cs10 hash function generated when hashing all the words.

