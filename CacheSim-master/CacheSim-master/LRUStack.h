#include "_bitset.h"
 
class LRUStack{
    private:
        _bitset stack;
        size_t block_size;
        size_t n;
    public:
        LRUStack(size_t n, size_t block_size);
        ~LRUStack();
        unsigned long int  push(unsigned long int );
        bool pop(unsigned long int);

};