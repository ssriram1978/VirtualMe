//
//  name_value_pair.c
//  
//
//  Created by Sriram Sridhar on 5/19/17.
//  Copyright © 2017 Sriram Sridhar. All rights reserved.
//

#include "name_value_pair.h"

name_value_pair *pNameValuePairHandle = NULL;

#define READ_WORDS_FROM_TEXT_FILE 1
#define AVL_TREE 1
#define UNSORTED_NAMES "unsorted_words.txt"

#ifndef USE_HEAP
name_value_pair g_name_value_pair[MAX_NUMBER_OF_WORDS_TO_BE_READ];

msgq_node g_msgq_node[MAX_NUMBER_OF_WORDS_TO_BE_READ];

msgQ g_msgQ;

#endif

#include <stdint.h>

// Define MT19937 constants (32-bit RNG)
enum
{
    // Assumes W = 32 (omitting this)
    N = 624,
    M = 397,
    R = 31,
    A = 0x9908B0DF,

    F = 1812433253,

    U = 11,
    // Assumes D = 0xFFFFFFFF (omitting this)

    S = 7,
    B = 0x9D2C5680,

    T = 15,
    C = 0xEFC60000,

    L = 18,

    MASK_LOWER = (1ull << R) - 1,
    MASK_UPPER = (1ull << R)
};

static uint32_t  mt[N];
static uint16_t  index2;

// Re-init with a given seed
void Initialize(const uint32_t  seed)
{
    uint32_t  i;

    mt[0] = seed;

    for ( i = 1; i < N; i++ )
    {
        mt[i] = (F * (mt[i - 1] ^ (mt[i - 1] >> 30)) + i);
    }

    index2 = N;
}

static void Twist()
{
    uint32_t  i, x, xA;

    for ( i = 0; i < N; i++ )
    {
        x = (mt[i] & MASK_UPPER) + (mt[(i + 1) % N] & MASK_LOWER);

        xA = x >> 1;

        if ( x & 0x1 )
            xA ^= A;

        mt[i] = mt[(i + M) % N] ^ xA;
    }

    index2 = 0;
}

// Obtain a 32-bit random number
uint32_t ExtractU32()
{
    uint32_t  y;
    int       i = index2;

    if ( index2 >= N )
    {
        Twist();
        i = index2;
    }

    y = mt[i];
    index2 = i + 1;

    y ^= (mt[i] >> U);
    y ^= (y << S) & B;
    y ^= (y << T) & C;
    y ^= (y >> L);

    return y;
}


//printing tree in ascii

int lprofile[MAX_HEIGHT];
int rprofile[MAX_HEIGHT];

//adjust gap between left and right nodes
int gap = 3;  

//used for printing next node in the same level, 
//this is the x coordinate of the next char printed
int print_next;    

int MIN (int X, int Y)  
{
  return ((X) < (Y)) ? (X) : (Y);
}

int MAX (int X, int Y)  
{
  return ((X) > (Y)) ? (X) : (Y);
}



//The following function fills in the lprofile array for the given tree.
//It assumes that the center of the label of the root of this tree
//is located at a position (x,y).  It assumes that the edge_length
//fields have been computed for this tree.
void compute_lprofile(asciinode *node, int x, int y) 
{
   int i, isleft;
   if (node == NULL) return;
   isleft = (node->parent_dir == -1);
   lprofile[y] = MIN(lprofile[y], x-((node->lablen-isleft)/2));
   if (node->left != NULL) 
   {
       for (i=1; i <= node->edge_length && y+i < MAX_HEIGHT; i++) 
       {
          lprofile[y+i] = MIN(lprofile[y+i], x-i);
       }
   }
   compute_lprofile(node->left, x-node->edge_length-1, y+node->edge_length+1);
   compute_lprofile(node->right, x+node->edge_length+1, y+node->edge_length+1);
}

void compute_rprofile(asciinode *node, int x, int y) 
{
   int i, notleft;
   if (node == NULL) return;
   notleft = (node->parent_dir != -1);
   rprofile[y] = MAX(rprofile[y], x+((node->lablen-notleft)/2));
   if (node->right != NULL) 
   {
       for (i=1; i <= node->edge_length && y+i < MAX_HEIGHT; i++) 
       {
          rprofile[y+i] = MAX(rprofile[y+i], x+i);
       }
   }
   compute_rprofile(node->left, x-node->edge_length-1, y+node->edge_length+1);
   compute_rprofile(node->right, x+node->edge_length+1, y+node->edge_length+1);
}

//This function prints the given level of the given tree, assuming
//that the node has the given x cordinate.
void print_level(asciinode *node, int x, int level) 
{
    int i, isleft;
    if (node == NULL) return;
    isleft = (node->parent_dir == -1);
    if (level == 0) 
    {
        for (i=0; i<(x-print_next-((node->lablen-isleft)/2)); i++) 
        {
          printf(" ");
        }
        print_next += i;
        printf("%s", node->label);
        print_next += node->lablen;
    } 
    else if (node->edge_length >= level) 
    {
        if (node->left != NULL) 
        {
            for (i=0; i<(x-print_next-(level)); i++) 
            {
                printf(" ");
            }
            print_next += i;
            printf("/");
            print_next++;
        }
        if (node->right != NULL) 
        {
            for (i=0; i<(x-print_next+(level)); i++) 
            {
                printf(" ");
            }
            print_next += i;
            printf("\\");
            print_next++;
        }
    } 
    else 
    {
        print_level(node->left, 
                  x-node->edge_length-1, 
                  level-node->edge_length-1);

        print_level(node->right, 
                  x+node->edge_length+1, 
                  level-node->edge_length-1);
    }
}

