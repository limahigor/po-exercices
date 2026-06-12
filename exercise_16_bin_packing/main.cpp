#include <cmath>
#include <iomanip>
#include <ilcplex/ilocplex.h>
#include <iostream>

#include "exercise_16_bin_packing.hpp"

ILOSTLBEGIN

namespace {

void print_header() {
  cout << "============================================================"
       << endl;
  cout << "Exercise 16 - Bin packing" << endl;
  cout << "============================================================" << endl
       << endl;
  cout << "Problem summary:" << endl;
  cout << "Pack all items into the smallest number of bins of capacity one."
       << endl
       << endl;
}

void print_failure(const char *message) {
  cout << "Solution status: " << message << endl;
}

} // namespace

int run_exercise_16_bin_packing() {
  IloEnv env;

  try {
    print_header();

    IloModel model(env);

    const int items = 8;
    const int bins = 8;

    double size[items] = {0.5, 0.3, 0.7, 0.4, 0.6, 0.2, 0.8, 0.35};

    IloArray<IloNumVarArray> x(env, items);
    for (int i = 0; i < items; ++i) {
      x[i] = IloNumVarArray(env, bins, 0.0, 1.0, ILOBOOL);
    }

    IloNumVarArray y(env, bins, 0.0, 1.0, ILOBOOL);

    IloExpr obj(env);
    for (int b = 0; b < bins; ++b) {
      obj += y[b];
    }

    model.add(IloMinimize(env, obj));
    obj.end();

    for (int i = 0; i < items; ++i) {
      IloExpr assign(env);

      for (int b = 0; b < bins; ++b) {
        assign += x[i][b];
      }

      model.add(assign == 1);
      assign.end();
    }

    for (int b = 0; b < bins; ++b) {
      IloExpr cap(env);

      for (int i = 0; i < items; ++i) {
        cap += size[i] * x[i][b];
      }

      model.add(cap <= y[b]);
      cap.end();
    }

    for (int b = 1; b < bins; ++b) {
      model.add(y[b] <= y[b - 1]);
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

    cout << "Bin allocation:" << endl;
    for (int b = 0; b < bins; ++b) {
      if (cplex.getValue(y[b]) > 0.5) {
        double load = 0.0;
        cout << "  Bin " << (b + 1) << ':';

        for (int i = 0; i < items; ++i) {
          if (cplex.getValue(x[i][b]) > 0.5) {
            load += size[i];
            cout << " item_" << (i + 1);
          }
        }

        cout << " | load: " << load << " / 1.00" << endl;
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
int main() { return run_exercise_16_bin_packing(); }
#endif
