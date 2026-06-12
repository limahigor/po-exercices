# Exercise 16 — Bin packing

This exercise models the assignment of items to bins with limited capacity. The model decides where each item should be placed and how many bins are needed, minimizing the number of used bins.

## Decision variables

* `x[i][b]`: equals 1 if item `i` is assigned to bin `b`; 0 otherwise.
* `y[b]`: equals 1 if bin `b` is used; 0 otherwise.

## Objective

* Minimize the number of bins used.

## Main constraints

* Every item must be assigned to exactly one bin.
* The total load in each bin cannot exceed its capacity.
* Ordering constraints reduce symmetry between equivalent bins.

## Build

```bash
make
```

## Run

```bash
make run
```
