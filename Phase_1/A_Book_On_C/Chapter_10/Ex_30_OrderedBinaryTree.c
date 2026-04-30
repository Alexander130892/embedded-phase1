/*
 * File:    Ex_30_OrderedBinaryTree.c
 * Author:  Alexander130892
 * Date:    5-4-2026
 *
 * Description:
 *   This file implements a binary search tree (BST) data structure
 *   that maintains elements in sorted order, providing functions to
 *   create the tree by inserting elements, traverse it in-order, and
 *   count various properties like total nodes, leaf nodes, and tree
 *   depth. The program demonstrates BST operations by building a tree
 *   from an integer array and testing the counting functions with
 *   expected results.
 */
 //Libraries
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <assert.h>
#include <limits.h>
#include <ctype.h>



typedef int    DATA;
typedef struct node{
    DATA data;
    struct node* left;
    struct node* right;
} NODE;

typedef NODE* BinaryTree;

BinaryTree initNode(DATA d, BinaryTree l, BinaryTree r);
BinaryTree createTree(DATA a[], int i, int size);
void inorder(BinaryTree root);
int countNodes(BinaryTree root);
int countIFNodes(BinaryTree root, DATA search);
int countLeaves(BinaryTree root);
int countDepth(BinaryTree root);
BinaryTree createBST(DATA a[], int size);
BinaryTree insert(BinaryTree root, DATA d);

int main() {
    // Array represents tree level by level (breadth-first):
    //
    //            A
    //          /   \
    //         B     C
    //        / \   / \
    //       D   E F   G
    //      / \
    //     H   I
    //
    DATA a[] = {5,4,7,3,8,1,6,9,0};
    int size  = sizeof(a) / sizeof(a[0]);

    BinaryTree tree = createBST(a, size);


    // Inorder: left → root → right 
    printf("Inorder   : ");
    inorder(tree);
    printf("\n");

     // Test counting of Nodes
    printf("\nCount of Nodes (Expect 9): %d\n",countNodes(tree));
    printf("\nCount of Nodes with '3' (Expect 1): %d\n",countIFNodes(tree, 3));
    printf("\nCount of Leaves (Expect 5): %d\n",countLeaves(tree));
    printf("\nDepth of tree (Expect 4): %d\n",countDepth(tree));

    // Test single node tree
    printf("\nSingle node:\n");
    BinaryTree single = createBST(a, 1);
    printf("Preorder  : ");
    preorder(single);
    printf("\n");

    // Test empty tree
    printf("\nEmpty tree:\n");
    BinaryTree empty = createBST(a, 0);
    printf("Preorder  : ");
    preorder(empty);
    printf("(nothing)\n");
    

    // Test manual construction via initNode
    printf("\nManual tree (X is root, Y left, Z right):\n");
    BinaryTree manual = initNode(5, initNode(8, NULL, NULL),
                                      initNode(3, NULL, NULL));
    printf("Inorder   : ");
    inorder(manual);
    printf("\n");

    
    return EXIT_SUCCESS;
}


BinaryTree initNode(DATA d, BinaryTree l, BinaryTree r){
    BinaryTree BT;
    BT = malloc(sizeof(NODE));
    BT->data = d;
    BT->left = l;
    BT->right = r;
    return BT;
}

BinaryTree insert(BinaryTree root, DATA d) {
    if (root == NULL)
        return initNode(d, NULL, NULL);  // found insertion point
    if (d < root->data)
        root->left  = insert(root->left, d);   // go left if smaller
    else if (d > root->data)
        root->right = insert(root->right, d);  // go right if larger
    // equal → ignore duplicate
    return root;
}
BinaryTree createBST(DATA a[], int size) {
    BinaryTree root = NULL;
    for (int i = 0; i < size; i++)
        root = insert(root, a[i]);
    return root;
}
int countNodes(BinaryTree root){
    int count =0;
    if(root==NULL)
        return count;
    else{
        count++;
        count+= countNodes(root->left);
        count+= countNodes(root->right);
        return count;
    }
}
int countIFNodes(BinaryTree root, DATA search){
    int count =0;
    if(root==NULL)
        return count;
    else{
        if(root->data == search) count++;
        count+= countIFNodes(root->left, search);
        count+= countIFNodes(root->right, search);
        return count;
    }
}
int countLeaves(BinaryTree root){
    int count =0;
    if(root==NULL)
        return count;
    else{
        if(root->left==NULL && root->right == NULL) count++;
        count+= countLeaves(root->left);
        count+= countLeaves(root->right);
        return count;
    }
}
int countDepth(BinaryTree root) {
    if (root == NULL) return 0;
    int left  = countDepth(root->left);
    int right = countDepth(root->right);
    return 1 + (left > right ? left : right);
}
void inorder(BinaryTree root){
    if(root != NULL){
        inorder(root->left);
        printf("%d ", root->data);
        inorder(root->right);
    }
}