//This function fills in the edge_length and 
//height fields of the specified tree
void compute_edge_lengths(asciinode *node) 
{
    int h, hmin, i, delta;
  
    if (node == NULL)
    {
       return;
    }
    
    compute_edge_lengths(node->left);
    compute_edge_lengths(node->right);
  
    /* first fill in the edge_length of node */
    if (node->right == NULL && node->left == NULL) 
    {
        node->edge_length = 0;
    } 
    else 
    {
       if (node->left != NULL) 
       {
           for (i=0; i<node->left->height && i < MAX_HEIGHT; i++) 
           {
               rprofile[i] = -INFINITY2;
           }
           compute_rprofile(node->left, 0, 0);
           hmin = node->left->height;
       } 
       else 
       {
           hmin = 0;
       }
       
       if (node->right != NULL) 
       {
           for (i=0; i<node->right->height && i < MAX_HEIGHT; i++) 
           {
               lprofile[i] = INFINITY2;
           }
           compute_lprofile(node->right, 0, 0);
           hmin = MIN(node->right->height, hmin);
       } 
       else 
       {
           hmin = 0;
       }
   
       delta = 4;
       
       for (i=0; i<hmin; i++) 
       {
           delta = MAX(delta, gap + 1 + rprofile[i] - lprofile[i]);
       }
         
       //If the node has two children of height 1, then we allow the
       //two leaves to be within 1, instead of 2 
       if (((node->left != NULL && node->left->height == 1) ||
           (node->right != NULL && node->right->height == 1))&&delta>4) 
       {
          delta--;
       }
           
       node->edge_length = ((delta+1)/2) - 1;
    }
  
    //now fill in the height of node
    h = 1;
    if (node->left != NULL) 
    {
        h = MAX(node->left->height + node->edge_length + 1, h);
    }
    if (node->right != NULL) 
    {
        h = MAX(node->right->height + node->edge_length + 1, h);
    }
    node->height = h;
}
 
asciinode* build_ascii_tree_recursive(Tree *t) 
{
  asciinode * node = NULL;
  
  if (t == NULL) 
  {
     return NULL;
  }

  node = calloc(1,sizeof(asciinode));
  node->left = build_ascii_tree_recursive(t->leftchild);
  node->right = build_ascii_tree_recursive(t->rightchild);
  
  if (node->left != NULL) 
  {
    node->left->parent_dir = LEFT;
  }

  if (node->right != NULL) 
  {
    node->right->parent_dir = RIGHT;
  }

  sprintf(node->label, "%s", t->name);
  node->lablen = strlen(node->label);

  return node;
}


//Copy the tree into the ascii node structre
asciinode* build_ascii_tree(Tree *t) 
{
  asciinode *node = NULL;
  if (t == NULL) 
  {
     return NULL;
  }

  //Prepare an ascii tree with all values from the given tree.
  node = build_ascii_tree_recursive(t);
  
  node->parent_dir = ROOT;

  return node;
}

//prints ascii tree for given Tree structure
void print_ascii_tree(Tree *t) 
{
  asciinode *proot = NULL;
  int xmin, i;
  
  if (t == NULL) 
  {
     printf("%s:%d Input parameters are NULL.\n",__FUNCTION__,__LINE__);
     return;
  }

  //Allocate an ascii tree.
  proot = build_ascii_tree(t);

  compute_edge_lengths(proot);
  
  for (i=0; i<proot->height && i < MAX_HEIGHT; i++) 
  {
      lprofile[i] = INFINITY2;
  }
  compute_lprofile(proot, 0, 0);
  xmin = 0;
  for (i = 0; i < proot->height && i < MAX_HEIGHT; i++) 
  {
      xmin = MIN(xmin, lprofile[i]);
  }
  for (i = 0; i < proot->height; i++) 
  {
      print_next = 0;
      print_level(proot, -xmin, i);
      printf("\n");
  }
  if (proot->height >= MAX_HEIGHT) 
  {
      printf("(This tree is taller than %d, and may be drawn incorrectly.)\n", MAX_HEIGHT);
  }
  free_ascii_tree(proot); 
}

//Free all the nodes of the given tree
void free_ascii_tree(asciinode *node) 
{
  if (node == NULL) return;
  free_ascii_tree(node->left);
  free_ascii_tree(node->right);
  free(node);
}


name_value_pair* get_name_value_pair_handle_single_ptr()
{
   return pNameValuePairHandle;
}


name_value_pair** get_name_value_pair_handle_double_ptr()
{
   return &pNameValuePairHandle;
}

char to_lower(unsigned char ch) 
{
    if (ch >= 'A' && ch <= 'Z')
        ch = 'a' + (ch - 'A');
    return ch;
 }

