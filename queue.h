//
//  queue.h
//  
//
//  Created by Sriram Sridhar on 2/19/17.
//  Copyright © 2017 Sriram Sridhar. All rights reserved.
//

#ifndef queue_h
#define queue_h

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <time.h>       /* time */
#include <sys/time.h>
#include <time.h>       /* time */
#include <sys/time.h>


typedef struct _queue
{
    long player_score;
    long player_id;
    struct _queue *next;
} queue;

void enqueue(long player_id, long player_score);
void dequeue(long *p_player_id, long *p_player_score);
long count_queue_size();
void print_queue();


#endif /* queue_h */

