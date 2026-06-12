#include <iostream>

#include "../exercise_01_feed_mix/exercise_01_feed_mix.hpp"
#include "../exercise_02_diet_problem/exercise_02_diet_problem.hpp"
#include "../exercise_03_crop_planning/exercise_03_crop_planning.hpp"
#include "../exercise_04_paint_blending/exercise_04_paint_blending.hpp"
#include "../exercise_05_transportation/exercise_05_transportation.hpp"
#include "../exercise_06_max_flow/exercise_06_max_flow.hpp"
#include "../exercise_07_shift_scheduling/exercise_07_shift_scheduling.hpp"
#include "../exercise_08_set_covering/exercise_08_set_covering.hpp"
#include "../exercise_09_binary_knapsack/exercise_09_binary_knapsack.hpp"
#include "../exercise_10_maximum_clique/exercise_10_maximum_clique.hpp"
#include "../exercise_11_cutting_patterns/exercise_11_cutting_patterns.hpp"
#include "../exercise_12_facility_location/exercise_12_facility_location.hpp"
#include "../exercise_13_graph_coloring/exercise_13_graph_coloring.hpp"
#include "../exercise_14_traveling_salesman/exercise_14_traveling_salesman.hpp"
#include "../exercise_15_capacitated_vehicle_routing/exercise_15_capacitated_vehicle_routing.hpp"
#include "../exercise_16_bin_packing/exercise_16_bin_packing.hpp"
#include "../exercise_17_shortest_path/exercise_17_shortest_path.hpp"

namespace {

struct ExerciseEntry {
  const char *name;
  int (*run)();
};

} // namespace

int main() {
  const ExerciseEntry exercises[] = {
      {"exercise_01_feed_mix", run_exercise_01_feed_mix},
      {"exercise_02_diet_problem", run_exercise_02_diet_problem},
      {"exercise_03_crop_planning", run_exercise_03_crop_planning},
      {"exercise_04_paint_blending", run_exercise_04_paint_blending},
      {"exercise_05_transportation", run_exercise_05_transportation},
      {"exercise_06_max_flow", run_exercise_06_max_flow},
      {"exercise_07_shift_scheduling", run_exercise_07_shift_scheduling},
      {"exercise_08_set_covering", run_exercise_08_set_covering},
      {"exercise_09_binary_knapsack", run_exercise_09_binary_knapsack},
      {"exercise_10_maximum_clique", run_exercise_10_maximum_clique},
      {"exercise_11_cutting_patterns", run_exercise_11_cutting_patterns},
      {"exercise_12_facility_location", run_exercise_12_facility_location},
      {"exercise_13_graph_coloring", run_exercise_13_graph_coloring},
      {"exercise_14_traveling_salesman", run_exercise_14_traveling_salesman},
      {"exercise_15_capacitated_vehicle_routing",
       run_exercise_15_capacitated_vehicle_routing},
      {"exercise_16_bin_packing", run_exercise_16_bin_packing},
      {"exercise_17_shortest_path", run_exercise_17_shortest_path},
  };

  std::cout << "Running all CPLEX examples..." << std::endl << std::endl;

  for (const ExerciseEntry &exercise : exercises) {
    if (exercise.run() != 0) {
      std::cout << std::endl
                << "Execution stopped at " << exercise.name << '.' << std::endl;

      return 1;
    }

    std::cout << std::endl;
  }

  std::cout << "All examples finished." << std::endl;

  return 0;
}
