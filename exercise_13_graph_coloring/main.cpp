#include <cmath>
#include <iomanip>
#include <ilcplex/ilocplex.h>
#include <iostream>

#include "exercise_13_graph_coloring.hpp"

ILOSTLBEGIN

namespace {

void print_header() {
  cout << "============================================================"
       << endl;
  cout << "Exercise 13 - Graph coloring" << endl;
  cout << "============================================================" << endl
       << endl;
  cout << "Problem summary:" << endl;
  cout << "Assign the smallest number of frequencies to antennas that "
          "interfere with each other."
       << endl
       << endl;
}

void print_failure(const char *message) {
  cout << "Solution status: " << message << endl;
}

} // namespace

int run_exercise_13_graph_coloring() {
  IloEnv env;

  try {
    print_header();

    IloModel model(env);

    const int antennas = 6;
    const int freqs = 6;

    int edges[][2] = {{0, 1}, {0, 2}, {1, 2}, {1, 3},
                      {2, 4}, {3, 4}, {3, 5}, {4, 5}};
    const int edgeCount = sizeof(edges) / sizeof(edges[0]);

    IloArray<IloNumVarArray> x(env, antennas);
    for (int a = 0; a < antennas; ++a) {
      x[a] = IloNumVarArray(env, freqs, 0.0, 1.0, ILOBOOL);
    }

    IloNumVarArray z(env, freqs, 0.0, 1.0, ILOBOOL);

    IloExpr obj(env);
    for (int k = 0; k < freqs; ++k) {
      obj += z[k];
    }

    model.add(IloMinimize(env, obj));
    obj.end();

    for (int a = 0; a < antennas; ++a) {
      IloExpr assign(env);

      for (int k = 0; k < freqs; ++k) {
        assign += x[a][k];
      }

      model.add(assign == 1);
      assign.end();
    }

    for (int e = 0; e < edgeCount; ++e) {
      for (int k = 0; k < freqs; ++k) {
        model.add(x[edges[e][0]][k] + x[edges[e][1]][k] <= 1);
      }
    }

    for (int k = 0; k < freqs; ++k) {
      for (int a = 0; a < antennas; ++a) {
        model.add(x[a][k] <= z[k]);
      }
    }

    for (int k = 1; k < freqs; ++k) {
      model.add(z[k] <= z[k - 1]);
    }

    IloCplex cplex(model);
    cplex.setOut(env.getNullStream());

    if (!cplex.solve()) {
      print_failure("No feasible solution found");
      env.end();

      return 1;
    }

    cout << std::fixed << std::setprecision(2);
    cout << "Solution status: " << cplex.getStatus() << endl;

    double objectiveValue = cplex.getObjValue();
    if (std::abs(objectiveValue) < 0.0005) {
      objectiveValue = 0.0;
    }

    cout << "Objective value: " << objectiveValue << endl << endl;

    cout << "Frequency assignments:" << endl;
    for (int a = 0; a < antennas; ++a) {
      for (int k = 0; k < freqs; ++k) {
        if (cplex.getValue(x[a][k]) > 0.5) {
          cout << "  - Antenna " << (a + 1) << " uses frequency " << (k + 1)
               << endl;
        }
      }
    }

    cout << endl;
    cout << "Frequencies used: " << objectiveValue << endl;
  } catch (const IloException &ex) {
    cerr << "CPLEX error: " << ex << endl;
    env.end();

    return 1;
  } catch (const std::exception &ex) {
    cerr << "Error: " << ex.what() << endl;
    env.end();

    return 1;
  }

  env.end();

  return 0;
}

#ifndef BUILD_ALL_EXERCISES
int main() { return run_exercise_13_graph_coloring(); }
#endif
