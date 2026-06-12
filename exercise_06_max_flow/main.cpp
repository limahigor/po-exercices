#include <cmath>
#include <iomanip>
#include <ilcplex/ilocplex.h>
#include <iostream>

#include "exercise_06_max_flow.hpp"

ILOSTLBEGIN

namespace {

void print_header() {
  cout << "============================================================"
       << endl;
  cout << "Exercise 06 - Maximum flow" << endl;
  cout << "============================================================" << endl
       << endl;
  cout << "Problem summary:" << endl;
  cout << "Push as much flow as possible from the source node to the sink "
          "within arc capacities."
       << endl
       << endl;
}

void print_failure(const char *message) {
  cout << "Solution status: " << message << endl;
}

} // namespace

int run_exercise_06_max_flow() {
  IloEnv env;

  try {
    print_header();

    IloModel model(env);

    struct Arc {
      int u;
      int v;
      double cap;
    };

    Arc arcs[] = {{0, 1, 5}, {0, 2, 4}, {0, 3, 6}, {1, 2, 4}, {1, 4, 6},
                  {2, 3, 3}, {2, 5, 4}, {3, 5, 6}, {3, 6, 5}, {4, 5, 5},
                  {4, 7, 5}, {5, 4, 5}, {5, 7, 3}, {6, 5, 7}, {6, 7, 6}};
    const int arcCount = sizeof(arcs) / sizeof(arcs[0]);

    IloNumVarArray f(env, arcCount, 0.0, IloInfinity, ILOFLOAT);
    for (int a = 0; a < arcCount; ++a) {
      f[a].setUB(arcs[a].cap);
    }

    IloExpr obj(env);
    for (int a = 0; a < arcCount; ++a) {
      if (arcs[a].u == 0) {
        obj += f[a];
      }
    }

    model.add(IloMaximize(env, obj));
    obj.end();

    for (int node = 1; node < 7; ++node) {
      IloExpr inflow(env);
      IloExpr outflow(env);

      for (int a = 0; a < arcCount; ++a) {
        if (arcs[a].v == node) {
          inflow += f[a];
        }
        if (arcs[a].u == node) {
          outflow += f[a];
        }
      }

      model.add(inflow == outflow);

      inflow.end();
      outflow.end();
    }

    IloCplex cplex(model);
    cplex.setOut(env.getNullStream());

    if (!cplex.solve()) {
      print_failure("No feasible solution found");
      env.end();

      return 1;
    }

    const char *nodeNames[] = {"s", "v1", "v2", "v3", "v4", "v5", "v6", "t"};

    cout << std::fixed << std::setprecision(2);
    cout << "Solution status: " << cplex.getStatus() << endl;

    double objectiveValue = cplex.getObjValue();
    if (std::abs(objectiveValue) < 0.0005) {
      objectiveValue = 0.0;
    }

    cout << "Objective value: " << objectiveValue << endl << endl;

    cout << "Arc flows:" << endl;
    for (int a = 0; a < arcCount; ++a) {
      double value = cplex.getValue(f[a]);
      if (value > 1e-6) {
        cout << "  - " << nodeNames[arcs[a].u] << " -> " << nodeNames[arcs[a].v]
             << ": " << value << " / " << arcs[a].cap << endl;
      }
    }

    cout << endl;
    cout << "Maximum flow from source to sink: " << objectiveValue << endl;
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
int main() { return run_exercise_06_max_flow(); }
#endif