int strcasecmp(const char *s1, const char *s2) 
{
    const unsigned char *us1 = (const u_char *)s1,
                        *us2 = (const u_char *)s2;

    while (to_lower(*us1) == to_lower(*us2++))
        if (*us1++ == '\0')
            return (0);
    return (to_lower(*us1) - to_lower(*--us2));
}

 
name_value_pair* add_name_to_name_value_pair(name_value_pair **ppname_value_pair, char *pWord, long value)
{
   if(!ppname_value_pair || !pWord || (strlen(pWord) >= MAX_WORD_LENGTH))
   {
      printf("Invalid argument.\n");
      return NULL;
   }

   if(*ppname_value_pair == NULL)
   {
      #ifdef USE_HEAP
         *ppname_value_pair = (name_value_pair *) calloc(1, sizeof(name_value_pair));
      #else
      {
         //Use stack.
         msgq_node *p_msgQ_node = NULL;         
         dequeue_a_free_node(&p_msgQ_node);

         if(p_msgQ_node)
         {
            *ppname_value_pair = (name_value_pair *) &g_name_value_pair[p_msgQ_node->index];
         }
         else
         {
            printf("p_msgQ_node is null.\n");
            return NULL;
         }
      }
      #endif
      
      (*ppname_value_pair)->leftchild = NULL;
      (*ppname_value_pair)->rightchild = NULL;
      strcpy((*ppname_value_pair)->name,pWord);
      (*ppname_value_pair)->value = value;
      (*ppname_value_pair)->height = max(height_of_name_value_pair((*ppname_value_pair)->leftchild),
                                      height_of_name_value_pair((*ppname_value_pair)->rightchild)) +1; 
   }
   else if(strncmp(((*ppname_value_pair)->name),pWord,strlen(pWord)) > 0)
   {
       (*ppname_value_pair)->leftchild = add_name_to_name_value_pair(&(*ppname_value_pair)->leftchild,pWord,value);
       (*ppname_value_pair)->height = max(height_of_name_value_pair((*ppname_value_pair)->leftchild),
                                       height_of_name_value_pair((*ppname_value_pair)->rightchild)) +1; 
   }
   else if(strncmp(((*ppname_value_pair)->name),pWord,strlen(pWord)) < 0)
   {
       (*ppname_value_pair)->rightchild = add_name_to_name_value_pair(&(*ppname_value_pair)->rightchild,pWord,value);
       (*ppname_value_pair)->height = max(height_of_name_value_pair((*ppname_value_pair)->leftchild),
                                       height_of_name_value_pair((*ppname_value_pair)->rightchild)) +1; 
   }
   else if(strncmp(((*ppname_value_pair)->name),pWord,strlen(pWord)) == 0)
   {
      //Duplicate. Ignore it for now.
   }

   return (*ppname_value_pair);
}

void print_name_starting_with(name_value_pair **ppname_value_pair, char *pWord)
{
   if(!ppname_value_pair || !(*ppname_value_pair) || !pWord || (strlen(pWord) >= MAX_WORD_LENGTH))
   {
      return;
   }
   
   if(strncmp(((*ppname_value_pair)->name),pWord,strlen(pWord)) > 0)
   {
       print_name_starting_with(&(*ppname_value_pair)->leftchild,pWord);
   }
   else if(strncmp(((*ppname_value_pair)->name),pWord,strlen(pWord)) < 0)
   {
       print_name_starting_with(&(*ppname_value_pair)->rightchild,pWord);
   }
   else if(strncmp(((*ppname_value_pair)->name),pWord,strlen(pWord)) == 0)
   {
       printf("Found %s\n",(*ppname_value_pair)->name);
       
       if(strlen(pWord) < strlen((*ppname_value_pair)->name))
       {
          print_name_starting_with(&(*ppname_value_pair)->leftchild,pWord);
          print_name_starting_with(&(*ppname_value_pair)->rightchild,pWord);
       }
   }
}

void delete_name_starting_with(name_value_pair **ppname_value_pair, char *pWord)
{
   if(!ppname_value_pair || !(*ppname_value_pair) || !pWord || (strlen(pWord) >= MAX_WORD_LENGTH))
   {
      return;
   }
   
   if(strncmp(((*ppname_value_pair)->name),pWord,strlen(pWord)) > 0)
   {
       delete_name_starting_with(&(*ppname_value_pair)->leftchild,pWord);
   }
   else if(strncmp(((*ppname_value_pair)->name),pWord,strlen(pWord)) < 0)
   {
       delete_name_starting_with(&(*ppname_value_pair)->rightchild,pWord);
   }
   else if(strncmp(((*ppname_value_pair)->name),pWord,strlen(pWord)) == 0)
   {
       printf("Deleting %s\n",(*ppname_value_pair)->name);
       
       delete_a_name_from_name_value_pair(ppname_value_pair,(*ppname_value_pair)->name);
       
       if(strlen(pWord) < strlen((*ppname_value_pair)->name))
       {
          delete_name_starting_with(&(*ppname_value_pair)->leftchild,pWord);
          delete_name_starting_with(&(*ppname_value_pair)->rightchild,pWord);
       }
   }
}


void fetch_name_value_pair_size(name_value_pair *pname_value_pair, long *pname_value_pairSize)
{
   if(!pname_value_pair || !pname_value_pairSize)
   {
      return;
   }
   
   fetch_name_value_pair_size(pname_value_pair->leftchild,pname_value_pairSize);   

   (*pname_value_pairSize)++;

   fetch_name_value_pair_size(pname_value_pair->rightchild,pname_value_pairSize);   
}

