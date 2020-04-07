#include "base.h"

using namespace std;

void LruHitProcess() // if the replacement policy is LRU,and hit
{
    if(t_assoc == full_associative)
    {   
        if(!LRU_stack[0]->pop(current_line)){
            cout<<"pop wrong"<<endl;
            exit(0);
        }
    }
    else if(t_assoc == set_associative)
    {   
        
        if(!LRU_stack[current_set]->pop(current_line)){
            cout<<"pop wrong"<<endl;
            exit(0);
        }
    }
}

void LruUnhitSpace() // if the replacement policy is LRU,and not hit,but there has a spaceline
{
    if(t_assoc == full_associative)
    {
        LRU_stack[0]->push(current_line);
    }
    else if(t_assoc == set_associative)
    {
        LRU_stack[current_set]->push(current_line);

    }
}

void LruUnhitUnspace()
{
    if(t_assoc == full_associative)
    {
        unsigned long int j = LRU_stack[0]->push(current_line);

        current_line = j;
    }

    if(t_assoc == set_associative)
    {
        unsigned long int j = LRU_stack[current_set]->push(current_line);

        current_line = j + current_set*i_cache_set;
    }
}


