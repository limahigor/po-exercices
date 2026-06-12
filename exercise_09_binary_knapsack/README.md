# Exercise 09 — Binary knapsack

This exercise models the classic 0-1 knapsack problem. The model must decide which items to include so that the total value is maximized without exceeding the knapsack capacity.

## Decision variables

* `x[i]`: equals 1 if item `i` is selected; 0 otherwise.

## Objective

* Maximize the total value of the selected items.

## Main constraints

* The total weight of the selected items cannot exceed the knapsack capacity.
* Each item can be either selected or not selected.

## Build

```bash
make
```

## Run

```bash
make run
```