void print_name_value_pair_content(name_value_pair *pname_value_pair)
{
   if(!pname_value_pair)
   {
      return;
   }
   
   print_name_value_pair_content(pname_value_pair->leftchild);

   printf("name = %s,height=%d,left=%d,right=%d\n",pname_value_pair->name,pname_value_pair->height,
    height_of_name_value_pair((pname_value_pair)->leftchild),
    height_of_name_value_pair((pname_value_pair)->rightchild));

   print_name_value_pair_content(pname_value_pair->rightchild);
}

void write_names_in_unsorted_format(name_value_pair *pname_value_pair, FILE *pOutFile)
{
   if(!pname_value_pair)
   {
      return;
   }
   
   write_names_in_unsorted_format(pname_value_pair->rightchild,pOutFile);

   fprintf (pOutFile, "%s\n",pname_value_pair->name);

   write_names_in_unsorted_format(pname_value_pair->leftchild,pOutFile);
}



void delete_a_name_from_name_value_pair(name_value_pair **ppname_value_pair, char *pWord)
{
#if 0
   if(!ppname_value_pair || !*ppname_value_pair || !pWord)
   {
      return;
   }

   if(strncmp(((*ppname_value_pair)->name),pWord,strlen(pWord)) > 0) 
   {
       delete_a_name_from_name_value_pair(&(*ppname_value_pair)->leftchild, pWord);
   }
   else if(strncmp(((*ppname_value_pair)->name),pWord,strlen(pWord)) < 0)
   {
       delete_a_name_from_name_value_pair(&(*ppname_value_pair)->rightchild, pWord);
   }
   else
   {
       if((*ppname_value_pair)->leftchild && (*ppname_value_pair)->rightchild)
       {
           //Identify the largest node from the left child.
           //swap it with the current node.
           //free the largest node on the left child.
           name_value_pair bt = {0};
           memset(&bt, 0, sizeof(name_value_pair));

           find_largest_name_from_this_current_node((*ppname_value_pair)->leftchild,&bt);

           strcpy((*ppname_value_pair)->name,bt.name);
           (*ppname_value_pair)->value = bt.value;

           //Delete the largest node from the left child.
           delete_a_name_from_name_value_pair(&(*ppname_value_pair)->leftchild,bt.name);
       }
       else if (!((*ppname_value_pair)->leftchild) && ((*ppname_value_pair)->rightchild))
       {
          //swap the right node with the current node and free the right child.
          printf("Over-writing %s with right node %s.\n",(*ppname_value_pair)->name,(*ppname_value_pair)->rightchild->name);

           memset((*ppname_value_pair)->name,0,sizeof(char)*MAX_WORD_LENGTH);
           strcpy((*ppname_value_pair)->name,(*ppname_value_pair)->rightchild->name);

           (*ppname_value_pair)->value = (*ppname_value_pair)->rightchild->value;

           #ifdef USE_HEAP
           free((*ppname_value_pair)->rightchild);
           #else
           memset((*ppname_value_pair)->rightchild,0,sizeof(name_value_pair));
           #endif

           (*ppname_value_pair)->leftchild = (*ppname_value_pair)->rightchild->leftchild;
           (*ppname_value_pair)->rightchild = (*ppname_value_pair)->rightchild->rightchild;
       }
       else if (((*ppname_value_pair)->leftchild) && !((*ppname_value_pair)->rightchild))
       {
           //swap the left node with the current node and free the left child.
           printf("Over-writing %s with left node %s.\n",(*ppname_value_pair)->name,(*ppname_value_pair)->leftchild->name);
           memset((*ppname_value_pair)->name,0,sizeof(char)*MAX_WORD_LENGTH);
           strcpy((*ppname_value_pair)->name,(*ppname_value_pair)->leftchild->name);

           (*ppname_value_pair)->value = (*ppname_value_pair)->leftchild->value;

           #ifdef USE_HEAP
           free((*ppname_value_pair)->leftchild);
           #else
           memset((*ppname_value_pair)->leftchild,0,sizeof(name_value_pair));
           #endif
           
           (*ppname_value_pair)->leftchild = (*ppname_value_pair)->leftchild->leftchild;
           (*ppname_value_pair)->rightchild = (*ppname_value_pair)->leftchild->rightchild;

       }
       else if (!((*ppname_value_pair)->leftchild) && !((*ppname_value_pair)->rightchild))
       {
           printf("clearing %s.\n",(*ppname_value_pair)->name);
           #ifdef USE_HEAP
           free((*ppname_value_pair));
           #else
           memset((*ppname_value_pair),0,sizeof(name_value_pair));
           #endif
           (*ppname_value_pair) = NULL;
       }
   }
#endif   
}

name_value_pair *find_min(name_value_pair *t)
{
  //In a binary tree,at any node the left child of that node will be the smallest and the 
  // right child of that node will be the largest.
  //Therefore, traverse only on the left branch to find the smallest.
  if (t == NULL)
  {
    return NULL;
  }
  else if (t->leftchild == NULL)
  {
    return t;
  }
  else
  {
    return find_min(t->leftchild);
  }
}

