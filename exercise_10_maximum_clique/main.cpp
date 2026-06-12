#include <cmath>
#include <iomanip>
#include <ilcplex/ilocplex.h>
#include <iostream>

#include "exercise_10_maximum_clique.hpp"

ILOSTLBEGIN

namespace {

void print_header() {
  cout << "============================================================"
       << endl;
  cout << "Exercise 10 - Maximum clique" << endl;
  cout << "============================================================" << endl
       << endl;
  cout << "Problem summary:" << endl;
  cout << "Find the largest clique in the given graph." << endl << endl;
}

void print_failure(const char *message) {
  cout << "Solution status: " << message << endl;
}

} // namespace

int run_exercise_10_maximum_clique() {
  IloEnv env;

  try {
    print_header();

    IloModel model(env);

    const int n = 7;

    bool edge[n][n] = {};
    int baseEdges[][2] = {{0, 1}, {0, 2}, {1, 2}, {1, 3}, {1, 4},
                          {2, 4}, {3, 4}, {3, 5}, {4, 5}, {5, 6}};

    for (auto &e : baseEdges) {
      edge[e[0]][e[1]] = true;
      edge[e[1]][e[0]] = true;
    }

    IloNumVarArray x(env, n, 0.0, 1.0, ILOBOOL);

    IloExpr obj(env);
    for (int i = 0; i < n; ++i) {
      obj += x[i];
    }

    model.add(IloMaximize(env, obj));
    obj.end();

    for (int i = 0; i < n; ++i) {
      for (int j = i + 1; j < n; ++j) {
        if (!edge[i][j]) {
          model.add(x[i] + x[j] <= 1);
        }
      }
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

    cout << "Vertices in the clique:" << endl;
    for (int i = 0; i < n; ++i) {
      if (cplex.getValue(x[i]) > 0.5) {
        cout << "  - Vertex " << (i + 1) << endl;
      }
    }
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
int main() { return run_exercise_10_maximum_clique(); }
#endif
