#include "AES.h"
#include <iostream>
#include <string.h>
#include <stdio.h>
#include <fstream> 
#include <cstdlib>

// #include <iostream> 
#include <bitset> 
using namespace std;  

 
// 轮常数，密钥扩展中用到。（AES-128只需要10轮） 
word Rcon[10] = {0x01000000, 0x02000000, 0x04000000, 0x08000000, 0x10000000,  
         0x20000000, 0x40000000, 0x80000000, 0x1b000000, 0x36000000}; 
 
/** 
 * 将4个 byte 转换为一个 word 
 */ 
word Word(byte& k1, byte& k2, byte& k3, byte& k4) 
{ 
  word result(0x00000000); 
  word temp; 
  temp = kto_ulong(); // K1 
  temp <<= 24; 
  result |= temp; 
  temp = kto_ulong(); // K2 
  temp <<= 16; 
  result |= temp; 
  temp = kto_ulong(); // K3 
  temp <<= 8; 
  result |= temp; 
  temp = kto_ulong(); // K4 
  result |= temp; 
  return result; 
} 
 
/** 
 * 按字节 循环左移一位 
 * 即把[a0, a1, a2, a3]变成[a1, a2, a3, a0] 
 */ 
word RotWord(word& rw) 
{ 
  word high = rw << 8; 
  word low = rw >> 24; 
  return high | low; 
} 
 
/** 
 * 对输入word中的每一个字节进行S-盒变换 
 */ 
word SubWord(word& sw) 
{ 
  word temp; 
  for(int i=0; i<32; i+=8) 
  { 
    int row = sw[i+7]*8 + sw[i+6]*4 + sw[i+5]*2 + sw[i+4]; 
    int col = sw[i+3]*8 + sw[i+2]*4 + sw[i+1]*2 + sw[i]; 
    byte val = S_Box[row][col]; 
    for(int j=0; j<8; ++j) 
      temp[i+j] = val[j]; 
  } 
  return temp; 
} 
 
/** 
 * 密钥扩展函数 - 对128位密钥进行扩展得到 w[4*(Nr+1)] 
 */  
void KeyExpansion(byte key[4*Nk], word w[4*(Nr+1)]) 
{ 
  word temp; 
  int i = 0; 
  // w[]的前4个就是输入的key 
  while(i < Nk)  
  { 
    w[i] = Word(key[4*i], key[4*i+1], key[4*i+2], key[4*i+3]); 
    ++i; 
  } 
 
  i = Nk; 
 
  while(i < 4*(Nr+1)) 
  { 
    temp = w[i-1]; // 记录前一个word 
    if(i % Nk == 0) 
      w[i] = w[i-Nk] ^ SubWord(RotWord(temp)) ^ Rcon[i/Nk-1]; 
    else  
      w[i] = w[i-Nk] ^ temp; 
    ++i; 
  } 
} 
 
int main() 
{ 
  byte key[16] = {0x2b, 0x7e, 0x15, 0x16,  
          0x28, 0xae, 0xd2, 0xa6,  
          0xab, 0xf7, 0x15, 0x88,  
          0x09, 0xcf, 0x4f, 0x3c}; 
 
  word w[4*(Nr+1)]; 
 
  cout << "KEY IS: "; 
  for(int i=0; i<16; ++i) 
    cout << hex << key[i]to_ulong() << " "; 
  cout << endl; 
 
  KeyExpansion(key, w); 
  // 测试 
  for(int i=0; i<4*(Nr+1); ++i) 
    cout << "w[" << dec << i << "] = " << hex << w[i]to_ulong() << endl; 
 
  return 0; 
} 

