#include "base.h"

LRUStack::LRUStack(size_t _n, size_t _block_size){
    n = _n;
    block_size = _block_size;
    stack = new _bitset(n * block_size);
    stack->clear();
}
LRUStack::~LRUStack(){
    delete stack;
}
unsigned long int LRUStack:: push(unsigned long int index){
    
#ifndef NDEBUG
            cout <<"push "<< index << endl;
#endif // NDEBUG
    
    index %= n;
    unsigned long int temp = stack->read(0, block_size);
    unsigned long int res = stack->read(block_size*(n-1), block_size);
    if(temp == index){
        return res;
    }
    stack->leftShift(block_size);
    for(size_t p=block_size; p>0; p--){
        stack->set(p-1,index&1);
        index>>=1;
    }

    // cout<<"stack: ";
    // for(i=0;i<n;++i){
    //     cout<<stack->read(i*block_size, block_size)<<" ";
        
    // }
    // cout<<endl;
    
    return res;
}
bool LRUStack:: pop(unsigned long int index){
    
#ifndef NDEBUG
            cout <<"pop "<< index << endl;
#endif // NDEBUG
    index %= n;
    // cout<<"before stack: ";
    // for(i=0;i<n;++i){
    //     cout<<stack->read(i*block_size, block_size)<<" ";
        
    // }
    // cout<<endl;

    
    unsigned long int temp;
    size_t i;
    bool finded = true;
    for(i=0;i<n;++i){
        temp = index;
        finded = true;
        // cout<<"temp: "<<temp<<endl;
        for(size_t j=block_size*(i+1); j>block_size*i; j--){
            // cout<<i<<": "<<stack->test(j-1)<<" -- "<<(temp&1)<<endl;
            if(stack->test(j-1)!=(temp&1)){
                finded = false;
                break;
            }
            temp>>=1;
        }
        if(finded) break;
    }
    if(finded){
        stack->partilyLeftShift(block_size,(i+1)*block_size);
        for(size_t p=block_size; p>0; p--){
            stack->set(p-1,index&1);
            index>>=1;
        }
    }
    // cout<<"after stack: ";
    // for(i=0;i<n;++i){
    //     cout<<stack->read(i*block_size, block_size)<<" ";
        
    // }
    // cout<<endl;
    return finded;
}