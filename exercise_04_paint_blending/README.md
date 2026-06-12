# Exercise 04 — Paint blending

This exercise models the preparation of two paint products from intermediate solutions and pure components. The goal is to satisfy the required paint volumes and composition rules while minimizing the total purchasing cost.

## Decision variables

* `solA_sr`, `solB_sr`, `sec_sr`, `cor_sr`: quantities used in the SR paint.
* `solA_sn`, `solB_sn`, `sec_sn`, `cor_sn`: quantities used in the SN paint.

## Objective

* Minimize the total cost of the selected components.

## Main constraints

* Each paint must reach its required final volume.
* Each paint must contain at least the required amount of SEC.
* Each paint must contain at least the required amount of COR.
* All component quantities must be nonnegative.

## Build

```bash
make
```

## Run

```bash
make run
```
