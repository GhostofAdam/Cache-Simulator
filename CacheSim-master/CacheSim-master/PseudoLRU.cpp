#include "base.h"

using namespace std;

void PseudoLruHitProcess() // if the replacement policy is LRU,and hit
{   
    switch(current_line%8){
        case 0:
            pseudo_LRU_flag[current_set]->set(0,true);
            pseudo_LRU_flag[current_set]->set(1,true);
            pseudo_LRU_flag[current_set]->set(2,true);
        break;
        case 1:
            pseudo_LRU_flag[current_set]->set(0,true);
            pseudo_LRU_flag[current_set]->set(1,true);
            pseudo_LRU_flag[current_set]->set(2,false);
        break;
        case 2:
            pseudo_LRU_flag[current_set]->set(0,true);
            pseudo_LRU_flag[current_set]->set(1,false);
            pseudo_LRU_flag[current_set]->set(3,true);
        break;
        case 3:
            pseudo_LRU_flag[current_set]->set(0,true);
            pseudo_LRU_flag[current_set]->set(1,false);
            pseudo_LRU_flag[current_set]->set(3,false);
        break;
        case 4:
            pseudo_LRU_flag[current_set]->set(0,false);
            pseudo_LRU_flag[current_set]->set(4,true);
            pseudo_LRU_flag[current_set]->set(5,true);
        break;
        case 5:
            pseudo_LRU_flag[current_set]->set(0,false);
            pseudo_LRU_flag[current_set]->set(4,true);
            pseudo_LRU_flag[current_set]->set(5,false);
        break;
        case 6:
            pseudo_LRU_flag[current_set]->set(0,false);
            pseudo_LRU_flag[current_set]->set(4,false);
            pseudo_LRU_flag[current_set]->set(6,true);
        break;
        case 7:
            pseudo_LRU_flag[current_set]->set(0,false);
            pseudo_LRU_flag[current_set]->set(4,false);
            pseudo_LRU_flag[current_set]->set(6,false);
        break;
    }
}

void PseudoLruUnhitSpace() // if the replacement policy is LRU,and not hit,but there has a spaceline
{
    return;
}

void PseudoLruUnhitUnspace()
{
    int temp = 0;
    if(pseudo_LRU_flag[current_set]->test(0))
    {
        if(pseudo_LRU_flag[current_set]->test(1))
        {
            if(pseudo_LRU_flag[current_set]->test(2))
            {
                temp = 0;
            }
            else
            {
                temp = 1;
            }
        }
        else
        {
            if(pseudo_LRU_flag[current_set]->test(3))
            {
                temp = 2;
            }
            else
            {
                temp = 3;
            }
        }
    }
    else
    {
        if(pseudo_LRU_flag[current_set]->test(4))
        {
            if(pseudo_LRU_flag[current_set]->test(5))
            {
                temp = 4;
            }
            else
            {
                temp = 5;
            }
        }
        else
        {
            if(pseudo_LRU_flag[current_set]->test(6))
            {
                temp = 6;
            }
            else
            {
                temp = 7;
            }
        }
    }
    current_line = current_set*i_cache_set+temp;
}


