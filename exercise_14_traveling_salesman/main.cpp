#include <cmath>
#include <iomanip>
#include <ilcplex/ilocplex.h>
#include <iostream>

#include "exercise_14_traveling_salesman.hpp"

ILOSTLBEGIN

namespace {

void print_header() {
  cout << "============================================================"
       << endl;
  cout << "Exercise 14 - Traveling salesman" << endl;
  cout << "============================================================" << endl
       << endl;
  cout << "Problem summary:" << endl;
  cout << "Find the shortest tour that visits every city exactly once and "
          "returns to the start."
       << endl
       << endl;
}

void print_failure(const char *message) {
  cout << "Solution status: " << message << endl;
}

} // namespace

int run_exercise_14_traveling_salesman() {
  IloEnv env;

  try {
    print_header();

    IloModel model(env);

    const int n = 6;

    double dist[n][n] = {{0, 10, 20, 15, 30, 25}, {10, 0, 35, 20, 10, 15},
                         {20, 35, 0, 30, 20, 10}, {15, 20, 30, 0, 25, 20},
                         {30, 10, 20, 25, 0, 15}, {25, 15, 10, 20, 15, 0}};

    IloArray<IloNumVarArray> x(env, n);
    for (int i = 0; i < n; ++i) {
      x[i] = IloNumVarArray(env);
      for (int j = 0; j < n; ++j) {
        if (i == j) {
          x[i].add(IloNumVar(env, 0.0, 0.0, ILOBOOL));
        } else {
          x[i].add(IloNumVar(env, 0.0, 1.0, ILOBOOL));
        }
      }
    }

    IloNumVarArray u(env, n, 1.0, n, ILOINT);

    IloExpr obj(env);
    for (int i = 0; i < n; ++i) {
      for (int j = 0; j < n; ++j) {
        if (i != j) {
          obj += dist[i][j] * x[i][j];
        }
      }
    }

    model.add(IloMinimize(env, obj));
    obj.end();

    for (int j = 0; j < n; ++j) {
      IloExpr in(env);
      IloExpr out(env);

      for (int i = 0; i < n; ++i) {
        if (i != j) {
          in += x[i][j];
        }
      }
      for (int k = 0; k < n; ++k) {
        if (k != j) {
          out += x[j][k];
        }
      }

      model.add(in == 1);
      model.add(out == 1);

      in.end();
      out.end();
    }

    model.add(u[0] == 1);
    for (int i = 1; i < n; ++i) {
      for (int j = 1; j < n; ++j) {
        if (i != j) {
          model.add(u[i] - u[j] + n * x[i][j] <= n - 1);
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

    cout << "Tour:" << endl;
    cout << "  1";

    int current = 0;
    for (int step = 0; step < n - 1; ++step) {
      for (int j = 0; j < n; ++j) {
        if (j != current && cplex.getValue(x[current][j]) > 0.5) {
          cout << " -> " << (j + 1);
          current = j;
          break;
        }
      }
    }

    cout << " -> 1" << endl;
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
int main() { return run_exercise_14_traveling_salesman(); }
#endif
