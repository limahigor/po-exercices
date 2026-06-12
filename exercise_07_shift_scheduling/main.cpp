#include <cmath>
#include <iomanip>
#include <ilcplex/ilocplex.h>
#include <iostream>

#include "exercise_07_shift_scheduling.hpp"

ILOSTLBEGIN

namespace {

void print_header() {
  cout << "============================================================"
       << endl;
  cout << "Exercise 07 - Shift scheduling" << endl;
  cout << "============================================================" << endl
       << endl;
  cout << "Problem summary:" << endl;
  cout << "Choose how many nurses start each day so daily staffing demand is "
          "covered with 4-day shifts."
       << endl
       << endl;
}

void print_failure(const char *message) {
  cout << "Solution status: " << message << endl;
}

} // namespace

int run_exercise_07_shift_scheduling() {
  IloEnv env;

  try {
    print_header();

    IloModel model(env);

    const int days = 7;
    int demand[days] = {3, 4, 3, 2, 2, 3, 2};

    IloNumVarArray x(env, days, 0.0, IloInfinity, ILOINT);

    IloExpr obj(env);
    for (int i = 0; i < days; ++i) {
      obj += x[i];
    }

    model.add(IloMinimize(env, obj));
    obj.end();

    for (int j = 0; j < days; ++j) {
      IloExpr cover(env);

      for (int k = 0; k < 4; ++k) {
        cover += x[(j - k + days) % days];
      }

      model.add(cover >= demand[j]);
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

    cout << "Start plan:" << endl;
    for (int i = 0; i < days; ++i) {
      double starts = cplex.getValue(x[i]);
      if (std::abs(starts) < 0.0005) {
        starts = 0.0;
      }

      cout << "  - Day " << (i + 1) << ": " << starts << " nurses start"
           << endl;
    }

    cout << endl;
    cout << "Demand check:" << endl;
    for (int j = 0; j < days; ++j) {
      double cover = 0.0;
      for (int k = 0; k < 4; ++k) {
        cover += cplex.getValue(x[(j - k + days) % days]);
      }

      if (std::abs(cover) < 0.0005) {
        cover = 0.0;
      }

      cout << "  - Day " << (j + 1) << ": " << cover
           << " assigned for demand " << demand[j] << endl;
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
int main() { return run_exercise_07_shift_scheduling(); }
#endif
