#include "SHA3.h"
#include <iostream>
#include <string.h>
#include <stdio.h>
#include <fstream> 
#include <cstdlib>
#include <sstream>
#include <map>
#include <vector>
#include <ctime>

using namespace std;

u64 rightShiftReg(const u64 &word,const int &num)
{
    return (word>>num)|(word<<(64-num));
}
u64 leftShiftReg(const u64 &word,const int &num)
{
    return (word<<num)|(word>>(64-num));
}


void KECCAK_f(u64 S[5][5])
{
    for ( int i = 0; i < 24; i++ )
    {
        // theta function
        u64 C[5],D[5];
        for(int j = 0; j < 5; j++){
            C[j] = S[j][0]^S[j][1]^S[j][2]^S[j][3]^S[j][4];
        }
        for(int j = 0; j < 5; j++){
            D[j] = C[(j+4)%5]^leftShiftReg(C[(j+1)%5],1);
        }
        for(int j = 0; j < 5; j++)
            for(int k =0; k < 5; k++){
                S[j][k] = S[j][k]^D[j];
            }

        // // ro function
        // u64 A_[5][5] = {};
        // for(int j = 0; j < 5; j++)
        //     for(int k =0; k < 5; k++)
        //         A_[j][k] = leftShiftReg(S[j][k],ro_off[j][k]);

        // // pi function
        // for(int j = 0; j < 5; j++)
        //     for(int k =0; k < 5; k++)
        //         S[j][k] = A_[(j+3*k)%5][j];

        // combine ro and pi
        u64 A_[5][5] = {};
        for(int j = 0; j < 5; j++)
            for(int k =0; k < 5; k++)
                A_[k][((j<<1)+k*3)%5] = leftShiftReg(S[j][k],ro_off[j][k]);

        // Chi function
        for(int j = 0; j < 5; j++)
            for(int k =0; k < 5; k++)
                S[j][k] = A_[j][k]^((~A_[(j+1)%5][k])&A_[(j+2)%5][k]);

        //rc function
        S[0][0] = S[0][0]^RC[i];
    }
}

void padding(unsigned char* message, int &len)
{
    int q = (1088>>3) - ((len>>3)%(1088>>3));
    if(q){
        if(q==1){
            message[len>>3] = 0x86;
            len+=8;
        }else{
            message[(len)>>3] = 0x06;
            len+=8;
            for(int j = 0; j < q-2;j++){
                message[(len)>>3] =  0x00;
                len+=8;
            }
            message[len>>3] = 0x80;
            len+=8;
        }
    }
}

void SHA3(unsigned char* plane, const int len, u64* cypher)
{
    u64 S[5][5] = {0};
    for (int i = 0; i < (len)/1088;i++){
        unsigned char* block;
        for ( int y = 0; y < 5; y++ )
            for ( int x = 0; x < 5; x++ )
                if ( x + 5 * y < 17 ){
                    block = &plane[i*(1088>>3)+(x+5*y)*8];
                    // swap(block[0],block[7]);
                    // swap(block[1],block[6]);
                    // swap(block[2],block[5]);
                    // swap(block[3],block[4]);
                    S[x][y] ^= *((u64*)block);
                }
        KECCAK_f(S);
    }
    int cnt = 0;
    for ( int y = 0, k = 0; y < 5; y++ )
    {
        for ( int x = 0; x < 5; x++ )
        {
            if ( x + 5 * y < 17 && k < 4 )
            {
                cypher[cnt++] = S[x][y];
                k++;
            }
        }
    }
}

int main()
{
    unsigned char *plane = new unsigned char[2*1024+512];
    unsigned char* cypher = new unsigned char[(256>>3)+1];
    cypher[256>>3] = '\0';
    for (int i=0;i<16*1024/8;i++)
    {
        unsigned char tmp = 'a'+rand()%26;
        cout << tmp;
        plane[i] = tmp;
    }cout << endl;
    int len = 16*1024;
    // int len = 0;
    clock_t a,b;
    a = clock();
    padding(plane,len);
    SHA3(plane,len,(u64*)cypher);
    b = clock();
    cout << "SHA3_256:";
    for(int i = 0; i < (256>>3); i++){
        if((unsigned int)cypher[i]<16)
            cout << '0';
        cout <<hex<< (unsigned int)cypher[i];
    }
    cout << endl;
    cout << dec <<"time: " << ((double)(b-a))/CLOCKS_PER_SEC << "s" << "  for: " << 16*1024 << "bit" << endl;
    cout << dec << "speed: " <<  ((double)(16*1024)/(1024*1024))/(((double)(b-a))/CLOCKS_PER_SEC) << "Mbps" << endl;

    return 0;
}