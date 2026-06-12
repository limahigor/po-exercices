#include <cmath>
#include <iomanip>
#include <ilcplex/ilocplex.h>
#include <iostream>

#include "exercise_17_shortest_path.hpp"

ILOSTLBEGIN

namespace {

void print_header() {
  cout << "============================================================"
       << endl;
  cout << "Exercise 17 - Shortest path" << endl;
  cout << "============================================================" << endl
       << endl;
  cout << "Problem summary:" << endl;
  cout << "Find the minimum-cost path from node a to node g." << endl << endl;
}

void print_failure(const char *message) {
  cout << "Solution status: " << message << endl;
}

} // namespace

int run_exercise_17_shortest_path() {
  IloEnv env;

  try {
    print_header();

    IloModel model(env);

    struct Arc {
      int u;
      int v;
      double w;
    };

    Arc arcs[] = {{0, 1, 7},  {1, 0, 7}, {0, 3, 5},  {3, 0, 5},  {1, 2, 8},
                  {2, 1, 8},  {1, 3, 9}, {3, 1, 9},  {1, 4, 7},  {4, 1, 7},
                  {2, 4, 5},  {4, 2, 5}, {3, 4, 15}, {4, 3, 15}, {3, 5, 6},
                  {5, 3, 6},  {4, 5, 8}, {5, 4, 8},  {4, 6, 9},  {6, 4, 9},
                  {5, 6, 11}, {6, 5, 11}};
    const int arcCount = sizeof(arcs) / sizeof(arcs[0]);
    const char *names[7] = {"a", "b", "c", "d", "e", "f", "g"};

    IloNumVarArray x(env, arcCount, 0.0, 1.0, ILOBOOL);

    IloExpr obj(env);
    for (int a = 0; a < arcCount; ++a) {
      obj += arcs[a].w * x[a];
    }

    model.add(IloMinimize(env, obj));
    obj.end();

    for (int node = 0; node < 7; ++node) {
      IloExpr out(env);
      IloExpr in(env);

      for (int a = 0; a < arcCount; ++a) {
        if (arcs[a].u == node) {
          out += x[a];
        }
        if (arcs[a].v == node) {
          in += x[a];
        }
      }

      if (node == 0) {
        model.add(out - in == 1);
      } else if (node == 6) {
        model.add(in - out == 1);
      } else {
        model.add(out == in);
      }

      out.end();
      in.end();
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

    cout << "Path:" << endl;
    cout << "  " << names[0];

    int current = 0;
    for (int step = 0; step < 7; ++step) {
      bool advanced = false;

      for (int a = 0; a < arcCount; ++a) {
        if (arcs[a].u == current && cplex.getValue(x[a]) > 0.5) {
          current = arcs[a].v;
          cout << " -> " << names[current];
          advanced = true;
          break;
        }
      }

      if (!advanced || current == 6) {
        break;
      }
    }

    cout << endl;
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
int main() { return run_exercise_17_shortest_path(); }
#endif
