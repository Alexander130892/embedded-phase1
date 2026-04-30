/*
 * File:    Ex_34_BT_levelOrder.c
 * Author:  Alexander130892
 * Date:    5-4-2026
 *
 * Description:
 *   This C file implements a binary search tree (BST) with various
 *   tree operations including insertion, deletion, traversal methods
 *   (inorder and level-order), and utility functions for counting
 *   nodes, leaves, and tree depth. The main function demonstrates
 *   these operations by creating a BST from an array and performing
 *   different tree analysis tasks.
 */
 //Libraries
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <assert.h>
#include <limits.h>
#include <ctype.h>
#define QUEUE_SIZE 100




typedef int    DATA;
typedef struct node{
    DATA data;
    struct node* left;
    struct node* right;
} NODE;

typedef NODE* BinaryTree;
typedef struct {
    BinaryTree data[QUEUE_SIZE];
    int front;
    int rear;
    int cnt;
} BTQueue;

BinaryTree initNode(DATA d, BinaryTree l, BinaryTree r);
BinaryTree createTree(DATA a[], int i, int size);
void inorder(BinaryTree root);
void levelOrder(BinaryTree root);
int countNodes(BinaryTree root);
int countIFNodes(BinaryTree root, DATA search);
int countLeaves(BinaryTree root);
int countDepth(BinaryTree root);
BinaryTree createBST(DATA a[], int size);
BinaryTree insert(BinaryTree root, DATA d);
void BST_to_array(BinaryTree root, DATA output[],int* index);
BinaryTree findmax(BinaryTree root);
BinaryTree deleteNode(BinaryTree root, DATA d);
void btq_init(BTQueue* q);
BinaryTree btq_dequeue(BTQueue* q);
void btq_enqueue(BTQueue* q, BinaryTree node);

int main() {
    // Array represents tree level by level (breadth-first):
    //
    //            5
    //          /   \
    //         4     7
    //        /     / \
    //       3     6   8
    //      /           \
    //     1             9   
    //    /
    //   0
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
    printf("\nCount of Leaves (Expect 3): %d\n",countLeaves(tree));
    printf("\nDepth of tree (Expect 5): %d\n",countDepth(tree));

    // LevelOrder
    levelOrder(tree);

    return EXIT_SUCCESS;
}
void BST_to_array(BinaryTree root, DATA output[], int* p_index){
    if(root != NULL){
        BST_to_array(root->left, output, p_index);
        output[(*p_index)++]=root->data;
        BST_to_array(root->right, output, p_index);
    }
}

BinaryTree deleteNode(BinaryTree root, DATA d) {
    if (root == NULL) return NULL;  // not found

    if (d < root->data)
        root->left  = deleteNode(root->left, d);   // search left
    else if (d > root->data)
        root->right = deleteNode(root->right, d);  // search right
    else {
        // FOUND — three cases:

        // Case 1 — leaf node, just remove it
        if (root->left == NULL && root->right == NULL) {
            free(root);
            return NULL;
        }
        // Case 2a — one child (right only)
        else if (root->left == NULL) {
            BinaryTree temp = root->right;
            free(root);
            return temp;
        }
        // Case 2b — one child (left only)
        else if (root->right == NULL) {
            BinaryTree temp = root->left;
            free(root);
            return temp;
        }
        // Case 3 — two children
        // replace with inorder successor (smallest in right subtree)
        else {
            BinaryTree successor = findmax(root->right);
            root->data  = successor->data;                    // copy value
            root->right = deleteNode(root->right, successor->data); // delete successor
        }
    }
    return root;
}

BinaryTree findmax(BinaryTree root) {
    while (root->right != NULL)
        root = root->right;
    return root;
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
void levelOrder(BinaryTree root) {
    if (root == NULL) return;

    BTQueue q;
    btq_init(&q);
    btq_enqueue(&q, root);

    int level     = 1;
    int curr_cnt  = 1;  // nodes remaining on current level
    int next_cnt  = 0;  // nodes queued for next level

    printf("Level %d: ", level);

    while (q.cnt > 0) {
        BinaryTree node = btq_dequeue(&q);
        printf("%d ", node->data);
        curr_cnt--;

        if (node->left != NULL) {
            btq_enqueue(&q, node->left);
            next_cnt++;
        }
        if (node->right != NULL) {
            btq_enqueue(&q, node->right);
            next_cnt++;
        }

        // finished current level — start next
        if (curr_cnt == 0 && q.cnt > 0) {
            curr_cnt = next_cnt;
            next_cnt = 0;
            printf("\nLevel %d: ", ++level);
        }
    }
    printf("\n");
}

void btq_init(BTQueue* q) {
    q->front = 0;
    q->rear  = 0;
    q->cnt   = 0;
}

void btq_enqueue(BTQueue* q, BinaryTree node) {
    if (q->cnt == QUEUE_SIZE) return;
    q->data[q->rear] = node;
    q->rear          = (q->rear + 1) % QUEUE_SIZE;  // wrap around
    q->cnt++;
}

BinaryTree btq_dequeue(BTQueue* q) {
    if (q->cnt == 0) return NULL;
    BinaryTree node = q->data[q->front];
    q->front        = (q->front + 1) % QUEUE_SIZE;  // wrap around
    q->cnt--;
    return node;
}