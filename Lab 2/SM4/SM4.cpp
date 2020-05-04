#include "SM4.h"
#include <iostream>
#include <string.h>
#include <stdio.h>
#include <fstream> 
#include <cstdlib>

using namespace std;  
typedef unsigned int u32;
void enc(unsigned int*, unsigned int*);
void dec(unsigned int*, unsigned int*);
void genKey(unsigned int*,unsigned int*);
int main(){
	unsigned int pass[36] = {
		0x6BC10BAB, 0x3DB75B49, 0x1AF90E9F, 0x0CCA3898
	};
	unsigned int plane[36] = {
		0x01234567, 0x89ABCDEF, 0xFEDCBA98, 0x76543210
	};
	unsigned int key[32] = {0};
	genKey(pass,key);
	enc(plane,key);

	dec(plane,key);
}
unsigned int S(unsigned int t){
	return (unsigned int)(S_Box[t&0b11111111]&0b11111111)+((unsigned int)(S_Box[(t>>8)&0b11111111]&0b11111111)<<8)+((unsigned int)(S_Box[(t>>16)&0b11111111]&0b11111111)<<16)+((unsigned int)(S_Box[(t>>24)&0b11111111]&0b11111111)<<24);
}
unsigned int D_key(unsigned int t){
	return (t^((t<<13)|(t>>19))^((t<<23)|(t>>9)));
}
unsigned int D_enc(unsigned int t){
	return (t^((t<<2)|(t>>30))^((t<<10)|(t>>22))^((t<<18)|(t>>14))^((t<<24)|(t>>8)));
}
void genKey(unsigned int* K,unsigned int* key){
	for(int i = 0; i < 4; i++){
		K[i] = K[i]^T[i];
	}
	for(int i = 0; i < 32; i++){
		key[i] = K[i]^D_key(S(K[i+1]^K[i+2]^K[i+3]^CK[i]));
		// cout << std::hex << key[i] << endl;
		K[i+4] = key[i];
	}
}
void enc(unsigned int* plane,unsigned int* key){
	for(int i = 0; i < 32; i++){
		plane[i&0b11] = (plane[i&0b11]^D_enc(S(plane[(i+1)&0b11]^plane[(i+2)&0b11]^plane[(i+3)&0b11]^key[i])));
		// cout << std::hex << plane[i&0b11] << endl;
	}
	swap(plane[0],plane[3]);
	swap(plane[1],plane[2]);
}

void dec(unsigned int* plane,unsigned int* key){
	for(int i = 0; i < 32; i++){
		plane[i&0b11] = (plane[i&0b11]^D_enc(S(plane[(i+1)&0b11]^plane[(i+2)&0b11]^plane[(i+3)&0b11]^key[31-i])));
		// cout << std::hex << plane[i&0b11] << endl;
	}
	swap(plane[0],plane[3]);
	swap(plane[1],plane[2]);
}