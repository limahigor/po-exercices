# Bin Packing — Single-solution local search

This folder contains a standalone metaheuristic for the Bin Packing problem. It does not use CPLEX. The goal is to place all items into the smallest possible number of bins with capacity 1.

## Solution representation

The solution is stored as a list of bins. Each bin keeps the indices of its items and its current load. A second vector stores the bin assigned to each item. This representation makes item relocation and bin removal straightforward during local search.

## Evaluation

The search works only with feasible solutions. A solution is feasible when every item appears exactly once and every bin load stays at or below 1. The objective value is the number of used bins, so fewer bins is always better.

## Solution

The construction phase uses First Fit Decreasing. Items are sorted from largest to smallest, then each item is placed into the first bin with enough remaining capacity. After that, local search tries to reduce the number of bins by emptying one bin at a time.

## Local search neighborhood

The local search uses first improvement with three feasible move types:

- try to empty one bin by relocating all of its items into the others;
- move one item from one bin to another and then test whether some bin can be emptied;
- swap one item from a bin with one item from another bin and then test whether some bin can be emptied.

All moves keep the solution feasible at every step.

## Stopping criterion

The program receives a time limit in seconds from the command line. The search stops when the time limit is reached or when no improving move is available.

## Instance input

You can run the program with a built-in default instance or pass a text file.

Input format:

```text
8
0.5
0.3
0.7
0.4
0.6
0.2
0.8
0.35
```

The first line is the number of items. Each following line is the size of one item.

## Build

```bash
make
```

## Run

Use the default instance with a 15-second limit:

```bash
make run
```

Run with an explicit time limit:

```bash
./bin_packing_metaheuristic 15
```

Run with an instance file:

```bash
./bin_packing_metaheuristic 15 instance.txt
```
