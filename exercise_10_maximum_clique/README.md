# Exercise 10 — Maximum clique

This exercise models the search for the largest clique in an undirected graph. A clique is a subset of vertices in which every pair of selected vertices is adjacent.

## Decision variables

* `x[i]`: equals 1 if vertex `i` belongs to the clique; 0 otherwise.

## Objective

* Maximize the number of selected vertices.

## Main constraints

* Two non-adjacent vertices cannot be selected at the same time.
* Decision variables are binary.

## Build

```bash
make
```

## Run

```bash
make run
```
