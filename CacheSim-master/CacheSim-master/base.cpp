/** \brief definition of almost all globle variables.
 *
 * \param
 * \param
 * \return
 *
 */

#include "base.h"

/******************************************/
unsigned int long i_cache_size = 0; //cache size
unsigned int long i_cache_line_size = 0; //cacheline size
unsigned int long i_cache_set = 0; //cache set

unsigned int long i_num_line = 0; //How many lines of the cache.
unsigned int long i_num_set = 0; //How many sets of the cache.

ASSOC t_assoc = direct_mapped; //associativity method,default direct_mapped
REPLACE t_replace = none; //replacement policy,default Random
WRITE t_write = write_back; //write policy,default write_back
DIS t_write_dis = write_with_distribution; // wirte distribution
/******************************************/

/******************************************/
short unsigned int bit_block = 0; //How many bits of the block.
short unsigned int bit_line = 0; //How many bits of the line.
short unsigned int bit_tag = 0; //How many bits of the tag.
short unsigned int bit_set = 0; //How many bits of the set.
/******************************************/
unsigned short int bit_data;
unsigned short int p_valid;
unsigned short int p_hit;
unsigned short int p_dirty;

/******************************************/
unsigned long int i_num_access = 0; //Number of cache access
unsigned long int i_num_load = 0; //Number of cache load
unsigned long int i_num_store = 0; //Number of cache store
unsigned long int i_num_space = 0; //Number of space line

unsigned long int i_num_hit = 0; //Number of cache hit
unsigned long int i_num_load_hit = 0; //Number of load hit
unsigned long int i_num_store_hit = 0; //Number of store hit
unsigned long int i_num_store_mem = 0;

float f_ave_rate = 0.0; //Average cache hit rate
float f_load_rate = 0.0; //Cache hit rate for loads
float f_store_rate = 0.0; //Cache hit rate for stores
/******************************************/

_bitset** cache_item; // 
LRUStack** LRU_stack; //For LRU policy's priority
_bitset** pseudo_LRU_flag;//For Preudo LRU
unsigned long int current_line = 0; // The line num which is processing
unsigned long int current_set = 0; // The set num which is processing
unsigned long int i=0,j=0; //For loop
unsigned long int temp = 0; //A temp varibale


/*************************************************
// functions.cpp
// 1.Defination of almost all functions.
*************************************************/

using namespace std;

bool GetHitNum(char *address,ofstream& file)
{
    bool is_store = false;
    bool is_load = false;
    bool is_space = false;
    bool hit = false;
    //cout<< *address<<endl;
    switch(address[0])
    {
    case 'w':
        is_store = true;
        break;

    case 'r':
        is_load = true;
        break;

    //case ' ':break; //Waring if a line has nothing,the first of it is a '\0' nor a ' '
    case '\0':
        is_space = true;
        break; //In case of space lines

    default:
        cout << "The address[0] is:" <<address[0] << endl;
        cout << "ERROR IN JUDGE!" << endl;
        return false;
    }

    temp = strtoul(address+2,NULL,16);
    bitset<64> flags(temp); // flags if the binary of address
#ifndef NDEBUG
    // cout << flags << endl;
#endif // NDEBUG
    hit = IsHit(flags);
#ifndef NLOG
    if(hit){
        file<<"Hit"<<endl;
    }
    else{
        file<<"Miss"<<endl;
    }
#endif
    if(hit && is_load) // 命中，读操作
    {
        i_num_access++;
        i_num_load++;
        i_num_load_hit++;
        i_num_hit++;
#ifndef NDEBUG
        cout << "Loading" << endl;
        cout << "Hit" << endl;
        cout << "Read from Cache!" << endl;
#endif // NDEBUG

        
        if(t_replace == LRU)
        {
            LruHitProcess();
        }
        else if(t_replace == PseudoLRU){
            PseudoLruHitProcess();
        }
    }
    else if(hit && is_store) // 命中，写操作
    {
        i_num_access++;
        i_num_store++;
        i_num_store_hit++;
        i_num_hit++;
#ifndef NDEBUG
        cout << "Storing" << endl;
        cout << "Hit" << endl;
        cout << "Write to Cache" << endl;
#endif // NDEBUG
        cache_item[current_line]->set(p_dirty,true); //设置dirty为true

        if(t_replace == LRU)
        {
            LruHitProcess();
        }
         else if(t_replace == PseudoLRU){
            PseudoLruHitProcess();
        }
        if(t_write==write_through){
            i_num_store_mem++;
        }
    }
    else if((!hit) && is_load) // 没命中，读操作
    {
        i_num_access++;
        i_num_load++;
#ifndef NDEBUG
        cout << "Loading" << endl;
        cout << "Not Hit" << endl;
#endif // NDEBUG
        GetRead(flags); // read data from memory
#ifndef NDEBUG
        cout << "Read from Cache!" << endl;
#endif // NDEBUG

        // if(t_replace == LRU)
        // {
        //     LruUnhitSpace();
        // }
        // else if(t_replace == PseudoLRU){
        //     PseudoLruUnhitSpace();
        // }
    }
    else if((!hit) && is_store) // 没命中，写操作
    {
        i_num_access++;
        i_num_store++;
#ifndef NDEBUG
        cout << "Storing" << endl;
        cout << "Not Hit" << endl;
#endif // NDEBUG
        if(t_write_dis == write_with_distribution)
            GetRead(flags); // read data from memory
#ifndef NDEBUG
        cout << "Write to Cache" << endl;
#endif // NDEBUG
        cache_item[current_line]->set(p_dirty,true); //设置dirty为true

        // if(t_replace == LRU)
        // {
        //     LruUnhitSpace();
        // }
        // else if(t_replace == PseudoLRU){
        //     PseudoLruUnhitSpace();
        // }
        if(t_write==write_through){
            i_num_store_mem++;
        }
    }
    else if(is_space)
    {
        i_num_space++;
    }
    else
    {
        cerr << "Something ERROR" << endl;
        return false;
    }

    if(i_num_space != 0)
    {
#ifndef NDEBUG
        // cout << "There have " << i_num_space << " space lines" << endl;
#endif // NDEBUG
    }

    return true;
}

