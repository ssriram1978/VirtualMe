//
//  binary_tree.c
//  
//
//  Created by Sriram Sridhar on 2/19/17.
//  Copyright © 2017 Sriram Sridhar. All rights reserved.
//

#include "binary_tree.h"

binary_tree *player_score_bt = NULL;
binary_tree *player_id_bt = NULL;

binary_tree** get_player_score_bt_address()
{
   return &player_score_bt;   
}

binary_tree** get_player_id_bt_address()
{
   return &player_id_bt;   
}

binary_tree* get_player_score_bt()
{
   return player_score_bt;   
}

binary_tree* get_player_id_bt()
{
   return player_id_bt;   
}

void add_to_player_score_btree(binary_tree **p_bt, long player_id, long player_score)
{
   if(*p_bt == NULL)
   {
      *p_bt = (binary_tree *) calloc(1, sizeof(binary_tree));
      (*p_bt)->leftchild = NULL;
      (*p_bt)->rightchild = NULL;
      (*p_bt)->player_id = player_id;
      (*p_bt)->player_score = player_score;
   }
   else if(player_score <= (*p_bt)->player_score)
   {
       add_to_player_score_btree(&(*p_bt)->leftchild,player_id,player_score);
   }
   else
   {
       add_to_player_score_btree(&(*p_bt)->rightchild,player_id,player_score);
   }
}

void add_to_player_id_btree(binary_tree **p_bt, long player_id, long player_score)
{
   if(*p_bt == NULL)
   {
      *p_bt = (binary_tree *) calloc(1, sizeof(binary_tree));
      (*p_bt)->leftchild = NULL;
      (*p_bt)->rightchild = NULL;
      (*p_bt)->player_id = player_id;
      (*p_bt)->player_score = player_score;
   }
   else if(player_id <= (*p_bt)->player_id)
   {
       add_to_player_id_btree(&(*p_bt)->leftchild,player_id,player_score);
   }
   else
   {
       add_to_player_id_btree(&(*p_bt)->rightchild,player_id,player_score);
   }
}

void return_the_largest_player_score_from_player_score_btree(binary_tree *p_bt, long *p_player_score)
{
    if(p_bt == NULL)
    {
       return;
    }

    return_the_largest_player_score_from_player_score_btree(p_bt->leftchild,p_player_score);

    if(*p_player_score <= p_bt->player_score)
    {
        *p_player_score = p_bt->player_score; 
    }

    return_the_largest_player_score_from_player_score_btree(p_bt->rightchild,p_player_score);
}

void return_player_id_from_binary_tree_that_matches_this_player_score(binary_tree *p_bt, long *p_player_id, long player_score)
{   
   if(p_bt == NULL)
   {
      return;
   }
   else if(player_score == p_bt->player_score)
   {
       *p_player_id = p_bt->player_id;
   }
   else if(player_score < p_bt->player_score)
   {
       return_player_id_from_binary_tree_that_matches_this_player_score(p_bt->leftchild,p_player_id,player_score);
   }
   else
   {
       return_player_id_from_binary_tree_that_matches_this_player_score(p_bt->rightchild,p_player_id,player_score);
   }
}

long largest_player_score = 0;

void return_player_id_from_binary_tree_that_belongs_to_this_percentile(long *p_player_id, unsigned char percentile)
{
   long computed_player_score = 0;
   struct timeval  start_time = {0};
   struct timeval  end_time = {0};
   struct timeval result = {0};

   if(!largest_player_score)
   {
      gettimeofday(&start_time, NULL);

      return_the_largest_player_score_from_player_score_btree(player_score_bt,&largest_player_score);


      gettimeofday(&end_time, NULL);
   
      timersub(&end_time,&start_time,&result);

   }
   computed_player_score = lround((largest_player_score * percentile)/100);

   printf("\nlargest player score = %ld, computed score at percentile(%d) = %ld.It took %ld.%06ld useconds to complete the search.\n",
    largest_player_score,
    percentile,
    computed_player_score,
    result.tv_sec,
    result.tv_usec);

   return_player_id_from_binary_tree_that_matches_this_player_score(player_score_bt,p_player_id,computed_player_score);
}

void read_player_score_from_player_id_btree(binary_tree *p_bt, long player_id, long *p_player_score)
{
   if(p_bt == NULL)
   {
      return;
   }
   else if(player_id == p_bt->player_id)
   {
       *p_player_score = p_bt->player_score;
   }
   else if(player_id < p_bt->player_id)
   {
       read_player_score_from_player_id_btree(p_bt->leftchild,player_id,p_player_score);
   }
   else
   {
       read_player_score_from_player_id_btree(p_bt->rightchild,player_id,p_player_score);
   }
}

void delete_from_btree(long p_player_id)
{

}

void count_binary_tree_size(binary_tree *p_bt, long *p_count)
{
    
    if(p_bt == NULL)
    {
       return;
    }

    count_binary_tree_size(p_bt->leftchild,p_count);

    (*p_count)++;
    
    count_binary_tree_size(p_bt->rightchild,p_count);
}

void print_binary_tree(binary_tree *p_bt)
{
    if(p_bt == NULL)
    {
       return;
    }

    print_binary_tree(p_bt->leftchild);

    printf("player_id=%ld,player_score=%ld\n",p_bt->player_id, p_bt->player_score);
    
    print_binary_tree(p_bt->rightchild);
}



