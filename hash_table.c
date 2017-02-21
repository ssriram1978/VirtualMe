//
//  hash_table.c
//  hashtable_c
//
//  Created by Sriram Sridhar on 2/18/17.
//  Copyright © 2017 Sriram Sridhar. All rights reserved.
//

#include "hash_table.h"

Node *hash[HASH_TABLE_SIZE+1] = {0};


Node* newNode(long key)
{
    Node *p = (Node *) calloc(1, sizeof(Node));
    p->player_id = key;
    
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


int add_entry_to_hash_table(long inKey)
{
    //return 1 if inKey is found; 0, otherwise
    //insert a new key in its appropriate list so list is in order
    unsigned long k = calculate_hash_key2(inKey);
    Node *curr = hash[k];
    Node *prev = NULL;
    Node *np = NULL;
    
    while (curr != NULL && (inKey > curr->player_id))
    {
        prev = curr;
        curr = curr->next;
    }
    
    if (curr != NULL && (inKey == curr->player_id))
    {
        return 1; //found
    }
    
    //not found; inKey is a new key; add it so list is in order
    np = newNode(inKey);
    
    np->next = curr;
    
    if (prev == NULL)
    {
        hash[k] = np;
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
        printf("%ld; ", top->player_id);
        top = top->next;
    }
} //end printList


void print_all_hash_table_entries()
{
    unsigned long h;
    
    for (h = 1; h <= HASH_TABLE_SIZE; h++)
    {
        if (hash[h] != NULL)
        {
            printf("\nhash[%lu]: ", h);
            printList(hash[h]);
        }
    }
}

unsigned long find_total_number_of_hash_table_entries()
{
    unsigned long count = 0;
    unsigned long h;
    for (h = 1; h <= HASH_TABLE_SIZE; h++)
    {
        if (hash[h] != NULL)
        {
            count++;
        }
    }
    
    return count;
}

unsigned long find_max_number_of_collisions()
{
    unsigned long count = 0;
    unsigned long h;
    for (h = 1; h <= HASH_TABLE_SIZE; h++)
    {
        if (hash[h] != NULL)
        {
            unsigned long temp_count = 0;
            Node *top = hash[h];
            
            while (top != NULL)
            {
                top = top->next;
                temp_count++;
                if (temp_count > count) count = temp_count;
            }
        }
    }
    
    return count;
}

int find_entry_in_hash_table(char *name)
{
    int found = 0;
    long hash_key = atol(name);
    unsigned long h = calculate_hash_key2(hash_key);
    
    if (hash[h] != NULL)
    {
        Node *top = hash[h];
        
        while (top != NULL)
        {
            if(hash_key == top->player_id)
            {
                found = 1;
                break;
            }
            top = top->next;
        }
    }
    return found;
}

