#include <algorithm>
#include <chrono>
#include <fstream>
#include <iomanip>
#include <iostream>
#include <numeric>
#include <sstream>
#include <stdexcept>
#include <string>
#include <vector>

namespace {

constexpr double kBinCapacity = 1.0;
constexpr double kTolerance = 1e-9;

struct Instance {
  std::vector<double> item_sizes;
};

struct Bin {
  std::vector<int> item_indices;
  double load = 0.0;
};

struct Solution {
  std::vector<Bin> bins;
  std::vector<int> item_to_bin;
};

Instance default_instance() {
  return {{0.5, 0.3, 0.7, 0.4, 0.6, 0.2, 0.8, 0.35}};
}

Instance read_instance(const std::string &path) {
  std::ifstream input(path);
  if (!input) {
    throw std::runtime_error("Could not open instance file: " + path);
  }

  int item_count = 0;
  input >> item_count;

  if (!input || item_count < 0) {
    throw std::runtime_error("Invalid item count in instance file.");
  }

  Instance instance;
  instance.item_sizes.resize(item_count);

  for (int i = 0; i < item_count; ++i) {
    input >> instance.item_sizes[i];

    if (!input) {
      throw std::runtime_error("Missing item size in instance file.");
    }
    if (instance.item_sizes[i] < 0.0 || instance.item_sizes[i] > 1.0) {
      throw std::runtime_error("Item sizes must stay within [0, 1].");
    }
  }

  return instance;
}

void rebuild_item_to_bin(Solution &solution, int item_count) {
  solution.item_to_bin.assign(item_count, -1);

  for (int bin_index = 0; bin_index < static_cast<int>(solution.bins.size());
       ++bin_index) {
    for (int item_index : solution.bins[bin_index].item_indices) {
      solution.item_to_bin[item_index] = bin_index;
    }
  }
}

void remove_empty_bins(Solution &solution, int item_count) {
  solution.bins.erase(
      std::remove_if(solution.bins.begin(), solution.bins.end(),
                     [](const Bin &bin) { return bin.item_indices.empty(); }),
      solution.bins.end());

  rebuild_item_to_bin(solution, item_count);
}

Solution first_fit_decreasing(const Instance &instance) {
  std::vector<int> order(instance.item_sizes.size());
  std::iota(order.begin(), order.end(), 0);

  std::sort(order.begin(), order.end(), [&](int lhs, int rhs) {
    return instance.item_sizes[lhs] > instance.item_sizes[rhs];
  });

  Solution solution;
  solution.item_to_bin.assign(instance.item_sizes.size(), -1);

  for (int item_index : order) {
    const double item_size = instance.item_sizes[item_index];
    bool placed = false;

    for (int bin_index = 0; bin_index < static_cast<int>(solution.bins.size());
         ++bin_index) {
      if (solution.bins[bin_index].load + item_size <=
          kBinCapacity + kTolerance) {
        solution.bins[bin_index].item_indices.push_back(item_index);
        solution.bins[bin_index].load += item_size;
        solution.item_to_bin[item_index] = bin_index;
        placed = true;
        break;
      }
    }

    if (!placed) {
      Bin bin;
      bin.item_indices.push_back(item_index);
      bin.load = item_size;
      solution.bins.push_back(bin);
      solution.item_to_bin[item_index] =
          static_cast<int>(solution.bins.size()) - 1;
    }
  }

  return solution;
}

bool is_feasible(const Solution &solution, const Instance &instance) {
  std::vector<bool> seen(instance.item_sizes.size(), false);

  for (const Bin &bin : solution.bins) {
    double load = 0.0;

    for (int item_index : bin.item_indices) {
      if (item_index < 0 ||
          item_index >= static_cast<int>(instance.item_sizes.size())) {
        return false;
      }
      if (seen[item_index]) {
        return false;
      }

      seen[item_index] = true;
      load += instance.item_sizes[item_index];
    }

    if (load > kBinCapacity + kTolerance) {
      return false;
    }
  }

  return std::all_of(seen.begin(), seen.end(),
                     [](bool value) { return value; });
}

std::string solution_status(const Solution &solution,
                            const Instance &instance) {
  return is_feasible(solution, instance) ? "feasible" : "infeasible";
}

bool relocate_item(Solution &solution, const Instance &instance, int source_bin,
                   int item_position, int target_bin) {
  if (source_bin == target_bin || source_bin < 0 || target_bin < 0 ||
      source_bin >= static_cast<int>(solution.bins.size()) ||
      target_bin >= static_cast<int>(solution.bins.size())) {
    return false;
  }

  const int item_index = solution.bins[source_bin].item_indices[item_position];
  const double item_size = instance.item_sizes[item_index];
  if (solution.bins[target_bin].load + item_size > kBinCapacity + kTolerance) {
    return false;
  }

  solution.bins[source_bin].item_indices.erase(
      solution.bins[source_bin].item_indices.begin() + item_position);
  solution.bins[source_bin].load -= item_size;
  solution.bins[target_bin].item_indices.push_back(item_index);
  solution.bins[target_bin].load += item_size;

  remove_empty_bins(solution, static_cast<int>(instance.item_sizes.size()));

  return true;
}

bool swap_items_between_bins(Solution &solution, const Instance &instance,
                             int first_bin, int first_pos, int second_bin,
                             int second_pos) {
  if (first_bin == second_bin || first_bin < 0 || second_bin < 0 ||
      first_bin >= static_cast<int>(solution.bins.size()) ||
      second_bin >= static_cast<int>(solution.bins.size())) {
    return false;
  }

  const int first_item = solution.bins[first_bin].item_indices[first_pos];
  const int second_item = solution.bins[second_bin].item_indices[second_pos];
  const double first_size = instance.item_sizes[first_item];
  const double second_size = instance.item_sizes[second_item];

  const double first_new_load =
      solution.bins[first_bin].load - first_size + second_size;
  const double second_new_load =
      solution.bins[second_bin].load - second_size + first_size;

  if (first_new_load > kBinCapacity + kTolerance ||
      second_new_load > kBinCapacity + kTolerance) {
    return false;
  }

  solution.bins[first_bin].item_indices[first_pos] = second_item;
  solution.bins[second_bin].item_indices[second_pos] = first_item;
  solution.bins[first_bin].load = first_new_load;
  solution.bins[second_bin].load = second_new_load;

  rebuild_item_to_bin(solution, static_cast<int>(instance.item_sizes.size()));

  return true;
}

bool try_empty_bin(Solution &solution, const Instance &instance,
                   int source_bin) {
  if (source_bin < 0 || source_bin >= static_cast<int>(solution.bins.size())) {
    return false;
  }

  Solution candidate = solution;
  std::vector<int> items = candidate.bins[source_bin].item_indices;

  std::sort(items.begin(), items.end(), [&](int lhs, int rhs) {
    return instance.item_sizes[lhs] > instance.item_sizes[rhs];
  });

  for (int item_index : items) {
    bool placed = false;

    for (int target_bin = 0;
         target_bin < static_cast<int>(candidate.bins.size()); ++target_bin) {
      if (target_bin == source_bin) {
        continue;
      }
      if (candidate.bins[target_bin].load + instance.item_sizes[item_index] <=
          kBinCapacity + kTolerance) {
        candidate.bins[target_bin].item_indices.push_back(item_index);
        candidate.bins[target_bin].load += instance.item_sizes[item_index];
        placed = true;
        break;
      }
    }

    if (!placed) {
      return false;
    }
  }

  candidate.bins[source_bin].item_indices.clear();
  candidate.bins[source_bin].load = 0.0;
  remove_empty_bins(candidate, static_cast<int>(instance.item_sizes.size()));
  solution = candidate;

  return true;
}

bool first_improving_move(Solution &solution, const Instance &instance) {
  const int original_bin_count = static_cast<int>(solution.bins.size());

  for (int source_bin = 0; source_bin < original_bin_count; ++source_bin) {
    Solution candidate = solution;
    if (try_empty_bin(candidate, instance, source_bin) &&
        static_cast<int>(candidate.bins.size()) < original_bin_count) {
      solution = candidate;
      return true;
    }
  }

  for (int source_bin = 0; source_bin < original_bin_count; ++source_bin) {
    for (int item_pos = 0;
         item_pos <
         static_cast<int>(solution.bins[source_bin].item_indices.size());
         ++item_pos) {
      for (int target_bin = 0; target_bin < original_bin_count; ++target_bin) {
        if (target_bin == source_bin) {
          continue;
        }

        Solution candidate = solution;
        if (!relocate_item(candidate, instance, source_bin, item_pos,
                           target_bin)) {
          continue;
        }

        for (int emptied_bin = 0;
             emptied_bin < static_cast<int>(candidate.bins.size());
             ++emptied_bin) {
          Solution improved = candidate;
          if (try_empty_bin(improved, instance, emptied_bin) &&
              static_cast<int>(improved.bins.size()) < original_bin_count) {
            solution = improved;
            return true;
          }
        }
      }
    }
  }

  for (int first_bin = 0; first_bin < original_bin_count; ++first_bin) {
    for (int second_bin = first_bin + 1; second_bin < original_bin_count;
         ++second_bin) {
      for (int first_pos = 0;
           first_pos <
           static_cast<int>(solution.bins[first_bin].item_indices.size());
           ++first_pos) {
        for (int second_pos = 0;
             second_pos <
             static_cast<int>(solution.bins[second_bin].item_indices.size());
             ++second_pos) {
          Solution candidate = solution;
          if (!swap_items_between_bins(candidate, instance, first_bin,
                                       first_pos, second_bin, second_pos)) {
            continue;
          }

          for (int emptied_bin = 0;
               emptied_bin < static_cast<int>(candidate.bins.size());
               ++emptied_bin) {
            Solution improved = candidate;
            if (try_empty_bin(improved, instance, emptied_bin) &&
                static_cast<int>(improved.bins.size()) < original_bin_count) {
              solution = improved;
              return true;
            }
          }
        }
      }
    }
  }

  return false;
}

struct SearchResult {
  Solution initial_solution;
  Solution final_solution;
  double elapsed_seconds = 0.0;
  int iterations = 0;
};

SearchResult local_search(const Instance &instance, double time_limit_seconds) {
  SearchResult result;
  result.initial_solution = first_fit_decreasing(instance);
  result.final_solution = result.initial_solution;

  const auto start = std::chrono::steady_clock::now();
  while (true) {
    const auto now = std::chrono::steady_clock::now();
    const double elapsed = std::chrono::duration<double>(now - start).count();

    if (elapsed >= time_limit_seconds) {
      break;
    }
    if (!first_improving_move(result.final_solution, instance)) {
      break;
    }

    ++result.iterations;
  }

  const auto end = std::chrono::steady_clock::now();
  result.elapsed_seconds = std::chrono::duration<double>(end - start).count();

  return result;
}

std::string format_bin_items(const Bin &bin, const Instance &instance) {
  std::ostringstream output;
  output << std::fixed << std::setprecision(2);

  for (std::size_t i = 0; i < bin.item_indices.size(); ++i) {
    if (i > 0) {
      output << ' ';
    }
    output << instance.item_sizes[bin.item_indices[i]];
  }

  return output.str();
}

void print_solution(const Instance &instance, const SearchResult &result,
                    double time_limit_seconds) {
  std::cout << "============================================================"
            << '\n';
  std::cout << "Metaheuristic 01 - Bin packing" << '\n';
  std::cout << "============================================================"
            << "\n\n";

  std::cout << std::fixed << std::setprecision(2);
  std::cout << "Items: " << instance.item_sizes.size() << '\n';
  std::cout << "Bin capacity: " << kBinCapacity << '\n';
  std::cout << "Time limit: " << time_limit_seconds << " seconds" << '\n';
  std::cout << "Method: First Fit Decreasing with local search" << "\n\n";

  std::cout << "Initial bins: " << result.initial_solution.bins.size() << '\n';
  std::cout << "Final bins  : " << result.final_solution.bins.size() << '\n';
  std::cout << "Iterations  : " << result.iterations << '\n';
  std::cout << "Elapsed time: " << result.elapsed_seconds << " seconds"
            << "\n\n";

  std::cout << "Bins:" << '\n';
  for (std::size_t i = 0; i < result.final_solution.bins.size(); ++i) {
    const Bin &bin = result.final_solution.bins[i];
    std::cout << "  Bin " << (i + 1) << " | load: " << bin.load
              << " | items: " << format_bin_items(bin, instance) << '\n';
  }

  std::cout << "\nSolution status: "
            << solution_status(result.final_solution, instance) << '\n';
}

} // namespace

int main(int argc, char *argv[]) {
  try {
    double time_limit_seconds = 15.0;
    std::string instance_path;

    if (argc >= 2) {
      time_limit_seconds = std::stod(argv[1]);
      if (time_limit_seconds <= 0.0) {
        throw std::runtime_error("Time limit must be positive.");
      }
    }

    if (argc >= 3) {
      instance_path = argv[2];
    }

    const Instance instance = instance_path.empty()
                                  ? default_instance()
                                  : read_instance(instance_path);
    const SearchResult result = local_search(instance, time_limit_seconds);

    print_solution(instance, result, time_limit_seconds);

    return is_feasible(result.final_solution, instance) ? 0 : 1;
  } catch (const std::exception &ex) {
    std::cerr << "Error: " << ex.what() << '\n';

    return 1;
  }
}
