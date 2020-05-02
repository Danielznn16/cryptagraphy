// DES
#include <iostream>
#include <string.h>
#include <stdio.h>
#include "DES.h"
using namespace std;

char* tmp = new char[64];
void swap_ip(char *);
void swap_ip_reverse(char *);
void Permuted_Choice_1(char*,char*);
void Permuted_Choice_2(char*,char*);
void Expansion(char*,char*);
void xOr(char*,char*,int);
void Substitution(char*,char*);
void Permutation(char*,char*);
int enc(string,string);
int main(){
	enc("Hello Hi","PassWord");
}
int enc(string Mstr,string Kstr){
	char* message= new char[65];
	{
		// string Mstr = "Hello Hi";
		strcpy(message,Mstr.c_str());
	}
    char* key= new char[65];
    {
    	// string Kstr = "PassWord";
    	strcpy(key,Kstr.c_str());
    }
    unsigned long M,K,tmp,tmp2;
    M = 0;
    K = 0;
    tmp = 0;
    tmp2 = 0;
    for(int i = 0; i < 8; i++){
    	M = (M<<8) + message[i];
    	K = (K<<8) + key[i];
    }
    // printf("Message:\n\t0x%lx\n", M);
    // printf("Key:\n\t0x%lx\n", K);

    unsigned long afterSip = 0;
    for(int i = 0; i < 64; i++){
    	afterSip = (afterSip<<1)|((M>>(IP[i]-1))&1);
    }
    for(int i = 0; i < 56; i++)
    	tmp = (tmp<<1)+((K>>(64-(PC_1[i]-1)))&1);
    K = tmp;
    unsigned int * left;
    unsigned int * right;
    left = (unsigned int *)&afterSip;
    right = &left[1];
    unsigned long keys[16] = {};
    for(int i = 0; i < 16; i++){
    	K = (K<<shift[i])+(K>>(64-shift[i]));
    	keys[i] = 0;
    	tmp = 0;
    	for(int j = 0; j < 48; j++){
        	keys[i] = (keys[i]<<1)+(K>>(64-PC_2[j])&1);
        	tmp = (tmp<<1)+(((*left)>>(32-E[j]))&1);
    	}
    	printf("i:%d\n\t0x%lx\n",i, keys[i]);
    	tmp = tmp^keys[i];
    	tmp2 = 0;
    	for(int j = 0; j < 8; j++){
    		((char*)(&tmp2))[j] = S_Box[j][((((tmp>>(48-i*6-1))&2)+((tmp>>(48-i*6-6))&1))<<4)+((tmp>>(48-i*6-5))&15)];
    	}
    	tmp = 0;
    	for(int j = 0; j < 32; j++){
    		tmp = (tmp<<1)+((tmp2>>(64-P[j]))&1);
    	}
    	(*left) = (*right);
    	(*right) = (unsigned int)tmp;
    }

    swap((*left),(*right));

    K = 0;
    for(int i = 0; i < 64; i++){
    	K = (K<<1)+((M>>(64-IP_Reverse[i]))&1);
    }
    printf("cypher:\n\t0x%lx\n", K);

	return K;
}

void swap_ip(char* Plane){
	for(int i = 0; i < 64; i++){
		tmp[i] = Plane[i];
	}
	for(int i = 0; i < 64; i++){
		Plane[i]= tmp[IP[i]-1];
	}
}

void swap_ip_reverse(char* Plane){
	for(int i = 0; i < 64; i++){
		tmp[i] = Plane[i];
	}
	for(int i = 0; i < 64; i++){
		Plane[i]= tmp[IP_Reverse[i]-1];
	}
}

void Permuted_Choice_1(char* K,char*re){
	for(int i = 0; i < 56+32; i++){
		re[i] = K[PC_1[i%56]-1];
	}
	re[56+32] = '\0';
}

void Permuted_Choice_2(char* K,char*re){
	for(int i = 0; i < 48; i++){
		re[i] = K[PC_2[i]-1];
	}
}

void Expansion(char* K,char*re){
	for(int i = 0; i < 48; i++){
		re[i] = K[E[i]-1];
	}
}

void xOr(char* a,char* b,int len){
	for(int i = 0; i < len; i++){
		a[i] = (a[i]==b[i])?'0':'1';
	}
}

void Substitution(char* K,char*re){
	for(int i = 0; i < 8; i++){
		static int tmp = ((K[i*6]-'0')<<5)+((K[i*6+5]-'0')<<4)+((K[i*6+1]-'0')<<3)+((K[i*6+2]-'0')<<2)+((K[i*6+3]-'0')<<1)+((K[i*6+4]-'0'));
		tmp = S_Box[i][tmp];
		re[i*4] = (tmp&8)==1?'1':'0';
		re[i*4+1] = (tmp&4)==1?'1':'0';
		re[i*4+2] = (tmp&2)==1?'1':'0';
		re[i*4+3] = (tmp&1)==1?'1':'0';
	}
}

void Permutation(char* in,char*re){
	for(int i = 0; i < 32; i++){
		re[i] = in[P[i]-1];
	}
}

