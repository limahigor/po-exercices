# Exercise 15 — Capacitated vehicle routing

This exercise models a vehicle routing problem with limited fleet capacity. Vehicles start at the depot, serve customers exactly once, and must respect both routing consistency and load propagation constraints while minimizing total travel distance.

## Decision variables

* `x[i][j][k]`: equals 1 if vehicle `k` travels directly from node `i` to node `j`; 0 otherwise.
* `load[i][k]`: cumulative load associated with vehicle `k` when node `i` is reached.

## Objective

* Minimize the total distance traveled by all vehicles.

## Main constraints

* Each customer must be visited exactly once.
* Vehicle flow must be conserved along each route.
* Each vehicle can leave the depot at most once.
* Load propagation must respect customer demand and vehicle capacity.

## Build

```bash
make
```

## Run

```bash
make run
```
