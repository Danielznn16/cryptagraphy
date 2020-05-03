// DES
#include <iostream>
#include <string.h>
#include <stdio.h>
#include <fstream> 
#include <cstdlib>
#include <ctime>
#include "DES.h"
using namespace std;
typedef unsigned long long u64;

const u64 mask28 = (1<<28)-1;
// char* tmp = new char[64];
// void swap_ip(char *);
// void swap_ip_reverse(char *);
// void Permuted_Choice_1(char*,char*);
// void Permuted_Choice_2(char*,char*);
// void Expansion(char*,char*);
// void xOr(char*,char*,int);
// void Substitution(char*,char*);
// void Permutation(char*,char*);

u64 enc(u64,u64*);
u64 dec(u64,u64);

int main(){
	u64 plane[256]={};
	u64 res[257]={};
	char key[] = "12345678";
	char * t;
		t = (char*)plane;
		for (int i = 0; i <256*8; i++)
		{
            t[i] = 'a';
		}
		t[256*8] = '\0';
	u64 K = 0;
    for(int i = 0; i < 8; i++){
        K = (K<<8) + key[i];
    }
    u64 IV = K;
    for(int i = 0; i < 8; i++){
        // ((char*)(&tmp))[i] = rand()%256;
        ((char*)(&IV))[i] = 'a';
    }

    u64 tmp = 0;
    for(int i = 0; i < 56; i++){
        tmp = (tmp<<1)+((K>>(64-(PC_1[i])))&1);
    }
    K = tmp;
    u64 keys[16] = {};
    u64 leftK,rightK;
    leftK = K>>28;
    rightK = K%(1<<28);

    for(int i = 0; i < 16; i++){
        leftK = (leftK << shift[i]) + (leftK>>(28-shift[i]))&mask28;
        rightK = (rightK << shift[i]) + (rightK>>(28-shift[i]))&mask28;
        K = (leftK<<28)+rightK;
        keys[i] = 0;
        for(int j = 0; j < 48; j++){
            keys[i] = (keys[i]<<1)+((K>>(56-PC_2[j]))&1);
        }
    }


    clock_t a,b;
    a = clock();
    for(int i = 0; i < 256; i++){
        res[i] = enc(plane[i]^IV,keys);
        IV = res[i];
    }
    b = clock();
    cout << "time: " << ((double)(b-a))/CLOCKS_PER_SEC << "s" << "  for: " << 256*64 << endl;
    cout << "speed: " <<  ((double)256*64/(1024*1024))/(((double)(b-a))/CLOCKS_PER_SEC) << "Mbps" << endl;
	ofstream outfile("./out.txt", ios::out); 
	for(int i = 0; i < 256; i++){
        // cout << std::hex << res[i] << endl;
		for(int j = 0; j < 64; j++){
			outfile << (((res[i]>>63)&1)!=0?'1':'0');
            res[i] = res[i]<<1;
		}
		outfile<<endl;
	}
	outfile.close();
    exit(0);
	return 0;
}

