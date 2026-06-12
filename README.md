# Optimization Exercises in C++ with CPLEX

A collection of optimization exercises implemented in C++ with IBM ILOG CPLEX.

The repository includes 17 CPLEX models, covering linear programming, integer programming,
network flow, routing, graph problems, bin packing, and other
classic operations research examples.

There is also a separate Bin Packing local-search metaheuristic.

## Requirements

- IBM ILOG CPLEX Optimization Studio
- A C++ compiler with C++17 support
- `make`

## CPLEX path

Set the CPLEX installation path when building:

```bash
make CPLEX_STUDIO=/path/to/CPLEX_Studio2211
```

Or export it once:

```bash
export CPLEX_STUDIO=/path/to/CPLEX_Studio2211
```

## Project structure

- `exercise_XX_*`: standalone CPLEX exercises
- `main/`: runner for all CPLEX exercises
- `metaheuristic_01_bin_packing/`: local-search Bin Packing heuristic

Each CPLEX exercise folder contains:

- `main.cpp`
- `exercise_XX_*.hpp`
- `Makefile`
- `README.md`

## Build and run one CPLEX exercise

```bash
cd exercise_01_feed_mix
make CPLEX_STUDIO=/path/to/CPLEX_Studio2211
make run CPLEX_STUDIO=/path/to/CPLEX_Studio2211
```

Use `make clean` to remove the local binary.

## Run all CPLEX exercises

```bash
cd main
make run CPLEX_STUDIO=/path/to/CPLEX_Studio2211
```

This builds the `run_cplex` binary and executes all CPLEX exercises in sequence.

## Run the Bin Packing metaheuristic

```bash
cd metaheuristic_01_bin_packing
make run
```

With an explicit time limit:

```bash
./bin_packing_metaheuristic 10
```

With an instance file:

```bash
./bin_packing_metaheuristic 10 instance.txt
```

## Solved problems

- [Exercise 01 — Feed mix](./exercise_01_feed_mix)
- [Exercise 02 — Diet problem](./exercise_02_diet_problem)
- [Exercise 03 — Crop planning](./exercise_03_crop_planning)
- [Exercise 04 — Paint blending](./exercise_04_paint_blending)
- [Exercise 05 — Transportation problem](./exercise_05_transportation)
- [Exercise 06 — Maximum flow](./exercise_06_max_flow)
- [Exercise 07 — Shift scheduling](./exercise_07_shift_scheduling)
- [Exercise 08 — Set covering](./exercise_08_set_covering)
- [Exercise 09 — Binary knapsack](./exercise_09_binary_knapsack)
- [Exercise 10 — Maximum clique](./exercise_10_maximum_clique)
- [Exercise 11 — Cutting patterns for cans](./exercise_11_cutting_patterns)
- [Exercise 12 — Facility location](./exercise_12_facility_location)
- [Exercise 13 — Graph coloring](./exercise_13_graph_coloring)
- [Exercise 14 — Traveling salesman](./exercise_14_traveling_salesman)
- [Exercise 15 — Capacitated vehicle routing](./exercise_15_capacitated_vehicle_routing)
- [Exercise 16 — Bin packing](./exercise_16_bin_packing)
- [Exercise 17 — Shortest path](./exercise_17_shortest_path)

## Metaheuristic

- [Bin Packing — Local search](./metaheuristic_01_bin_packing)

## Authors

This project was developed by:

- [Higor Lima](https://github.com/limahigor)
- [Rita Lemos](https://github.com/ritalemos)
