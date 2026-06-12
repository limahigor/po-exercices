# Exercise 11 — Cutting patterns for cans

This exercise combines cutting decisions and can assembly planning. The model chooses how often each cutting pattern is used and how many cans are assembled in order to maximize the final profit.

## Decision variables

* `y[i]`: number of times cutting pattern `i` is used.
* `cans`: number of cans assembled from the produced parts.

## Objective

* Maximize revenue from assembled cans minus the costs associated with unused bodies and produced lids.

## Main constraints

* The number of assembled cans cannot exceed the available can bodies.
* The number of lids must be sufficient for assembly.
* Sheet usage and total production time are limited.
* Decision variables are integral.

## Build

```bash
make
```

## Run

```bash
make run
```
