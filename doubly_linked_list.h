//
//  doubly_linked_list.h
//  hashtable_c
//
//  Created by Sriram Sridhar on 2/19/17.
//  Copyright © 2017 Sriram Sridhar. All rights reserved.
//

#ifndef doubly_linked_list_h
#define doubly_linked_list_h

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <time.h>       /* time */
#include <sys/time.h>
#include <time.h>       /* time */
#include <sys/time.h>
#include "global_vars.h"

#ifndef NUMBER_OF_MIDDLE_POINTERS
#define NUMBER_OF_MIDDLE_POINTERS 1000
#endif



typedef struct _doubly_linked_list
{
    long player_score;
    long player_id;
    struct _doubly_linked_list *next;
    struct _doubly_linked_list *prev;
} doubly_linked_list;

void add_node_to_doubly_linked_list(long player_id, long player_score);
void delete_node_from_doubly_linked_list(long player_id);
long count_total_nodes_in_doubly_linked_list();
doubly_linked_list *return_nodes_from_doubly_linked_list_belonging_to_this_percentile(unsigned char percentile);
void print_nodes_in_doubly_linked_list();
void print_nodes_in_middle_doubly_linked_list();


#endif /* doubly_linked_list_h */
