//
//  doubly_linked_list.c
//  
//
//  Created by Sriram Sridhar on 2/19/17.
//  Copyright © 2017 Sriram Sridhar. All rights reserved.
//

#include "doubly_linked_list.h"
#include <math.h>

static doubly_linked_list *head = NULL;
static doubly_linked_list *tail = NULL;
static long count = 0;

static doubly_linked_list *middle_nodes[MAX_NUMBER_OF_ELEMENTS/NUMBER_OF_MIDDLE_POINTERS];

doubly_linked_list* fetch_valid_middle_node_near_to_player_score(int index,long player_score)
{
   int index2 = index;
   doubly_linked_list *ptr = NULL;
    
   while(index2 && middle_nodes[index2] == NULL)
   {
      index2--;
   }

   if(index2 == 0)
   {
      //All done. Return the middle node.
      ptr = middle_nodes[index2];
   }
   else
   {
      if(middle_nodes[index2] != NULL)
      {
         int low =0;
         int high = index2;
         int mid = 0;
         
         while(low <= high)
         {
            mid = low + lround((high -low)/2);
             
            if(middle_nodes[mid] && middle_nodes[mid]->player_score == player_score)
            {
               return middle_nodes[mid];
            }
            else if (middle_nodes[mid] && middle_nodes[mid]->player_score < player_score)
            {
               low = mid+1;         
            }
            else if (middle_nodes[mid] && middle_nodes[mid]->player_score > player_score)
            {
               high = mid-1;
            }
             
            ptr = middle_nodes[mid];
         }
     }
   }
    return ptr;
}

long add_node_to_doubly_linked_list2(long player_id, long player_score, long linkedlist_starting_search_index)
{
    doubly_linked_list *ptr = NULL;
    long new_linkedlist_index = 0;
    
    ptr = (doubly_linked_list *) calloc(1,sizeof(doubly_linked_list));
    ptr->player_id = player_id;
    ptr->player_score = player_score;
    
    new_linkedlist_index = (long) ptr;
    
    if(head == NULL)
    {
        head = ptr;
        ptr->next = NULL;
        ptr->prev = NULL;
        tail = head;
        count++;
        //printf("Adding player id =%ld for the first time, count=%ld \n",player_id, count);
    }
    else
    {
        //start searching for a node from the provided linkedlist_index.
        
        doubly_linked_list *node = NULL;
        if(linkedlist_starting_search_index == 0)
        {
            node = head;
        }
        else
        {
            node = (doubly_linked_list *) linkedlist_starting_search_index;
        }
        
        while(node &&
              (node->next != NULL) &&
              (player_score >= node->player_score))
        {
            node = node->next;
        }
        
        if(node)
        {
            if (player_score <= node->player_score)
            {
                //Add ptr before this node.
                ptr->next = node;
                ptr->prev = node->prev;
                
                if(node->prev)
                {
                    //Change previous node to point to this ptr
                    node->prev->next = ptr;
                }
                
                //change node previous to ptr
                node->prev = ptr;
                
                count++;
                //printf("Adding player id =%ld before %ld, count=%ld\n",player_id,node->player_id,count);
                
                if(head == node)
                {
                    //Adjust head to point to this new node.
                    head = ptr;
                    //printf("Adjusting head  pointer to this player id =%ld\n",player_id);
                }
            }
            else
            {
                //You reached the end of the list. Add it at the end.
                count++;
                //printf("Adding player id =%ld after %ld,count=%ld \n",player_id,prev->player_id,count);
                
                //Add ptr after this node.
                ptr->next = NULL;
                ptr->prev = node;
                
                //Change node next to ptr
                node->next = ptr;
                
                if(tail == node)
                {
                    //Adjust tail to point to this new node.
                    tail = ptr;
                    //printf("Adjusting tail  pointer to this player id =%ld\n",player_id);
                }
            }
        }
    }
    
    return new_linkedlist_index;
}

