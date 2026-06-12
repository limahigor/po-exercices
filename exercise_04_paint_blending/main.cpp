#include <cmath>
#include <iomanip>
#include <ilcplex/ilocplex.h>
#include <iostream>

#include "exercise_04_paint_blending.hpp"

ILOSTLBEGIN

namespace {

void print_header() {
  cout << "============================================================"
       << endl;
  cout << "Exercise 04 - Paint blending" << endl;
  cout << "============================================================" << endl
       << endl;
  cout << "Problem summary:" << endl;
  cout << "Blend intermediate solutions and pure components to satisfy paint "
          "recipes at minimum cost."
       << endl
       << endl;
}

void print_failure(const char *message) {
  cout << "Solution status: " << message << endl;
}

} // namespace

int run_exercise_04_paint_blending() {
  IloEnv env;

  try {
    print_header();

    IloModel model(env);

    IloNumVar solA_sr(env, 0.0, IloInfinity, ILOFLOAT, "SolA_SR");
    IloNumVar solB_sr(env, 0.0, IloInfinity, ILOFLOAT, "SolB_SR");
    IloNumVar sec_sr(env, 0.0, IloInfinity, ILOFLOAT, "SEC_SR");
    IloNumVar cor_sr(env, 0.0, IloInfinity, ILOFLOAT, "COR_SR");
    IloNumVar solA_sn(env, 0.0, IloInfinity, ILOFLOAT, "SolA_SN");
    IloNumVar solB_sn(env, 0.0, IloInfinity, ILOFLOAT, "SolB_SN");
    IloNumVar sec_sn(env, 0.0, IloInfinity, ILOFLOAT, "SEC_SN");
    IloNumVar cor_sn(env, 0.0, IloInfinity, ILOFLOAT, "COR_SN");

    IloExpr obj(env);
    obj += 1.5 * solA_sr + 1.0 * solB_sr + 4.0 * sec_sr + 6.0 * cor_sr;
    obj += 1.5 * solA_sn + 1.0 * solB_sn + 4.0 * sec_sn + 6.0 * cor_sn;

    model.add(IloMinimize(env, obj));
    obj.end();

    model.add(solA_sr + solB_sr + sec_sr + cor_sr == 1000.0);
    model.add(solA_sn + solB_sn + sec_sn + cor_sn == 250.0);

    model.add(0.30 * solA_sr + 0.60 * solB_sr + sec_sr >= 250.0);
    model.add(0.70 * solA_sr + 0.40 * solB_sr + cor_sr >= 500.0);
    model.add(0.30 * solA_sn + 0.60 * solB_sn + sec_sn >= 50.0);
    model.add(0.70 * solA_sn + 0.40 * solB_sn + cor_sn >= 125.0);

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

    cout << "Purchasing plan:" << endl;
    cout << "  SR paint (1000 L):" << endl;
    cout << "    - SolA: " << cplex.getValue(solA_sr) << endl;
    cout << "    - SolB: " << cplex.getValue(solB_sr) << endl;
    cout << "    - SEC : " << cplex.getValue(sec_sr) << endl;
    cout << "    - COR : " << cplex.getValue(cor_sr) << endl << endl;

    cout << "  SN paint (250 L):" << endl;
    cout << "    - SolA: " << cplex.getValue(solA_sn) << endl;
    cout << "    - SolB: " << cplex.getValue(solB_sn) << endl;
    cout << "    - SEC : " << cplex.getValue(sec_sn) << endl;
    cout << "    - COR : " << cplex.getValue(cor_sn) << endl;
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
int main() { return run_exercise_04_paint_blending(); }
#endif
