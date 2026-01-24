# FinanceSim Project Summary

**Last updated:** 2026-01-24

## Project Overview

A C++17 financial simulation library with Python bindings via pybind11. Uses an event-driven architecture where models communicate through an EventBus.

## Build System

- **CMake-based** with pybind11 for Python bindings
- **Build command:** `./buildSim` - configures and builds the project
- **Docs command:** `./buildDocs` - generates Doxygen documentation to `build/docs/html/`

## Directory Structure

```
├── src/
│   ├── core/               # Core framework (Event, EventBus, Model, Schedule)
│   ├── models/
│   │   ├── income/         # Income models (IncomeBase, CareerJob)
│   │   ├── expenses/       # Expense models (ExpensesBase)
│   │   ├── assets/         # Asset models (AssetsBase)
│   │   └── liabilities/    # Liability models (LiabilitiesBase)
│   └── bindings/           # pybind11 Python bindings
├── system/                 # Python module output (financesim_cpp.so)
├── tests/
│   └── cpp/                # C++ tests
├── docs/                   # Documentation sources
│   └── mainpage.md         # Doxygen main page
├── build/                  # CMake build directory (gitignored)
├── buildSim                # Build script
├── buildDocs               # Documentation build script
├── Doxyfile                # Doxygen configuration
└── CMakeLists.txt
```

## Architecture

- **EventBus:** Central pub/sub system for model communication
- **Model:** Base class with lifecycle methods (initialize, update, finalize, reset)
- **Events:** IncomeEvent, ExpenseEvent, AssetEvent, LiabilityEvent
- **Base Models:** IncomeBase, ExpensesBase, AssetsBase, LiabilitiesBase

## Git Branches

- `master` - Main branch
- `Doxygen` - Added Doxygen documentation framework

## Dependencies

- CMake 3.16+
- C++17 compiler
- Python 3 + pybind11-dev (system package)
- Doxygen + Graphviz (for docs)

## Recent Changes

1. Created `.gitignore` for build artifacts, Python cache, IDE files
2. Created `buildSim` script for easy building
3. Added Doxygen framework with CMake integration
4. Created `buildDocs` script for documentation generation
5. Added `docs/mainpage.md` - Doxygen front page with architecture overview and TOC for model descriptions
6. Enabled treeview sidebar navigation in Doxygen
7. Created MDR placeholder pages: income_models.md, expense_models.md, asset_models.md, liability_models.md
8. Added CareerJob income model (semi-monthly salary payments)
9. Reorganized models/ into subdirectories: income/, expenses/, assets/, liabilities/