bool IsHit(bitset<64> flags)
{
    bool ret = false;

    if(t_assoc == direct_mapped)
    {
        bitset<64> flags_line; // a temp variable

        for(j=0,i=(bit_block); i<(bit_block+bit_line); j++,i++) //判断在cache多少行
        {
            flags_line[j] = flags[i];
        }

        current_line = flags_line.to_ulong();
        assert(cache_item[current_line]->test(p_valid)==true);

        if(cache_item[current_line]->test(p_hit)==true) //判断hit位是否为真
        {
            ret = true;

            for(i=63,j=bit_tag-1; i>(63ul-bit_tag); i--,j--) //判断标记是否相同,i:address,j:cache
            {
                if(flags[i] != cache_item[current_line]->test(j))
                {
                    ret = false;
                    break;
                }
            }
        }
    }
    else if(t_assoc == full_associative)
    {
        for(temp=0; temp<i_num_line; temp++)
        {
            if(cache_item[temp]->test(p_hit)==true) //判断hit位是否为真
            {
                ret = true;

                for(i=63,j=bit_tag-1; i>(63ul-bit_tag); i--,j--) //判断标记是否相同,i:address,j:cache
                {
                    if(flags[i] != cache_item[temp]->test(j))
                    {
                        ret = false;
                        break;
                    }
                }
            }

            if(ret == true)
            {
                current_line = temp;
                break;
            }
        }
    }
    else if(t_assoc == set_associative)
    {
        bitset<64> flags_set;

        for(j=0,i=(bit_block); i<(bit_block+bit_set); j++,i++) //判断在cache多少组
        {
            flags_set[j] = flags[i];
        }

        current_set = flags_set.to_ulong();

        for(temp=(current_set*i_cache_set); temp<((current_set+1)*i_cache_set); temp++)
        {
            if(cache_item[temp]->test(p_hit)==true) //判断hit位是否为真
            {
                ret = true;

                for(i=63,j=bit_tag-1; i>(63ul-bit_tag); i--,j--) //判断标记是否相同,i:address,j:cache
                {
                    if(flags[i] != cache_item[temp]->test(j))
                    {
                        ret = false;
                        break;
                    }
                }
            }

            if(ret == true)
            {
                current_line = temp;
                break;
            }
        }
    }

    return ret;
}

