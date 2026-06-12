#include <cmath>
#include <iomanip>
#include <ilcplex/ilocplex.h>
#include <iostream>

#include "exercise_11_cutting_patterns.hpp"

ILOSTLBEGIN

namespace {

void print_header() {
  cout << "============================================================"
       << endl;
  cout << "Exercise 11 - Cutting patterns for cans" << endl;
  cout << "============================================================" << endl
       << endl;
  cout << "Problem summary:" << endl;
  cout << "Choose cutting patterns and can assembly levels to maximize profit."
       << endl
       << endl;
}

void print_failure(const char *message) {
  cout << "Solution status: " << message << endl;
}

} // namespace

int run_exercise_11_cutting_patterns() {
  IloEnv env;

  try {
    print_header();

    IloModel model(env);

    const int p = 4;

    int sheetType[p] = {1, 2, 1, 1};
    int bodies[p] = {1, 2, 0, 4};
    int lids[p] = {7, 3, 9, 4};
    int time[p] = {2, 3, 2, 1};

    IloNumVarArray y(env, p, 0.0, IloInfinity, ILOINT);
    IloNumVar cans(env, 0.0, IloInfinity, ILOINT, "cans");

    IloExpr totalBodies(env);
    IloExpr totalLids(env);
    IloExpr t1(env);
    IloExpr t2(env);
    IloExpr totalTime(env);

    for (int i = 0; i < p; ++i) {
      totalBodies += bodies[i] * y[i];
      totalLids += lids[i] * y[i];
      totalTime += time[i] * y[i];

      if (sheetType[i] == 1) {
        t1 += y[i];
      } else {
        t2 += y[i];
      }
    }

    model.add(cans <= totalBodies);
    model.add(2 * cans <= totalLids);
    model.add(t1 <= 200);
    model.add(t2 <= 90);
    model.add(totalTime <= 100);

    model.add(
        IloMaximize(env, 50 * cans - 5 * (totalBodies - cans) - 3 * totalLids));

    totalBodies.end();
    totalLids.end();
    t1.end();
    t2.end();
    totalTime.end();

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

    cout << "Pattern usage:" << endl;
    for (int i = 0; i < p; ++i) {
      double value = cplex.getValue(y[i]);
      if (std::abs(value) < 0.0005) {
        value = 0.0;
      }

      cout << "  - Pattern " << (i + 1) << ": " << value << " prints" << endl;
    }

    cout << endl;
    cout << "Production summary:" << endl;
    cout << "  - Cans assembled: " << cplex.getValue(cans) << endl;
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
int main() { return run_exercise_11_cutting_patterns(); }
#endif
