#ifndef name_value_pair_H
#define name_value_pair_H

#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include <string.h>
#include <time.h>
#include <sys/time.h>
#include <unistd.h>
#include <pthread.h>

#define MAX_WORD_LENGTH 255

typedef struct _name_value_pair
{
    long value;
    char name[MAX_WORD_LENGTH];
    struct _name_value_pair *leftchild;
    struct _name_value_pair *rightchild;
} name_value_pair;

name_value_pair* get_name_value_pair_handle_single_ptr();
name_value_pair** get_name_value_pair_handle_double_ptr();
void add_name_to_name_value_pair(name_value_pair **pname_value_pair, char *pWord,long value);
void print_name_starting_with(name_value_pair **pname_value_pair, char *pWord); 
void fetch_name_value_pair_size(name_value_pair *pname_value_pair, long *pname_value_pairSize);
void print_name_value_pair_content(name_value_pair *pname_value_pair);
void delete_a_name_from_name_value_pair(name_value_pair **pname_value_pair, char *pWord);
void find_the_depth_of_the_name_value_pair(name_value_pair **pname_value_pair, long *pname_value_pairDepth);

#endif

