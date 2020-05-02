// DES
#include <iostream>
#include <string.h>
#include <stdio.h>
#include <fstream> 
#include <cstdlib>
#include "DES.h"
using namespace std;
typedef unsigned long long u64;

// char* tmp = new char[64];
// void swap_ip(char *);
// void swap_ip_reverse(char *);
// void Permuted_Choice_1(char*,char*);
// void Permuted_Choice_2(char*,char*);
// void Expansion(char*,char*);
// void xOr(char*,char*,int);
// void Substitution(char*,char*);
// void Permutation(char*,char*);
u64 enc(u64,u64);
u64 dec(u64,u64);
int main(){
	// enc("Hello Hi","PassWord");
	u64 plane[256]={};
	u64 res[257]={};
	char key[] = "12345678";
	char * t;
	// {
		t = (char*)plane;
		for (int i = 0; i <256*8; i++)
		{
			// t[i] = rand()%256;
            t[i] = 'a';
		}
		t[256*8] = '\0';
	// }
	// cout << t << "plane end"<< endl;
	u64 brk = 0;
    for(int i = 0; i < 8; i++){
        brk = (brk<<8) + key[i];
    }
    u64 tmp = brk;
    for(int i = 0; i < 256; i++){
        res[i] = enc(plane[i]^tmp,brk);
        tmp = res[i];
        // cout << brk << endl;
        // res[i] = brk;
    }

    // brk = 0;
    // for(int i = 0; i < 8; i++){
    //     brk = (brk<<8) + key[i];
    // }
    // for(int i = 0; i < 256; i++){
    //     res[i] = enc(plane[i],brk);
    //     // cout << brk << endl;
    //      // = brk;
    // }

	// cout << t << endl;
	ofstream outfile("./out.txt", ios::out); 
	for(int i = 0; i < 256; i++){
		for(int j = 0; j < 64; j++){
			outfile << (((res[i]>>63)&1)!=0?'1':'0');
            res[i] = res[i]<<1;
		}
		outfile<<endl;
	}
	outfile.close();
	return 0;
}

u64 dec(u64 m, u64 k){
    u64 M = m;
    u64 K = k;
    u64 tmp,tmp2;
    tmp2 = 0;
    // printf("Message:\n\t0x%llx\n", M);
    // printf("Key:\n\t0x%llx\n", K);

    u64 afterSip = 0;
    for(int i = 0; i < 64; i++){
        afterSip = (afterSip<<1)|((M>>(IP[i]-1))&1);
    }
    tmp = 0;
    // for (int i = 0; i < 64; ++i)
    // {
    //  cout << i << ":" << (K>>i) << endl;
    // }
    for(int i = 0; i < 56; i++){
        // cout << tmp << ":" << (K>>(64-(PC_1[i]))) << endl;
            tmp = (tmp<<1)+((K>>(64-(PC_1[i])))&1);
        }
    K = tmp;
    // printf("Key:\t0x%llx\n", tmp);
    unsigned int * left;
    unsigned int * right;
    left = (unsigned int *)&afterSip;
    right = &left[1];
    u64 keys[16] = {};
    for(int i = 0; i < 16; i++){
        K = (K<<shift[i])+(K>>(64-shift[i]));
        keys[i] = 0;
        for(int j = 0; j < 48; j++){
            keys[i] = (keys[i]<<1)+((K>>(56-PC_2[j]))&1);
            // tmp = (tmp<<1)+(((*left)>>(32-E[j]))&1);
        }
    }
    for(int i = 15; i >-1; i--){
        tmp = 0;
        for(int j = 0; j < 48; j++){
            tmp = (tmp<<1)+(((*right)>>(32-E[j]))&1);
        }
        // printf("i:%d\t0x%llx\n",i, keys[i]);
        tmp = tmp^keys[i];
        tmp2 = 0;
        for(int j = 0; j < 8; j++){
            ((char*)(&tmp2))[j] = (((char*)(&tmp2))[j]<<4)+S_Box[j][((((tmp>>(48-i*6-1))&2)+((tmp>>(48-i*6-6))&1))<<4)+((tmp>>(48-i*6-5))&15)];
        }
        tmp = 0;
        for(int j = 0; j < 32; j++){
            tmp = (tmp<<1)+((tmp2>>(32-P[j]))&1);
        }
        (*right) = (*left);
        (*left) = (unsigned long)tmp;
    }

    swap((*right),(*left));

    K = 0;
    for(int i = 0; i < 64; i++){
        K = (K<<1)+((M>>(64-IP_Reverse[i]))&1);
    }
    // printf("cypher:\n\t0x%llx\n", K);

    return K;
}

u64 enc(u64 m, u64* keys){
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
	u64 M = m;
	u64 K = k;
    u64 tmp,tmp2;
    tmp2 = 0;
    // printf("Message:\n\t0x%llx\n", M);
    // printf("Key:\n\t0x%llx\n", K);

    u64 afterSip = 0;
    for(int i = 0; i < 64; i++){
    	afterSip = (afterSip<<1)|((M>>(IP[i]-1))&1);
    }
    tmp = 0;
    // for (int i = 0; i < 64; ++i)
    // {
    // 	cout << i << ":" << (K>>i) << endl;
    // }
    for(int i = 0; i < 56; i++){
    	// cout << tmp << ":" << (K>>(64-(PC_1[i]))) << endl;
        	tmp = (tmp<<1)+((K>>(64-(PC_1[i])))&1);
        }
    K = tmp;
    // printf("Key:\t0x%llx\n", tmp);
    unsigned int * left;
    unsigned int * right;
    left = (unsigned int *)&afterSip;
    right = &left[1];
    // u64 keys[16] = {};
    for(int i = 0; i < 16; i++){
    	K = (K<<shift[i])+(K>>(64-shift[i]));
    	keys[i] = 0;
    	tmp = 0;
    	for(int j = 0; j < 48; j++){
        	keys[i] = (keys[i]<<1)+((K>>(56-PC_2[j]))&1);
        	tmp = (tmp<<1)+(((*left)>>(32-E[j]))&1);
    	}
    	// printf("i:%d\t0x%llx\n",i, keys[i]);
       	tmp = tmp^keys[i];
    	tmp2 = 0;
    	for(int j = 0; j < 8; j++){
    		((char*)(&tmp2))[j] = (((char*)(&tmp2))[j]<<4)+S_Box[j][((((tmp>>(48-i*6-1))&2)+((tmp>>(48-i*6-6))&1))<<4)+((tmp>>(48-i*6-5))&15)];
    	}
    	tmp = 0;
    	for(int j = 0; j < 32; j++){
    		tmp = (tmp<<1)+((tmp2>>(32-P[j]))&1);
    	}
    	(*left) = (*right);
    	(*right) = (unsigned long)tmp;
    }

    swap((*left),(*right));

    K = 0;
    for(int i = 0; i < 64; i++){
    	K = (K<<1)+((M>>(64-IP_Reverse[i]))&1);
    }
    // printf("cypher:\n\t0x%llx\n", K);

	return K;
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

