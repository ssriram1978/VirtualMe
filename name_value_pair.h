#ifndef name_value_pair_H
#define name_value_pair_H

#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <string.h>
#include <time.h>
#include <sys/time.h>
#include <unistd.h>
#include <pthread.h>

#define MAX_WORD_LENGTH 10
#define MAX_COMMAND_LENGTH 20
#define MAX_NUMBER_OF_WORDS_TO_BE_READ 200000
#define MAX_NUMBER_OF_ALPHABETS 26

typedef struct _name_value_pair
{
    long value;
    char name[MAX_WORD_LENGTH];
    struct _name_value_pair *leftchild;
    struct _name_value_pair *rightchild;
    int height;
} name_value_pair;

#ifndef USE_HEAP
//Use a linked list.
typedef struct _msgq_node
{
   long index;
   struct _msgq_node *pLink;
} msgq_node;

typedef struct _msgQ
{
   msgq_node *pFront;
   msgq_node *pRear;
} msgQ;

void *create_msgq(void *arg);
void dequeue_a_free_node(msgq_node **pp_msgQ_node);
#endif

typedef name_value_pair Tree;
typedef struct asciinode_struct asciinode;

struct asciinode_struct
{
  asciinode * left, * right;

  //length of the edge from this node to its children
  int edge_length; 
    
  int height;      

  int lablen;

  //-1=I am left, 0=I am root, 1=right   
  int parent_dir;   
                         
  //max supported unit32 in dec, 10 digits max
  char label[11];  
};


#define MAX_HEIGHT MAX_NUMBER_OF_WORDS_TO_BE_READ+10
#define INFINITY2 (1<<20)
#define ROOT 0
#define LEFT -1
#define RIGHT 1

#define max(a,b) ((a > b) ? a: b)

void print_level(asciinode *node, int x, int level);
void print_ascii_tree(Tree * t);
void free_ascii_tree(asciinode *node);
asciinode * build_ascii_tree(Tree *t);
void compute_edge_lengths(asciinode *node);
void compute_rprofile(asciinode *node, int x, int y);
void compute_lprofile(asciinode *node, int x, int y);
asciinode * build_ascii_tree_recursive(Tree *t);

int height_of_name_value_pair(name_value_pair *node);





name_value_pair* get_name_value_pair_handle_single_ptr();
name_value_pair** get_name_value_pair_handle_double_ptr();
name_value_pair* add_name_to_name_value_pair(name_value_pair **ppname_value_pair, char *pWord,long value);
void print_name_starting_with(name_value_pair **ppname_value_pair, char *pWord); 
void fetch_name_value_pair_size(name_value_pair *pname_value_pair, long *pname_value_pairSize);
void print_name_value_pair_content(name_value_pair *pname_value_pair);
void delete_a_name_from_name_value_pair(name_value_pair **pname_value_pair, char *pWord);
void find_the_depth_of_the_name_value_pair(name_value_pair *pname_value_pair, long *pname_value_pairDepth);
void create_unsorted_name_text_file();
void find_largest_name_from_this_current_node(name_value_pair *pname_value_pair,name_value_pair *pLargest_name_value_pair);
void delete_name_starting_with(name_value_pair **ppname_value_pair, char *pWord);

#endif

