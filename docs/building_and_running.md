@page building_and_running Building and Running

# Building and Running FinanceSim

This guide covers system requirements, build instructions, and how to run simulations.

## System Requirements

### Required

| Requirement | Minimum Version | Notes |
|-------------|-----------------|-------|
| CMake | 3.16+ | Build system generator |
| C++ Compiler | C++17 support | GCC 7+, Clang 5+, or MSVC 2017+ |
| Python | 3.8+ | With development headers |
| pybind11 | 2.6+ | Python/C++ bindings |

### Optional

| Requirement | Purpose |
|-------------|---------|
| Doxygen | Generate this documentation |
| Graphviz | Dependency graphs in documentation |

### Installing Dependencies

**Ubuntu/Debian:**
```bash
sudo apt update
sudo apt install cmake g++ python3-dev python3-pip doxygen graphviz
pip3 install pybind11
```

**macOS (Homebrew):**
```bash
brew install cmake python pybind11 doxygen graphviz
```

**Fedora/RHEL:**
```bash
sudo dnf install cmake gcc-c++ python3-devel python3-pip doxygen graphviz
pip3 install pybind11
```

## Building the Simulation

### Quick Build

The simplest way to build is using the provided script:

```bash
./buildSim
```

This creates a `build/` directory, runs CMake configuration, and compiles the library.

### Manual Build

For more control over the build process:

```bash
# Create and enter build directory
mkdir -p build
cd build

# Configure with CMake
cmake ..

# Build (use -j for parallel compilation)
cmake --build . --parallel

# Optionally run tests
ctest --output-on-failure
```

### Build Options

| Option | Default | Description |
|--------|---------|-------------|
| `CMAKE_BUILD_TYPE` | Release | Set to `Debug` for debugging symbols |
| `BUILD_TESTS` | ON | Set to `OFF` to skip test compilation |

Example with options:
```bash
cmake .. -DCMAKE_BUILD_TYPE=Debug -DBUILD_TESTS=OFF
```

### Build Output

After a successful build, the Python module is located at:
```
system/financesim_cpp.cpython-*.so
```

This module is automatically placed in the `system/` directory for direct import.

## Building Documentation

Generate this documentation with:

```bash
./buildDocs
```

The HTML documentation is generated at `build/docs/html/index.html`.

Alternatively, using CMake directly:
```bash
cmake --build build --target docs
```

## Running Simulations

### Python API

FinanceSim is designed to be used primarily through Python:

```python
from system.simulation import Simulation
from system.financesim_cpp import CareerJob

# Create simulation
sim = Simulation()

# Add models
job = CareerJob("job1", "Software Engineer", 120000)
sim.add_model(job)

# Initialize and run for 365 days
sim.initialize()
sim.run(365)

# Access results
print(f"Final time: {sim.current_time}")
for event in sim.event_bus.event_log():
    print(f"  {event.type_name()}: {event.timestamp()}")

# Cleanup
sim.finalize()
```

### Using Scenarios

Scenarios provide pre-configured simulation setups. The easiest way to run them is with the command-line runner.

### Command-Line Runner

The `run_scenario.py` script provides a simple way to run scenarios from the terminal:

```bash
# List available scenarios
./run_scenario.py --list

# Run a scenario for 1 year (default: 365 days)
./run_scenario.py early_career

# Run with verbose output (shows all events)
./run_scenario.py early_career -v

# Run for a custom duration (e.g., 5 years)
./run_scenario.py early_career -d 1825

# Compare multiple scenarios side-by-side
./run_scenario.py early_career senior_engineer
```

**Command-line options:**

| Option | Description |
|--------|-------------|
| `--list` | List all available scenarios |
| `-d, --duration` | Simulation duration in days (default: 365) |
| `-v, --verbose` | Show detailed event log |
| `-h, --help` | Show help message |

### Scenarios via Python API

For more control, load scenarios directly in Python:

```python
from system.simulation import Simulation, compare_scenarios

# Load a single scenario
sim = Simulation.from_scenario("early_career")
sim.initialize()
sim.run(365)
sim.finalize()

# Compare multiple scenarios
results = compare_scenarios(["early_career", "senior_engineer"], duration=365)
for name, data in results.items():
    print(f"{name}: {data}")
```

### Creating Custom Scenarios

Create a Python file in the `scenarios/` directory:

```python
# scenarios/my_scenario.py
from system.financesim_cpp import CareerJob

SCENARIO_NAME = "My Custom Scenario"

def create_models():
    """Return list of models for this scenario."""
    return [
        CareerJob("job1", "My Job Title", 100000),
        # Add more models as needed
    ]
```

Then load it:
```python
sim = Simulation.from_scenario("my_scenario")
```

## Troubleshooting

### Import Errors

If you get `ModuleNotFoundError` when importing `financesim_cpp`:
1. Ensure the build completed successfully
2. Run Python from the project root directory
3. Verify the `.so` file exists in `system/`

### CMake Can't Find pybind11

Install pybind11 with pip and ensure CMake can find it:
```bash
pip3 install pybind11
# pybind11 installs CMake config files automatically
```

### Segmentation Faults

If the simulation crashes:
1. Ensure all models are added before calling `initialize()`
2. Don't call `run()` without `initialize()` first
3. Check that the C++ module was built with the same Python version you're using
