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

void find_largest_from_this_node(binary_tree *p_bt, binary_tree *p_outbt)
{
   if(p_bt)
   {
       find_largest_from_this_node(p_bt->leftchild,p_outbt);
       if(p_outbt->player_id <= p_bt->player_id)
       {
           p_outbt->player_id = p_bt->player_id;
           p_outbt->player_score = p_bt->player_score;
           p_outbt->leftchild = p_bt->leftchild;
           p_outbt->rightchild = p_bt->rightchild;
       }
       find_largest_from_this_node(p_bt->rightchild,p_outbt);
   }
}
#if 0
void print_node_at_depth(binary_tree *p_bt,long depth)
{
    char buffer[100];
    char buffer2[100];
    int i = 0;
    
    memset(buffer,0,100);
    memset(buffer2,0,100);
    
    for(i=0; i < depth; i++)
    {
        strcat(buffer," ");
    }
    sprintf(buffer,"%ld",p_bt->player_id);
    printf("%s\n",buffer);
    
    if(p_bt->leftchild)
    {
        for(i=0; i < depth-2; i++)
        {
            strcat(buffer," ");
        }
        strcat(buffer,"/");
    }
    
}

void print_btree(binary_tree *p_bt)
{
    long depth_of_btree = 0;
    
   if(!p_bt)
   {
       return;
   }
   find_depth_of_binary_tree(p_bt, &depth_of_btree);
   
    print_node_at_depth(p_bt,depth_of_btree);
    
    printf("%ld\n",p_bt->player_id);
    print_btree(p_bt->leftchild);
    print_btree(p_bt->rightchild);
}
#endif

void delete_from_btree(binary_tree **p_bt, long p_player_id)
{
   if(!p_bt || !*p_bt)
   {
       return;
   }
   else
   {
      if(p_player_id < (*p_bt)->player_id)
      {
          delete_from_btree(&(*p_bt)->leftchild, p_player_id);
      }
      else if(p_player_id > (*p_bt)->player_id)
      {
          delete_from_btree(&(*p_bt)->rightchild, p_player_id);
      }
      else
      {
          if((*p_bt)->leftchild && (*p_bt)->rightchild)
          {
              //Identify the largest node from the left child.
              //swap it with the current node.
              //free the largest node on the left child.
              binary_tree bt = {0};
              
              find_largest_from_this_node((*p_bt)->leftchild,&bt);
              
              (*p_bt)->player_id = bt.player_id;
              (*p_bt)->player_score = bt.player_score;
              delete_from_btree(&(*p_bt)->leftchild,bt.player_id);
          }
          else if (!((*p_bt)->leftchild) && ((*p_bt)->rightchild))
          {
             //swap the right node with the current node and free the right child.
              (*p_bt)->player_id = (*p_bt)->rightchild->player_id;
              (*p_bt)->player_score = (*p_bt)->rightchild->player_score;
              free((*p_bt)->rightchild);
              (*p_bt)->rightchild = NULL;
          }
          else if (((*p_bt)->leftchild) && !((*p_bt)->rightchild))
          {
              //swap the left node with the current node and free the left child.
              (*p_bt)->player_id = (*p_bt)->leftchild->player_id;
              (*p_bt)->player_score = (*p_bt)->leftchild->player_score;
              free((*p_bt)->leftchild);
              (*p_bt)->leftchild = NULL;
          }
          else if (!((*p_bt)->leftchild) && !((*p_bt)->rightchild))
          {
              free(*p_bt);
              *p_bt = NULL;
          }
      }
   }
}

void delete_binary_tree(binary_tree **p_bt)
{
   //post order traversal
   if(!p_bt || !*p_bt)
   {
       return;
   }
   else
   {
       delete_binary_tree(&(*p_bt)->leftchild);
       delete_binary_tree(&(*p_bt)->rightchild);
       free(*p_bt);
       *p_bt = NULL;
   }
}

void find_depth_of_binary_tree(binary_tree *p_bt, long *p_bt_length)
{
    if(!p_bt || !p_bt_length)
    {
        return;
    }
    else
    {
        //find depth of left subtree.
        //find depth of right subtree.
        //compare the depths of both left and right subtree and pick the one that is greater.
        //add 1 to the computed depth and return.
        
        long left_depth = 0;
        find_depth_of_binary_tree(p_bt->leftchild, &left_depth);
        long right_depth = 0;
        find_depth_of_binary_tree(p_bt->rightchild, &right_depth);
        
        //printf("left_depth=%ld,right_depth=%ld\n",left_depth,right_depth);
        
        if(left_depth > right_depth)
        {
            *p_bt_length +=(left_depth+1);
        }
        else
        {
            *p_bt_length +=(right_depth+1);
        }
        //printf("returning *p_bt_length=%ld\n",*p_bt_length);
    }
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



