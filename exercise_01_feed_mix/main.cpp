#include <cmath>
#include <ilcplex/ilocplex.h>
#include <iomanip>
#include <iostream>

#include "exercise_01_feed_mix.hpp"

ILOSTLBEGIN

namespace {

void print_header() {
  cout << "============================================================"
       << endl;
  cout << "Exercise 01 - Feed mix" << endl;
  cout << "============================================================" << endl
       << endl;
  cout << "Problem summary:" << endl;
  cout << "Select production levels for two feed products to maximize profit "
          "under meat and cereal limits."
       << endl
       << endl;
}

void print_failure(const char *message) {
  cout << "Solution status: " << message << endl;
}

} // namespace

int run_exercise_01_feed_mix() {
  IloEnv env;

  try {
    print_header();

    IloModel model(env);

    IloNumVar amgs(env, 0.0, IloInfinity, ILOFLOAT, "AMGS");
    IloNumVar re(env, 0.0, IloInfinity, ILOFLOAT, "RE");

    model.add(IloMaximize(env, 11.0 * amgs + 12.0 * re));
    model.add(amgs + 4.0 * re <= 10000.0);
    model.add(5.0 * amgs + 2.0 * re <= 30000.0);

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

    cout << "Production plan:" << endl;
    cout << "  - AMGS: " << cplex.getValue(amgs) << " units" << endl;
    cout << "  - RE  : " << cplex.getValue(re) << " units" << endl << endl;

    cout << "Resource use:" << endl;
    cout << "  - Meat   : " << (cplex.getValue(amgs) + 4.0 * cplex.getValue(re))
         << " / 10000" << endl;
    cout << "  - Cereal : "
         << (5.0 * cplex.getValue(amgs) + 2.0 * cplex.getValue(re))
         << " / 30000" << endl;
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
int main() { return run_exercise_01_feed_mix(); }
#endif