name_value_pair* delete_a_name_from_name_value_pair2(name_value_pair *pname_value_pair, char *pWord)
{
   //Deletes node from the tree
   // Return a pointer to the resulting tree
   name_value_pair *x;
   name_value_pair *tmp_cell;
   
   if (!pname_value_pair)
   {
      return NULL;
   }
   
   if(strncmp(((pname_value_pair)->name),pWord,strlen(pWord)) > 0) 
   {
     printf("%s is greater than %s. Going to the left child..\n",((pname_value_pair)->name),pWord);
     (pname_value_pair)->leftchild = delete_a_name_from_name_value_pair2((pname_value_pair)->leftchild,pWord);
   } 
   else if (strncmp(((pname_value_pair)->name),pWord,strlen(pWord)) < 0)
   {
       printf("%s is lesser than %s. Going to the right child..\n",((pname_value_pair)->name),pWord);
       (pname_value_pair)->rightchild = delete_a_name_from_name_value_pair2((pname_value_pair)->rightchild,pWord);
   } 
   else if ((pname_value_pair)->leftchild && (pname_value_pair)->rightchild)
   {
     printf("Found the node(%s) with left(%s) and right(%s) child.\n",
      (pname_value_pair)->name,
      (pname_value_pair)->leftchild->name,
      (pname_value_pair)->rightchild->name);

     //Found the node.
     //Now, find the minimum node on the right child and swap it with the current node that is about to be deleted.
     tmp_cell = find_min((pname_value_pair)->rightchild);

     if(tmp_cell)
     {
        printf("Found a node(%s) on the right of this current node (%s) with no left child.\n",
         tmp_cell->name,
         (pname_value_pair)->name);
        
        memset((pname_value_pair)->name,0,MAX_WORD_LENGTH);
        strcpy((pname_value_pair)->name,tmp_cell->name);
        (pname_value_pair)->value = tmp_cell->value;

        //GO ahead and delete the node that you just swapped with the current node.
        (pname_value_pair)->rightchild = delete_a_name_from_name_value_pair2((pname_value_pair)->rightchild,(pname_value_pair)->name);
     }
   }
   else
   {
      printf("Found the node with only one child.\n");
      tmp_cell = (pname_value_pair);
      if ((pname_value_pair)->rightchild)
      {
        //The current node has only one right child. 
        //Move the current node pointer to this right child for the recursive function to assign it to the previous node.
        (pname_value_pair) = (pname_value_pair)->rightchild;
      }
      else if ((pname_value_pair)->leftchild)
      {
        //The current node has only one left child. 
        //Move the current node pointer to this left child for the recursive function to assign it to the previous node.
        (pname_value_pair) = (pname_value_pair)->leftchild;
      }
#ifdef USE_HEAP
      free(tmp_cell);
#else
      memset(tmp_cell,0,sizeof(name_value_pair));
#endif
   }
   
   return (pname_value_pair);
}

//height of an AVL tree is O(log n)
int height_of_name_value_pair(name_value_pair *node)
{
   if(!node)
    return -1;
   else
    return node->height;
}

name_value_pair *single_rotate_left(name_value_pair *X)
{
   if(!X)
    return NULL;
   
   name_value_pair *W = X->leftchild;

   if(!W)
    return NULL;
   
   X->leftchild = W->rightchild;
   W->rightchild = X;
   X->height = max(height_of_name_value_pair(X->leftchild),
                   height_of_name_value_pair(X->rightchild)) +1;
   W->height = max(height_of_name_value_pair(W->leftchild),
                   (X->height)) +1;
   return W;
}

name_value_pair *single_rotate_right(name_value_pair *W)
{
   if(!W)
    return NULL;
   
   name_value_pair *X = W->rightchild;

   if(!X)
    return NULL;
   
   W->rightchild = X->leftchild;
   X->leftchild = W;
   W->height = max(height_of_name_value_pair(W->rightchild),
                   height_of_name_value_pair(W->leftchild)) +1;
   X->height = max(height_of_name_value_pair(X->rightchild),
                   (W->height)) +1;
   return X;
}

name_value_pair *double_rotate_left_to_right(name_value_pair *Z)
{
   Z->leftchild = single_rotate_right(Z->leftchild);
   return single_rotate_left(Z);
}

name_value_pair *double_rotate_right_to_left(name_value_pair *Z)
{
   Z->rightchild = single_rotate_left(Z->rightchild);
   return single_rotate_right(Z);
}

name_value_pair* avl_tree_rebalance(name_value_pair *pname_value_pair)
{
   if(!pname_value_pair)
   {
      return NULL;
   }

   avl_tree_rebalance(pname_value_pair->leftchild);
   
   printf("name = %s,height=%d,left=%d,right=%d\n",pname_value_pair->name,pname_value_pair->height,
    height_of_name_value_pair((pname_value_pair)->leftchild),
    height_of_name_value_pair((pname_value_pair)->rightchild));
   
   if((height_of_name_value_pair((pname_value_pair)->leftchild) > height_of_name_value_pair((pname_value_pair)->rightchild)) &&
      ((height_of_name_value_pair((pname_value_pair)->leftchild) - height_of_name_value_pair((pname_value_pair)->rightchild)) >= 2))
   {
      printf("Before rebalancing on left side. (%s)...\n",(pname_value_pair)->name);      
      //print_ascii_tree(pname_value_pair);
      
      if((pname_value_pair)->leftchild)
      {      
         printf("Performing single_rotate_left.root(%s:%d) left(%s,%d).\n",
         ((pname_value_pair)->name),
         height_of_name_value_pair((pname_value_pair)),
         ((pname_value_pair)->leftchild->name),
         height_of_name_value_pair((pname_value_pair)->leftchild));
         
         (pname_value_pair) = single_rotate_left((pname_value_pair));
      }
   }
   else if((height_of_name_value_pair((pname_value_pair)->leftchild) < height_of_name_value_pair((pname_value_pair)->rightchild)) &&
      ((height_of_name_value_pair((pname_value_pair)->rightchild) - height_of_name_value_pair((pname_value_pair)->leftchild)) >= 2))
   {
       printf("Before rebalancing on right side. (%s)...\n",(pname_value_pair)->name);      
       //print_ascii_tree(pname_value_pair);
       
       if((pname_value_pair)->rightchild)
       {      
          printf("Performing single_rotate_right.root(%s:%d) right(%s,%d).\n",
          ((pname_value_pair)->name),
          height_of_name_value_pair((pname_value_pair)),
          ((pname_value_pair)->rightchild->name),
          height_of_name_value_pair((pname_value_pair)->rightchild));
          
          (pname_value_pair) = single_rotate_right((pname_value_pair));
       }
   }

   avl_tree_rebalance(pname_value_pair->rightchild);

}


