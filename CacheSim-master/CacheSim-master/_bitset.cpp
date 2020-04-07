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
_bitset::_bitset(){
    
}
_bitset::~_bitset(){
    delete[] bits;
}
void _bitset::set(size_t pos, bool target){
    if(pos>size||pos<0){
        cout<<"set "<<size<<" "<<pos<<endl;
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
        cout<<"test "<<size<<" "<<pos<<endl;
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
        char* temp = new char[n_block];
        memcpy(temp,bits+eleNum,(n_block-eleNum)*BLOCK_BYTES);
        memcpy(bits,temp,(n_block-eleNum)*BLOCK_BYTES);
        memset(bits+n_block-eleNum,0,eleNum*BLOCK_BYTES);
        delete[] temp;
    }
    if(bitNum!=0){
        char* temp = bits;
        for(;temp<bits+n_block-1;++temp){
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
        char* temp = new char[n_block];
        memcpy(temp,bits,(n_block-eleNum)*BLOCK_BYTES);
        memcpy(bits+eleNum,temp,(n_block-eleNum)*BLOCK_BYTES);
        memset(bits,0,eleNum*BLOCK_BYTES);
        delete[] temp;
    }
    if(bitNum!=0){
        char* temp = bits+n_block-1;
        for(;temp>bits;--temp){
            *temp = (*temp<<bitNum) | (*(temp-1)>>(BLOCK_BYTES-bitNum));

        }
        *temp <<=bitNum;
    }
}
void _bitset::clear(){
    memset(bits,0,n_block * BLOCK_BYTES);
}

unsigned long int _bitset::read(size_t base, size_t offset){
    unsigned long int res = 0;
    for(size_t i=base;i<base+offset;++i){
        res<<=1;
        if(test(i)){
            res+=1;
        }
    }
    return res;
}

void _bitset::partilyLeftShift(size_t n, size_t end){
    if(n>=end){
        for(size_t i=0;i<end;i++)
            set(i,false);
        return;
    }
    size_t eleNum = n / BLOCK_BYTES;
    size_t bitNum = n % BLOCK_BYTES;
    size_t end_eleNum = end / BLOCK_BYTES;
    size_t end_bitNum = end % BLOCK_BYTES;
    if(eleNum!=0){
        char* temp = new char[n_block];
        memcpy(temp,bits,(end_eleNum-eleNum)*BLOCK_BYTES);
        memcpy(bits+eleNum,temp,(end_eleNum-eleNum)*BLOCK_BYTES);
        memset(bits,0,eleNum*BLOCK_BYTES);
        delete[] temp;
    }

    if(bitNum!=0){
        char* temp = bits+end_eleNum-1;

        char t = (*temp<<bitNum) | (*(temp-1)>>(BLOCK_BYTES-bitNum));
        *temp = (*temp>>end_bitNum)<<end_bitNum | (t<<(BLOCK_BYTES-end_bitNum))>>(BLOCK_BYTES-end_bitNum);
        temp--;
        
        for(;temp>bits;--temp){
            *temp = (*temp<<bitNum) | (*(temp-1)>>(BLOCK_BYTES-bitNum));

        }
        *temp <<=bitNum;
    }

}