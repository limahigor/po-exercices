#include <cmath>
#include <iomanip>
#include <ilcplex/ilocplex.h>
#include <iostream>

#include "exercise_05_transportation.hpp"

ILOSTLBEGIN

namespace {

void print_header() {
  cout << "============================================================"
       << endl;
  cout << "Exercise 05 - Transportation problem" << endl;
  cout << "============================================================" << endl
       << endl;
  cout << "Problem summary:" << endl;
  cout << "Ship goods from factories to depots at minimum transportation cost."
       << endl
       << endl;
}

void print_failure(const char *message) {
  cout << "Solution status: " << message << endl;
}

} // namespace

int run_exercise_05_transportation() {
  IloEnv env;

  try {
    print_header();

    IloModel model(env);

    const int m = 3;
    const int n = 3;

    double supply[m] = {120, 80, 80};
    double demand[n] = {150, 70, 60};
    double cost[m][n] = {{8, 5, 6}, {15, 10, 12}, {3, 9, 10}};

    IloArray<IloNumVarArray> x(env, m);
    for (int i = 0; i < m; ++i) {
      x[i] = IloNumVarArray(env, n, 0.0, IloInfinity, ILOFLOAT);
    }

    IloExpr obj(env);
    for (int i = 0; i < m; ++i) {
      for (int j = 0; j < n; ++j) {
        obj += cost[i][j] * x[i][j];
      }
    }

    model.add(IloMinimize(env, obj));
    obj.end();

    for (int i = 0; i < m; ++i) {
      IloExpr expr(env);

      for (int j = 0; j < n; ++j) {
        expr += x[i][j];
      }

      model.add(expr == supply[i]);
      expr.end();
    }

    for (int j = 0; j < n; ++j) {
      IloExpr expr(env);

      for (int i = 0; i < m; ++i) {
        expr += x[i][j];
      }

      model.add(expr == demand[j]);
      expr.end();
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

    cout << "Shipping plan:" << endl;
    for (int i = 0; i < m; ++i) {
      double shipped = 0.0;
      cout << "  Factory " << (i + 1) << ':' << endl;

      for (int j = 0; j < n; ++j) {
        double value = cplex.getValue(x[i][j]);
        shipped += value;

        if (value > 1e-6) {
          cout << "    - To depot " << (j + 1) << ": " << value << endl;
        }
      }

      cout << "    Total shipped: " << shipped << " / " << supply[i] << endl;
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
int main() { return run_exercise_05_transportation(); }
#endif
