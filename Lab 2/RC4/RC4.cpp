#include "RC4.h"
#include <iostream>
#include <string.h>
#include <stdio.h>
#include <fstream> 
#include <cstdlib>

using namespace std;  
typedef unsigned int u32;
void genKey(unsigned char*,unsigned char*, unsigned char*, int);
void init(unsigned char*,unsigned char*,char*,int);
void disturbS(unsigned char*,unsigned char*);
int main(){
	unsigned char S[256];
	unsigned char T[256];
	char input[] = "hello";
	init(S,T,input,5);
	disturbS(S,T);

	unsigned char keys[2049];
	genKey(S,T,keys,2048);
	keys[2048] = (unsigned char)('\0');
	for(int i = 0; i < 512; i++){
		cout << std::hex << ((unsigned int*)keys)[i] << endl;
	}
}
void init(unsigned char* S,unsigned char* T, char* userInput, int len){
	for(int i = 0; i < 256; i++){
		S[i] = i;
		T[i] = (userInput[i%len]&0b11111111);
	}
}

void disturbS(unsigned char* S,unsigned char* T){
	unsigned char j = 0;
	for(int i = 0 ; i < 256 ; i++){
		j = (j + S[i] + T[i])%256;
		swap(S[i] , S[j]);
	}
}

void genKey(unsigned char* S,unsigned char* T,unsigned char* K,int len){
	unsigned char i=0;
	unsigned char j=0;
	unsigned char t=0;
	while(len--){//相当于执行1024次，这样生成的秘钥流也是1024个字节
		i = (i+1)%256;
		j = (j+S[i])%256;
		swap(S[i],S[j]);
		t = (S[i]+S[j])%256;
		K[len] = S[t];
	}
}