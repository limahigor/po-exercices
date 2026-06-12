# Exercise 17 — Shortest path

This exercise models the selection of a minimum-cost path between two nodes in a network. The model chooses which directed arcs belong to the path so that flow is preserved from the source to the destination and the total path cost is minimized.

## Decision variables

* `x[a]`: equals 1 if arc `a` belongs to the selected path; 0 otherwise.

## Objective

* Minimize the total path cost.

## Main constraints

* The source must send one unit of flow.
* The destination must receive one unit of flow.
* Intermediate nodes must satisfy flow conservation.
* Decision variables are binary.

## Build

```bash
make
```

## Run

```bash
make run
```
