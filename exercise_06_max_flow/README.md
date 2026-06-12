# Exercise 06 — Maximum flow

This exercise models a flow network with a source, a sink, and intermediate nodes. The objective is to send as much flow as possible from the source to the sink without violating arc capacities.

## Decision variables

* `f[a]`: amount of flow sent through arc `a`.

## Objective

* Maximize the total flow leaving the source.

## Main constraints

* Each arc has a maximum flow capacity.
* Flow must be conserved at every intermediate node.
* Flow values cannot be negative.

## Build

```bash
make
```

## Run

```bash
make run
```
