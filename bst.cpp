//
// Created by ssriram78 on 9/25/18.
//
#include <stdio.h>
#include <stdlib.h>
#include <iostream>
#include <string.h>

#include "bst.h"
#include "name_value_pair.h"

bool bst::check_is_bst_recurse(bst::Node *head,int *min,int *max,int *height) {
    bool is_bst = false;
    if (head == nullptr) {
        return true;
    }
    int this_left_min=*min;
    int this_left_max=head->val;
    int this_left_height=0;

    bool is_left_bst = check_is_bst_recurse(head->left,
            &this_left_min,
            &this_left_max,
            &this_left_height);
    printf("For current node %d, Left min = %d, left max = %d, left height = %d.\n",
           head->val,
           this_left_min,
           this_left_max,
           this_left_height);
    int this_right_min=head->val;
    int this_right_max=*max;
    int this_right_height=0;

    bool is_right_bst = check_is_bst_recurse(head->right,
            &this_right_min,
            &this_right_max,
            &this_right_height);
    printf("For current node %d, Right min = %d, Right max = %d, Right height = %d.\n",
            head->val,
            this_right_min,
            this_right_max,
            this_right_height);

    if (is_left_bst && is_right_bst) {
        printf("Both sides are BST\n");
        is_bst = true;
    }
    *min = this_left_min;
    *max = this_right_max;
    *height = max(this_left_height,this_right_height)+1;

    if (is_bst) {
        is_bst = head->val > *min && head->val < *max;
        return is_bst;
    }
}

bool bst::check_is_bst(void *head) {
    bool is_bst = false;
    if (head == NULL) {
        return is_bst;
    }
    int min = bst::MIN_NUM;
    int max = bst::MAX_NUM;
    int height = 0;
    is_bst = check_is_bst_recurse((Node *)head,&min,&max,&height);
    printf("Finally, is_bst is %d, min=%d, max=%d, height=%d.\n",
            is_bst,
            min,
            max,
            height);
}

bool bst::check_is_bst() {
    return this->check_is_bst(this->bst_node);
}

void *bst::create_bst() {
    Node *head = (Node *) calloc(1,sizeof(struct Node));
    return (void *)head;
}

void *bst::add_to_bst_recurse(Node *head, int val) {
    if (head == NULL) {
        Node *p_current_node = (Node *) calloc(1, sizeof(struct Node));
        p_current_node->val = val;
        return (void *)p_current_node;
    }
    else if (val < head->val)
    {
        head->left = (Node *) add_to_bst_recurse(head->left, val);
    }
    else {
        head->right = (Node *) add_to_bst_recurse(head->right, val);
    }
}

void bst::add_to_bst(void *head, int val) {
    if (head == NULL) {
        printf("Head is NULL.\n");
        return;
    }

    add_to_bst_recurse((Node *)head,val);
}

void bst::add_to_bst(int val) {
    this->bst_node = (Node *) add_to_bst_recurse(bst_node,val);
}

bst::bst() {
    this->bst_node = NULL;
}

void bst::print_bst(void *head) {
    if (head == NULL) {
        return;
    }
    print_bst(((Node *)head)->left);
    printf("value=%d\n",((Node *)head)->val);
    print_bst(((Node *)head)->right);
}

void bst::print_bst() {
    print_bst(this->bst_node);
}

int main() {
    bst bst_obj;
    void *head = bst_obj.create_bst();
    int array[] = {1,9,4,2,8,5,0,3,7};
    for (int i = 0; i < sizeof(array)/sizeof(array[0]); i++) {
        printf("Adding %d to bst.\n",array[i]);
        bst_obj.add_to_bst(head,array[i]);
    }
    bst_obj.print_bst(head);
    for (int i = 0; i < sizeof(array)/sizeof(array[0]); i++) {
        printf("Adding %d to bst.\n",array[i]);
        bst_obj.add_to_bst(array[i]);
    }

    bst_obj.print_bst();

    bst_obj.check_is_bst();
}
