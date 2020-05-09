#include "AES.h"
#include <iostream>
#include <string.h>
#include <stdio.h>
#include <fstream> 
#include <cstdlib>

using namespace std;  
typedef unsigned int u32;
void enc(char*, unsigned int*);
void dec(char*, unsigned int*);
void genKey(char*,char*);
int main(){
	char* planeT = new char[16*1024/8];
	char* dcp = new char[16*1024/8];

	cout << "plane:\n";
	for(int i = 0; i < 16*1024/8;i++){
		planeT[i] = 'a'+rand()%26;
		cout << planeT[i];
	}cout << endl;

	unsigned int IV[36] = {
		0x01234567, 0x89ABCDEF, 0xFEDCBA98, 0x76543210
	};
	char* plane = new char[16];
	cout << "IV: \n";
	for(int i = 0; i < 16;i++){
		((unsigned char*)plane)[i] = 'a'+rand()%26;
		((unsigned char*)IV)[i] = ((unsigned char*)plane)[i];
		cout << plane[i];
	}cout << endl;
	char* pass = new char[16];

	cout << "Key: \n";
	for(int i = 0; i < 16;i++){
		pass[i] = 'a'+rand()%26;
		cout << pass[i];
	}cout << endl;

	unsigned int key[44] = {0};
	clock_t a,b;
    a = clock();
	genKey(pass,(char*)key);

	for(int i = 0; i < 16*1024/32; i+=4){
		((unsigned int*)plane)[0] ^= ((unsigned int*)planeT)[i];
		((unsigned int*)plane)[1] ^= ((unsigned int*)planeT)[i+1];
		((unsigned int*)plane)[2] ^= ((unsigned int*)planeT)[i+2];
		((unsigned int*)plane)[3] ^= ((unsigned int*)planeT)[i+3];

		enc(plane,key);

		((unsigned int*)planeT)[i] = ((unsigned int*)plane)[0];
		((unsigned int*)planeT)[i+1] = ((unsigned int*)plane)[1];
		((unsigned int*)planeT)[i+2] = ((unsigned int*)plane)[2];
		((unsigned int*)planeT)[i+3] = ((unsigned int*)plane)[3];

	}
	b = clock();

    cout << endl;
    cout << dec <<"time: " << ((double)(b-a))/CLOCKS_PER_SEC << "s" << "  for: " << 16*1024 << "bit" << endl;
    cout << dec << "speed: " <<  ((double)(16*1024)/(1024*1024))/(((double)(b-a))/CLOCKS_PER_SEC) << "Mbps" << endl;
	
	cout << "cypher:\n";
	for(int i = 0; i < 16*1024/32;i++){
		cout << hex << ((((unsigned int*)planeT)[i])) << endl;
	}cout << endl;

	for(int i = 0; i < 16*1024/32; i+=4){
		((unsigned int*)plane)[0] = ((unsigned int*)planeT)[i];
		((unsigned int*)plane)[1] = ((unsigned int*)planeT)[i+1];
		((unsigned int*)plane)[2] = ((unsigned int*)planeT)[i+2];
		((unsigned int*)plane)[3] = ((unsigned int*)planeT)[i+3];

		dec(plane,key);

		((unsigned int*)dcp)[i] = ((unsigned int*)plane)[0]^IV[0];
		((unsigned int*)dcp)[i+1] = ((unsigned int*)plane)[1]^IV[1];
		((unsigned int*)dcp)[i+2] = ((unsigned int*)plane)[2]^IV[2];
		((unsigned int*)dcp)[i+3] = ((unsigned int*)plane)[3]^IV[3];

		IV[0] = ((unsigned int*)planeT)[i];
		IV[1] = ((unsigned int*)planeT)[i+1];
		IV[2] = ((unsigned int*)planeT)[i+2];
		IV[3] = ((unsigned int*)planeT)[i+3];

	}
	cout << "decrypt:\n";
	for(int i = 0; i < 16*1024/8;i++){
		cout << dcp[i];
	}cout << endl;
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
		        plane[i] = 	   Mul_02[tmpChar[i]&0b11111111] ^ Mul_03[tmpChar[i+4*1]&0b11111111] ^ tmpChar[i+4*2] ^ tmpChar[i+4*3];
		        plane[i+4*1] = tmpChar[i] ^ Mul_02[tmpChar[i+4*1]&0b11111111] ^ Mul_03[tmpChar[i+4*2]&0b11111111] ^ tmpChar[i+4*3];
		        plane[i+4*2] = tmpChar[i] ^ tmpChar[i+4*1] ^ Mul_02[tmpChar[i+4*2]&0b11111111] ^ Mul_03[tmpChar[i+4*3]&0b11111111];
		        plane[i+4*3] = Mul_03[tmpChar[i]&0b11111111] ^ tmpChar[i+4*1] ^ tmpChar[i+4*2] ^ Mul_02[tmpChar[i+4*3]&0b11111111];
		    }

		for(int i = 0; i < 4; i++){
			((unsigned int *)plane)[i] = ((unsigned int *)plane)[i]^key[i+4*j];
		}
	}
}
void dec(char* plane,unsigned int* key){
	for(int j = 10; j > 0; j--){
		for(int i = 0; i < 4; i++){
			((unsigned int *)plane)[i] = ((unsigned int *)plane)[i]^key[i+4*j];
		}

		// Inv - mixColumes
		memcpy(tmpChar,plane,sizeof(char)*16);
		if(j!=10)
			for(int i = 0; i < 4;i++){
		        plane[i] =	   Mul_0e[tmpChar[i]&0b11111111] ^ Mul_0b[tmpChar[i+4*1]&0b11111111] ^ Mul_0d[tmpChar[i+4*2]&0b11111111] ^ Mul_09[tmpChar[i+4*3]&0b11111111];
		        plane[i+4*1] = Mul_09[tmpChar[i]&0b11111111] ^ Mul_0e[tmpChar[i+4*1]&0b11111111] ^ Mul_0b[tmpChar[i+4*2]&0b11111111] ^ Mul_0d[tmpChar[i+4*3]&0b11111111];
		        plane[i+4*2] = Mul_0d[tmpChar[i]&0b11111111] ^ Mul_09[tmpChar[i+4*1]&0b11111111] ^ Mul_0e[tmpChar[i+4*2]&0b11111111] ^ Mul_0b[tmpChar[i+4*3]&0b11111111];
		        plane[i+4*3] = Mul_0b[tmpChar[i]&0b11111111] ^ Mul_0d[tmpChar[i+4*1]&0b11111111] ^ Mul_09[tmpChar[i+4*2]&0b11111111] ^ Mul_0e[tmpChar[i+4*3]&0b11111111];
		    }

		// Inv - shiftRows
		((unsigned int*)(&(plane[4])))[0] = (((unsigned int*)(&(plane[4])))[0] << 8) + (((unsigned int*)(&(plane[4])))[0]>>24);
		((unsigned int*)(&(plane[8])))[0] = (((unsigned int*)(&(plane[8])))[0] << 16) + (((unsigned int*)(&(plane[8])))[0]>>16);
		((unsigned int*)(&(plane[12])))[0] = (((unsigned int*)(&(plane[12])))[0] << 24) + (((unsigned int*)(&(plane[12])))[0]>>8);
		
		// Inv - subBytes
		for(int i = 0; i < 16; i++)
			plane[i] = Inv_S_Box[plane[i]&0b11111111];

	}
	for(int i = 0; i < 4; i++){
		// cout << endl <<std::hex << ((unsigned int *)plane)[i] << endl;
		((unsigned int *)plane)[i] = ((unsigned int *)plane)[i]^key[i];
		// cout << std::hex << ((unsigned int *)plane)[i] << endl;
	}
}