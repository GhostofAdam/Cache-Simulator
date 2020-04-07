#include "base.h"

using namespace std;

void CreateCache()
{
    temp = i_num_line;
#ifndef NDEBUG

    for(i=0; i<100; i++)
    {
        cout << cache_item[i] << endl;
    }

#endif // NDEBUG
    cache_item = new _bitset* [i_num_line];
    for(i=0; i<temp; i++)
    {
        cache_item[i] = new _bitset(bit_data);
        cache_item[i]->set(p_valid, true);
    }

#ifndef NDEBUG

    for(i=0; i<100; i++)
    {
        cout << cache_item[i] << endl;
    }

#endif // NDEBUG

    if(t_replace == LRU){
         if (t_assoc == full_associative){
            int n = log2(i_num_line);
            LRU_stack = new LRUStack*[1];
            LRU_stack[0] = new LRUStack(i_num_line,n); 
        }
        else if(t_assoc == set_associative){
            int n = log2(i_cache_set);
            LRU_stack = new LRUStack*[i_cache_set];
            for(int i=0;i<i_num_set;i++){
                LRU_stack[i] = new LRUStack(i_cache_set,n); 
            }
        }
    }
    else if(t_replace = PseudoLRU){
         if (t_assoc == full_associative){
            int n = log2(i_num_line);
            pseudo_LRU_flag = new _bitset*[1];
            pseudo_LRU_flag[0] = new _bitset(n-1); 
        }
        else if(t_assoc == set_associative){
            int n = log2(i_cache_set);
            pseudo_LRU_flag = new _bitset*[i_cache_set];
            for(int i=0;i<i_num_set;i++){
                pseudo_LRU_flag[i] = new _bitset(n-1); 
            }
        }
    }
}
