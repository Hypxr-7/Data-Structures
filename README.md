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
  - [Max Indexed Priority Queue](#max-indexed-priority-queue-1)
  - [Min Indexed Priority Queue](#min-indexed-priority-queue)
  - [Min Indexed Priority Queue](#min-indexed-priority-queue-1)
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
  - [Strongly Connected Components (Kosarahu-Sharir)](#strongly-connected-components-kosarahu-sharir)
  - [Strongly Connected Components (Tarjan)](#strongly-connected-components-tarjan)
  - [Strongly Connected Components (Gabow)](#strongly-connected-components-gabow)
  - [Edge Weighted Graph](#edge-weighted-graph)
  - [Minimum Spanning Tree (Lazy Prim)](#minimum-spanning-tree-lazy-prim)
  - [Minimum Spanning Tree (Eager Prim)](#minimum-spanning-tree-eager-prim)
  - [Minimum Spanning Tree (Kruskal)](#minimum-spanning-tree-kruskal)
  - [Minimum Spanning Tree (Boruvka)](#minimum-spanning-tree-boruvka)
  - [Edge Weighted Diraph](#edge-weighted-diraph)
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
| `Stack<Item>`          |                                    |
| ---------------------- | ---------------------------------- |
| `Stack()`              | *creates an empty stack*           |
| `bool empty()`         | *is stack empty?*                  |
| `int size()`           | *number of items in stack*         |
| `void push(Item item)` | *add item to stack*                |
| `Item pop()`           | *remove item from stack*           |
| `Item peek()`          | *get item at top without removing* |

## Queue

| `Queue<Item>`             |                                      |
| ------------------------- | ------------------------------------ |
| `Queue()`                 | *creates an empty queue*             |
| `bool empty()`            | *is queue empty?*                    |
| `int size()`              | *number of items in queue*           |
| `void enqueue(Item item)` | *enqueue item*                       |
| `Item dequeue()`          | *dequeue item*                       |
| `Item peek()`             | *get item at top without dequeueing* |

## Union Find (Quick Find)

| `QuickFindUF`                  |                                     |
| ------------------------------ | ----------------------------------- |
| `QuickFindUF(int p)`           | *initialize UF with p elements*     |
| `int count()`                  | *number of components*              |
| `int find(int p)`              | *ID of elements p*                  |
| `bool connected(int p, int q)` | *are elements p and q connected?*   |
| `void unite(int p, int q)`     | *add p and q to the same component* |

## Union Find (Quick Union)

| `QuickUnionUF`                 |                                     |
| ------------------------------ | ----------------------------------- |
| `QuickUnionUF(int p)`          | *initialize UF with p elements*     |
| `int count()`                  | *number of components*              |
| `int find(int p)`              | *ID of elements p*                  |
| `bool connected(int p, int q)` | *are elements p and q connected?*   |
| `void unite(int p, int q)`     | *add p and q to the same component* |

## Union Find (Weighted Quick Union)

| `WeightedQuickUnionUF`         |                                     |
| ------------------------------ | ----------------------------------- |
| `WeightedQuickUnionUF(int p)`  | *initialize UF with p elements*     |
| `int count()`                  | *number of components*              |
| `int find(int p)`              | *ID of elements p*                  |
| `bool connected(int p, int q)` | *are elements p and q connected?*   |
| `void unite(int p, int q)`     | *add p and q to the same component* |

## Union Find (Weighted Quick Union with Path Compression)

| `WeightedQuickUnionPathCompressionUF`        |                                     |
| -------------------------------------------- | ----------------------------------- |
| `WeightedQuickUnionPathCompressionUF(int p)` | *initialize UF with p elements*     |
| `int count()`                                | *number of components*              |
| `int find(int p)`                            | *ID of elements p*                  |
| `bool connected(int p, int q)`               | *are elements p and q connected?*   |
| `void unite(int p, int q)`                   | *add p and q to the same component* |


# Sorting

## Insertion Sort (Standard)

`InsertionSort<T>::sort(std::vector<T> arr)`

sorts the array with standard insertion sort

## Insertion Sort (Optimized)

`InsertionSortOptimized<T>::sort(std::vector<T> arr)`

sorts the array using insertion sort making use of shifting instead of swaps

## Insertion Sort (Binary Search)

`InsertionSortBinary<T>::sort(std::vector<T> arr)`

uses binary search to locate insertion point

## Selection Sort

`SelectionSort<T>::sort(std::vector<T> arr)`

uses standard selection sort

## Shell Sort

`ShellSort<T>::sort(std::vector<T> arr)`

uses shell sort with $3x+1$ gap

## Mergesort (Standard)

`Mergesort<T>::sort(std::vector<T> arr)`

uses standard mergesort

## Mergesort (Bottoms Up)

`MergesortBottomsUp<T>::sort(std::vector<T> arr)`

uses a non-recursive mergesort

## Mergesort (Optimized)

`MergesortOptimized<T>::sort(std::vector<T> arr)`

optimizes standard mergesort by:
1. adding a cutoff to insertion sort
2. swapping `arr` and `aux` preventing the need to copy in `merge()`
3. avoid calling `merge()` if sub-arrays already sorted
 

## Quicksort (Standard)

`Quicksort<T>::sort(std::vector<T> arr)`

uses standard quicksort

## Quicksort (Hoare)

`QuicksortHoare<T>::sort(std::vector<T> arr)`

uses the original Hoare's quicksort

## Quicksort (Dual Pivot)

`QuicksortDualPivot<T>::sort(std::vector<T> arr)`

makes use of two pivots

## Quicksort (Three Way)

`QuicksortThreeWay<T>::sort(std::vector<T> arr)`

sort in three sections: less than pivot, equal to pivot, and greater than pivot

## Quicksort (Bently-McIlroy)

`QuicksortBentlyMcIlroy<T>::sort(std::vector<T> arr)`

optimized quicksort with:
1. uses insertion sort for smaller sub-arrays
2. uses median of 3 for smaller sub-arrays for pivot selection
3. For larger, uses Tukey Ninther
4. Uses Bently-McIlroy 3-way partitioning

## Max Priority Queue

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


## Min Priority Queue

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


## Max Indexed Priority Queue

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


## Min Indexed Priority Queue

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

# Graphs

## Graph

## Graph Generator

## Depth First Paths

## Breadth First Paths

## Connected Components

## Bipartite

## Cycle

## Eulerian Cycle

## Eulerian Path

## Digraph

## Digraph Generator

## Depth First Directed Paths

## Breadth First Directed Paths

## Directed Cycle

## Directed Eulerian Cycle

## Directed Eulerian Path

## Depth First Order

## Topological

## Transitive Closure

## Strongly Connected Components (Kosarahu-Sharir)

## Strongly Connected Components (Tarjan)
 
## Strongly Connected Components (Gabow)

## Edge Weighted Graph

## Minimum Spanning Tree (Lazy Prim)

## Minimum Spanning Tree (Eager Prim)

## Minimum Spanning Tree (Kruskal)

## Minimum Spanning Tree (Boruvka)

## Edge Weighted Diraph

## Dijkstra - Shortest Paths

## Dijkstra - Shortest Paths (Undirected)

## Dijkstra - All Shortest Paths

## DAG - Shortest Paths

TODO

## DAG - Longest Paths

TODO

## Bellman Ford - Shortest Path

TODO

## Edge Weighted Directed Cycle

TODO

## Floyd Warshall - All Shortest Paths (Dense)

TODO

## Adjacency Matrix - Edge Weighted Digraph

TODO
