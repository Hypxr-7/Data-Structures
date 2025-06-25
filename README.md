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

## Union Find (Quick Union)

## Union Find (Weighted Quick Union)

## Union Find (Weighted Quick Union with Path Compression)


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

# Searching

## Sequential Search

## Binary Search

## Binary Search Tree

## Red-Black Binary Search Tree

## Separate Chaining Hashing

## Linear Probing Hashing

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

## DAG - Longest Paths

## Bellman Ford - Shortest Path

## Edge Weighted Directed Cycle

## Floyd Warshall - All Shortest Paths (Dense)

## Adjacency Matrix - Edge Weighted Digraph

