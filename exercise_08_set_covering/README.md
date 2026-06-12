# Exercise 08 — Set covering

This exercise models the placement of schools in neighborhoods of a city. The model must choose the smallest number of locations so that every neighborhood is covered either by its own school or by a school in an adjacent neighborhood.

## Decision variables

* `x[i]`: equals 1 if a school is opened in neighborhood `i`; 0 otherwise.

## Objective

* Minimize the number of opened schools.

## Main constraints

* Every neighborhood must be covered by at least one selected location.
* Decision variables are binary.

## Build

```bash
make
```

## Run

```bash
make run
```