name_value_pair* avl_tree_insert(name_value_pair **ppname_value_pair, char *pWord, long value)
{
   if(!ppname_value_pair || !pWord || (strlen(pWord) >= MAX_WORD_LENGTH))
   {
      printf("Invalid argument.\n");
      return NULL;
   }

   if(*ppname_value_pair == NULL)
   {
      #ifdef USE_HEAP
         *ppname_value_pair = (name_value_pair *) calloc(1, sizeof(name_value_pair));
      #else
      {
         //Use stack.
         msgq_node *p_msgQ_node = NULL;         
         dequeue_a_free_node(&p_msgQ_node);

         if(p_msgQ_node)
         {
            *ppname_value_pair = (name_value_pair *) &g_name_value_pair[p_msgQ_node->index];
         }
         else
         {
            printf("p_msgQ_node is null.\n");
            return NULL;
         }
      }
      #endif
      
      (*ppname_value_pair)->leftchild = NULL;
      (*ppname_value_pair)->rightchild = NULL;
      strcpy((*ppname_value_pair)->name,pWord);
      (*ppname_value_pair)->value = value;
   }
   else if(strncmp(((*ppname_value_pair)->name),pWord,strlen(pWord)) > 0)
   {
       (*ppname_value_pair)->leftchild = avl_tree_insert(&(*ppname_value_pair)->leftchild,pWord,value);

       printf("Left:(%s:%s)%d \n",
        (*ppname_value_pair)->name,
        pWord,
        (*ppname_value_pair)->height);

       if((height_of_name_value_pair((*ppname_value_pair)->leftchild) - height_of_name_value_pair((*ppname_value_pair)->rightchild)) >= 2)
       {
          printf("Before rebalancing on left side. (%s:%s)...\n",
           (*ppname_value_pair)->name,
           pWord);
          
          //print_ascii_tree(get_name_value_pair_handle_single_ptr());

          if((*ppname_value_pair)->leftchild)
          {

             printf("Performing single_rotate_left.root(%s:%d) left(%s,%d).\n",
              ((*ppname_value_pair)->name),
              height_of_name_value_pair((*ppname_value_pair)),
              ((*ppname_value_pair)->leftchild->name),
              height_of_name_value_pair((*ppname_value_pair)->leftchild));
             
             (*ppname_value_pair) = single_rotate_left((*ppname_value_pair));             

#if 0          
             if(strncmp(((*ppname_value_pair)->leftchild->name),pWord,strlen(pWord)) > 0)
             {
                printf("Performing single_rotate_left.root(%s:%d) left(%s,%d).\n",
                 ((*ppname_value_pair)->name),
                 height_of_name_value_pair((*ppname_value_pair)),
                 ((*ppname_value_pair)->leftchild->name),
                 height_of_name_value_pair((*ppname_value_pair)->leftchild));
                
                (*ppname_value_pair) = single_rotate_left((*ppname_value_pair));             
             }
             else
             {
                printf("Performing double_rotate_left_to_right.root(%s:%d) left(%s,%d).\n",
                 ((*ppname_value_pair)->name),
                 height_of_name_value_pair((*ppname_value_pair)),
                 ((*ppname_value_pair)->leftchild->name),
                 height_of_name_value_pair((*ppname_value_pair)->leftchild));
                
                (*ppname_value_pair) = double_rotate_left_to_right((*ppname_value_pair));
             }
#endif             
          }
       }
   }
   else if(strncmp(((*ppname_value_pair)->name),pWord,strlen(pWord)) < 0)
   {
      (*ppname_value_pair)->rightchild = avl_tree_insert(&(*ppname_value_pair)->rightchild,pWord,value);

      printf("Right:(%s:%s)%d \n",
       (*ppname_value_pair)->name,
       pWord,
       (*ppname_value_pair)->height);


      if((height_of_name_value_pair((*ppname_value_pair)->rightchild) - height_of_name_value_pair((*ppname_value_pair)->leftchild)) >= 2)
      {
         printf("Before rebalancing on right side. (%s:%s)...\n",
          (*ppname_value_pair)->name,
          pWord);

         //print_ascii_tree(get_name_value_pair_handle_single_ptr());

         if((*ppname_value_pair)->rightchild)
         {
            printf("Performing single_rotate_right.root(%s:%d) right(%s,%d).\n",
             ((*ppname_value_pair)->name),
             height_of_name_value_pair((*ppname_value_pair)),
             ((*ppname_value_pair)->rightchild->name),
             height_of_name_value_pair((*ppname_value_pair)->rightchild));
          
          (*ppname_value_pair) = single_rotate_right((*ppname_value_pair));             

            #if 0
            if(strncmp(((*ppname_value_pair)->rightchild->name),pWord,strlen(pWord)) > 0)
            {
               printf("Performing single_rotate_right.root(%s:%d) right(%s,%d).\n",
                ((*ppname_value_pair)->name),
                height_of_name_value_pair((*ppname_value_pair)),
                ((*ppname_value_pair)->rightchild->name),
                height_of_name_value_pair((*ppname_value_pair)->rightchild));
   
               (*ppname_value_pair) = single_rotate_right((*ppname_value_pair));             
            }
            else
            {
               printf("Performing double_rotate_right_to_left.root(%s:%d) right(%s,%d).\n",
                ((*ppname_value_pair)->name),
                height_of_name_value_pair((*ppname_value_pair)),
                ((*ppname_value_pair)->rightchild->name),
                height_of_name_value_pair((*ppname_value_pair)->rightchild));
               
               (*ppname_value_pair) = double_rotate_right_to_left((*ppname_value_pair));
            }
            #endif
            
         }
      }
   }
   else if(strncmp(((*ppname_value_pair)->name),pWord,strlen(pWord)) == 0)
   {
      //Duplicate. Ignore it for now.
   }

   (*ppname_value_pair)->height = max(height_of_name_value_pair((*ppname_value_pair)->leftchild),
                                      height_of_name_value_pair((*ppname_value_pair)->rightchild)) +1;


   return (*ppname_value_pair);
}

