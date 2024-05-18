#ifndef DEHASH_H
#define DEHASH_H

#include <stdio.h>
#include <openssl/sha.h>
#include <stddef.h>
#include <stdint.h>
#include <stdlib.h>
#include <inttypes.h>

/*IMPORTANT: must add the flag -L/usr/lib -lcrypto in compilation!
	I'm currently compiling it as: gcc -o ssltest ssltest.c -std=c99 -L/usr/lib -lcrypto */

uint64_t dehash(uint8_t* hash, uint64_t start, uint64_t end);
int compare(uint8_t* array1, uint8_t* array2);
void print_byte_by_byte(uint8_t* array1);
void printHashHex(char *hash);

#endif
