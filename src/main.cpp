/**
 * @file main.cpp
 * @author Harish Rohan Kambhampaty
 * @brief Entry point for CPU burn executable
 */

#include "cpu_burn/cpu_burn.hpp"
#include <chrono>
#include <getopt.h>
#include <iostream>
#include <string>

void print_usage(const char *program_name) {
  std::cerr << "Usage: " << program_name << " [OPTIONS] [matrix_size]"
            << std::endl;
  std::cerr << "OPTIONS:" << std::endl;
  std::cerr << "  -t, --time SECONDS       Run for specified duration "
               "(default: 10 seconds)"
            << std::endl;
  std::cerr
      << "  -i, --iterations COUNT   Run for specified number of iterations"
      << std::endl;
  std::cerr << "  -h, --help               Display this help message"
            << std::endl;
}

int main(int argc, char *argv[]) {
  // Default values
  size_t matrix_size = 2000;
  std::chrono::seconds duration(10);
  size_t iterations = 0;
  bool use_iterations = false;

  // Parse command line arguments using getopt
  static struct option long_options[] = {
      {"time", required_argument, nullptr, 't'},
      {"iterations", required_argument, nullptr, 'i'},
      {"help", no_argument, nullptr, 'h'},
      {nullptr, 0, nullptr, 0}};

  int opt;
  int option_index = 0;

  while ((opt = getopt_long(argc, argv, "t:i:h", long_options,
                            &option_index)) != -1) {
    switch (opt) {
    case 't':
      try {
        duration = std::chrono::seconds(std::stoi(optarg));
      } catch (const std::exception &e) {
        std::cerr << "Invalid duration: " << optarg << std::endl;
        print_usage(argv[0]);
        return 1;
      }
      break;
    case 'i':
      try {
        iterations = std::stoull(optarg);
        use_iterations = true;
      } catch (const std::exception &e) {
        std::cerr << "Invalid iterations: " << optarg << std::endl;
        print_usage(argv[0]);
        return 1;
      }
      break;
    case 'h':
      print_usage(argv[0]);
      return 0;
    case '?':
      print_usage(argv[0]);
      return 1;
    }
  }

  // Check for matrix_size as non-option argument
  if (optind < argc) {
    try {
      matrix_size = std::stoull(argv[optind]);
    } catch (const std::exception &e) {
      std::cerr << "Invalid matrix size: " << argv[optind] << std::endl;
      print_usage(argv[0]);
      return 1;
    }
  }

  std::cout << "Starting CPU burn test..." << std::endl;
  std::cout << "Matrix size: " << matrix_size << "x" << matrix_size
            << std::endl;

  try {
    cpu_burn::CPUBurner burner(matrix_size);

    if (use_iterations) {
      std::cout << "Running " << iterations << " iterations" << std::endl;
      burner.run_iterations(iterations);

      std::cout << "Results:" << std::endl;
      std::cout << "- Execution time: " << burner.get_execution_time_ns()
                << " ns";
      std::cout << " (" << burner.get_elapsed_time() << " s)" << std::endl;
    } else {
      std::cout << "Running for " << duration.count() << " seconds"
                << std::endl;
      burner.run(duration);

      std::cout << "Results:" << std::endl;
      std::cout << "- Iterations completed: " << burner.get_iterations()
                << std::endl;
    }

    std::cout << "- Energy consumed: " << burner.get_energy_consumption()
              << " J" << std::endl;
    std::cout << "- Average power: " << burner.get_power_consumption() << " W"
              << std::endl;
    std::cout << "- Elapsed time: " << burner.get_elapsed_time() << " s"
              << std::endl;
  } catch (const std::exception &e) {
    std::cerr << "Error: " << e.what() << std::endl;
    return 1;
  }

  return 0;
}
