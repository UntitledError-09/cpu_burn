/**
 * @file cpu_burn.hpp
 * @author Harish Rohan Kambhampaty
 * @brief CPU burn library for stress testing and power measurement using RAPL
 */

#ifndef CPU_BURN_HPP
#define CPU_BURN_HPP

#include <chrono>
#include <string>

namespace cpu_burn {

class CPUBurner {
public:
  CPUBurner(size_t matrix_size = 2000);
  ~CPUBurner();

  // Run the DGEMM workload for the specified duration
  void run(std::chrono::seconds duration);

  // Run the DGEMM workload for a specific number of iterations
  void run_iterations(size_t iterations);

  // Get the energy consumption in Joules (converted from microjoules)
  double get_energy_consumption() const;

  // Get the power consumption in Watts
  double get_power_consumption() const;

  // Get the elapsed time in seconds
  double get_elapsed_time() const;

  // Get the execution time in nanoseconds
  int64_t get_execution_time_ns() const;

  // Get the number of iterations performed
  size_t get_iterations() const;

private:
  // Matrix size for DGEMM
  size_t matrix_size_;

  // Matrices for DGEMM
  double *a_, *b_, *c_;

  // RAPL paths
  std::string energy_path_;
  std::string max_energy_path_;

  // Energy readings
  unsigned long long start_energy_;
  unsigned long long end_energy_;
  unsigned long long max_energy_range_;

  // Timing
  std::chrono::time_point<std::chrono::high_resolution_clock> start_time_;
  std::chrono::time_point<std::chrono::high_resolution_clock> end_time_;

  // Iteration counter
  size_t iterations_performed_;

  // Initialize matrices with random values
  void initialize_matrices();

  // Read energy value from RAPL
  unsigned long long read_energy() const;

  // Read max energy range from RAPL
  unsigned long long read_max_energy_range() const;

  // Handle energy counter overflow
  unsigned long long handle_overflow(unsigned long long start,
                                     unsigned long long end) const;

  // Perform one iteration of DGEMM
  void dgemm_iteration();
};

} // namespace cpu_burn

#endif // CPU_BURN_HPP
