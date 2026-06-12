#include <cmath>
#include <iomanip>
#include <ilcplex/ilocplex.h>
#include <iostream>

#include "exercise_09_binary_knapsack.hpp"

ILOSTLBEGIN

namespace {

void print_header() {
  cout << "============================================================"
       << endl;
  cout << "Exercise 09 - Binary knapsack" << endl;
  cout << "============================================================" << endl
       << endl;
  cout << "Problem summary:" << endl;
  cout << "Select items to maximize total value without exceeding the knapsack "
          "capacity."
       << endl
       << endl;
}

void print_failure(const char *message) {
  cout << "Solution status: " << message << endl;
}

} // namespace

int run_exercise_09_binary_knapsack() {
  IloEnv env;

  try {
    print_header();

    IloModel model(env);

    const int n = 6;

    double weight[n] = {4, 3, 3, 2, 5, 2};
    double value[n] = {3, 5, 2, 3, 6, 4};

    IloNumVarArray x(env, n, 0.0, 1.0, ILOBOOL);

    IloExpr obj(env);
    IloExpr cap(env);

    for (int i = 0; i < n; ++i) {
      obj += value[i] * x[i];
      cap += weight[i] * x[i];
    }

    model.add(IloMaximize(env, obj));
    model.add(cap <= 10.0);

    obj.end();
    cap.end();

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

    double totalWeight = 0.0;
    double totalValue = 0.0;

    cout << "Selected items:" << endl;
    for (int i = 0; i < n; ++i) {
      if (cplex.getValue(x[i]) > 0.5) {
        totalWeight += weight[i];
        totalValue += value[i];

        cout << "  - Item " << (i + 1) << " | weight: " << weight[i]
             << " | value: " << value[i] << endl;
      }
    }

    cout << endl;
    cout << "Totals:" << endl;
    cout << "  - Weight: " << totalWeight << " / 10" << endl;
    cout << "  - Value : " << static_cast<int>(totalValue + 0.5) << endl;
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
int main() { return run_exercise_09_binary_knapsack(); }
#endif
