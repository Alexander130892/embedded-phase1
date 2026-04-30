/*
 * File:    Ex_29_BinaryTree.c
 * Author:  Alexander130892
 * Date:    5-4-2026
 *
 * Description:
 *   This file implements a binary tree data structure with character
 *   data, providing functions to create trees from arrays using
 *   level-order indexing and perform common tree operations. It
 *   includes tree traversal algorithms (preorder, inorder, postorder),
 *   counting functions for nodes/leaves/depth, and demonstrates these
 *   operations with a sample tree containing characters A through I.
 */
 //Libraries
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <assert.h>
#include <limits.h>
#include <ctype.h>



typedef char    DATA;
typedef struct node{
    DATA data;
    struct node* left;
    struct node* right;
} NODE;

typedef NODE* BinaryTree;

BinaryTree initNode(DATA d, BinaryTree l, BinaryTree r);
BinaryTree createTree(DATA a[], int i, int size);
void inorder(BinaryTree root);
void postorder(BinaryTree root);
void preorder(BinaryTree root);
int countNodes(BinaryTree root);
int countIFNodes(BinaryTree root, DATA search);
int countLeaves(BinaryTree root);
int countDepth(BinaryTree root);

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
    DATA a[] = {'A', 'B', 'C', 'D', 'B', 'F', 'G', 'H', 'I'};
    int size  = sizeof(a) / sizeof(a[0]);

    BinaryTree tree = createTree(a, 0, size);

    // Preorder: root → left → right (expect: A B D H I E C F G)
    printf("Preorder  : ");
    preorder(tree);
    printf("\n");

    // Inorder: left → root → right (expect: H D I B E A F C G)
    printf("Inorder   : ");
    inorder(tree);
    printf("\n");

    // Postorder: left → right → root (expect: H I D E B F G C A)
    printf("Postorder : ");
    postorder(tree);
    printf("\n");

     // Test counting of Nodes
    printf("\nCount of Nodes (Expect 9): %d\n",countNodes(tree));
    printf("\nCount of Nodes with 'B' (Expect 2): %d\n",countIFNodes(tree, 'B'));
    printf("\nCount of Leaves (Expect 5): %d\n",countLeaves(tree));
    printf("\nDepth of tree (Expect 4): %d\n",countDepth(tree));

    // Test single node tree
    printf("\nSingle node:\n");
    BinaryTree single = createTree(a, 0, 1);
    printf("Preorder  : ");
    preorder(single);
    printf("\n");

    // Test empty tree
    printf("\nEmpty tree:\n");
    BinaryTree empty = createTree(a, 0, 0);
    printf("Preorder  : ");
    preorder(empty);
    printf("(nothing)\n");
    

    // Test manual construction via initNode
    printf("\nManual tree (X is root, Y left, Z right):\n");
    BinaryTree manual = initNode('X', initNode('Y', NULL, NULL),
                                      initNode('Z', NULL, NULL));
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

BinaryTree createTree(DATA a[], int i, int size){
    if (i >= size){
        return NULL;
    }else {
        return(initNode(a[i], createTree(a, 2*i+1, size), createTree(a, 2*i+2, size)));
    }
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
        printf("%c ", root->data);
        inorder(root->right);
    }
}
void preorder(BinaryTree root){
    if(root != NULL){
        printf("%c ", root->data);
        preorder(root->left);
        preorder(root->right);
    }
}
void postorder(BinaryTree root){
    if(root != NULL){
        postorder(root->left);
        postorder(root->right);
        printf("%c ", root->data);
    }
}
    