#include <cmath>
#include <iomanip>
#include <ilcplex/ilocplex.h>
#include <iostream>

#include "exercise_08_set_covering.hpp"

ILOSTLBEGIN

namespace {

void print_header() {
  cout << "============================================================"
       << endl;
  cout << "Exercise 08 - Set covering" << endl;
  cout << "============================================================" << endl
       << endl;
  cout << "Problem summary:" << endl;
  cout << "Open the smallest number of schools so every neighborhood is covered."
       << endl
       << endl;
}

void print_failure(const char *message) {
  cout << "Solution status: " << message << endl;
}

} // namespace

int run_exercise_08_set_covering() {
  IloEnv env;

  try {
    print_header();

    IloModel model(env);

    const int n = 7;

    int adj[n][4] = {{0, 1, 3, -1}, {0, 1, 2, 4},  {1, 2, 5, -1},
                     {0, 3, 4, 6},  {1, 3, 4, 5},  {2, 4, 5, -1},
                     {3, 6, -1, -1}};
    int adjSize[n] = {3, 4, 3, 4, 4, 3, 2};

    IloNumVarArray x(env, n, 0.0, 1.0, ILOBOOL);

    IloExpr obj(env);
    for (int i = 0; i < n; ++i) {
      obj += x[i];
    }

    model.add(IloMinimize(env, obj));
    obj.end();

    for (int i = 0; i < n; ++i) {
      IloExpr cover(env);

      for (int j = 0; j < adjSize[i]; ++j) {
        cover += x[adj[i][j]];
      }

      model.add(cover >= 1);
      cover.end();
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

    cout << "School locations:" << endl;
    for (int i = 0; i < n; ++i) {
      if (cplex.getValue(x[i]) > 0.5) {
        cout << "  - Neighborhood " << (i + 1) << endl;
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
int main() { return run_exercise_08_set_covering(); }
#endif
