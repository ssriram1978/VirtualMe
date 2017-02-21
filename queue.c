//
//  queue.c
//  
//
//  Created by Sriram Sridhar on 2/19/17.
//  Copyright © 2017 Sriram Sridhar. All rights reserved.
//

#include "queue.h"


static queue *head = NULL;
static queue *tail = NULL;
static long count = 0;

void enqueue(long player_id, long player_score)
{
    queue *ptr = NULL;
    
    ptr = (queue *) calloc(1,sizeof(queue));
    ptr->player_id = player_id;
    ptr->player_score = player_score;
    ptr->next = NULL;
    
    if(tail == NULL)
    {
        tail = ptr;
        count++;
        
        if(head == NULL)
        {
           head = tail;
        }
        //printf("Adding player id =%ld for the first time, count=%ld \n",player_id, count);
    }
    else
    {
       tail->next = ptr;
       tail = ptr;       
    }
}

void dequeue(long *p_player_id, long *p_player_score)
{
    queue *node = head;

    if(!node || !p_player_id || !p_player_score)
    {
        return;
    }

    head = node->next;

    *p_player_id = node->player_id;

    *p_player_score = node->player_score;
    
    free(node);
}

long count_queue_size()
{
    long count = 0;
    queue *node = head;
    
    while(node)
    {
        count++;
        node = node->next;
    }
    
    return count;
}

void print_queue()
{
    long count = 0;
    queue *node = head;
    
    while(node)
    {
        printf("Node at index =%ld has player id =%ld,score=%ld\n",count,node->player_id,node->player_score);
        node = node->next;
        count++;
    }
}
