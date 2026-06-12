# Exercise 13 — Graph coloring

This exercise models a graph coloring problem interpreted as frequency assignment. The model assigns frequencies to antennas so that interfering antennas do not share the same frequency, while using as few frequencies as possible.

## Decision variables

* `x[a][k]`: equals 1 if antenna `a` uses frequency `k`; 0 otherwise.
* `z[k]`: equals 1 if frequency `k` is used by at least one antenna; 0 otherwise.

## Objective

* Minimize the number of used frequencies.

## Main constraints

* Each antenna must receive exactly one frequency.
* Interfering antennas cannot share the same frequency.
* Frequency activation variables must reflect actual use.
* Ordering constraints reduce symmetry in the solution.

## Build

```bash
make
```

## Run

```bash
make run
```
