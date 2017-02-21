//
//  sorted_array.h
//  hashtable_c
//
//  Created by Sriram Sridhar on 2/18/17.
//  Copyright © 2017 Sriram Sridhar. All rights reserved.
//

#ifndef sorted_array_h
#define sorted_array_h

#include "hash_table.h"

#ifndef PLAYER_NAME_MAX
#define PLAYER_NAME_MAX 100
#endif

typedef struct _player_details
{
    char player_name[PLAYER_NAME_MAX];
    long player_score;
    
} player_details;


#endif /* sorted_array_h */
