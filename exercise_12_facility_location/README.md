# Exercise 12 — Facility location

This exercise models the choice of depots and the assignment of customers to them. The objective is to minimize the sum of fixed opening costs and customer service costs.

## Decision variables

* `y[i]`: equals 1 if depot `i` is opened; 0 otherwise.
* `x[i][j]`: equals 1 if customer `j` is assigned to depot `i`; 0 otherwise.

## Objective

* Minimize total opening and assignment costs.

## Main constraints

* Every customer must be assigned to exactly one depot.
* A customer can only be assigned to a depot that is open.
* Decision variables are binary.

## Build

```bash
make
```

## Run

```bash
make run
```