u64 enc(u64 M, u64 *keys){
	// char* message= new char[65];
	// {
	// 	// string Mstr = "Hello Hi";
	// 	strcpy(message,Mstr.c_str());
	// }
 //    char* key= new char[65];
 //    {
 //    	// string Kstr = "PassWord";
 //    	strcpy(key,Kstr.c_str());
 //    }
    u64 tmp,tmp2;
    tmp2 = 0;
    // printf("Message:\n\t0x%llx\n", M);
    // printf("Key:\n\t0x%llx\n", K);

    u64 afterSip = 0;
    for(int i = 0; i < 64; i++){
    	afterSip = (afterSip<<1)|((M>>(64-IP[i]))&1);
    }
    // cout << "SIP" << std::hex<<afterSip<<endl;
    // 0xf0ccaaf556678f
    // cout << "PC_1: " << std::hex << K << endl;
    unsigned int * left;
    unsigned int * right;
    right = (unsigned int *)&afterSip;
    left = &right[1];
    
    // cout << (*left)<<endl<<(*right)<<endl<<afterSip<<endl;
    for(int i = 0; i < 16; i++){
        // cout << i <<  ":" << std::hex << K << endl;
    	
        tmp = 0;
        for(int j = 0; j < 48; j++){
            tmp = (tmp<<1)+(((*right)>>(32-E[j]))&1);
        }
        // cout << i << "E" << i << ":" << std::hex << tmp<< endl;
        // 1b02effc7072
        // cout << "K" << i << ": " << std::hex << keys[i] << endl;
    	// printf("i:%d\t0x%llx\n",i, keys[i]);
       	tmp = tmp^keys[i];
        // cout << i << "^ result:" << std::hex << tmp<< endl;
    	tmp2 = 0;
    	for(int j = 0; j < 8; j++){
    		tmp2+=((S_Box[7-j][((tmp&0b100000))|((tmp&1)<<4)|((tmp>>1)&0b1111)])<<(j*4));
            tmp = tmp>>6;
            // if(i==4)
            // cout << j << ":" << std::hex << (tmp&0b111111)<<" value:\t" << tmp2 << "|" << (int)(S_Box[7-j][((tmp&0b100000))|((tmp&1)<<4)|((tmp>>1)&0b1111)])<< endl;
    	}
        // cout << i << "S_Box result:" << std::hex << tmp2<< endl;
    	tmp = 0;
    	for(int j = 0; j < 32; j++){
    		tmp = (tmp<<1)+((tmp2>>(32-P[j]))&1);
    	}
        // cout << "Permutation " << i << " : " << std::hex << tmp<< endl;
        // cout << "before Round " << i << " : " << std::hex << (afterSip)<< endl;
        swap((*left),(*right));
    	(*right) = (*right)^(unsigned int)tmp;
        // cout << "after Round " << i << " : " << std::hex << (afterSip)<< endl << endl;
    }

    swap((*left),(*right));

    M = 0;
    for(int i = 0; i < 64; i++){
    	M = (M<<1)+((afterSip>>(64-IP_Reverse[i]))&1);
    }
    // printf("cypher:\n\t0x%llx\n", K);

	return M;
}

// void swap_ip(char* Plane){
// 	for(int i = 0; i < 64; i++){
// 		tmp[i] = Plane[i];
// 	}
// 	for(int i = 0; i < 64; i++){
// 		Plane[i]= tmp[IP[i]-1];
// 	}
// }

// void swap_ip_reverse(char* Plane){
// 	for(int i = 0; i < 64; i++){
// 		tmp[i] = Plane[i];
// 	}
// 	for(int i = 0; i < 64; i++){
// 		Plane[i]= tmp[IP_Reverse[i]-1];
// 	}
// }

// void Permuted_Choice_1(char* K,char*re){
// 	for(int i = 0; i < 56+32; i++){
// 		re[i] = K[PC_1[i%56]-1];
// 	}
// 	re[56+32] = '\0';
// }

// void Permuted_Choice_2(char* K,char*re){
// 	for(int i = 0; i < 48; i++){
// 		re[i] = K[PC_2[i]-1];
// 	}
// }

// void Expansion(char* K,char*re){
// 	for(int i = 0; i < 48; i++){
// 		re[i] = K[E[i]-1];
// 	}
// }

// void xOr(char* a,char* b,int len){
// 	for(int i = 0; i < len; i++){
// 		a[i] = (a[i]==b[i])?'0':'1';
// 	}
// }

// void Substitution(char* K,char*re){
// 	for(int i = 0; i < 8; i++){
// 		static int tmp = ((K[i*6]-'0')<<5)+((K[i*6+5]-'0')<<4)+((K[i*6+1]-'0')<<3)+((K[i*6+2]-'0')<<2)+((K[i*6+3]-'0')<<1)+((K[i*6+4]-'0'));
// 		tmp = S_Box[i][tmp];
// 		re[i*4] = (tmp&8)==1?'1':'0';
// 		re[i*4+1] = (tmp&4)==1?'1':'0';
// 		re[i*4+2] = (tmp&2)==1?'1':'0';
// 		re[i*4+3] = (tmp&1)==1?'1':'0';
// 	}
// }

// void Permutation(char* in,char*re){
// 	for(int i = 0; i < 32; i++){
// 		re[i] = in[P[i]-1];
// 	}
// }

