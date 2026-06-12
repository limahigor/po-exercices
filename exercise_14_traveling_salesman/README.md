# Exercise 14 — Traveling Salesman

This exercise models a traveling salesman tour through a set of cities. The route must visit every city exactly once, return to the starting city, and minimize the total travel distance.

## Decision variables

* `x[i][j]`: equals 1 if the route goes directly from city `i` to city `j`; 0 otherwise.
* `u[i]`: order variable used by the MTZ constraints to eliminate subtours.

## Objective

* Minimize the total distance traveled.

## Main constraints

* Each city has exactly one incoming arc.
* Each city has exactly one outgoing arc.
* MTZ constraints prevent disconnected subtours.

## Build

```bash
make
```

## Run

```bash
make run
```