void find_the_depth_of_the_name_value_pair(name_value_pair *pname_value_pair, long *pname_value_pairDepth)
{
   if(!pname_value_pair || !pname_value_pairDepth)
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
       find_the_depth_of_the_name_value_pair(pname_value_pair->leftchild, &left_depth);
       long right_depth = 0;
       find_the_depth_of_the_name_value_pair(pname_value_pair->rightchild, &right_depth);
       
       //printf("left_depth=%ld,right_depth=%ld\n",left_depth,right_depth);
       
       if(left_depth > right_depth)
       {
           *pname_value_pairDepth +=(left_depth+1);
       }
       else
       {
           *pname_value_pairDepth +=(right_depth+1);
       }
       //printf("returning *p_bt_length=%ld\n",*p_bt_length);
   }
   
}

int random_between(int min, long max)
{
   return (rand() % (max - min + 1) + min);
}

void generate_a_name_value_pair()
{
   long index = 0;
   char word[MAX_WORD_LENGTH];
   
   Initialize((unsigned)(time(NULL)));
   srand((unsigned) (time(NULL)));

   while(index < MAX_NUMBER_OF_WORDS_TO_BE_READ)
   {
     int count = 0;
     int random_number = 0;
     int random_number2 = 0;

     memset((unsigned char *)word,0,MAX_WORD_LENGTH);         
     char *pWord = word;

     for(count=0; count < MAX_WORD_LENGTH-1; count++)
     {
        char c = 0;
        //random_number = random_between(1,(MAX_WORD_LENGTH-1));
        random_number2 = rand()%(MAX_NUMBER_OF_ALPHABETS);
        random_number = ExtractU32()%MAX_NUMBER_OF_ALPHABETS;      
        //printf("random_number=%d,random_number2=%d\n",random_number,random_number2);
        c = 'a'+(random_number2%MAX_NUMBER_OF_ALPHABETS);
        *(pWord + count) = c;
     }   
      //printf("word=%s,strlen=%ld\n",word,strlen(word));
      avl_tree_insert(get_name_value_pair_handle_double_ptr(),word,index);
      index++;
   }

}


#ifndef USE_HEAP

void *create_msgq(void *arg)
{
   long index = 0;

   for(index =0; index < MAX_NUMBER_OF_WORDS_TO_BE_READ; index++)
   {
      memset(&g_msgq_node[index],0,sizeof(msgq_node));
      g_msgq_node[index].index = index;
      g_msgq_node[index].pLink = NULL;
      
      if((g_msgQ.pFront == NULL) || (g_msgQ.pRear == NULL))
      {
         g_msgQ.pFront = g_msgQ.pRear = (msgq_node *) &(g_msgq_node[index]);
      }
      else
      {
         g_msgQ.pRear->pLink = (msgq_node *) &(g_msgq_node[index]);
         g_msgQ.pRear = (msgq_node *) g_msgQ.pRear->pLink;
      }
   }
   
   pthread_exit(NULL);

   return NULL;
}

void dequeue_a_free_node(msgq_node **pp_msgQ_node)
{
   if(g_msgQ.pFront == NULL)
   {
      printf("No message in messageQ.\n");
      return;
   }

   if(!pp_msgQ_node)
   {
      printf("p_msgQ_node is NULL.\n");
      return;
   }

   *pp_msgQ_node = g_msgQ.pFront;

   g_msgQ.pFront = (msgq_node *) g_msgQ.pFront->pLink;   
}

#endif



