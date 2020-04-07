#include "base.h"
using namespace std;

void DestroyCache(){
    while(*cache_item!=NULL){
        _bitset* p = *cache_item;
        free(p);
        cache_item++;
    }
    free(cache_item);
    while(*LRU_stack!=NULL){
        LRUStack* p = *LRU_stack;
        free(p);
        LRU_stack++;
    }
    free(LRU_stack);
    while(*pseudo_LRU_flag!=NULL){
        _bitset* p = *pseudo_LRU_flag;
        free(p);
        pseudo_LRU_flag++;
    }
    free(pseudo_LRU_flag);
}