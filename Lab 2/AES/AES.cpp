#include "AES.h"
#include <iostream>
#include <string.h>
#include <stdio.h>
#include <fstream> 
#include <cstdlib>

using namespace std;  
typedef unsigned int u32;
void enc(char*, unsigned int*);
void genKey(char*,char*);
int main(){
	char pass[] = {
		0x2b,0x28,0xab,0x09,
		0x7e,0xae,0xf7,0xcf,
		0x15,0xd2,0x15,0x4f,
		0x16,0xa6,0x88,0x3c
	};
	unsigned int key[44] = {0};
	genKey(pass,(char*)key);

	cout << "passW: ";
	for(int i = 0; i < 16; i++){
		if((unsigned int)(pass[i]&0b11111111)<16)
			cout << "0";
		cout << std::hex << (unsigned int)(pass[i]&0b11111111);
	}cout << endl;

	char plane[] = {
		0x32,0x88,0x31,0xe0,
		0x43,0x5a,0x31,0x37,
		0xf6,0x30,0x98,0x07,
		0xa8,0x8d,0xa2,0x34
	};

	cout << "plane: ";
	for(int i = 0; i < 16; i++){
		if((unsigned int)(plane[i]&0b11111111)<16)
			cout << "0";
		cout << std::hex << (unsigned int)(plane[i]&0b11111111);
	}cout << endl;

	enc(plane,key);

	cout << "cyphe: ";

	for(int k = 0; k < 4; k ++){
		for(int l = 0; l < 4; l++){
			if(((unsigned int)plane[k*4+l]&0b11111111)<16)
				cout << '0';
			cout << std::hex << ((unsigned int)(plane[k*4+l]&0b11111111));
		}
	// cout << endl;
	}
	cout << endl;

}
void genKey(char* K,char*key){
	for(int j = 0; j < 16; j++){
		key[j] = K[j]&0b11111111;
		}
	// return ;
	unsigned char kr[4] = {0};

	for(int i = 4; i < 44; i++){
		if(i%4==0)
			kr[0] = RC[i/4];
		for(int j = 0; j < 4; j++)
		if(j==0){
			// key[i*4+j] = key[((i-4)/4*4+(i+1)%4)*4+3];
			key[i*4+j] = key[i*4+j-16]^S_Box[key[((i-4)/4*4+(i+1)%4)*4+3]&0b11111111]^kr[i%4];
		}else{
			key[i*4+j] = key[i*4+j-1]^key[(i-4)*4+j];
		}
	}
}
char tmpChar[16] = {0};
void enc(char* plane,unsigned int* key){
	for(int i = 0; i < 4; i++){
		// cout << endl <<std::hex << ((unsigned int *)plane)[i] << endl;
		((unsigned int *)plane)[i] = ((unsigned int *)plane)[i]^key[i];
		// cout << std::hex << ((unsigned int *)plane)[i] << endl;
	}
	for(int j = 1; j < 11; j++){
		// subBytes
		for(int i = 0; i < 16; i++)
			plane[i] = S_Box[plane[i]&0b11111111];
		// shiftRows
		((unsigned int*)(&(plane[4])))[0] = (((unsigned int*)(&(plane[4])))[0] >> 8) + (((unsigned int*)(&(plane[4])))[0]<<24);
		((unsigned int*)(&(plane[8])))[0] = (((unsigned int*)(&(plane[8])))[0] >> 16) + (((unsigned int*)(&(plane[8])))[0]<<16);
		((unsigned int*)(&(plane[12])))[0] = (((unsigned int*)(&(plane[12])))[0] >> 24) + (((unsigned int*)(&(plane[12])))[0]<<8);

		memcpy(tmpChar,plane,sizeof(char)*16);
		// mixColumes
		if(j!=10)
			for(int i = 0; i < 4;i++){
		        plane[i] = x2time(tmpChar[i]) ^ x3time(tmpChar[i+4*1]) ^ tmpChar[i+4*2] ^ tmpChar[i+4*3];
		        plane[i+4*1] = tmpChar[i+4*0] ^ x2time(tmpChar[i+4*1]) ^ x3time(tmpChar[i+4*2]) ^ tmpChar[i+4*3];
		        plane[i+4*2] = tmpChar[i+4*0] ^ tmpChar[i+4*1] ^ x2time(tmpChar[i+4*2]) ^ x3time(tmpChar[i+4*3]);
		        plane[i+4*3] = x3time(tmpChar[i+4*0]) ^ tmpChar[i+4*1] ^ tmpChar[i+4*2] ^ x2time(tmpChar[i+4*3]);
		    }

		for(int i = 0; i < 4; i++){
			((unsigned int *)plane)[i] = ((unsigned int *)plane)[i]^key[i+4*j];
		}


		// for(int k = 0; k < 4; k ++){
		// 	for(int l = 0; l < 4; l++){
		// 		if(((unsigned int)plane[k*4+l]&0b11111111)<16)
		// 			cout << '0';
		// 		cout << std::hex << ((unsigned int)plane[k*4+l]&0b11111111);
		// 	}
		// cout << endl;
		// }
		// cout << endl;

	}
}