#include <cmath>
#include <iomanip>
#include <ilcplex/ilocplex.h>
#include <iostream>

#include "exercise_03_crop_planning.hpp"

ILOSTLBEGIN

namespace {

void print_header() {
  cout << "============================================================"
       << endl;
  cout << "Exercise 03 - Crop planning" << endl;
  cout << "============================================================" << endl
       << endl;
  cout << "Problem summary:" << endl;
  cout << "Distribute crop areas across farms to maximize profit while "
          "respecting land, water, and crop limits."
       << endl
       << endl;
}

void print_failure(const char *message) {
  cout << "Solution status: " << message << endl;
}

} // namespace

int run_exercise_03_crop_planning() {
  IloEnv env;

  try {
    print_header();

    IloModel model(env);

    const int farms = 3;
    const int crops = 3;

    double areaFarm[farms] = {400, 650, 350};
    double waterFarm[farms] = {1800, 2200, 950};
    double areaMax[crops] = {660, 880, 400};
    double waterPerArea[crops] = {5.5, 4.0, 3.5};
    double profit[crops] = {5000, 4000, 1800};
    const char *cropNames[crops] = {"Corn", "Rice", "Beans"};

    IloArray<IloNumVarArray> x(env, farms);
    for (int i = 0; i < farms; ++i) {
      x[i] = IloNumVarArray(env, crops, 0.0, IloInfinity, ILOFLOAT);
    }

    IloExpr obj(env);
    for (int i = 0; i < farms; ++i) {
      for (int j = 0; j < crops; ++j) {
        obj += profit[j] * x[i][j];
      }
    }

    model.add(IloMaximize(env, obj));
    obj.end();

    for (int i = 0; i < farms; ++i) {
      IloExpr areaExpr(env);
      IloExpr waterExpr(env);

      for (int j = 0; j < crops; ++j) {
        areaExpr += x[i][j];
        waterExpr += waterPerArea[j] * x[i][j];
      }

      model.add(areaExpr <= areaFarm[i]);
      model.add(waterExpr <= waterFarm[i]);

      areaExpr.end();
      waterExpr.end();
    }

    for (int j = 0; j < crops; ++j) {
      IloExpr cropExpr(env);

      for (int i = 0; i < farms; ++i) {
        cropExpr += x[i][j];
      }

      model.add(cropExpr <= areaMax[j]);
      cropExpr.end();
    }

    for (int i = 1; i < farms; ++i) {
      IloExpr lhs(env);
      IloExpr rhs(env);

      for (int j = 0; j < crops; ++j) {
        lhs += x[i][j];
        rhs += x[0][j];
      }

      model.add(lhs * areaFarm[0] == rhs * areaFarm[i]);

      lhs.end();
      rhs.end();
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

    cout << "Planting plan:" << endl;
    for (int i = 0; i < farms; ++i) {
      cout << "  Farm " << (i + 1) << ':' << endl;

      double usedArea = 0.0;
      double usedWater = 0.0;

      for (int j = 0; j < crops; ++j) {
        double value = cplex.getValue(x[i][j]);
        usedArea += value;
        usedWater += waterPerArea[j] * value;

        cout << "    - " << cropNames[j] << ": " << value << " acres" << endl;
      }

      cout << "    Area used : " << usedArea << " / " << areaFarm[i] << endl;
      cout << "    Water used: " << usedWater << " / " << waterFarm[i] << endl;
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
int main() { return run_exercise_03_crop_planning(); }
#endif
