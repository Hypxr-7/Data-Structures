# Data Structures <!-- omit in toc -->

This repository contains various data structures from the book: **Algorithms by Kevin Wayne 
and Robert Sedgewick**

- [Fundamentals](#fundamentals)
  - [Stack](#stack)
  - [Queue](#queue)
- [Sorting](#sorting)
  - [Insertion Sort (Standard)](#insertion-sort-standard)
  - [Insertion Sort (Optimized)](#insertion-sort-optimized)
  - [Insertion Sort (Binary Search)](#insertion-sort-binary-search)


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


# Sorting

## Insertion Sort (Standard)

`InsertionSort<T>::sort(std::vector<T> arr)`

*sorts the array with standard insertion sort*

## Insertion Sort (Optimized)

`InsertionSortOptimized<T>::sort(std::vector<T> arr)`

*sorts the array using insertion sort making use of shifting instead of swaps*

## Insertion Sort (Binary Search)

`InsertionSortBinary<T>::sort(std::vector<T> arr)`

*uses binary search to locate insertion point*

