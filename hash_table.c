//
//  hash_table.c
//  hashtable_c
//
//  Created by Sriram Sridhar on 2/18/17.
//  Copyright © 2017 Sriram Sridhar. All rights reserved.
//

#include "hash_table.h"
#include <math.h>

Node *hash[HASH_TABLE_SIZE+1] = {0};

Node *hash2[HASH_TABLE_SIZE+1] = {0};

Node* newNode(long key)
{
    Node *p = (Node *) calloc(1, sizeof(Node));
    p->key = key;
    return p;
}

void Pearson16(unsigned char *x, size_t len, unsigned long *hex, size_t hexlen)
{
    size_t i;
    size_t j;
    unsigned char h;
    unsigned char hh[8];
    static const unsigned char T[256] = {
        // 256 values 0-255 in any (random) order suffices
        98,  6, 85,150, 36, 23,112,164,135,207,169,  5, 26, 64,165,219, //  1
        61, 20, 68, 89,130, 63, 52,102, 24,229,132,245, 80,216,195,115, //  2
        90,168,156,203,177,120,  2,190,188,  7,100,185,174,243,162, 10, //  3
        237, 18,253,225,  8,208,172,244,255,126,101, 79,145,235,228,121, //  4
        123,251, 67,250,161,  0,107, 97,241,111,181, 82,249, 33, 69, 55, //  5
        59,153, 29,  9,213,167, 84, 93, 30, 46, 94, 75,151,114, 73,222, //  6
        197, 96,210, 45, 16,227,248,202, 51,152,252,125, 81,206,215,186, //  7
        39,158,178,187,131,136,  1, 49, 50, 17,141, 91, 47,129, 60, 99, //  8
        154, 35, 86,171,105, 34, 38,200,147, 58, 77,118,173,246, 76,254, //  9
        133,232,196,144,198,124, 53,  4,108, 74,223,234,134,230,157,139, // 10
        189,205,199,128,176, 19,211,236,127,192,231, 70,233, 88,146, 44, // 11
        183,201, 22, 83, 13,214,116,109,159, 32, 95,226,140,220, 57, 12, // 12
        221, 31,209,182,143, 92,149,184,148, 62,113, 65, 37, 27,106,166, // 13
        3, 14,204, 72, 21, 41, 56, 66, 28,193, 40,217, 25, 54,179,117, // 14
        238, 87,240,155,180,170,242,212,191,163, 78,218,137,194,175,110, // 15
        43,119,224, 71,122,142, 42,160,104, 48,247,103, 15, 11,138,239  // 16
    };
    
    for (j = 0; j < 8; ++j)
    {
        h = T[(x[0] + j) % 256];
        for (i = 1; i < len; ++i) {
            h = T[h ^ x[i]];
        }
        hh[j] = h;
    }
    
    snprintf((char *)hex, hexlen, "%02X%02X%02X%02X%02X%02X%02X%02X",
             hh[0], hh[1], hh[2], hh[3],
             hh[4], hh[5], hh[6], hh[7]);
}


unsigned long calculate_hash_key( char *inKey)
{
    unsigned long key = 0;
    
    if(!inKey)
    {
        printf("\nKey is NULL");
        return 0;
    }
    
    if(strlen(inKey) == 1)
    {
        Pearson16((unsigned char *)inKey,strlen(inKey),&key,strlen(inKey)+1);
    }
    else
    {
        Pearson16((unsigned char *)inKey,strlen(inKey),&key,strlen(inKey));
    }
    
    //printf("\n Key=%lu\n",key);
    
    key = key % HASH_TABLE_SIZE +1;
    
    return key;
}

unsigned long calculate_hash_key2( long inKey)
{
    unsigned long key = 0;
    
    if(!inKey)
    {
        printf("\nKey is NULL");
        return 0;
    }
    
    key = (inKey);
    //printf("\n Key=%lu\n",key);
    
    key = key % HASH_TABLE_SIZE +1;
    
    return key;
}

long largest_value = 0;

int add_entry_to_hash_table(long inKey, long linkedlist_index, long value, unsigned char table_id)
{
    //return 1 if inKey is found; 0, otherwise
    //insert a new key in its appropriate list so list is in order
    unsigned long k = calculate_hash_key2(inKey);
    Node *curr = NULL;
    Node *prev = NULL;
    Node *np = NULL;
    Node **hash_table = NULL;
    
    if(table_id == PLAYER_ID_HASH_TABLE)
    {
        hash_table = hash;
    }
    else if(table_id == PLAYER_SCORE_HASH_TABLE)
    {
        hash_table = hash2;
    }
    
    //printf("\nk =%ld\n",k);
    
    curr = hash_table[k];
        
    while ((curr != NULL) && (inKey > curr->key))
    {
        prev = curr;
        curr = curr->next;
    }
    
    //not found; inKey is a new key; add it so list is in order
    np = newNode(inKey);
    
    np->next = curr;
    np->value = value;
    
    if(value > largest_value)
    {
        largest_value = value;
    }
    
    np->linkedlist_index = linkedlist_index;
    
    if (prev == NULL)
    {
        hash_table[k] = np;
    }
    else
    {
        prev->next = np;
    }
    
    return 0;
} //end search


