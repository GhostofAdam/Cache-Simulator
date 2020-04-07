#include <cstring>
#include <iostream>
using namespace std;

class _bitset{
    public:
        _bitset(size_t n);
        _bitset();
        ~_bitset();
        void set(size_t pos,bool target = true);
        bool test(size_t pos) const;
        void rightShift (size_t n);
        void partilyLeftShift(size_t n, size_t end);
        void leftShift (size_t n);
        void clear();
        unsigned long int read(size_t base, size_t offset);
    private:
        size_t size;
        char *bits;
        char mask;
        size_t BLOCK_BYTES = sizeof(char);
        size_t n_block;


};