/**
 * @file cpu_burn.cpp
 * @author Harish Rohan Kambhampaty
 * @brief Implementation of CPU burn library
 */

#include "cpu_burn/cpu_burn.hpp"
#include <cblas.h>
#include <fstream>
#include <iostream>
#include <random>
#include <stdexcept>

namespace cpu_burn {

CPUBurner::CPUBurner(size_t matrix_size)
    : matrix_size_(matrix_size), a_(nullptr), b_(nullptr), c_(nullptr),
      energy_path_("/sys/class/powercap/intel-rapl/intel-rapl:0/energy_uj"),
      max_energy_path_(
          "/sys/class/powercap/intel-rapl/intel-rapl:0/max_energy_range_uj"),
      start_energy_(0), end_energy_(0), max_energy_range_(0),
      iterations_performed_(0) {
  // Allocate memory for matrices
  a_ = new double[matrix_size_ * matrix_size_];
  b_ = new double[matrix_size_ * matrix_size_];
  c_ = new double[matrix_size_ * matrix_size_];

  // Initialize matrices with random values
  initialize_matrices();

  // Read the max energy range
  max_energy_range_ = read_max_energy_range();
}

CPUBurner::~CPUBurner() {
  delete[] a_;
  delete[] b_;
  delete[] c_;
}

void CPUBurner::initialize_matrices() {
  std::random_device rd;
  std::mt19937 gen(rd());
  std::uniform_real_distribution<double> dist(0.0, 1.0);

  for (size_t i = 0; i < matrix_size_ * matrix_size_; ++i) {
    a_[i] = dist(gen);
    b_[i] = dist(gen);
    c_[i] = 0.0;
  }
}

unsigned long long CPUBurner::read_energy() const {
  std::ifstream energy_file(energy_path_);
  if (!energy_file) {
    throw std::runtime_error("Failed to open RAPL energy file: " +
                             energy_path_);
  }

  unsigned long long energy_uj;
  energy_file >> energy_uj;
  return energy_uj;
}

unsigned long long CPUBurner::read_max_energy_range() const {
  std::ifstream max_energy_file(max_energy_path_);
  if (!max_energy_file) {
    throw std::runtime_error("Failed to open RAPL max energy file: " +
                             max_energy_path_);
  }

  unsigned long long max_energy_uj;
  max_energy_file >> max_energy_uj;
  return max_energy_uj;
}

unsigned long long CPUBurner::handle_overflow(unsigned long long start,
                                              unsigned long long end) const {
  if (end >= start) {
    return end - start;
  } else {
    // Counter has overflowed
    return (max_energy_range_ - start) + end;
  }
}

void CPUBurner::dgemm_iteration() {
  // Perform DGEMM: C = alpha*A*B + beta*C
  // Where A, B, C are matrices, alpha and beta are scalars
  const double alpha = 1.0;
  const double beta = 0.0;

  cblas_dgemm(CblasRowMajor, CblasNoTrans, CblasNoTrans, matrix_size_,
              matrix_size_, matrix_size_, alpha, a_, matrix_size_, b_,
              matrix_size_, beta, c_, matrix_size_);
}

void CPUBurner::run(std::chrono::seconds duration) {
  iterations_performed_ = 0;
  start_time_ = std::chrono::high_resolution_clock::now();
  start_energy_ = read_energy();

  auto end_time = start_time_ + duration;

  while (std::chrono::high_resolution_clock::now() < end_time) {
    dgemm_iteration();
    iterations_performed_++;
  }

  end_time_ = std::chrono::high_resolution_clock::now();
  end_energy_ = read_energy();

  std::cout << "Run completed. Energy consumption: " << get_energy_consumption()
            << " J, Power: " << get_power_consumption() << " W" << std::endl;
}

void CPUBurner::run_iterations(size_t iterations) {
  start_time_ = std::chrono::high_resolution_clock::now();
  start_energy_ = read_energy();

  iterations_performed_ = iterations;
  for (size_t i = 0; i < iterations; ++i) {
    dgemm_iteration();
  }

  end_time_ = std::chrono::high_resolution_clock::now();
  end_energy_ = read_energy();

  std::cout << "Run completed. Energy consumption: " << get_energy_consumption()
            << " J, Power: " << get_power_consumption() << " W" << std::endl;
}

double CPUBurner::get_energy_consumption() const {
  unsigned long long energy_uj = handle_overflow(start_energy_, end_energy_);
  return static_cast<double>(energy_uj) / 1000000.0; // Convert µJ to J
}

double CPUBurner::get_power_consumption() const {
  double energy_j = get_energy_consumption();
  double time_s = get_elapsed_time();
  return energy_j / time_s;
}

double CPUBurner::get_elapsed_time() const {
  std::chrono::duration<double> elapsed = end_time_ - start_time_;
  return elapsed.count();
}

int64_t CPUBurner::get_execution_time_ns() const {
  return std::chrono::duration_cast<std::chrono::nanoseconds>(end_time_ -
                                                              start_time_)
      .count();
}

size_t CPUBurner::get_iterations() const { return iterations_performed_; }

} // namespace cpu_burn
