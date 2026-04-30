/*
 * File:    Ex_33_heapsort.c
 * Author:  Alexander130892
 * Date:    5-4-2026
 *
 * Description:
 *   This file implements heapsort using a min-heap data structure to
 *   sort integer arrays in ascending order. It uses a notable
 *   three-phase approach: builds a min-heap, extracts elements to
 *   create descending order, then reverses the array to achieve the
 *   final ascending sort.
 */
 //Libraries
#include <stdio.h>
#include <stdlib.h>

// --- Min-heap: parent is always SMALLER than children ---

void swap(int a[], int i, int j) {
    int tmp = a[i];
    a[i]    = a[j];
    a[j]    = tmp;
}

void sift_down(int a[], int i, int n) {
    int smallest = i;
    int left     = 2*i + 1;
    int right    = 2*i + 2;

    if (left  < n && a[left]  < a[smallest]) smallest = left;
    if (right < n && a[right] < a[smallest]) smallest = right;

    if (smallest != i) {
        swap(a, i, smallest);
        sift_down(a, smallest, n);  // recurse down
    }
}

void build_min_heap(int a[], int n) {
    for (int i = n/2 - 1; i >= 0; i--)  // start at last non-leaf
        sift_down(a, i, n);
}

void heapsort_(int a[], int n) {
    // Phase 1 — build min-heap
    build_min_heap(a, n);

    // Phase 2 — extract min to end, re-heapify
    // Note: min-heap gives descending order when sorted in-place
    // so we reverse at the end to get ascending
    for (int i = n-1; i > 0; i--) {
        swap(a, 0, i);       // move min to end
        sift_down(a, 0, i);  // re-heapify reduced heap
    }

    // Phase 3 — reverse to get ascending order
    for (int i = 0, j = n-1; i < j; i++, j--)
        swap(a, i, j);
}

void print_array(int a[], int n, const char* label) {
    printf("%-20s: ", label);
    for (int i = 0; i < n; i++)
        printf("%3d", a[i]);
    printf("\n");
}

void print_heap_tree(int a[], int n) {
    printf("\nHeap tree structure:\n");
    int level     = 0;
    int level_cnt = 1;
    int i         = 0;
    while (i < n) {
        printf("Level %d: ", level);
        for (int j = 0; j < level_cnt && i < n; j++, i++)
            printf("%3d", a[i]);
        printf("\n");
        level++;
        level_cnt *= 2;
    }
    printf("\n");
}

int main(void) {
    // Test 1 — general case
    printf("=== Test 1: general case ===\n");
    int a[] = {9, 3, 7, 1, 5, 8, 2, 6, 4};
    int n   = sizeof(a) / sizeof(a[0]);
    print_array(a, n, "Unsorted");
    build_min_heap(a, n);
    print_array(a, n, "After heapify");
    print_heap_tree(a, n);
    heapsort_(a, n);
    print_array(a, n, "Sorted ascending");

    // Test 2 — already sorted
    printf("\n=== Test 2: already sorted ===\n");
    int b[] = {1, 2, 3, 4, 5, 6, 7};
    int nb  = sizeof(b) / sizeof(b[0]);
    print_array(b, nb, "Unsorted");
    heapsort_(b, nb);
    print_array(b, nb, "Sorted ascending");

    // Test 3 — reverse sorted
    printf("\n=== Test 3: reverse sorted ===\n");
    int c[] = {9, 8, 7, 6, 5, 4, 3, 2, 1};
    int nc  = sizeof(c) / sizeof(c[0]);
    print_array(c, nc, "Unsorted");
    heapsort_(c, nc);
    print_array(c, nc, "Sorted ascending");

    // Test 4 — duplicates
    printf("\n=== Test 4: duplicates ===\n");
    int d[] = {3, 1, 4, 1, 5, 9, 2, 6, 5, 3};
    int nd  = sizeof(d) / sizeof(d[0]);
    print_array(d, nd, "Unsorted");
    heapsort_(d, nd);
    print_array(d, nd, "Sorted ascending");

    // Test 5 — single element
    printf("\n=== Test 5: single element ===\n");
    int e[] = {42};
    heapsort_(e, 1);
    print_array(e, 1, "Sorted ascending");

    // Test 6 — two elements
    printf("\n=== Test 6: two elements ===\n");
    int f[] = {5, 2};
    heapsort_(f, 2);
    print_array(f, 2, "Sorted ascending");

    return EXIT_SUCCESS;
}
/*

**The min-heap tradeoff worth understanding:**

A max-heap naturally produces ascending order because you repeatedly extract the maximum to the end. 
A min-heap extracts the minimum to the end, which produces **descending** order in-place — hence the reverse step at the end. 
```
Min-heap after build:    [1, 3, 2, 6, 5, 8, 7, 9, 4]
                              ↑
                         smallest always at root



Time complexity     O(n log n) always
Space complexity    O(1) — sorts in place
Stable              No — equal elements may swap
Cache friendly      No — jumps around memory

 Compared to your BST approach:
Your BST_to_array + inorder traversal is essentially treesort — also O(n log n) average but O(n²) worst case on sorted input, 
and uses O(n) extra memory for the nodes. Heapsort guarantees O(n log n) with no extra memory, which is why it's preferred in practice.
*/