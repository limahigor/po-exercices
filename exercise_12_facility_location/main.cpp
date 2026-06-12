#include <cmath>
#include <iomanip>
#include <ilcplex/ilocplex.h>
#include <iostream>

#include "exercise_12_facility_location.hpp"

ILOSTLBEGIN

namespace {

void print_header() {
  cout << "============================================================"
       << endl;
  cout << "Exercise 12 - Facility location" << endl;
  cout << "============================================================" << endl
       << endl;
  cout << "Problem summary:" << endl;
  cout << "Decide which depots to open and how to assign customers at minimum "
          "cost."
       << endl
       << endl;
}

void print_failure(const char *message) {
  cout << "Solution status: " << message << endl;
}

} // namespace

int run_exercise_12_facility_location() {
  IloEnv env;

  try {
    print_header();

    IloModel model(env);

    const int depots = 4;
    const int customers = 5;

    double openCost[depots] = {100, 150, 80, 120};
    double service[depots][customers] = {{10, 8, 5, 12, 9},
                                         {7, 11, 6, 8, 10},
                                         {12, 5, 9, 7, 8},
                                         {6, 9, 8, 10, 6}};

    IloNumVarArray y(env, depots, 0.0, 1.0, ILOBOOL);
    IloArray<IloNumVarArray> x(env, depots);
    for (int i = 0; i < depots; ++i) {
      x[i] = IloNumVarArray(env, customers, 0.0, 1.0, ILOBOOL);
    }

    IloExpr obj(env);
    for (int i = 0; i < depots; ++i) {
      obj += openCost[i] * y[i];
      for (int j = 0; j < customers; ++j) {
        obj += service[i][j] * x[i][j];
      }
    }

    model.add(IloMinimize(env, obj));
    obj.end();

    for (int j = 0; j < customers; ++j) {
      IloExpr assign(env);

      for (int i = 0; i < depots; ++i) {
        assign += x[i][j];
      }

      model.add(assign == 1);
      assign.end();
    }

    for (int i = 0; i < depots; ++i) {
      for (int j = 0; j < customers; ++j) {
        model.add(x[i][j] <= y[i]);
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

    cout << "Open depots and assignments:" << endl;
    for (int i = 0; i < depots; ++i) {
      if (cplex.getValue(y[i]) > 0.5) {
        cout << "  Depot " << (i + 1) << ':' << endl;

        for (int j = 0; j < customers; ++j) {
          if (cplex.getValue(x[i][j]) > 0.5) {
            cout << "    - Customer " << (j + 1) << endl;
          }
        }
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
int main() { return run_exercise_12_facility_location(); }
#endif
