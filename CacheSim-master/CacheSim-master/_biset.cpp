#include "_bitset.h"

_bitset::_bitset(size_t n){
    size = n;
    size_t free_bits = (BLOCK_BYTES-size%BLOCK_BYTES)%BLOCK_BYTES;
    n_block = size/BLOCK_BYTES+free_bits;
    bits = new char[n_block];
    clear();
    mask = ~(char)0;
    mask >>=free_bits;
}
_bitset::~_bitset(){
    delete[] bits;
}
void _bitset::set(size_t pos, bool target){
    if(pos>size||pos<0){
        cout<<"wrong pos!"<<endl;
        exit(0);
    }
    size_t eleNum = pos / BLOCK_BYTES;
    size_t bitNum = pos % BLOCK_BYTES;
    char temp_mask = (char)1;
    temp_mask<<=bitNum;
    if(target){
        bits[eleNum] |= temp_mask;
    }
    else{
        bits[eleNum] &= ~temp_mask;
    }
}
bool _bitset::test(size_t pos) const{
    if(pos>size||pos<0){
        cout<<"wrong pos!"<<endl;
        exit(0);
    }
    size_t eleNum = pos / BLOCK_BYTES;
    size_t bitNum = pos % BLOCK_BYTES;
    char temp_mask = (char)1;
    temp_mask<<=bitNum;
    return bits[eleNum] &temp_mask;

}
void _bitset:: rightShift(size_t n){
    if(n>=size){
        clear();
        return;
    }
    size_t eleNum = n / BLOCK_BYTES;
    size_t bitNum = n % BLOCK_BYTES;
    if(eleNum!=0){
        char* temp = new char[size];
        memcpy(temp,bits+eleNum,(size-eleNum)*BLOCK_BYTES);
        memcpy(bits,temp,(size-eleNum)*BLOCK_BYTES);
        memset(bits+size-eleNum,0,eleNum*BLOCK_BYTES);
        delete[] temp;
    }
    if(bitNum!=0){
        char* temp = bits;
        for(;temp<bits+size-1;++temp){
            *temp = (*temp>>bitNum)|(*(temp+1)<<(BLOCK_BYTES-bitNum));

        }
        *temp >>=bitNum;
    }
}
void _bitset:: leftShift(size_t n){
    if(n>=size){
        clear();
        return;
    }
    size_t eleNum = n / BLOCK_BYTES;
    size_t bitNum = n % BLOCK_BYTES;
    if(eleNum!=0){
        char* temp = new char[size];
        memcpy(temp,bits,(size-eleNum)*BLOCK_BYTES);
        memcpy(bits+eleNum,temp,(size-eleNum)*BLOCK_BYTES);
        memset(bits,0,eleNum*BLOCK_BYTES);
        delete[] temp;
    }
    if(bitNum!=0){
        char* temp = bits+size-1;
        for(;temp>bits;--temp){
            *temp = (*temp<<bitNum) | (*(temp-1)>>(BLOCK_BYTES-bitNum));

        }
        *temp <<=bitNum;
    }
}
void _bitset::clear(){
    memset(bits,0,size * BLOCK_BYTES);
}