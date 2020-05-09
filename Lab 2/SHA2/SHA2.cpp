#include "SHA2.h"
#include <iostream>
#include <string.h>
#include <stdio.h>
#include <fstream> 
#include <cstdlib>
#include <sstream>
#include <ctime>

using namespace std;
typedef unsigned long long u64;

unsigned rightShiftReg(const unsigned &word,const int &num)
{
    return (word>>num)|(word<<(32-num));
}

unsigned ch(const unsigned x, const unsigned y, const unsigned z){
    return ((x&y)^((~x)&z));
}

unsigned ma(const unsigned x, const unsigned y, const unsigned z){
    return ((x&y)^(x&z)^(y&z));
}

unsigned Sig0(const unsigned data){
    return (rightShiftReg(data,2)^rightShiftReg(data,13)^rightShiftReg(data,22));
}

unsigned Sig1(const unsigned data){
    return (rightShiftReg(data,6)^rightShiftReg(data,11)^rightShiftReg(data,25));
}

unsigned sig0(const unsigned data){
    return (rightShiftReg(data,7)^rightShiftReg(data,18)^(data>>3));
}

unsigned sig1(const unsigned data){
    return (rightShiftReg(data,17)^rightShiftReg(data,19)^(data>>10));
}
string sha256(const string &str)
{
    unsigned H[8]={0x6a09e667,0xbb67ae85,0x3c6ef372,0xa54ff53a,0x510e527f,0x9b05688c,0x1f83d9ab,0x5be0cd19};
    string strs=str;
    strs+=(char)(0x80);
    while ((strs.size()<<3) % 512 != 448)
    {
        strs+=(char)0;
    }
    for (int i=7;i>-1;i--)
    {
        strs+=(char)((str.size()<<3)>>(i<<3));
    }
    unsigned chunk[64]={0};
    for (int i=0;i<strs.size();i+=64)
    {
        for (int j=0;j<16;j++)
        {
            chunk[j]=((unsigned)strs[i+4*j]&0xff)<<24|
                        ((unsigned)strs[i+4*j+1]&0xff)<<16|
                        ((unsigned)strs[i+4*j+2]&0xff)<<8|
                        ((unsigned)strs[i+4*j+3]&0xff);
        }
        for (int t=16;t<64;t++)
        {
            chunk[t]=sig1(chunk[t-2])+chunk[t-7]+sig0(chunk[t-15])+chunk[t-16];
        }
        unsigned a=H[0];
        unsigned b=H[1];
        unsigned c=H[2];
        unsigned d=H[3];
        unsigned e=H[4];
        unsigned f=H[5];
        unsigned g=H[6];
        unsigned h=H[7];
        for (int t=0;t<64;t++)
        {
            unsigned sum1=h+Sig1(e)+ch(e,f,g)+K[t]+chunk[t];
            unsigned sum2=Sig0(a)+ma(a,b,c)+sum1;
            h=g;
            g=f;
            f=e;
            e=d+sum1;
            d=c;
            c=b;
            b=a;
            a=sum2;
        }
        H[0]+=a;
        H[1]+=b;
        H[2]+=c;
        H[3]+=d;
        H[4]+=e;
        H[5]+=f;
        H[6]+=g;
        H[7]+=h;
    }
    stringstream ss;
    ss<<hex<<H[0]<<H[1]<<H[2]<<H[3]<<H[4]<<H[5]<<H[6]<<H[7];
    ss>>strs;
    return strs;
}

int main()
{
    string str;
    cout << "plane:\n";
    for (int i=0;i<(16*1024/8);i++)
    {
        char tmp = 'a'+rand()%26;
        str+=tmp;
        cout << tmp;
    }cout << endl;

    clock_t a,b;
    a = clock();
    string out = sha256(str);
    b = clock();
    cout<<"result: "<<out<<endl;
    cout << endl;
    cout << dec <<"time: " << ((double)(b-a))/CLOCKS_PER_SEC << "s" << "  for: " << 16*1024 << "bit" << endl;
    cout << dec << "speed: " <<  ((double)(16*1024)/(1024*1024))/(((double)(b-a))/CLOCKS_PER_SEC) << "Mbps" << endl;
    return 0;
}