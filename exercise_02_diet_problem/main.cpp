#include <cmath>
#include <iomanip>
#include <ilcplex/ilocplex.h>
#include <iostream>

#include "exercise_02_diet_problem.hpp"

ILOSTLBEGIN

namespace {

void print_header() {
  cout << "============================================================"
       << endl;
  cout << "Exercise 02 - Diet problem" << endl;
  cout << "============================================================" << endl
       << endl;
  cout << "Problem summary:" << endl;
  cout << "Choose ingredient amounts at minimum cost while meeting the vitamin "
          "requirements."
       << endl
       << endl;
}

void print_failure(const char *message) {
  cout << "Solution status: " << message << endl;
}

} // namespace

int run_exercise_02_diet_problem() {
  IloEnv env;

  try {
    print_header();

    IloModel model(env);

    const int n = 6;
    double preco[n] = {35, 30, 60, 50, 27, 22};
    double vitA[n] = {1, 0, 2, 2, 1, 2};
    double vitC[n] = {0, 1, 3, 1, 3, 2};

    IloNumVarArray x(env, n, 0.0, IloInfinity, ILOFLOAT);

    IloExpr obj(env);
    IloExpr exprA(env);
    IloExpr exprC(env);

    for (int i = 0; i < n; ++i) {
      obj += preco[i] * x[i];
      exprA += vitA[i] * x[i];
      exprC += vitC[i] * x[i];
    }

    model.add(IloMinimize(env, obj));

    model.add(exprA >= 9.0);
    model.add(exprC >= 19.0);

    obj.end();
    exprA.end();
    exprC.end();

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

    cout << "Ingredient levels:" << endl;
    for (int i = 0; i < n; ++i) {
      cout << "  - Ingredient " << (i + 1) << ": " << cplex.getValue(x[i])
           << endl;
    }

    double totalA = 0.0;
    double totalC = 0.0;

    for (int i = 0; i < n; ++i) {
      totalA += vitA[i] * cplex.getValue(x[i]);
      totalC += vitC[i] * cplex.getValue(x[i]);
    }

    cout << endl;
    cout << "Nutrition check:" << endl;
    cout << "  - Vitamin A: " << totalA << " / 9" << endl;
    cout << "  - Vitamin C: " << totalC << " / 19" << endl;
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
int main() { return run_exercise_02_diet_problem(); }
#endif
