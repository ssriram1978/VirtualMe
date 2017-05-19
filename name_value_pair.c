//
//  name_value_pair.c
//  
//
//  Created by Sriram Sridhar on 5/19/17.
//  Copyright © 2017 Sriram Sridhar. All rights reserved.
//

#include "name_value_pair.h"

name_value_pair *pNameValuePairHandle = NULL;

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


void add_name_to_name_value_pair(name_value_pair **pname_value_pair, char *pWord, long value)
{
   if(!pname_value_pair || !pWord || (strlen(pWord) >= MAX_WORD_LENGTH))
   {
      printf("Invalid argument.\n");
      return;
   }

   if(*pname_value_pair == NULL)
   {
      *pname_value_pair = (name_value_pair *) calloc(1, sizeof(name_value_pair));
      (*pname_value_pair)->leftchild = NULL;
      (*pname_value_pair)->rightchild = NULL;
      strcpy((*pname_value_pair)->name,pWord);
      (*pname_value_pair)->value = value;
   }
   else if(strcasecmp(((*pname_value_pair)->name),pWord) > 0)
   {
       add_name_to_name_value_pair(&(*pname_value_pair)->leftchild,pWord,value);
   }
   else
   {
       add_name_to_name_value_pair(&(*pname_value_pair)->rightchild,pWord,value);
   }
}

void print_name_starting_with(name_value_pair **pname_value_pair, char *pWord)
{
   if(!pname_value_pair || !(*pname_value_pair) || !pWord || (strlen(pWord) >= MAX_WORD_LENGTH))
   {
      return;
   }
   
   if(strncmp(((*pname_value_pair)->name),pWord,strlen(pWord)) > 0)
   {
       print_name_starting_with(&(*pname_value_pair)->leftchild,pWord);
   }
   else if(strncmp(((*pname_value_pair)->name),pWord,strlen(pWord)) < 0)
   {
       print_name_starting_with(&(*pname_value_pair)->rightchild,pWord);
   }
   else if(strncmp(((*pname_value_pair)->name),pWord,strlen(pWord)) == 0)
   {
       printf("Found %s\n",(*pname_value_pair)->name);
       
       if(strlen(pWord) < strlen((*pname_value_pair)->name))
       {
          print_name_starting_with(&(*pname_value_pair)->leftchild,pWord);
          print_name_starting_with(&(*pname_value_pair)->rightchild,pWord);
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

   printf("%s\n",pname_value_pair->name);

   print_name_value_pair_content(pname_value_pair->rightchild);
}

void delete_a_name_from_name_value_pair(name_value_pair **pname_value_pair, char *pWord)
{
   //Difficult one. TBD
}

void find_the_depth_of_the_name_value_pair(name_value_pair **pname_value_pair, long *pname_value_pairDepth)
{
   //TBD
}

int random_between(int min, long max)
{
   return (rand() % (max - min + 1) + min);
}

void generate_a_word(char *pWord)
{
   int count = 0;
   int random_number = 0;
   
   if(!pWord)
    return;

   srand((unsigned) time(NULL));

   for(count=0; count < MAX_WORD_LENGTH-1; count++)
   {
      char c = 0;
      random_number = random_between(1,(MAX_WORD_LENGTH-1));
      //printf("random_number=%d\n",random_number);
      c = 'a'+random_number;
      *(pWord + count) = c;
   }   
}


void *create_name_value_pair_database(void *arg)
{
   FILE *pFile = NULL;
   char word[MAX_WORD_LENGTH];
   long count = 0;

   pFile = fopen ("../english-words/words2.txt","r+");
   
   if (pFile == NULL)
   {
     printf("Unable to open the file\n");
     return NULL;
   }
   else
   {
      printf("Opened the file successfully.\n");
   
      //generate_a_word(word);
      while (fgets (word,MAX_WORD_LENGTH,pFile) != NULL)
      {
         //puts(word);
         //printf("word=%s\n",word);
         add_name_to_name_value_pair(get_name_value_pair_handle_double_ptr(),word,count);
         memset((unsigned char *)word,0,MAX_WORD_LENGTH);
      }
      //usleep(1000000);
      
      fclose (pFile);
   
      print_name_value_pair_content(get_name_value_pair_handle_single_ptr());
   }
   
   pthread_exit(NULL);

   return NULL;

}

void main()
{
   char input_str[MAX_WORD_LENGTH];
   pthread_t g_thread;
   int s = 0;
   
   pthread_attr_t attr;
   
   pthread_attr_init(&attr);

   s = pthread_create(&g_thread, &attr, create_name_value_pair_database, NULL);

   do
   {
      long total_number_of_elements = 0;
      
      fetch_name_value_pair_size(get_name_value_pair_handle_single_ptr(),&total_number_of_elements);
      
      printf("Total number of elements = %ld\n",total_number_of_elements);
      printf("\n Enter the string to search:");
      memset(input_str,0,MAX_WORD_LENGTH);
      scanf("%s",input_str);
      printf("You entered %s\n",input_str);
      print_name_starting_with(get_name_value_pair_handle_double_ptr(),input_str);
   } while(strcmp("quit",input_str) != 0);
}