void add_node_to_doubly_linked_list(long player_id, long player_score)
{
    doubly_linked_list *ptr = NULL;
    long temp_count = 0;
    
    ptr = (doubly_linked_list *) calloc(1,sizeof(doubly_linked_list));
    ptr->player_id = player_id;
    ptr->player_score = player_score;
    
    if(head == NULL)
    {
        head = ptr;
        ptr->next = NULL;
        ptr->prev = NULL;
        tail = head;
        count++;
        //printf("Adding player id =%ld for the first time, count=%ld \n",player_id, count);
    }
    else
    {
//#if 0
        doubly_linked_list *node = head;
        
        //As long as player_score is greater than the player score in the node,
        //Keep switching to the next node.
        //Note that we are building a ascending order linked list with
        //head pointing to the least node and
        //tail pointing to the largest node.
        while(node &&
              (node->next != NULL) &&
              (player_score >= node->player_score))
        {
            node = node->next;
            temp_count++;
            if(temp_count % NUMBER_OF_MIDDLE_POINTERS == 0)
            {
                unsigned char middle_index = (temp_count/NUMBER_OF_MIDDLE_POINTERS)-1;
                middle_nodes[middle_index] = node;
            }
        }
//#endif
        
#if 0
        //search the known middle nodes for a valid node.
        doubly_linked_list *node = fetch_valid_middle_node_near_to_player_score(MAX_NUMBER_OF_ELEMENTS/NUMBER_OF_MIDDLE_POINTERS,
                                                                                player_score);
        if(node)
        {
           if(node->player_score < player_score)
           {
               //use next pointer
               while(node &&
                     (node->next != NULL) &&
                     (player_score >= node->player_score))
               {
                   node = node->next;
               }
           }
           else if(node->player_score > player_score)
           {
               //use previous pointer
               while(node &&
                     (node->prev != NULL) &&
                     (player_score <= node->player_score))
               {
                   node = node->prev;
               }
           }
           else
           {
              //found the node.
           }
        }
#endif
        if(node)
        {
            if (player_score <= node->player_score)
            {
               //Add ptr before this node.
               ptr->next = node;
               ptr->prev = node->prev;
            
               if(node->prev)
               {
                  //Change previous node to point to this ptr
                  node->prev->next = ptr;
               }
                
               //change node previous to ptr
               node->prev = ptr;
            
               count++;
               //printf("Adding player id =%ld before %ld, count=%ld\n",player_id,node->player_id,count);
               
               if(head == node)
               {
                   //Adjust head to point to this new node.
                   head = ptr;
                   //printf("Adjusting head  pointer to this player id =%ld\n",player_id);
               }
            }
            else
            {
                //You reached the end of the list. Add it at the end.
                count++;
                //printf("Adding player id =%ld after %ld,count=%ld \n",player_id,prev->player_id,count);
                
                //Add ptr after this node.
                ptr->next = NULL;
                ptr->prev = node;
                
                //Change node next to ptr
                node->next = ptr;
                
                if(tail == node)
                {
                    //Adjust tail to point to this new node.
                    tail = ptr;
                    //printf("Adjusting tail  pointer to this player id =%ld\n",player_id);
                }
            }
        }
    }
}

void delete_node_from_doubly_linked_list(long player_id)
{
    doubly_linked_list *node = head;
    
    while(node &&
          (node->next != NULL) &&
          (node->player_id != player_id))
    {
        node = node->next;
    }
    
    if(!node || (node->player_id != player_id))
    {
        printf("Unable to locate the node with player id =%ld\n",player_id);
        return;
    }
    
    if(node->prev)
    {
       //Make the previous node point to the next node.
       node->prev->next = node->next;
    }
    
    if(node->next)
    {
       //Make the next node to point to the previous node.
       node->next->prev = node->prev;
    }
    
    //Adjust head and tail.
    if(head == node)
    {
        //Adjust head to point to the next node.
        head = node->next;
    }
    
    if(tail == node)
    {
        //Adjust the tail to point to the previous node.
        tail = node->prev;
    }
    
    free(node);
}

long count_total_nodes_in_doubly_linked_list()
{
    long count = 0;
    doubly_linked_list *node = head;
    
    while(node)
    {
        count++;
        node = node->next;
    }
    
    return count;
}

void print_nodes_in_doubly_linked_list()
{
    long count = 0;
    doubly_linked_list *node = head;
    
    while(node)
    {
        printf("Node at index =%ld has player id =%ld,score=%ld\n",count,node->player_id,node->player_score);
        node = node->next;
        count++;
    }
}

void print_nodes_in_middle_doubly_linked_list()
{
    int index = 0;
    
    for(index=0; index < (MAX_NUMBER_OF_ELEMENTS/NUMBER_OF_MIDDLE_POINTERS); index++ )
    {
        doubly_linked_list *node = middle_nodes[index];
        if(node)
        {
           printf("\nNode at index =%d has player id =%ld,score=%ld\n",index,node->player_id,node->player_score);
        }
    }
}

void get_player_score_at_this_linkedlist_index(long linkedlist_index, long player_index, long *player_score)
{
    doubly_linked_list *node = NULL;

    if(!linkedlist_index || !player_index || !player_score)
    {
        return;
    }
    
    node = (doubly_linked_list *) linkedlist_index;
    
    if(node && node->player_id && (node->player_id == player_index))
    {
        *player_score = node->player_score;
    }
}

doubly_linked_list *return_nodes_from_doubly_linked_list_belonging_to_this_percentile(unsigned char percentile)
{
    doubly_linked_list *node = NULL;
    
    if(percentile > 50)
    {
        //search from tail.
        node = tail;
        while(node)
        {
            if(node->player_score == (tail->player_score * percentile/100))
            {
                break;
            }
            if(node->player_score < (tail->player_score * percentile/100))
            {
                node = NULL;
                break;
            }
            node = node->prev;
        }
    }
    else
    {
        //search from head.
        node = head;
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
    }
    
    return node;
}
