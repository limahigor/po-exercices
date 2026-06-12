# Exercise 05 — Transportation problem

This exercise models the shipment of goods from factories to depots. The model decides how much should be transported along each route so that supply and demand are satisfied with minimum total transportation cost.

## Decision variables

* `x[i][j]`: quantity shipped from factory `i` to depot `j`.

## Objective

* Minimize the total transportation cost.

## Main constraints

* Each factory must ship according to its available supply.
* Each depot must receive its required demand.
* Shipment quantities cannot be negative.

## Build

```bash
make
```

## Run

```bash
make run
```
