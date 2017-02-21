//
//  single_linked_list.c
//  hashtable_c
//
//  Created by Sriram Sridhar on 2/19/17.
//  Copyright © 2017 Sriram Sridhar. All rights reserved.
//

#include "single_linked_list.h"

static single_linked_list *head = NULL;
static single_linked_list *tail = NULL;
long count = 0;


void add_node_to_linked_list(long player_id, long player_score)
{
    single_linked_list *ptr = NULL;
    
    ptr = (single_linked_list *) calloc(1,sizeof(single_linked_list));
    ptr->player_id = player_id;
    ptr->player_score = player_score;
    
    if(head == NULL)
    {
        head = ptr;
        ptr->next = NULL;
        tail = head;
        count++;
        //printf("Adding player id =%ld for the first time, count=%ld \n",player_id, count);
    }
    else
    {
        single_linked_list *prev = head;
        single_linked_list *node = head;
        
        //As long as player_score is greater than the player score in the node,
        //Keep switching to the next node.
        //Note that we are building a ascending order linked list with
        //head pointing to the least node and
        //tail pointing to the largest node.
        while(node && player_score >= node->player_score)
        {
           prev = node;
           node = node->next;
        }
        
        if(node && (player_score < node->player_score))
        {
           if(prev == head)
           {
               //Add this node before head and move head pointer to this new node.
               ptr->next = prev;
               head = ptr;
           }
           else
           {
              //Add ptr before this node.
              ptr->next = prev->next;
              prev->next = ptr;
           }
           count++;
           //printf("Adding player id =%ld before %ld, count=%ld\n",player_id,node->player_id,count);
        }
        else
        {
            //You reached the end of the list. Add it at the end.
            count++;
            //printf("Adding player id =%ld after %ld,count=%ld \n",player_id,prev->player_id,count);
            //Add ptr after this node.
            prev->next = ptr;
            ptr->next = NULL;

            if(tail == prev)
            {
                //Adjust tail to point to this new node.
                tail = ptr;
                //printf("Adjusting tail  pointer to this player id =%ld\n",player_id);
            }
        }
    }
}

void delete_node_from_linked_list(long player_id)
{
    single_linked_list *node = head;
    single_linked_list *prev = head;
    
    while(node && node->player_id != player_id)
    {
        prev = node;
        node = node->next;
    }
    
    if(!node)
    {
        printf("Unable to locate the node with player id =%ld\n",player_id);
        return;
    }
    
    prev->next = node->next;
    
    //Adjust head and tail.
    if(head == node)
    {
       //Adjust head to point to the next node.
        head = node->next;
    }
    
    if(tail == node)
    {
       //Adjust the tail to point to the next node.
        tail = prev;
    }
    
    free(node);
}

long count_total_nodes_in_linked_list()
{
    long count = 0;
    single_linked_list *node = head;
    
    while(node)
    {
        count++;
        node = node->next;
    }
    
    return count;
}

void print_nodes_in_linked_list()
{
    long count = 0;
    single_linked_list *node = head;
    
    while(node)
    {
        printf("Node at index =%ld has player id =%ld,score=%ld\n",count,node->player_id,node->player_score);
        node = node->next;
        count++;
    }
}

single_linked_list *return_nodes_belonging_to_this_percentile(unsigned char percentile)
{
    single_linked_list *node = head;
    
    while(node)
    {
        if(node->player_score == (tail->player_score * percentile/100))
        {
            break;
        }
        if(node->player_score > (tail->player_score * percentile/100))
        {
            node = NULL;
            break;
        }
        node = node->next;
    }
    
    return node;
}
