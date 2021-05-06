#!/bin/sh

FILE="./dex"
SIGN1="./msg1.sign"
SIGN2="./msg2.sign"
HASH1="./msg1.hash"
HASH2="./msg2.hash"
CFILE="./dex.c"
EFILE="./dex"
PUB="./dsa_pub.pem"

echo "Compiling..."
gcc "$CFILE" -o "$EFILE" -lcrypto
echo "Compiled"

echo "Generating private key"
"$FILE" dsa_pub.pem msg1.hash msg1.sign msg2.hash msg2.sign
echo "Key generated"

echo "Signing msg1 with sha1"
openssl dgst -binary -sha1 -sign fake_private.pem -out signature1.sign msg1.txt
echo "msg1 signed"

echo "Signing msg2 with sha1"
openssl dgst -binary -sha1 -sign fake_private.pem -out signature2.sign msg2.txt
echo "msg2 signed"

echo "Verifying msg1 with sha1"
openssl dgst -binary -sha1 -verify dsa_pub.pem -signature signature1.sign msg1.txt

echo "Verifying msg2 with sha1"
openssl dgst -binary -sha1 -verify dsa_pub.pem -signature signature2.sign msg2.txt

echo "Finished"
