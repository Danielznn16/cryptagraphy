#include "SM3.h"
#include <iostream>
#include <string.h>
#include <stdio.h>
#include <fstream> 
#include <cstdlib>
#include <sstream>
// citation: I found help in http://www.oscca.gov.cn/sca/xxgk/2010-12/17/1002389/files/302a3ada057c4a73830536d03e683110.pdf
using namespace std;
typedef unsigned long long u64;

unsigned rightShiftReg(const unsigned &word,const int &num)
{
    return (word>>num)|(word<<(32-num));
}
unsigned T(unsigned j){
    return ((j<16)?0x79cc4519:0x7a879d8a);
}

unsigned FF(unsigned x,unsigned y,unsigned z,unsigned j){
    return ((j<16)?(x^y^z):((x&y)|(x&z)|(y&z)));
}

unsigned GG(unsigned x,unsigned y,unsigned z,unsigned j){
    return ((j<16)?(x^y^z):((x&y)|((~x)&z)));
}

unsigned P0(unsigned x){
    return (x^rightShiftReg(x,9)^rightShiftReg(x,17));
}

unsigned P1(unsigned x){
    return (x^rightShiftReg(x,15)^rightShiftReg(x,23));
}

string sm3(const string &str)
{
    unsigned H[8]={0x7380166f,0x4914b2b9,0x172442d7,0xda8a0600,0xa96f30bc,0x163138aa,0xe38dee4d,0xb0fb0e4e};
    string strs=str;
    strs+=(char)(0x80);
    unsigned W[68];
    unsigned WP[64];
    while ((strs.size()<<3) % 512 != 448)
    {
        strs+=(char)0;
    }
    for (int i=7;i>-1;i--)
    {
        strs+=(char)((str.size()<<3)>>(i<<3));
    }

    for (int i=0; i<strs.size(); i+=64){
        for(int j=0;j<16;j++)
        {
            W[j]=((unsigned)strs[i+4*j]&0xff)<<24|
                 ((unsigned)strs[i+4*j+1]&0xff)<<16|
                 ((unsigned)strs[i+4*j+2]&0xff)<<8|
                 ((unsigned)strs[i+4*j+3]&0xff);
        }
        for(int j = 16; j < 68; j++){
            W[j] = P1(W[j-16]^W[j-9]^rightShiftReg(W[j-3],15))^rightShiftReg(W[j-13],7)^W[j-6];
        }
        for(int j = 0; j < 64; j++){
            WP[j] = W[j]^W[j+4];
        }
        unsigned A=H[0];
        unsigned B=H[1];
        unsigned C=H[2];
        unsigned D=H[3];
        unsigned E=H[4];
        unsigned F=H[5];
        unsigned G=H[6];
        unsigned V=H[7];
        for(int j = 0; j < 64; j++){
            unsigned SS1 = rightShiftReg(rightShiftReg(A,12)+E+rightShiftReg(T(j),j),7);
            unsigned SS2 = SS1^rightShiftReg(A,12);
            unsigned TT1 = FF(A,B,C,j)+D+SS2+WP[j];
            unsigned TT2 = GG(E,F,G,j)+V+SS1+W[j];
            D = C;
            C = rightShiftReg(B,9);
            B = A;
            A = TT1;
            V = G;
            G = rightShiftReg(F,19);
            F = E;
            E = P0(TT2);
        }
        H[0]=H[0]^A;
        H[1]=H[1]^B;
        H[2]=H[2]^C;
        H[3]=H[3]^D;
        H[4]=H[4]^E;
        H[5]=H[5]^F;
        H[6]=H[6]^G;
        H[7]=H[7]^V;
    }
    stringstream ss;
    for(int i = 0; i < 8; i++)
        ss <<hex<< H[i];
    ss >> strs;
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
    string out = sm3(str);
    b = clock();
    cout<<"result: "<<out<<endl;
    cout << endl;
    cout << dec <<"time: " << ((double)(b-a))/CLOCKS_PER_SEC << "s" << "  for: " << 16*1024 << "bit" << endl;
    cout << dec << "speed: " <<  ((double)(16*1024)/(1024*1024))/(((double)(b-a))/CLOCKS_PER_SEC) << "Mbps" << endl;
    return 0;
}