#include <cmath>
#include <iomanip>
#include <ilcplex/ilocplex.h>
#include <iostream>

#include "exercise_15_capacitated_vehicle_routing.hpp"

ILOSTLBEGIN

namespace {

void print_header() {
  cout << "============================================================"
       << endl;
  cout << "Exercise 15 - Capacitated vehicle routing" << endl;
  cout << "============================================================" << endl
       << endl;
  cout << "Problem summary:" << endl;
  cout << "Build vehicle routes from the depot to serve every customer within "
          "vehicle capacity."
       << endl
       << endl;
}

void print_failure(const char *message) {
  cout << "Solution status: " << message << endl;
}

} // namespace

int run_exercise_15_capacitated_vehicle_routing() {
  IloEnv env;

  try {
    print_header();

    IloModel model(env);

    typedef IloArray<IloNumVarArray> IloNumVarMatrix;
    typedef IloArray<IloNumVarMatrix> IloNumVarTensor;

    const int nodes = 6;
    const int customers = 5;
    const int vehicles = 3;

    int demand[nodes] = {0, 10, 15, 20, 10, 25};
    double dist[nodes][nodes] = {
        {0, 12, 18, 25, 14, 20}, {12, 0, 10, 22, 16, 18},
        {18, 10, 0, 15, 20, 12}, {25, 22, 15, 0, 18, 10},
        {14, 16, 20, 18, 0, 14}, {20, 18, 12, 10, 14, 0}};

    IloNumVarTensor x(env, nodes);
    for (int i = 0; i < nodes; ++i) {
      x[i] = IloNumVarMatrix(env, nodes);
      for (int j = 0; j < nodes; ++j) {
        x[i][j] = IloNumVarArray(env, vehicles);
        for (int k = 0; k < vehicles; ++k) {
          x[i][j][k] =
              IloNumVar(env, i == j ? 0.0 : 0.0, i == j ? 0.0 : 1.0, ILOBOOL);
        }
      }
    }

    IloArray<IloNumVarArray> load(env, nodes);
    for (int i = 0; i < nodes; ++i) {
      load[i] = IloNumVarArray(env, vehicles, 0.0, 50.0, ILOFLOAT);
    }

    IloExpr obj(env);
    for (int i = 0; i < nodes; ++i) {
      for (int j = 0; j < nodes; ++j) {
        if (i != j) {
          for (int k = 0; k < vehicles; ++k) {
            obj += dist[i][j] * x[i][j][k];
          }
        }
      }
    }

    model.add(IloMinimize(env, obj));
    obj.end();

    for (int j = 1; j <= customers; ++j) {
      IloExpr visit(env);

      for (int i = 0; i < nodes; ++i) {
        if (i != j) {
          for (int k = 0; k < vehicles; ++k) {
            visit += x[i][j][k];
          }
        }
      }

      model.add(visit == 1);
      visit.end();
    }

    for (int k = 0; k < vehicles; ++k) {
      model.add(load[0][k] == 0.0);

      IloExpr outDepot(env);
      IloExpr inDepot(env);

      for (int j = 1; j <= customers; ++j) {
        outDepot += x[0][j][k];
        inDepot += x[j][0][k];
      }

      model.add(outDepot == inDepot);
      model.add(outDepot <= 1);

      outDepot.end();
      inDepot.end();

      for (int h = 1; h <= customers; ++h) {
        IloExpr inflow(env);
        IloExpr outflow(env);

        for (int i = 0; i < nodes; ++i) {
          if (i != h) {
            inflow += x[i][h][k];
          }
        }
        for (int j = 0; j < nodes; ++j) {
          if (j != h) {
            outflow += x[h][j][k];
          }
        }

        model.add(inflow == outflow);
        inflow.end();
        outflow.end();

        model.add(load[h][k] >= demand[h]);
        model.add(load[h][k] <= 50.0);
      }

      for (int i = 0; i < nodes; ++i) {
        for (int j = 1; j <= customers; ++j) {
          if (i != j) {
            model.add(load[j][k] >=
                      load[i][k] + demand[j] - 50.0 * (1 - x[i][j][k]));
          }
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

    cout << "Routes:" << endl;
    for (int k = 0; k < vehicles; ++k) {
      bool used = false;
      for (int j = 1; j <= customers; ++j) {
        if (cplex.getValue(x[0][j][k]) > 0.5) {
          used = true;
        }
      }

      if (!used) {
        cout << "  Route " << (k + 1) << ": vehicle not used" << endl;
        continue;
      }

      cout << "  Route " << (k + 1) << ": depot";

      int current = 0;
      for (int step = 0; step < customers + 1; ++step) {
        bool advanced = false;

        for (int j = 0; j < nodes; ++j) {
          if (j != current && cplex.getValue(x[current][j][k]) > 0.5) {
            if (j == 0) {
              cout << " -> depot";
            } else {
              cout << " -> customer_" << j;
            }

            current = j;
            advanced = true;
            break;
          }
        }

        if (!advanced || current == 0) {
          break;
        }
      }

      if (current != 0) {
        cout << " -> depot";
      }

      cout << endl;
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
int main() { return run_exercise_15_capacitated_vehicle_routing(); }
#endif