void GetRead(bitset<64> flags)
{
    if(t_assoc == direct_mapped)
    {
        if(cache_item[current_line]->test(p_hit) == false) //hit is false
        {
#ifndef NDEBUG
            cout << "Read from Main Memory to Cache!" << endl;
#endif // NDEBUG

            for(i=63,j=bit_tag-1; i>(63ul-bit_tag); i--,j--) //设置标记
            {
                cache_item[current_line]->set(j,flags[i]);
                assert(j>=0);
            }

            cache_item[current_line]->set(p_hit, true); //设置hit位为true
        }
        else
        {
            GetReplace(flags);
        }
    }
    else if(t_assoc == full_associative)
    {
        bool space = false;

        for(temp=0; temp<i_num_line; temp++)
        {
            if(cache_item[temp]->test(p_hit) == false) //find a space line
            {
                space = true;
                break;
            }
        }

        if(space == true)
        {
            current_line = temp; // 此处，temp不需减1，因为一旦发现空行，上面for循环会break，此时temp尚未++
#ifndef NDEBUG
            cout << "Read from Main Memory to Cache!" << endl;
#endif // NDEBUG

            for(i=63,j=bit_tag-1; i>(63ul-bit_tag); i--,j--) //设置标记
            {
                cache_item[current_line]->set(j, flags[i]);
                assert(j>=0);
            }

            cache_item[current_line]->set(p_hit, true); //设置hit位为true.

            if(t_replace == LRU)
            {
                LruUnhitSpace();
            }
            else if(t_replace == PseudoLRU){
                PseudoLruUnhitSpace();
            }
        }
        else
        {
            GetReplace(flags);
        }
    }
    else if(t_assoc == set_associative)
    {
        bool space = false;

        for(temp=(current_set*i_cache_set); temp<((current_set+1)*i_cache_set); temp++)
        {
            if(cache_item[temp]->test(p_hit) == false) //find a space line
            {
                space = true;
                break;
            }
        }

        if(space == true)
        {
            current_line = temp; // 此处，temp不需减1，因为一旦发现空行，上面for循环会break，此时temp尚未++
#ifndef NDEBUG
            cout << "Read from Main Memory to Cache!" << endl;
#endif // NDEBUG

            for(i=63,j=bit_tag-1; i>(63ul-bit_tag); i--,j--) //设置标记
            {
                cache_item[current_line]->set(j, flags[i]);
                assert(j>=0);
            }

            cache_item[current_line]->set(p_hit, true); //设置hit位为true.

            if(t_replace == LRU)
            {
                LruUnhitSpace();
            }
            else if(t_replace == PseudoLRU){
                PseudoLruUnhitSpace();
            }
        }
        else
        {
            GetReplace(flags);
        }
    }
}

void GetReplace(bitset<64> flags)
{
    if(t_assoc == direct_mapped)
    {
    }
    else if(t_assoc == full_associative)
    {
        if(t_replace == Random)
        {
            current_line = rand()/(RAND_MAX/i_num_line+1); // a random line in(0,i_num_line-1)
        }
        else if(t_replace == LRU)
        {
            LruUnhitUnspace();
        }
        else if(t_replace == PseudoLRU){
            PseudoLruUnhitUnspace();
        }
    }
    else if(t_assoc == set_associative) // 从本组中任选一行，进行替换
    {
        if(t_replace == Random)
        {
            temp = rand()/(RAND_MAX/i_cache_set+1); // a random line in(0,i_cache_set-1)
            current_line = current_set*i_cache_set+temp; // a random line in current_set
        }
        else if(t_replace == LRU)
        {
            LruUnhitUnspace();
        }
        else if(t_replace == PseudoLRU){
            PseudoLruUnhitUnspace();
        }
    }

    if(cache_item[current_line]->test(p_dirty) == true) //dirty位必须为1才写入
    {
        GetWrite(); //写入内存
    }

#ifndef NDEBUG
    cout << "Read from Main Memory to Cache: " << endl;
#endif // NDEBUG

    for(i=63,j=bit_tag-1; i>(63ul-bit_tag); i--,j--) //设置标记
    {
        cache_item[current_line]->set(j, flags[i]);
        assert(j>=0);
    }

    cache_item[current_line]->set(p_hit, true); //设置hit位为true
}

void GetWrite() //写入内存
{
#ifndef NDEBUG
    cout << "Writing to the Main Memory!" <<endl;
#endif
    cache_item[current_line]->set(p_dirty, false); //设置dirty为false
    cache_item[current_line]->set(p_hit, false); //设置hit为false
    if(t_write==write_back){
        i_num_store_mem++;
    }
}

void GetHitRate()
{
    assert(i_num_access != 0);
    assert(i_num_load != 0);
    assert(i_num_store != 0);
    f_ave_rate = ((double)i_num_hit)/i_num_access; //Average cache hit rate
    f_load_rate = ((double)i_num_load_hit)/i_num_load; //Cache hit rate for loads
    f_store_rate = ((double)i_num_store_hit)/i_num_store; //Cache hit rate for stores
}

