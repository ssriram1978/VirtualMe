//
//  main.c
//  datastructures
//
//  Created by Sriram Sridhar on 2/22/17.
//  Copyright © 2017 Sriram Sridhar. All rights reserved.
//

//
//  main.c
//  hashtable_c
//
//  Created by Sriram Sridhar on 2/18/17.
//  Copyright © 2017 Sriram Sridhar. All rights reserved.
//

#include "global_vars.h"
#include "hash_table.h"
#include "single_linked_list.h"
#include "doubly_linked_list.h"
#include "queue.h"
#include "binary_tree.h"

void make_unsorted_array2(long size);

int random_between(int min, long max)
{
    return (rand() % (max - min + 1) + min);
}


void make_unsorted_array2(long size)
{
    unsigned long index = 0;
    long player_id = 0,player_score=0;
    time_t t = {0};
    
    srand((unsigned) time(&t));
    
    for(index = 0; index < size; index++)
    {
        long linkedlist_index = 0, linkedlist_starting_search_index =0;
        /* Intializes random number generator */
        player_id = random_between(1,size);
        player_score = random_between(1,size);
        //random_number++;
        //printf("Random number = %ld\n",random_number);
        //add_node_to_linked_list(random_number,random_number);
        //linkedlist_starting_search_index = find_nearest_entry_in_hash_table(player_id,PLAYER_ID_HASH_TABLE);
        
        //printf("\n For player_id=%ld,nearest hash index =%ld \n",player_id,linkedlist_starting_search_index);
        
        if(linkedlist_starting_search_index >= 0)
        {
           //linkedlist_index = add_node_to_doubly_linked_list2(player_id,player_score,linkedlist_starting_search_index);
           //printf("\nlinkedlist_index=%ld\n",linkedlist_index);
           add_entry_to_hash_table(player_id,linkedlist_index,player_score,PLAYER_ID_HASH_TABLE);
           add_entry_to_hash_table(player_score,linkedlist_index,player_id,PLAYER_SCORE_HASH_TABLE);
        }
        
        //add_node_to_doubly_linked_list(random_number,random_number);
        //enqueue(random_number,random_number);
        add_to_player_score_btree(get_player_score_bt_address(),player_id,player_score);
        add_to_player_id_btree(get_player_id_bt_address(),player_id,player_score);
    }
}

