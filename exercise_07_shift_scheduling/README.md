# Exercise 07 — Shift scheduling

This exercise models weekly nurse scheduling with shifts that last four consecutive days. The model decides how many nurses should start on each day so that daily demand is covered with the smallest possible workforce.

## Decision variables

* `x[i]`: number of nurses whose shift starts on day `i`.

## Objective

* Minimize the total number of nurses used in the weekly plan.

## Main constraints

* Each day must be covered by enough nurses on duty.
* Coverage depends on the nurses who started in the current day and the previous three days.
* The number of nurses starting each day must be nonnegative and integral.

## Build

```bash
make
```

## Run

```bash
make run
```
