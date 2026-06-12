# Exercise 01 — Feed mix

This exercise models the production of two feed products that compete for the same raw materials. The goal is to decide how much of each product to produce in order to maximize profit without exceeding the available amounts of meat and cereal.

## Decision variables

* `amgs`: quantity produced of AMGS feed.
* `re`: quantity produced of RE feed.

## Objective

* Maximize the total profit from AMGS and RE production.

## Main constraints

* Meat availability limits the combined production.
* Cereal availability also limits the production plan.
* Production quantities cannot be negative.

## Build

```bash
make
```

## Run

```bash
make run
```