int main(int argc, const char * argv[])
{
    // insert code here...
    char player_id[40] = {0};
    unsigned char percentile = 1;
    struct timeval  start_time = {0};
    struct timeval  end_time = {0};
    struct timeval result = {0};
    long linkedlist_index = 0;
    
    long index = 0, queuesize =0, player_id_btree_size=0, score_btree_size=0;
    
    make_unsorted_array2(MAX_NUMBER_OF_ELEMENTS);
    queuesize = count_queue_size();
    //print_all_hash_table_entries();
    //print_nodes_in_linked_list();
    printf("\n Total number of entries in linked list = %ld\n",count_total_nodes_in_linked_list());
    printf("\n Total number of entries in doubly linked list = %ld\n",count_total_nodes_in_doubly_linked_list());
    //print_nodes_in_doubly_linked_list();
    printf("\n Total number of hash table entries = %lu\n",find_total_number_of_hash_table_entries(PLAYER_ID_HASH_TABLE));
    printf("\n Total number of hash table entries2 = %lu\n",find_total_number_of_hash_table_entries(PLAYER_SCORE_HASH_TABLE));
    //print_all_hash_table_entries();
    printf("\n Maximum number of elements in queue = %lu\n",queuesize);
    printf("\n Maximum number of elements in one row = %lu\n",find_max_number_of_collisions(PLAYER_ID_HASH_TABLE));
    printf("\n Maximum number of elements in one row2 = %lu\n",find_max_number_of_collisions(PLAYER_SCORE_HASH_TABLE));
    count_binary_tree_size(get_player_id_bt(),&player_id_btree_size);
    printf("\n Maximum number of elements in player_id_binary_tree = %lu\n",player_id_btree_size);
    count_binary_tree_size(get_player_score_bt(),&score_btree_size);
    printf("\n Maximum number of elements in player_score_binary_tree = %lu\n",score_btree_size);
    //print_nodes_in_middle_doubly_linked_list();
    //print_queue();
    //printf("Dumping player_id binary tree \n");
    //print_binary_tree(get_player_id_bt());
    //printf("Dumping player_score binary tree \n");
    //print_binary_tree(get_player_score_bt());
    //print_nodes_in_linked_list();
    //reverse_linked_list();
    //print_nodes_in_linked_list();
    //delete_binary_tree(get_player_id_bt_address());
    //printf("Dumping player_id binary tree \n");
    //print_binary_tree(get_player_id_bt());
    long player_bt_length = 0;
    find_depth_of_binary_tree(get_player_id_bt(),&player_bt_length);
    printf("player_bt_length=%ld\n",player_bt_length);
    //delete_from_btree(get_player_id_bt_address(),3);
    //printf("Dumping player_id binary tree \n");
    //print_binary_tree(get_player_id_bt());
    
    for(index=0; index < queuesize; index++)
    {
        long playerid, player_score;
        dequeue(&playerid,&player_score);
        //printf("playerid=%ld,player_score=%ld\n",playerid,player_score);
    }
    
    while(player_id[0] != 'x')
    {
        long player_score = 0;
        long player_index = 0;
        
        printf("\n Enter the player_id to search:");
        scanf("%s",player_id);
        gettimeofday(&start_time, NULL);
        linkedlist_index = find_entry_in_hash_table(atol(player_id),PLAYER_ID_HASH_TABLE);
        
        //if(linkedlist_index > 0)
        {
            gettimeofday(&end_time, NULL);
            timersub(&end_time,&start_time,&result);
            player_index = atol(player_id);
            //get_player_score_at_this_linkedlist_index(linkedlist_index, player_index, &player_score);
            
            printf("\nHash table search: found player_id=%ld, linkedlist_index = %ld, player_score=%ld It took %ld.%06ld useconds to complete the search.\n",
                   player_index,
                   linkedlist_index,
                   player_score,
                   result.tv_sec,
                   result.tv_usec);
            
        }

        
        gettimeofday(&start_time, NULL);
        
        read_player_score_from_player_id_btree(get_player_id_bt(),atol(player_id),&player_score);
        
        gettimeofday(&end_time, NULL);
        
        timersub(&end_time,&start_time,&result);
        
        printf("\n Binary tree: player_id=%s. player_score =%ld It took %ld.%06ld useconds to complete the search.\n",player_id,player_score,result.tv_sec,result.tv_usec);
    }
    
    while(percentile != 0)
    {
        single_linked_list *ptr = NULL;
        doubly_linked_list *ptr2 = NULL;
        long player_id = 0;
        long player_score =0;
        
        printf("\n Enter the percentile to search:");
        scanf("%d",(int *)&percentile);
        gettimeofday(&start_time, NULL);
        
        //ptr = return_nodes_belonging_to_this_percentile(percentile);
        
        if(ptr)
        {
            gettimeofday(&end_time, NULL);
            timersub(&end_time,&start_time,&result);
            printf("\nfor percentile = %d, found player_id=%ld,player_score=%ld.It took %ld.%06ld useconds to complete the search.\n",percentile,
                   ptr->player_id,
                   ptr->player_score,
                   result.tv_sec,
                   result.tv_usec);
        }
        else
        {
            gettimeofday(&end_time, NULL);
            timersub(&end_time,&start_time,&result);
            printf("\nNot found. It took %ld.%06ld useconds to complete the search.\n",result.tv_sec,result.tv_usec);
        }
        
        gettimeofday(&start_time, NULL);
        
        //ptr2 = return_nodes_from_doubly_linked_list_belonging_to_this_percentile(percentile);
        
        if(ptr2)
        {
            gettimeofday(&end_time, NULL);
            timersub(&end_time,&start_time,&result);
            printf("\nfor percentile = %d, found player_id=%ld,player_score=%ld.It took %ld.%06ld useconds to complete the search.\n",percentile,
                   ptr2->player_id,
                   ptr2->player_score,
                   result.tv_sec,
                   result.tv_usec);
        }
        else
        {
            gettimeofday(&end_time, NULL);
            timersub(&end_time,&start_time,&result);
            printf("\nNot found. It took %ld.%06ld useconds to complete the search.\n",result.tv_sec,result.tv_usec);
        }
        
        gettimeofday(&start_time, NULL);
        
        player_id = find_entry_in_hash_table2(percentile,&player_score, PLAYER_ID_HASH_TABLE);
        
        
        {
            gettimeofday(&end_time, NULL);
            timersub(&end_time,&start_time,&result);
            printf("\n Hash table2: for percentile = %d, found player_id=%ld,player_score=%ld.It took %ld.%06ld useconds to complete the search.\n",percentile,
                   player_id,
                   player_score,
                   result.tv_sec,
                   result.tv_usec);
        }
        
        player_id = 0;
        
        return_player_id_from_binary_tree_that_belongs_to_this_percentile(&player_id,percentile);
        
        gettimeofday(&end_time, NULL);
        
        timersub(&end_time,&start_time,&result);
        
        printf("\n Binary search: player_id=%ld belongs to this percentile (%d). It took %ld.%06ld useconds to complete the search.\n",player_id,percentile,result.tv_sec,result.tv_usec);
        
    }
    
    return 0;
}
