//
// Created by ssriram78 on 9/25/18.
//

#ifndef C_C_BST_H
#define C_C_BST_H


class bst {
private:
    struct Node {
        int val;
        Node *left;
        Node *right;
    };
    Node *bst_node;
public:
int  MIN_NUM = -999999;
int MAX_NUM  = 9999999;

    bool check_is_bst_recurse(bst::Node *head,int *min,int *max,int *height);

    bool check_is_bst(void *head);

    bool check_is_bst();

    void *create_bst();

    void *add_to_bst_recurse(Node *head, int val);

    void print_bst(void *head);

    void print_bst();

    void add_to_bst(void *head, int val);

    void add_to_bst(int val);

    bst();
};


#endif //C_C_BST_H
