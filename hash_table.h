//
//  hash_table.h
//  hashtable_c
//
//  Created by Sriram Sridhar on 2/18/17.
//  Copyright © 2017 Sriram Sridhar. All rights reserved.
//

#ifndef hash_table_h
#define hash_table_h


#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <time.h>       /* time */
#include <sys/time.h>
#include <time.h>       /* time */
#include <sys/time.h>
#include "global_vars.h"

#ifndef HASH_TABLE_SIZE
#define HASH_TABLE_SIZE 100000
#endif

#define PLAYER_ID_HASH_TABLE    1
#define PLAYER_SCORE_HASH_TABLE 2

typedef struct node
{
   long key;
   long value;
   long linkedlist_index;
   struct node *next;
}Node;

long find_entry_in_hash_table2(long percentile,long *key, unsigned char table_id);
int add_entry_to_hash_table(long inKey,long linkedlist_index, long value, unsigned char table_id);
unsigned long calculate_hash_key( char *inKey);
unsigned long calculate_hash_key2( long inKey);
Node* newNode(long key);
void Pearson16(unsigned char *x, size_t len, unsigned long *hex, size_t hexlen);
void printList(Node *top);
void print_all_hash_table_entries(unsigned char table_id);
unsigned long find_total_number_of_hash_table_entries(unsigned char table_id);
unsigned long find_max_number_of_collisions(unsigned char table_id);
long find_entry_in_hash_table(long player_id,unsigned char table_id);
long find_nearest_entry_in_hash_table(long key,unsigned char table_id);

#endif /* hash_table_h */
