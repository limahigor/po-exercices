# Exercise 02 — Diet problem

This exercise models a diet formulation problem with several ingredients and minimum nutritional requirements. The objective is to choose how much of each ingredient to use so that vitamin requirements are satisfied at the lowest possible cost.

## Decision variables

* `x[i]`: amount of ingredient `i` included in the diet.

## Objective

* Minimize the total ingredient cost.

## Main constraints

* The total amount of vitamin A must meet the minimum required level.
* The total amount of vitamin C must also meet the minimum required level.
* Ingredient quantities cannot be negative.

## Build

```bash
make
```

## Run

```bash
make run
```
