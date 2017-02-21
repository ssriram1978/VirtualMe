//
//  single_linked_list.h
//  hashtable_c
//
//  Created by Sriram Sridhar on 2/19/17.
//  Copyright © 2017 Sriram Sridhar. All rights reserved.
//

#ifndef single_linked_list_h
#define single_linked_list_h

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <time.h>       /* time */
#include <sys/time.h>
#include <time.h>       /* time */
#include <sys/time.h>
#include "global_vars.h"


typedef struct _single_linked_list
{
    long player_score;
    long player_id;
    struct _single_linked_list *next;
} single_linked_list;

void add_node_to_linked_list(long player_id, long player_score);
void delete_node_from_linked_list(long player_id);
long count_total_nodes_in_linked_list();
single_linked_list *return_nodes_belonging_to_this_percentile(unsigned char percentile);
void print_nodes_in_linked_list();

#endif /* single_linked_list_h */