void printList(Node *top)
{
    while (top != NULL)
    {
        //printf("%s dup_keys-%lu; ", top->key_name,top->duplicate_keys);
        printf("%ld; ", top->key);
        top = top->next;
    }
} //end printList


void print_all_hash_table_entries(unsigned char table_id)
{
    unsigned long h;
    Node **hash_table = NULL;
    
    if(table_id == PLAYER_ID_HASH_TABLE)
    {
        hash_table = hash;
    }
    else if(table_id == PLAYER_SCORE_HASH_TABLE)
    {
        hash_table = hash2;
    }
    
    for (h = 1; h <= HASH_TABLE_SIZE; h++)
    {
        if (hash_table[h] != NULL)
        {
            printf("\nhash[%lu]: ", h);
            printList(hash_table[h]);
        }
    }
}

unsigned long find_total_number_of_hash_table_entries(unsigned char table_id)
{
    unsigned long count = 0;
    unsigned long h;
    Node **hash_table = NULL;
    
    if(table_id == PLAYER_ID_HASH_TABLE)
    {
        hash_table = hash;
    }
    else if(table_id == PLAYER_SCORE_HASH_TABLE)
    {
        hash_table = hash2;
    }

    for (h = 1; h <= HASH_TABLE_SIZE; h++)
    {
        if (hash_table[h] != NULL)
        {
            count++;
        }
    }
    
    return count;
}

unsigned long find_max_number_of_collisions(unsigned char table_id)
{
    unsigned long count = 0;
    unsigned long h;
    Node **hash_table = NULL;
    
    if(table_id == PLAYER_ID_HASH_TABLE)
    {
        hash_table = hash;
    }
    else if(table_id == PLAYER_SCORE_HASH_TABLE)
    {
        hash_table = hash2;
    }
    
    for (h = 1; h <= HASH_TABLE_SIZE; h++)
    {
        if (hash_table[h] != NULL)
        {
            unsigned long temp_count = 0;
            Node *top = hash_table[h];
            
            while (top != NULL)
            {
                top = top->next;
                temp_count++;
            }
            if (temp_count > count)
            {
               count = temp_count;
            }
        }
    }
    
    return count;
}

long find_entry_in_hash_table(long player_id,unsigned char table_id)
{
    int found = 0;
    long hash_key = player_id;
    unsigned long h = calculate_hash_key2(hash_key);
    long linkedlist_index = 0;
    Node *top = NULL;
    Node **hash_table = NULL;

    if(table_id == PLAYER_ID_HASH_TABLE)
    {
        hash_table = hash;
    }
    else if(table_id == PLAYER_SCORE_HASH_TABLE)
    {
        hash_table = hash2;
    }

    if (hash_table[h] != NULL)
    {
        top = hash_table[h];
        
        while (top != NULL)
        {
            if(hash_key == top->key)
            {
                found = 1;
                break;
            }
            top = top->next;
        }
    }
    if(found && top)
    {
        linkedlist_index = top->linkedlist_index;
    }
    
    return linkedlist_index;
}

long find_entry_in_hash_table2(long percentile,long *key, unsigned char table_id)
{
    int found = 0;
    long hash_key = lround((largest_value * percentile)/100);
    unsigned long h = calculate_hash_key2(hash_key);
    long value = 0;
    Node *top = NULL;
    Node **hash_table = NULL;
    
    if(table_id == PLAYER_ID_HASH_TABLE)
    {
        hash_table = hash;
    }
    else if(table_id == PLAYER_SCORE_HASH_TABLE)
    {
        hash_table = hash2;
    }
    
    if (hash_table[h] != NULL)
    {
        top = hash_table[h];
        
        while (top != NULL)
        {
            if(hash_key == top->key)
            {
                found = 1;
                break;
            }
            top = top->next;
        }
    }
    if(found && top)
    {
        *key = top->key;
        value = top->value;
    }
    
    return value;
}


long find_nearest_entry_in_hash_table(long key,unsigned char table_id)
{
    struct timeval  start_time = {0};
    struct timeval  end_time = {0};
    struct timeval result = {0};
    
   //come up with an algorithm to search the hashtable effectively
   //for now just do a linear search
    long index = 0;
    long linkedlist_index = 0;
    
    {
        gettimeofday(&start_time, NULL);
 
        for (index = key; index > 0; index--)
        {
            linkedlist_index = find_entry_in_hash_table(index,table_id);
            if(linkedlist_index)
            {
                break;
            }
        }
        gettimeofday(&end_time, NULL);
        timersub(&end_time,&start_time,&result);
        if(result.tv_usec >=2000)
        {
        printf("\nfound player_id=%ld, linkedlist_index = %ld, It took %ld.%06ld useconds to complete the search.\n",
               key,
               linkedlist_index,
               result.tv_sec,
               result.tv_usec);
        }
    }
    
    return linkedlist_index;
}