void *create_name_value_pair_database(void *arg)
{
   FILE *pFile = NULL;
   FILE *pOutFile = NULL;
   char word[MAX_WORD_LENGTH];
   long count = 0;
   long index = 0;
   long identifier = 0;

   #ifdef READ_WORDS_FROM_TEXT_FILE
   
   pFile = fopen (UNSORTED_NAMES,"r+");

   if (pFile == NULL)
   {
     printf("Unable to open the file\n");
     return NULL;
   }
   else
   {
      printf("Opened the input file successfully.\n");

      while((index < MAX_NUMBER_OF_WORDS_TO_BE_READ) &&
            (fscanf (pFile,"%s %ld",word,&identifier) > 0))
      {
         //puts(word);
         if(strlen(word) > 1)
         {
            printf("word=%s,strlen=%ld\n",word,strlen(word));
            #ifdef AVL_TREE
            avl_tree_insert(get_name_value_pair_handle_double_ptr(),word,identifier);
            #else
            add_name_to_name_value_pair(get_name_value_pair_handle_double_ptr(),word,identifier);
            //print_ascii_tree(get_name_value_pair_handle_single_ptr());
            #endif
            index++;
         }
         memset((unsigned char *)word,0,MAX_WORD_LENGTH);         
         identifier = 0;
      }
      //usleep(1000000);
      
      fclose (pFile);
   
      //print_name_value_pair_content(get_name_value_pair_handle_single_ptr());
      //create_unsorted_name_text_file();
   }
   #else
       generate_a_name_value_pair();
   #endif

   pthread_exit(NULL);

   return NULL;

}

void create_unsorted_name_text_file()
{
   FILE *pOutFile = NULL;
   char word[MAX_WORD_LENGTH];
   long count = 0;
   long index = 0;
   
   //pOutFile = fopen ("../english-words/unsorted-words4.txt","w+");
   pOutFile = fopen ("../english-words/unsorted-words4.txt","w+");

   if (pOutFile == NULL)
   {
     printf("Unable to open the pOutFile \n");
     return;
   }
   else
   {
      printf("Opened the output file successfully.\n");
   
      write_names_in_unsorted_format(get_name_value_pair_handle_single_ptr(),pOutFile);
      
      fclose (pOutFile);   
   }
}

void find_largest_name_from_this_current_node(name_value_pair *pname_value_pair,name_value_pair *pLargest_name_value_pair)
{
   if(!pname_value_pair || !pLargest_name_value_pair)
   {
      return;
   }

   find_largest_name_from_this_current_node(pname_value_pair->leftchild,pLargest_name_value_pair);
   
   if(strncmp(pname_value_pair->name,pLargest_name_value_pair->name,strlen(pLargest_name_value_pair->name)) > 0)
   {
      //current node is greater than the pLargerst name value pair.Overwrite, pname_value_pair with this one.
      memset(pLargest_name_value_pair,0,sizeof(name_value_pair));
      memcpy(pLargest_name_value_pair,pname_value_pair,sizeof(name_value_pair));
   }

   find_largest_name_from_this_current_node(pname_value_pair->rightchild,pLargest_name_value_pair);
}



void main()
{
   char input_str[MAX_WORD_LENGTH];
   char command[MAX_COMMAND_LENGTH];
   pthread_t g_thread;
   int s = 0;
   #ifndef USE_HEAP
   pthread_t g_create_msgQ_thread;
   #endif
    
   pthread_attr_t attr;
   
   pthread_attr_init(&attr);

#ifndef USE_HEAP
   s = pthread_create(&g_create_msgQ_thread, &attr, create_msgq, NULL);
   sleep(1);
#endif

   s = pthread_create(&g_thread, &attr, create_name_value_pair_database, NULL);


   do
   {
      long total_number_of_elements = 0;
      
      printf("\n Enter your command:<count|dump|depth|delete|find|tree|rebalance|quit>\n");
      memset(command,0,MAX_COMMAND_LENGTH);
      scanf("%s",command);
      printf("You entered %s\n",command);

      if(strcmp("count",command) == 0)
      {
         fetch_name_value_pair_size(get_name_value_pair_handle_single_ptr(),&total_number_of_elements);      
         printf("Total number of elements = %ld\n",total_number_of_elements);
      }
      else if(strcmp("dump",command) == 0)
      {
         print_name_value_pair_content(get_name_value_pair_handle_single_ptr());
      }
      else if(strcmp("tree",command) == 0)
      {
         print_ascii_tree(get_name_value_pair_handle_single_ptr());
      }
      else if(strcmp("rebalance",command) == 0)
      {
         avl_tree_rebalance(get_name_value_pair_handle_single_ptr());
      }
      else if (strcmp("depth",command) == 0)
      {
         long name_value_pairDepth = 0;
         find_the_depth_of_the_name_value_pair(get_name_value_pair_handle_single_ptr(),&name_value_pairDepth);
         printf("name_value_pairDepth = %ld\n",name_value_pairDepth);
      }
      else if (strcmp("delete",command) == 0)
      {
         memset(input_str,0,MAX_WORD_LENGTH);
         printf("\n Enter the name to be deleted:\n");
         scanf("%s",input_str);

         //delete_name_starting_with(get_name_value_pair_handle_double_ptr(),p_ch);

         printf("Deleting %s\n",input_str);
         
         delete_a_name_from_name_value_pair2(get_name_value_pair_handle_single_ptr(),input_str);
      }
      else if (strcmp("find",command) == 0)
      {
         memset(input_str,0,MAX_WORD_LENGTH);
         printf("\n Enter the name to be found:\n");
         scanf("%s",input_str);
         
         printf("Finding names that sound like %s\n",input_str);
         
         print_name_starting_with(get_name_value_pair_handle_double_ptr(),input_str);
      }
   } while(strcmp("quit",command) != 0);
}


