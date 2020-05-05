#include "BM.h"
#include <iostream>
#include <string.h>
#include <stdio.h>
#include <fstream> 
#include <cstdlib>

using namespace std;
typedef unsigned long long u64;
void B_M(unsigned char*,int);
int main()
{
    unsigned char a[18] = {0};
    for(int i = 0; i < 10; i++){
    	a[i] = rand()%256;
    }
    // for(int i = 0; i < 10; i++){
    // 	cout << std::hex << ((unsigned int)a[i]) << "|" << *((u64 *)&a[i]) << endl;
    // }

    B_M(a,80);
    return 0;
}

void B_M(unsigned char* a,int len)
{
    int n = 0;
    u64 *f = new u64[len];
    int *l = new int[len];
    int m = 0;
    f[0] = 1;
    l[0] = 0;
    for(int i = 0; i < len; i++){
    	u64 tmp = (*((u64 *)&a[i/8]))>>(i&0b111);
    	// cout << std::hex << tmp;
    	tmp = tmp&f[i];
    	int cnt = 0;
    	while(tmp > 0){
    		cnt = cnt^(tmp&0b1);
    		tmp = tmp>>1;
    	}
    	// cout << i << ":" << cnt;
    	if(cnt){
    		if(l[i]==0){
    			f[i+1] = (1<<(i+1))+1;
    			// cout << "f[i+1]: " << f[i+1];
    			l[i+1] = i+1;
    			m = i;
    		}else{
    			f[i+1] = f[i]^(f[m]<<(i-m));
    			// cout << "F[i]" << f[i] << "f[m]"<< f[m]<< "(i-m)" <<(i-m);
    			l[i+1] = (l[i]>i+1-l[i])?l[i]:(i+1-l[i]);
    			if(l[i+1]>l[i])
    				m = i;
    		}
    	}else{
    		f[i+1] = f[i];
    		l[i+1] = l[i];
    	}
    	// cout << ":" << std::hex << f[i+1] << endl;
    }
    int k = 0;
    cout << "0";
    do{
    	if(f[len]&1){
    		cout << "+x^" << k++;
    	}else{
    		k++;
    	}f[len]= f[len]>>1;
    }while(f[len]>0);cout << endl;
}