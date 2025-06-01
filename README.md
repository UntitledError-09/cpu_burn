# CPU Burn

A C++20 application that uses OpenBLAS to create CPU load and measures power consumption using RAPL.

## Overview

CPU Burn is a tool designed to stress the CPU using matrix multiplication operations (DGEMM) via OpenBLAS. It can measure and display power consumption using Intel's RAPL (Running Average Power Limit) interface. The tool can be used both as a standalone application and as a library in other projects.

## Author

Harish Rohan Kambhampaty ([@UntitledError-09](https://github.com/UntitledError-09))

## Features

- High CPU load generation using OpenBLAS DGEMM operations
- Real-time power consumption measurements using RAPL
- Run for a specified duration or number of iterations
- Configurable matrix sizes for different load patterns
- Both executable and shared library formats

## Requirements

- C++20 compatible compiler (GCC 10+, Clang 10+)
- CMake 3.14 or higher
- OpenBLAS

> **Note:** We recommend using [Spack](https://spack.io/) for managing dependencies, but it's not required.

## Installation

### Building with CMake

```bash
mkdir build
cd build
cmake ..
make
```

### Installation

```bash
sudo make install
```

## Usage

```bash
cpu_burn [OPTIONS] [matrix_size]
```

### Options

- `-t, --time SECONDS`: Run for specified duration (default: 10 seconds)
- `-i, --iterations COUNT`: Run for specified number of iterations
- `-h, --help`: Display help message

### Examples

Run for 30 seconds with default matrix size:
```bash
cpu_burn -t 30
```

Run 100 iterations with 1000x1000 matrices:
```bash
cpu_burn -i 100 1000
```

## Using as a Library

Include the header in your C++ code:

```cpp
#include <cpu_burn/cpu_burn.hpp>

int main() {
    cpu_burn::CPUBurner burner(1000);  // Matrix size 1000x1000
    
    // Run for 5 seconds
    burner.run(std::chrono::seconds(5));
    
    // Or run for a specific number of iterations
    // burner.run_iterations(100);
    
    std::cout << "Energy consumed: " << burner.get_energy_consumption() << " J\n";
    std::cout << "Average power: " << burner.get_power_consumption() << " W\n";
    std::cout << "Execution time: " << burner.get_execution_time_ns() << " ns\n";
    std::cout << "Iterations performed: " << burner.get_iterations() << "\n";
    
    return 0;
}
```

Link against the library:

```bash
g++ -std=c++20 your_program.cpp -o your_program -lcpu_burn
```

## License

This project is licensed under the MIT License - see the [LICENSE](LICENSE) file for details.

## Contributing

Contributions are welcome! Please see [CONTRIBUTING.md](CONTRIBUTING.md) for details.

## Repository

https://github.com/UntitledError-09/cpu_burn
