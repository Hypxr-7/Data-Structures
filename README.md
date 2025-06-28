# Data Structures <!-- omit in toc -->

**(WIP)**

This repository contains various data structures from the book: **Algorithms by Kevin Wayne 
and Robert Sedgewick**

# Table of Contents <!-- omit in toc -->

- [Fundamentals](#fundamentals)
  - [Stack](#stack)
  - [Queue](#queue)
  - [Union Find (Quick Find)](#union-find-quick-find)
  - [Union Find (Quick Union)](#union-find-quick-union)
  - [Union Find (Weighted Quick Union)](#union-find-weighted-quick-union)
  - [Union Find (Weighted Quick Union with Path Compression)](#union-find-weighted-quick-union-with-path-compression)
- [Sorting](#sorting)
  - [Insertion Sort (Standard)](#insertion-sort-standard)
  - [Insertion Sort (Optimized)](#insertion-sort-optimized)
  - [Insertion Sort (Binary Search)](#insertion-sort-binary-search)
  - [Selection Sort](#selection-sort)
  - [Shell Sort](#shell-sort)
  - [Mergesort (Standard)](#mergesort-standard)
  - [Mergesort (Bottoms Up)](#mergesort-bottoms-up)
  - [Mergesort (Optimized)](#mergesort-optimized)
  - [Quicksort (Standard)](#quicksort-standard)
  - [Quicksort (Hoare)](#quicksort-hoare)
  - [Quicksort (Dual Pivot)](#quicksort-dual-pivot)
  - [Quicksort (Three Way)](#quicksort-three-way)
  - [Quicksort (Bently-McIlroy)](#quicksort-bently-mcilroy)
  - [Max Priority Queue](#max-priority-queue)
  - [Min Priority Queue](#min-priority-queue)
  - [Max Indexed Priority Queue](#max-indexed-priority-queue)
  - [Min Indexed Priority Queue](#min-indexed-priority-queue)
- [Searching](#searching)
  - [Sequential Search](#sequential-search)
  - [Binary Search](#binary-search)
  - [Binary Search Tree](#binary-search-tree)
  - [Red-Black Binary Search Tree](#red-black-binary-search-tree)
  - [Separate Chaining Hashing](#separate-chaining-hashing)
  - [Linear Probing Hashing](#linear-probing-hashing)
- [Graphs](#graphs)
  - [Graph](#graph)
  - [Graph Generator](#graph-generator)
  - [Depth First Paths](#depth-first-paths)
  - [Breadth First Paths](#breadth-first-paths)
  - [Connected Components](#connected-components)
  - [Bipartite](#bipartite)
  - [Cycle](#cycle)
  - [Eulerian Cycle](#eulerian-cycle)
  - [Eulerian Path](#eulerian-path)
  - [Digraph](#digraph)
  - [Digraph Generator](#digraph-generator)
  - [Depth First Directed Paths](#depth-first-directed-paths)
  - [Breadth First Directed Paths](#breadth-first-directed-paths)
  - [Directed Cycle](#directed-cycle)
  - [Directed Eulerian Cycle](#directed-eulerian-cycle)
  - [Directed Eulerian Path](#directed-eulerian-path)
  - [Depth First Order](#depth-first-order)
  - [Topological](#topological)
  - [Transitive Closure](#transitive-closure)
  - [Strongly Connected Components (Kosaraju-Sharir)](#strongly-connected-components-kosaraju-sharir)
  - [Strongly Connected Components (Tarjan)](#strongly-connected-components-tarjan)
  - [Strongly Connected Components (Gabow)](#strongly-connected-components-gabow)
  - [Edge Weighted Graph](#edge-weighted-graph)
  - [Minimum Spanning Tree (Lazy Prim)](#minimum-spanning-tree-lazy-prim)
  - [Minimum Spanning Tree (Eager Prim)](#minimum-spanning-tree-eager-prim)
  - [Minimum Spanning Tree (Kruskal)](#minimum-spanning-tree-kruskal)
  - [Minimum Spanning Tree (Boruvka)](#minimum-spanning-tree-boruvka)
  - [Edge Weighted Digraph](#edge-weighted-digraph)
  - [Dijkstra - Shortest Paths](#dijkstra---shortest-paths)
  - [Dijkstra - Shortest Paths (Undirected)](#dijkstra---shortest-paths-undirected)
  - [Dijkstra - All Shortest Paths](#dijkstra---all-shortest-paths)
  - [DAG - Shortest Paths](#dag---shortest-paths)
  - [DAG - Longest Paths](#dag---longest-paths)
  - [Bellman Ford - Shortest Path](#bellman-ford---shortest-path)
  - [Edge Weighted Directed Cycle](#edge-weighted-directed-cycle)
  - [Floyd Warshall - All Shortest Paths (Dense)](#floyd-warshall---all-shortest-paths-dense)
  - [Adjacency Matrix - Edge Weighted Digraph](#adjacency-matrix---edge-weighted-digraph)


# Fundamentals

## Stack

A Last-In-First-Out (LIFO) data structure that supports adding and removing elements from the top. Implemented using a resizing array or linked list.

| `Stack<Item>`          |                                    |
| ---------------------- | ---------------------------------- |
| `Stack()`              | *creates an empty stack*           |
| `bool empty()`         | *is stack empty?*                  |
| `int size()`           | *number of items in stack*         |
| `void push(Item item)` | *add item to stack*                |
| `Item pop()`           | *remove item from stack*           |
| `Item peek()`          | *get item at top without removing* |

**Time Complexity**: All operations $O(1)$ amortized. **Space Complexity**: $O(n)$ where n is the number of elements.

## Queue

A First-In-First-Out (FIFO) data structure that supports adding elements at the rear and removing from the front. Implemented using a resizing array or linked list.

| `Queue<Item>`             |                                      |
| ------------------------- | ------------------------------------ |
| `Queue()`                 | *creates an empty queue*             |
| `bool empty()`            | *is queue empty?*                    |
| `int size()`              | *number of items in queue*           |
| `void enqueue(Item item)` | *enqueue item*                       |
| `Item dequeue()`          | *dequeue item*                       |
| `Item peek()`             | *get item at top without dequeueing* |

**Time Complexity**: All operations $O(1)$ amortized. **Space Complexity**: $O(n)$ where n is the number of elements.

## Union Find (Quick Find)

A data structure that maintains a collection of disjoint sets and supports union and find operations. Quick Find uses an array where id[i] is the component identifier for element i.

| `QuickFindUF`                  |                                     |
| ------------------------------ | ----------------------------------- |
| `QuickFindUF(int p)`           | *initialize UF with p elements*     |
| `int count()`                  | *number of components*              |
| `int find(int p)`              | *ID of elements p*                  |
| `bool connected(int p, int q)` | *are elements p and q connected?*   |
| `void unite(int p, int q)`     | *add p and q to the same component* |

**Time Complexity**: Find $O(1)$, Union $O(n)$, Connected $O(1)$. **Space Complexity**: $O(n)$.

## Union Find (Quick Union)

Uses a forest of trees where each element points to its parent. The root of each tree serves as the component identifier.

| `QuickUnionUF`                 |                                     |
| ------------------------------ | ----------------------------------- |
| `QuickUnionUF(int p)`          | *initialize UF with p elements*     |
| `int count()`                  | *number of components*              |
| `int find(int p)`              | *ID of elements p*                  |
| `bool connected(int p, int q)` | *are elements p and q connected?*   |
| `void unite(int p, int q)`     | *add p and q to the same component* |

**Time Complexity**: Find $O(n)$ worst case, Union $O(n)$, Connected $O(n)$. **Space Complexity**: $O(n)$.

## Union Find (Weighted Quick Union)

Improves Quick Union by always attaching the smaller tree to the root of the larger tree, keeping trees balanced.

| `WeightedQuickUnionUF`         |                                     |
| ------------------------------ | ----------------------------------- |
| `WeightedQuickUnionUF(int p)`  | *initialize UF with p elements*     |
| `int count()`                  | *number of components*              |
| `int find(int p)`              | *ID of elements p*                  |
| `bool connected(int p, int q)` | *are elements p and q connected?*   |
| `void unite(int p, int q)`     | *add p and q to the same component* |

**Time Complexity**: Find $O(\log n)$, Union $O(\log n)$, Connected $O(\log n)$. **Space Complexity**: $O(n)$.

## Union Find (Weighted Quick Union with Path Compression)

Enhances Weighted Quick Union by making every node point directly to the root during find operations, nearly achieving constant time.

| `WeightedQuickUnionPathCompressionUF`        |                                     |
| -------------------------------------------- | ----------------------------------- |
| `WeightedQuickUnionPathCompressionUF(int p)` | *initialize UF with p elements*     |
| `int count()`                                | *number of components*              |
| `int find(int p)`                            | *ID of elements p*                  |
| `bool connected(int p, int q)`               | *are elements p and q connected?*   |
| `void unite(int p, int q)`                   | *add p and q to the same component* |

**Time Complexity**: Find $O(\alpha(n))$, Union $O(\alpha(n))$, Connected $O(\alpha(n))$ where α is the inverse Ackermann function (nearly constant). **Space Complexity**: $O(n)$.

# Sorting

## Insertion Sort (Standard)

A simple comparison-based sorting algorithm that builds the sorted array one element at a time by inserting each element into its correct position.

`InsertionSort<T>::sort(std::vector<T> arr)`

sorts the array with standard insertion sort

**Time Complexity**: $O(n^2)$ worst case, $O(n)$ best case. **Space Complexity**: $O(1)$.

## Insertion Sort (Optimized)

An optimized version that uses shifting instead of swapping to reduce the number of array writes.

`InsertionSortOptimized<T>::sort(std::vector<T> arr)`

sorts the array using insertion sort making use of shifting instead of swaps

**Time Complexity**: $O(n^2)$ worst case, $O(n)$ best case. **Space Complexity**: $O(1)$.

## Insertion Sort (Binary Search)

Uses binary search to find the insertion position, reducing the number of comparisons but not the number of shifts.

`InsertionSortBinary<T>::sort(std::vector<T> arr)`

uses binary search to locate insertion point

**Time Complexity**: $O(n^2 + n \log n)$ - $O(n \log n)$ comparisons, $O(n^2)$ shifts. **Space Complexity**: $O(1)$.

## Selection Sort

A simple sorting algorithm that repeatedly finds the minimum element and places it at the beginning.

`SelectionSort<T>::sort(std::vector<T> arr)`

uses standard selection sort

**Time Complexity**: $O(n^2)$ in all cases. **Space Complexity**: $O(1)$.

## Shell Sort

An extension of insertion sort that allows the exchange of items that are far apart, using a sequence of gap values.

`ShellSort<T>::sort(std::vector<T> arr)`

uses shell sort with $3x+1$ gap

**Time Complexity**: $O(n^{3/2})$ for 3x+1 gap sequence. **Space Complexity**: $O(1)$.

## Mergesort (Standard)

A divide-and-conquer algorithm that divides the array into halves, sorts them recursively, and merges the results.

`Mergesort<T>::sort(std::vector<T> arr)`

uses standard mergesort

**Time Complexity**: $O(n \log n)$ in all cases. **Space Complexity**: $O(n)$.

## Mergesort (Bottoms Up)

A non-recursive implementation of mergesort that iteratively merges subarrays of increasing size.

`MergesortBottomsUp<T>::sort(std::vector<T> arr)`

uses a non-recursive mergesort

**Time Complexity**: $O(n \log n)$ in all cases. **Space Complexity**: $O(n)$.

## Mergesort (Optimized)

An enhanced version of mergesort with several optimizations for practical performance improvements.

`MergesortOptimized<T>::sort(std::vector<T> arr)`

optimizes standard mergesort by:
1. adding a cutoff to insertion sort
2. swapping `arr` and `aux` preventing the need to copy in `merge()`
3. avoid calling `merge()` if sub-arrays already sorted

**Time Complexity**: $O(n \log n)$ worst case, better constants and $O(n)$ best case. **Space Complexity**: $O(n)$.

## Quicksort (Standard)

A divide-and-conquer algorithm that partitions the array around a pivot and recursively sorts the partitions.

`Quicksort<T>::sort(std::vector<T> arr)`

uses standard quicksort

**Time Complexity**: $O(n^2)$ worst case, $O(n \log n)$ average case. **Space Complexity**: $O(\log n)$ average, $O(n)$ worst case.

## Quicksort (Hoare)

The original quicksort algorithm developed by Tony Hoare with his partitioning scheme.

`QuicksortHoare<T>::sort(std::vector<T> arr)`

uses the original Hoare's quicksort

**Time Complexity**: $O(n^2)$ worst case, $O(n \log n)$ average case. **Space Complexity**: $O(\log n)$ average, $O(n)$ worst case.

## Quicksort (Dual Pivot)

A variant that uses two pivots to partition the array into three parts, improving performance on certain datasets.

`QuicksortDualPivot<T>::sort(std::vector<T> arr)`

makes use of two pivots

**Time Complexity**: $O(n^2)$ worst case, $O(n \log n)$ average case with better constants. **Space Complexity**: $O(\log n)$ average.

## Quicksort (Three Way)

Efficiently handles arrays with many duplicate keys by partitioning into three sections: less than, equal to, and greater than the pivot.

`QuicksortThreeWay<T>::sort(std::vector<T> arr)`

sort in three sections: less than pivot, equal to pivot, and greater than pivot

**Time Complexity**: $O(n^2)$ worst case, $O(n)$ when all keys equal, $O(n \log n)$ average. **Space Complexity**: $O(\log n)$ average.

## Quicksort (Bently-McIlroy)

A highly optimized quicksort implementation with multiple performance enhancements for real-world usage.

`QuicksortBentlyMcIlroy<T>::sort(std::vector<T> arr)`

optimized quicksort with:
1. uses insertion sort for smaller sub-arrays
2. uses median of 3 for smaller sub-arrays for pivot selection
3. For larger, uses Tukey Ninther
4. Uses Bently-McIlroy 3-way partitioning

**Time Complexity**: $O(n^2)$ worst case, $O(n \log n)$ average with excellent constants. **Space Complexity**: $O(\log n)$ average.

## Max Priority Queue

A data structure that maintains a collection of elements with priorities and supports extracting the maximum element efficiently. Implemented using a binary heap.

| `MaxPQ<Key>`                                   |                                         |
| ---------------------------------------------- | --------------------------------------- |
| `MaxPQ<Key>()`                                 | *initialize empty PQ*                   |
| `MaxPQ<Key>(std::vector<Key> items)`           | *initialize PQ with elements in vector* |
| `MaxPQ<Key>(std::initializer_list<Key> items)` | *initialize PQ with elements in list*   |
| `bool empty()`                                 | *is PQ empty?*                          |
| `int size()`                                   | *number of elements in PQ*              |
| `Key max()`                                    | *get the max element*                   |
| `void insert(Key item)`                        | *add item into PQ*                      |
| `Key deleteMax()`                              | *remove max item from PQ*               |
| `std::vector<Key> getElements()`               | *retrieve underlying array*             |
| `void clear()`                                 | *reset the PQ*                          |

**Time Complexity**: Insert $O(\log n)$, Delete-max $O(\log n)$, Max $O(1)$. **Space Complexity**: $O(n)$.

## Min Priority Queue

A data structure that maintains a collection of elements with priorities and supports extracting the minimum element efficiently. Implemented using a binary heap.

| `MinPQ<Key>`                                   |                                         |
| ---------------------------------------------- | --------------------------------------- |
| `MinPQ<Key>()`                                 | *initialize empty PQ*                   |
| `MinPQ<Key>(std::vector<Key> items)`           | *initialize PQ with elements in vector* |
| `MinPQ<Key>(std::initializer_list<Key> items)` | *initialize PQ with elements in list*   |
| `bool empty()`                                 | *is PQ empty?*                          |
| `int size()`                                   | *number of elements in PQ*              |
| `Key min()`                                    | *get the min element*                   |
| `void insert(Key item)`                        | *add item into PQ*                      |
| `Key deleteMin()`                              | *remove min item from PQ*               |
| `std::vector<Key> getElements()`               | *retrieve underlying array*             |
| `void clear()`                                 | *reset the PQ*                          |

**Time Complexity**: Insert $O(\log n)$, Delete-min $O(\log n)$, Min $O(1)$. **Space Complexity**: $O(n)$.

## Max Indexed Priority Queue

The `IndexMaxPQ<Key>` maintains a priority queue where each element has both an index (0 to maxCap-1) and a priority key, allowing efficient access and updates by index.

| `IndexMaxPQ<Key>`                       |                                        |
| --------------------------------------- | -------------------------------------- |
| `IndexMaxPQ<Key>(int maxCap)`           | *initialize PQ with given capacity*    |
| `bool empty()`                          | *is PQ empty?*                         |
| `int size()`                            | *number of elements in PQ*             |
| `bool contains(int i)`                  | *is given index in PQ?*                |
| `void insert(int i, Key key)`           | *add element at index with priority*   |
| `int maxIndex()`                        | *index with maximum priority*          |
| `Key maxKey()`                          | *maximum priority key*                 |
| `int removeMax()`                       | *remove and return max priority index* |
| `Key keyOf(int i)`                      | *get priority key at index*            |
| `void changeKey(int i, Key key)`        | *change priority at index*             |
| `void increaseKey(int i, Key key)`      | *increase priority at index*           |
| `void decreaseKey(int i, Key key)`      | *decrease priority at index*           |
| `void remove(int i)`                    | *remove element at index*              |
| `std::vector<int> indices()`            | *get all indices in heap order*        |
| `std::vector<Key> keys_in_heap_order()` | *get all keys in heap order*           |

**Time Complexity**: Insert/Delete/Change-key $O(\log n)$, Contains/Key-of $O(1)$. **Space Complexity**: $O(n)$.

## Min Indexed Priority Queue

The `IndexMinPQ<Key>` maintains a priority queue where each element has both an index (0 to maxCap-1) and a priority key, allowing efficient access and updates by index. Elements with smaller keys have higher priority.

| `IndexMinPQ<Key>`                       |                                        |
| --------------------------------------- | -------------------------------------- |
| `IndexMinPQ<Key>(int maxCap)`           | *initialize PQ with given capacity*    |
| `bool empty()`                          | *is PQ empty?*                         |
| `int size()`                            | *number of elements in PQ*             |
| `bool contains(int i)`                  | *is given index in PQ?*                |
| `void insert(int i, Key key)`           | *add element at index with priority*   |
| `int minIndex()`                        | *index with minimum priority*          |
| `Key minKey()`                          | *minimum priority key*                 |
| `int removeMin()`                       | *remove and return min priority index* |
| `Key keyOf(int i)`                      | *get priority key at index*            |
| `void changeKey(int i, Key key)`        | *change priority at index*             |
| `void decreaseKey(int i, Key key)`      | *decrease priority at index*           |
| `void increaseKey(int i, Key key)`      | *increase priority at index*           |
| `void remove(int i)`                    | *remove element at index*              |
| `std::vector<int> indices()`            | *get all indices in heap order*        |
| `std::vector<Key> keys_in_heap_order()` | *get all keys in heap order*           |

**Time Complexity**: Insert/Delete/Change-key $O(\log n)$, Contains/Key-of $O(1)$. **Space Complexity**: $O(n)$.

# Searching

## Sequential Search

The `SequentialSearchST<Key, Value>` implements an unordered symbol table using a linked list. It performs sequential search through all key-value pairs.

| `SequentialSearchST<Key, Value>` |                                 |
| -------------------------------- | ------------------------------- |
| `SequentialSearchST()`           | *initialize empty symbol table* |
| `bool empty()`                   | *is symbol table empty?*        |
| `int size()`                     | *number of key-value pairs*     |
| `bool contains(Key key)`         | *is given key in symbol table?* |
| `Value get(Key key)`             | *get value associated with key* |
| `void put(Key key, Value value)` | *insert key-value pair*         |
| `void remove(Key key)`           | *remove key-value pair*         |
| `std::vector<Key> keys()`        | *get all keys in symbol table*  |

**Time Complexity**: Search $O(n)$, Insert $O(n)$, Delete $O(n)$. **Space Complexity**: $O(n)$.

## Binary Search

The `BinarySearchST<Key, Value>` implements an ordered symbol table using two parallel sorted arrays. It performs binary search for efficient key lookup.

| `BinarySearchST<Key, Value>`            |                                      |
| --------------------------------------- | ------------------------------------ |
| `BinarySearchST()`                      | *initialize empty symbol table*      |
| `bool empty()`                          | *is symbol table empty?*             |
| `int size()`                            | *number of key-value pairs*          |
| `bool contains(Key key)`                | *is given key in symbol table?*      |
| `Value get(Key key)`                    | *get value associated with key*      |
| `void put(Key key, Value value)`        | *insert key-value pair*              |
| `void remove(Key key)`                  | *remove key-value pair*              |
| `void removeMin()`                      | *remove smallest key*                |
| `void removeMax()`                      | *remove largest key*                 |
| `Key min()`                             | *smallest key*                       |
| `Key max()`                             | *largest key*                        |
| `Key select(int k)`                     | *key of rank k*                      |
| `int rank(Key key)`                     | *number of keys less than given key* |
| `Key floor(Key key)`                    | *largest key ≤ given key*            |
| `Key ceiling(Key key)`                  | *smallest key ≥ given key*           |
| `int size(Key lo, Key hi)`              | *number of keys in range [lo, hi]*   |
| `std::vector<Key> keys()`               | *all keys in sorted order*           |
| `std::vector<Key> keys(Key lo, Key hi)` | *keys in range [lo, hi]*             |

**Time Complexity**: Search $O(\log n)$, Insert $O(n)$, Delete $O(n)$, Min/Max/Select/Rank/Floor/Ceiling $O(\log n)$. **Space Complexity**: $O(n)$.

## Binary Search Tree

The `BST<Key, Value>` implements an ordered symbol table using a binary search tree. It maintains keys in sorted order and provides efficient search operations.

| `BST<Key, Value>`                       |                                      |
| --------------------------------------- | ------------------------------------ |
| `BST()`                                 | *initialize empty BST*               |
| `bool empty()`                          | *is BST empty?*                      |
| `int size()`                            | *number of key-value pairs*          |
| `bool contains(Key key)`                | *is given key in BST?*               |
| `Value get(Key key)`                    | *get value associated with key*      |
| `void put(Key key, Value value)`        | *insert key-value pair*              |
| `void remove(Key key)`                  | *remove key-value pair*              |
| `void removeMin()`                      | *remove smallest key*                |
| `void removeMax()`                      | *remove largest key*                 |
| `Key min()`                             | *smallest key*                       |
| `Key max()`                             | *largest key*                        |
| `Key select(int k)`                     | *key of rank k*                      |
| `int rank(Key key)`                     | *number of keys less than given key* |
| `Key floor(Key key)`                    | *largest key ≤ given key*            |
| `Key ceiling(Key key)`                  | *smallest key ≥ given key*           |
| `int size(Key lo, Key hi)`              | *number of keys in range [lo, hi]*   |
| `std::vector<Key> keys()`               | *all keys in sorted order*           |
| `std::vector<Key> keys(Key lo, Key hi)` | *keys in range [lo, hi]*             |
| `int height()`                          | *height of the tree*                 |
| `std::vector<Key> levelOrder()`         | *keys in level-order traversal*      |

**Time Complexity**: All operations $O(n)$ worst case, $O(\log n)$ average case. **Space Complexity**: $O(n)$.

## Red-Black Binary Search Tree

The `RedBlackBST<Key, Value>` implements a self-balancing binary search tree using red-black tree properties. Guarantees logarithmic performance for all operations.

| `RedBlackBST<Key, Value>`               |                                      |
| --------------------------------------- | ------------------------------------ |
| `RedBlackBST()`                         | *initialize empty red-black BST*     |
| `bool empty()`                          | *is BST empty?*                      |
| `int size()`                            | *number of key-value pairs*          |
| `bool contains(Key key)`                | *is given key in BST?*               |
| `Value get(Key key)`                    | *get value associated with key*      |
| `void put(Key key, Value value)`        | *insert key-value pair*              |
| `void remove(Key key)`                  | *remove key-value pair*              |
| `void removeMin()`                      | *remove smallest key*                |
| `void removeMax()`                      | *remove largest key*                 |
| `Key min()`                             | *smallest key*                       |
| `Key max()`                             | *largest key*                        |
| `Key select(int k)`                     | *key of rank k*                      |
| `int rank(Key key)`                     | *number of keys less than given key* |
| `Key floor(Key key)`                    | *largest key ≤ given key*            |
| `Key ceiling(Key key)`                  | *smallest key ≥ given key*           |
| `int size(Key lo, Key hi)`              | *number of keys in range [lo, hi]*   |
| `std::vector<Key> keys()`               | *all keys in sorted order*           |
| `std::vector<Key> keys(Key lo, Key hi)` | *keys in range [lo, hi]*             |
| `int height()`                          | *height of the tree*                 |

**Time Complexity**: All operations $O(\log n)$ guaranteed. **Space Complexity**: $O(n)$.

## Separate Chaining Hashing

The `SeparateChainingHashST<Key, Value>` implements a hash table using separate chaining for collision resolution. Each bucket contains a linked list of key-value pairs that hash to the same location.

| `SeparateChainingHashST<Key, Value>`  |                                       |
| ------------------------------------- | ------------------------------------- |
| `SeparateChainingHashST(int cap = 8)` | *initialize hash table with capacity* |
| `bool empty()`                        | *is hash table empty?*                |
| `int size()`                          | *number of key-value pairs*           |
| `bool contains(Key key)`              | *is given key in hash table?*         |
| `Value get(Key key)`                  | *get value associated with key*       |
| `void put(Key key, Value value)`      | *insert key-value pair*               |
| `void remove(Key key)`                | *remove key-value pair*               |
| `std::vector<Key> keys()`             | *all keys in hash table*              |
| `double loadFactor()`                 | *current load factor (n/m)*           |
| `int capacity()`                      | *current table capacity*              |

**Time Complexity**: Search/Insert/Delete $O(1)$ average, $O(n)$ worst case. **Space Complexity**: $O(n + m)$ where m is table size.

## Linear Probing Hashing

The `LinearProbingHashST<Key, Value>` implements a hash table using linear probing for collision resolution. When a collision occurs, it searches linearly for the next available slot.

| `LinearProbingHashST<Key, Value>`  |                                       |
| ---------------------------------- | ------------------------------------- |
| `LinearProbingHashST(int cap = 8)` | *initialize hash table with capacity* |
| `bool empty()`                     | *is hash table empty?*                |
| `int size()`                       | *number of key-value pairs*           |
| `bool contains(Key key)`           | *is given key in hash table?*         |
| `Value get(Key key)`               | *get value associated with key*       |
| `void put(Key key, Value value)`   | *insert key-value pair*               |
| `void remove(Key key)`             | *remove key-value pair*               |
| `std::vector<Key> keys()`          | *all keys in hash table*              |
| `double loadFactor()`              | *current load factor (n/m)*           |
| `int capacity()`                   | *current table capacity*              |

**Time Complexity**: Search/Insert/Delete $O(1)$ average, $O(n)$ worst case. **Space Complexity**: $O(m)$ where m is table size.

# Graphs

## Graph

The `Graph` class implements an undirected graph using adjacency lists representation. Each vertex maintains a list of its adjacent vertices.

| `Graph`                       |                                        |
| ----------------------------- | -------------------------------------- |
| `Graph(int V)`                | *initialize graph with V vertices*     |
| `int V()`                     | *number of vertices*                   |
| `int E()`                     | *number of edges*                      |
| `void addEdge(int v, int w)`  | *add edge between vertices v and w*    |
| `std::vector<int> adj(int v)` | *vertices adjacent to vertex v*        |
| `int degree(int v)`           | *degree of vertex v*                   |
| `bool hasEdge(int v, int w)`  | *check if edge exists between v and w* |
| `int maxDegree()`             | *maximum degree in graph*              |
| `double averageDegree()`      | *average degree of vertices*           |
| `int totalDegree()`           | *sum of all vertex degrees*            |
| `int numberOfSelfLoops()`     | *number of self-loops in graph*        |

**Time Complexity**: Add edge $O(1)$, adjacency queries $O(\text{degree}(v))$, space $O(V + E)$.

## Graph Generator

The `GraphGenerator` class provides static methods for creating various types of graphs for testing and research purposes.

| `GraphGenerator`                                   |                                                    |
| -------------------------------------------------- | -------------------------------------------------- |
| `static Graph simple(int V, int E)`                | *random simple graph with V vertices, E edges*     |
| `static Graph simple(int V, double p)`             | *Erdős-Rényi random graph with edge probability p* |
| `static Graph complete(int V)`                     | *complete graph on V vertices*                     |
| `static Graph completeBipartite(int V1, int V2)`   | *complete bipartite graph*                         |
| `static Graph bipartite(int V1, int V2, int E)`    | *random bipartite graph with E edges*              |
| `static Graph bipartite(int V1, int V2, double p)` | *random bipartite graph with probability p*        |
| `static Graph path(int V)`                         | *path graph on V vertices*                         |
| `static Graph cycle(int V)`                        | *cycle graph on V vertices*                        |
| `static Graph star(int V)`                         | *star graph with V vertices*                       |
| `static Graph wheel(int V)`                        | *wheel graph on V vertices*                        |
| `static Graph tree(int V)`                         | *random tree on V vertices*                        |
| `static Graph regular(int V, int k)`               | *random k-regular graph*                           |
| `static Graph binaryTree(int V)`                   | *complete binary tree*                             |
| `static Graph eulerianCycle(int V, int E)`         | *graph with Eulerian cycle*                        |
| `static Graph eulerianPath(int V, int E)`          | *graph with Eulerian path*                         |

**Time Complexity**: Varies by method, typically $O(V + E)$ construction time.

## Depth First Paths

The `DepthFirstPaths` class finds paths in a graph using depth-first search from a single source vertex.

| `DepthFirstPaths`                 |                                         |
| --------------------------------- | --------------------------------------- |
| `DepthFirstPaths(Graph g, int s)` | *find paths from source vertex s*       |
| `bool hasPathTo(int v)`           | *is there a path from source to v?*     |
| `std::vector<int> pathTo(int v)`  | *path from source to v (empty if none)* |

**Time Complexity**: Preprocessing $O(V + E)$, queries $O(V)$ worst case. **Space Complexity**: $O(V)$.

## Breadth First Paths

The `BreadthFirstPaths` class finds shortest paths in a graph using breadth-first search from a single source vertex.

| `BreadthFirstPaths`                 |                                      |
| ----------------------------------- | ------------------------------------ |
| `BreadthFirstPaths(Graph g, int s)` | *find shortest paths from source s*  |
| `bool hasPathTo(int v)`             | *is there a path from source to v?*  |
| `int distTo(int v)`                 | *shortest distance from source to v* |
| `std::vector<int> pathTo(int v)`    | *shortest path from source to v*     |

**Time Complexity**: Preprocessing $O(V + E)$, queries $O(V)$ worst case. **Space Complexity**: $O(V)$.

## Connected Components

The `ConnectedComponents` class identifies all connected components in an undirected graph using depth-first search.

| `ConnectedComponents`          |                                     |
| ------------------------------ | ----------------------------------- |
| `ConnectedComponents(Graph g)` | *find all connected components*     |
| `int id(int v)`                | *component identifier for vertex v* |
| `int size(int v)`              | *size of component containing v*    |
| `int count()`                  | *number of connected components*    |
| `bool connected(int v, int w)` | *are vertices v and w connected?*   |

**Time Complexity**: Preprocessing $O(V + E)$, queries $O(1)$. **Space Complexity**: $O(V)$.

## Bipartite

The `Bipartite` class determines if a graph is bipartite and finds an odd cycle if it's not.

| `Bipartite`                   |                               |
| ----------------------------- | ----------------------------- |
| `Bipartite(Graph g)`          | *check if graph is bipartite* |
| `bool isBipartite()`          | *is the graph bipartite?*     |
| `bool color(int v)`           | *color/partition of vertex v* |
| `std::vector<int> oddCycle()` | *odd cycle if not bipartite*  |

**Time Complexity**: $O(V + E)$. **Space Complexity**: $O(V)$.

## Cycle

The `Cycle` class detects cycles in an undirected graph using depth-first search.

| `Cycle`                    |                                       |
| -------------------------- | ------------------------------------- |
| `Cycle(Graph g)`           | *detect cycles in graph*              |
| `bool hasCycle()`          | *does the graph contain a cycle?*     |
| `std::vector<int> cycle()` | *vertices in a cycle (empty if none)* |

**Time Complexity**: $O(V + E)$. **Space Complexity**: $O(V)$.

## Eulerian Cycle

The `EulerianCycle` class finds an Eulerian cycle in a graph (a cycle that visits every edge exactly once).

| `EulerianCycle`            |                                   |
| -------------------------- | --------------------------------- |
| `EulerianCycle(Graph g)`   | *find Eulerian cycle in graph*    |
| `bool hasEulerianCycle()`  | *does graph have Eulerian cycle?* |
| `std::vector<int> cycle()` | *vertices in Eulerian cycle*      |

**Time Complexity**: $O(V + E)$. **Space Complexity**: $O(V + E)$.

## Eulerian Path

The `EulerianPath` class finds an Eulerian path in a graph (a path that visits every edge exactly once).

| `EulerianPath`            |                                  |
| ------------------------- | -------------------------------- |
| `EulerianPath(Graph g)`   | *find Eulerian path in graph*    |
| `bool hasEulerianPath()`  | *does graph have Eulerian path?* |
| `std::vector<int> path()` | *vertices in Eulerian path*      |

**Time Complexity**: $O(V + E)$. **Space Complexity**: $O(V + E)$.

## Digraph

The `Digraph` class implements a directed graph using adjacency lists representation. Each vertex maintains a list of vertices it points to.

| `Digraph`                     |                                       |
| ----------------------------- | ------------------------------------- |
| `Digraph(int V)`              | *initialize digraph with V vertices*  |
| `int V()`                     | *number of vertices*                  |
| `int E()`                     | *number of edges*                     |
| `void addEdge(int v, int w)`  | *add directed edge from v to w*       |
| `std::vector<int> adj(int v)` | *vertices adjacent from vertex v*     |
| `int outdegree(int v)`        | *out-degree of vertex v*              |
| `int indegree(int v)`         | *in-degree of vertex v*               |
| `Digraph reverse()`           | *reverse digraph (all edges flipped)* |

**Time Complexity**: Adding edge $O(1)$, adjacency queries $O(\text{outdegree}(v))$, space $O(V + E)$.

## Digraph Generator

The `DigraphGenerator` class provides static methods for creating various types of directed graphs for testing and research purposes.

| `DigraphGenerator`                           |                                                      |
| -------------------------------------------- | ---------------------------------------------------- |
| `static Digraph simple(int V, int E)`        | *random simple digraph with V vertices, E edges*     |
| `static Digraph simple(int V, double p)`     | *Erdős-Rényi random digraph with edge probability p* |
| `static Digraph complete(int V)`             | *complete digraph on V vertices*                     |
| `static Digraph dag(int V, int E)`           | *random directed acyclic graph*                      |
| `static Digraph tournament(int V)`           | *random tournament (complete directed graph)*        |
| `static Digraph completeRootedInDAG(int V)`  | *complete rooted-in DAG*                             |
| `static Digraph rootedInDAG(int V, int E)`   | *random rooted-in DAG with E edges*                  |
| `static Digraph completeRootedOutDAG(int V)` | *complete rooted-out DAG*                            |
| `static Digraph rootedOutDAG(int V, int E)`  | *random rooted-out DAG with E edges*                 |
| `static Digraph rootedInTree(int V)`         | *random rooted-in tree*                              |
| `static Digraph rootedOutTree(int V)`        | *random rooted-out tree*                             |
| `static Digraph path(int V)`                 | *directed path on V vertices*                        |
| `static Digraph binaryTree(int V)`           | *complete binary tree (directed)*                    |
| `static Digraph cycle(int V)`                | *directed cycle on V vertices*                       |
| `static Digraph eulerianCycle(int V, int E)` | *digraph with Eulerian cycle*                        |
| `static Digraph eulerianPath(int V, int E)`  | *digraph with Eulerian path*                         |
| `static Digraph strong(int V, int E, int c)` | *digraph with c strong components*                   |

**Time Complexity**: Varies by method, typically $O(V + E)$ construction time.

## Depth First Directed Paths

The `DepthFirstDirectedPaths` class finds directed paths in a digraph using depth-first search from a single source vertex.

| `DepthFirstDirectedPaths`                   |                                      |
| ------------------------------------------- | ------------------------------------ |
| `DepthFirstDirectedPaths(Digraph g, int s)` | *find directed paths from source s*  |
| `bool hasPathTo(int v)`                     | *is there a directed path to v?*     |
| `std::vector<int> pathTo(int v)`            | *directed path to v (empty if none)* |

**Time Complexity**: Preprocessing $O(V + E)$, queries $O(V)$ worst case. **Space Complexity**: $O(V)$.

## Breadth First Directed Paths

The `BreadthFirstDirectedPaths` class finds shortest directed paths in a digraph using breadth-first search from a single source vertex.

| `BreadthFirstDirectedPaths`                   |                                              |
| --------------------------------------------- | -------------------------------------------- |
| `BreadthFirstDirectedPaths(Digraph g, int s)` | *find shortest directed paths from source s* |
| `bool hasPathTo(int v)`                       | *is there a directed path to v?*             |
| `int distTo(int v)`                           | *shortest directed distance to v*            |
| `std::vector<int> pathTo(int v)`              | *shortest directed path to v*                |

**Time Complexity**: Preprocessing $O(V + E)$, queries $O(V)$ worst case. **Space Complexity**: $O(V)$.

## Directed Cycle

The `DirectedCycle` class detects directed cycles in a digraph using depth-first search.

| `DirectedCycle`            |                                              |
| -------------------------- | -------------------------------------------- |
| `DirectedCycle(Digraph g)` | *detect directed cycles in digraph*          |
| `bool hasCycle()`          | *does digraph contain a directed cycle?*     |
| `std::vector<int> cycle()` | *vertices in directed cycle (empty if none)* |

**Time Complexity**: $O(V + E)$. **Space Complexity**: $O(V)$.

## Directed Eulerian Cycle

The `DirectedEulerianCycle` class finds an Eulerian cycle in a digraph (a cycle that visits every edge exactly once).

| `DirectedEulerianCycle`            |                                     |
| ---------------------------------- | ----------------------------------- |
| `DirectedEulerianCycle(Digraph g)` | *find Eulerian cycle in digraph*    |
| `bool hasEulerianCycle()`          | *does digraph have Eulerian cycle?* |
| `std::vector<int> cycle()`         | *vertices in Eulerian cycle*        |

**Time Complexity**: $O(V + E)$. **Space Complexity**: $O(V + E)$.

## Directed Eulerian Path

The `DirectedEulerianPath` class finds an Eulerian path in a digraph (a path that visits every edge exactly once).

| `DirectedEulerianPath`            |                                    |
| --------------------------------- | ---------------------------------- |
| `DirectedEulerianPath(Digraph g)` | *find Eulerian path in digraph*    |
| `bool hasEulerianPath()`          | *does digraph have Eulerian path?* |
| `std::vector<int> path()`         | *vertices in Eulerian path*        |

**Time Complexity**: $O(V + E)$. **Space Complexity**: $O(V + E)$.

## Depth First Order

The `DepthFirstOrder` class computes depth-first search preorder and postorder for vertices in a digraph.

| `DepthFirstOrder`                |                                    |
| -------------------------------- | ---------------------------------- |
| `DepthFirstOrder(Digraph g)`     | *compute DFS ordering for digraph* |
| `int pre(int v)`                 | *preorder number of vertex v*      |
| `std::vector<int> pre()`         | *vertices in preorder*             |
| `int post(int v)`                | *postorder number of vertex v*     |
| `std::vector<int> post()`        | *vertices in postorder*            |
| `std::vector<int> reversePost()` | *vertices in reverse postorder*    |

**Time Complexity**: $O(V + E)$. **Space Complexity**: $O(V)$.

## Topological

The `Topological` class computes a topological ordering of vertices in a directed acyclic graph (DAG).

| `Topological`              |                                           |
| -------------------------- | ----------------------------------------- |
| `Topological(Digraph g)`   | *compute topological ordering*            |
| `std::vector<int> order()` | *topological ordering (empty if cycle)*   |
| `bool hasOrder()`          | *is graph a DAG (has topological order)?* |
| `int rank(int v)`          | *position of vertex v in ordering*        |

**Time Complexity**: $O(V + E)$. **Space Complexity**: $O(V)$.

## Transitive Closure

The `TransitiveClosure` class computes the transitive closure of a directed graph using DFS from each vertex.

| `TransitiveClosure`            |                                        |
| ------------------------------ | -------------------------------------- |
| `TransitiveClosure(Digraph g)` | *compute transitive closure*           |
| `bool reachable(int v, int w)` | *is vertex w reachable from vertex v?* |

**Time Complexity**: $O(V \times (V + E))$. **Space Complexity**: $O(V^2)$.

## Strongly Connected Components (Kosaraju-Sharir)

The `KosarajuSharirSCC` class finds strongly connected components in a directed graph using the Kosaraju-Sharir algorithm.

| `KosarajuSharirSCC`                    |                                           |
| -------------------------------------- | ----------------------------------------- |
| `KosarajuSharirSCC(Digraph g)`         | *find SCCs using Kosaraju-Sharir*         |
| `int count()`                          | *number of strongly connected components* |
| `bool stronglyConnected(int v, int w)` | *are v and w in same SCC?*                |
| `int id(int v)`                        | *SCC identifier for vertex v*             |

**Time Complexity**: $O(V + E)$. **Algorithm**: Two DFS passes - first on reverse graph for ordering, second on original graph.

## Strongly Connected Components (Tarjan)

The `TarjanSCC` class finds strongly connected components using Tarjan's single-pass algorithm with low-link values.

| `TarjanSCC`                            |                                           |
| -------------------------------------- | ----------------------------------------- |
| `TarjanSCC(Digraph g)`                 | *find SCCs using Tarjan's algorithm*      |
| `int count()`                          | *number of strongly connected components* |
| `bool stronglyConnected(int v, int w)` | *are v and w in same SCC?*                |
| `int id(int v)`                        | *SCC identifier for vertex v*             |

**Time Complexity**: $O(V + E)$. **Algorithm**: Single DFS with stack and low-link values to detect SCC roots.

## Strongly Connected Components (Gabow)

The `GabowSCC` class finds strongly connected components using Gabow's two-stack algorithm.

| `GabowSCC`                             |                                           |
| -------------------------------------- | ----------------------------------------- |
| `GabowSCC(Digraph g)`                  | *find SCCs using Gabow's algorithm*       |
| `int count()`                          | *number of strongly connected components* |
| `bool stronglyConnected(int v, int w)` | *are v and w in same SCC?*                |
| `int id(int v)`                        | *SCC identifier for vertex v*             |

**Time Complexity**: $O(V + E)$. **Algorithm**: Single DFS with two stacks to track path and potential SCC roots.

## Edge Weighted Graph

The `EdgeWeightedGraph` class represents an undirected graph with weighted edges, used for minimum spanning tree algorithms.

| `EdgeWeightedGraph`               |                                         |
| --------------------------------- | --------------------------------------- |
| `EdgeWeightedGraph(int V)`        | *initialize graph with V vertices*      |
| `EdgeWeightedGraph(int V, int E)` | *random graph with V vertices, E edges* |
| `int V()`                         | *number of vertices*                    |
| `int E()`                         | *number of edges*                       |
| `void addEdge(Edge e)`            | *add weighted edge to graph*            |
| `std::vector<Edge> adj(int v)`    | *edges incident to vertex v*            |
| `int degree(int v)`               | *degree of vertex v*                    |
| `std::vector<Edge> edges()`       | *all edges in graph*                    |

**Time Complexity**: Adding edge $O(1)$, adjacency queries $O(\text{degree}(v))$, space $O(V + E)$.

## Minimum Spanning Tree (Lazy Prim)

The `LazyPrimMST` class computes minimum spanning trees using Prim's algorithm with lazy deletion from priority queue.

| `LazyPrimMST`                      |                                 |
| ---------------------------------- | ------------------------------- |
| `LazyPrimMST(EdgeWeightedGraph g)` | *compute MST using lazy Prim's* |
| `std::vector<Edge> edges()`        | *edges in the MST*              |
| `double weight()`                  | *total weight of MST*           |

**Time Complexity**: $O(E \log E)$. **Algorithm**: Maintains all crossing edges in priority queue, filters obsolete edges lazily.

## Minimum Spanning Tree (Eager Prim)

The `PrimMST` class computes minimum spanning trees using Prim's algorithm with eager priority queue updates.

| `PrimMST`                      |                                  |
| ------------------------------ | -------------------------------- |
| `PrimMST(EdgeWeightedGraph g)` | *compute MST using eager Prim's* |
| `std::vector<Edge> edges()`    | *edges in the MST*               |
| `double weight()`              | *total weight of MST*            |

**Time Complexity**: $O(E \log V)$. **Algorithm**: Maintains one edge per vertex in indexed priority queue, updates eagerly.

## Minimum Spanning Tree (Kruskal)

The `KruskalMST` class computes minimum spanning trees using Kruskal's algorithm with Union-Find for cycle detection.

| `KruskalMST`                      |                               |
| --------------------------------- | ----------------------------- |
| `KruskalMST(EdgeWeightedGraph g)` | *compute MST using Kruskal's* |
| `std::vector<Edge> edges()`       | *edges in the MST*            |
| `double weight()`                 | *total weight of MST*         |

**Time Complexity**: $O(E \log E)$ due to sorting. **Algorithm**: Sort edges by weight, add to MST if no cycle created.

## Minimum Spanning Tree (Boruvka)

The `BoruvkaMST` class computes minimum spanning trees using Borůvka's algorithm with component-based edge selection.

| `BoruvkaMST`                      |                               |
| --------------------------------- | ----------------------------- |
| `BoruvkaMST(EdgeWeightedGraph g)` | *compute MST using Borůvka's* |
| `std::vector<Edge> edges()`       | *edges in the MST*            |
| `double weight()`                 | *total weight of MST*         |

**Time Complexity**: $O(E \log V)$. **Algorithm**: Each component finds cheapest outgoing edge, components merge in iterations.

## Edge Weighted Digraph

The `EdgeWeightedDigraph` class represents a directed graph with weighted edges, used for shortest path algorithms.

| `EdgeWeightedDigraph`                  |                                           |
| -------------------------------------- | ----------------------------------------- |
| `EdgeWeightedDigraph(int V)`           | *initialize digraph with V vertices*      |
| `EdgeWeightedDigraph(int V, int E)`    | *random digraph with V vertices, E edges* |
| `int V()`                              | *number of vertices*                      |
| `int E()`                              | *number of edges*                         |
| `void addEdge(DirectedEdge e)`         | *add weighted directed edge*              |
| `std::vector<DirectedEdge> adj(int v)` | *edges leaving vertex v*                  |
| `int outdegree(int v)`                 | *out-degree of vertex v*                  |
| `int indegree(int v)`                  | *in-degree of vertex v*                   |
| `std::vector<DirectedEdge> edges()`    | *all edges in digraph*                    |
| `EdgeWeightedDigraph reverse()`        | *reverse digraph (edges flipped)*         |

**Time Complexity**: Adding edge $O(1)$, adjacency queries $O(\text{outdegree}(v))$, space $O(V + E)$.

## Dijkstra - Shortest Paths

The `DijkstraSP` class computes single-source shortest paths in edge-weighted digraphs with non-negative weights.

| `DijkstraSP`                               |                                |
| ------------------------------------------ | ------------------------------ |
| `DijkstraSP(EdgeWeightedDigraph g, int s)` | *shortest paths from source s* |
| `double distTo(int v)`                     | *distance from source to v*    |
| `bool hasPathTo(int v)`                    | *is there a path to v?*        |
| `std::vector<DirectedEdge> pathTo(int v)`  | *shortest path to v*           |

**Time Complexity**: $O((V + E) \log V)$. **Requirements**: Non-negative edge weights.

## Dijkstra - Shortest Paths (Undirected)

The `DijkstraUndirectedSP` class computes single-source shortest paths in edge-weighted undirected graphs.

| `DijkstraUndirectedSP`                             |                                |
| -------------------------------------------------- | ------------------------------ |
| `DijkstraUndirectedSP(EdgeWeightedGraph g, int s)` | *shortest paths from source s* |
| `double distTo(int v)`                             | *distance from source to v*    |
| `bool hasPathTo(int v)`                            | *is there a path to v?*        |
| `std::vector<Edge> pathTo(int v)`                  | *shortest path to v*           |

**Time Complexity**: $O((V + E) \log V)$. **Requirements**: Non-negative edge weights.

## Dijkstra - All Shortest Paths

The `DijkstraAllPairsSP` class computes all-pairs shortest paths by running Dijkstra's algorithm from each vertex.

| `DijkstraAllPairsSP`                           |                                |
| ---------------------------------------------- | ------------------------------ |
| `DijkstraAllPairsSP(EdgeWeightedDigraph g)`    | *all-pairs shortest paths*     |
| `double dist(int s, int v)`                    | *distance from s to v*         |
| `bool hasPath(int s, int v)`                   | *is there a path from s to v?* |
| `std::vector<DirectedEdge> path(int s, int v)` | *shortest path from s to v*    |

**Time Complexity**: $O(V \times (V + E) \log V)$. **Space**: $O(V^2)$. **Use**: When many shortest path queries needed.

## DAG - Shortest Paths

*Coming soon - Shortest paths in directed acyclic graphs using topological ordering.*

## DAG - Longest Paths

*Coming soon - Longest paths in directed acyclic graphs.*

## Bellman Ford - Shortest Path

*Coming soon - Bellman-Ford algorithm for graphs with negative edge weights.*

## Edge Weighted Directed Cycle

*Coming soon - Detect negative cycles in edge-weighted directed graphs.*

## Floyd Warshall - All Shortest Paths (Dense)

*Coming soon - Floyd-Warshall algorithm for all-pairs shortest paths.*

## Adjacency Matrix - Edge Weighted Digraph

*Coming soon - Matrix representation of edge-weighted directed graphs.*