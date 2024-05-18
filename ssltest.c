#include <stdio.h>
#include <openssl/sha.h>
#include <stddef.h>
#include <stdint.h>
#include <stdlib.h>
#include <inttypes.h>
#include <string.h>

/*IMPORTANT: must add the flag -L/usr/lib -lcrypto in compilation!
	I'm currently compiling it as: gcc -o ssltest ssltest.c -std=c99 -L/usr/lib -lcrypto */

uint64_t dehash(uint8_t* hash, uint64_t start, uint64_t end);
int compare(uint8_t* array1, uint8_t* array2);
void print_byte_by_byte(uint8_t* array1);

void printHashHex(char *hash) {
    int i;
    for(i = 0; i < 32; i++){
        printf("%02x", (unsigned char)hash[i]);
    }
    printf("\n");
}

// int main() {



// 	uint64_t* data;
// 	size_t count;
// 	uint8_t* res;
// 	uint64_t start;
// 	uint64_t end;
// 	uint64_t dehash_result;
// 	SHA256_CTX* ctx;
// 	ctx = (SHA256_CTX *) malloc(sizeof(SHA256_CTX));
	
// 	SHA256_Init(ctx);
	
	
// 	data = (uint64_t*) malloc(sizeof(uint64_t));
// 	*data = 1234577;
// 	printf("number to be hashed: %jd\n", *data);
// 	count = sizeof(uint64_t);
// 	res = (uint8_t*) malloc(256);

// 	SHA256_Update(ctx,(const void *) data, count);
// 	SHA256_Final((unsigned char *) res, ctx);
	 	
// 	start = 1;
// 	end = 9899999;

//         dehash_result = dehash(res,start,end);

// 	//printf("hash of the number 5: %jd \n; byte by byte: \n", *res);
// 	//print_byte_by_byte(res);

// 	//printf("dehash result = %jd", dehash_result);


// 	printf("Return value: %d", (dehash_result == *data));

// 	return 0;


// }

uint64_t dehash(uint8_t* hash, uint64_t start, uint64_t end) {
	/* takes in, in LITTLE ENDIAN, a pointer to a uint8_t array with the hash; a uint64_t value for the start boundary, and a uint64_t value with the end boundary.
	
	Returns the uint64_t integer that generated the hash.

	the hash functions work like so:

	int SHA256_Init(SHA256_CTX *c);
	int SHA256_Update(SHA256_CTX *c, const void *data, size_t len);
	int SHA256_Final(unsigned char *md, SHA256_CTX *c);
	
	c is a pointer to SHA256_CTX structure. We just need to declare it and pass it in the function.
	data is a pointer to the number that we want to generate a hash from;
	len is the size of the data. A good practice for size_t variables is 
           size_t len = sizeof(array);
        md_buf is where the hash will be stored (must have 256 bytes allocated to it)	

        */

	uint64_t original = 9999; // the value that produced the hash, will be returned at the end of this function
	uint64_t test; // value used for trying the hash values

	uint8_t * hash_result; // hash that resulted from trying with the test variable
	// hash_result = (uint8_t *)malloc(256);

	size_t count;
 	count = sizeof(uint64_t);
	// printf("will declare SHA256 struct\n");
	SHA256_CTX ctx;
	// printf("declared the struct \n");
	// SHA256_Init(ctx);

	for(test=(uint64_t)start;test<=end;test+=(uint64_t)1){

	    hash_result = (uint8_t *)malloc(256); // far from ideal but here for testing purposes	

	    SHA256_Init(&ctx);
	    // SHA256((const unsigned char *)testptr, count,(unsigned char *) hash_result);
	    SHA256_Update(&ctx,(const void *) &test, count);
	    SHA256_Final((unsigned char *) hash_result, &ctx);

	    // if(test==(uint64_t)5){
		// printf("value of test: %jd \n", *testptr);
		// printf("\n(inside dehash) hash of the number 5: %jd\nbyte by byte: \n", *hash_result);	
	    //     print_byte_by_byte(hash_result);	
		// }

		// printHashHex(hash);
		// printHashHex(hash_result);
	
	    // printf("about to compare, iteration %jd\n", test);	
	    if(compare(hash_result, hash)==1){
			original = test;
            printf("found it!!\n");
			break;	
	    }
	
	    free(hash_result);
	}

	return original;
	
}

int compare(uint8_t* array1, uint8_t* array2) {
	/* takes in two uint8 pointers, compares the bytes one-to-one up to byte 255 and  
          returns 1 if the arrays are the same; 0 if otherwise
        */
	
	int res = 1;
	for(int i=0; i<32;i++){
	    if((array1[i]) != (array2[i])){
		res = 0;
		break;
	    }	
	}

	return res; 



}

void print_byte_by_byte(uint8_t* array1) {

	for(int i=0; i<32;i++){
		printf("%"PRIu8" ", *(array1+i));	
	}

	printf("\n\n");

}


