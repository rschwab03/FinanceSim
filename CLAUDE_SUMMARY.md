# FinanceSim Project Summary

**Last updated:** 2026-02-01

## Project Overview

A C++17 financial simulation library with Python bindings via pybind11. Uses an event-driven architecture where models communicate through an EventBus.

## Build System

- **CMake-based** with pybind11 for Python bindings
- **Build command:** `./buildSim` - configures and builds the project
- **Docs command:** `./buildDocs` - generates Doxygen documentation to `build/docs/html/`

## Directory Structure

```
├── src/
│   ├── core/               # Core framework (Event, EventBus, Model, Schedule, Logger)
│   ├── models/
│   │   ├── income/         # Income models (IncomeBase, CareerJob)
│   │   ├── expenses/       # Expense models (ExpensesBase)
│   │   ├── assets/         # Asset models (AssetsBase)
│   │   ├── liabilities/    # Liability models (LiabilitiesBase)
│   │   └── accounts/       # Account models (CheckingAccount, SavingsAccount)
│   └── bindings/           # pybind11 Python bindings
├── system/                 # Python module output (financesim_cpp.so)
├── tests/
│   └── cpp/                # C++ tests
├── scenarios/              # Simulation scenarios
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
- **Logger:** Service that subscribes to EventBus and writes events to configured writers
- **Events:** IncomeEvent, ExpenseEvent, AssetEvent, LiabilityEvent, AccountEvent, TransferEvent
- **Base Models:** IncomeBase, ExpensesBase, AssetsBase, LiabilitiesBase, AccountBase

## Logging Framework

The Logger is a service (not a Model) that subscribes to all events via EventBus:

```python
from system.financesim_cpp import Logger, ConsoleWriter, JsonWriter, LogLevel

logger = Logger()
logger.set_level(LogLevel.INFO)
logger.add_writer(ConsoleWriter())
logger.add_writer(JsonWriter("simulation.jsonl"))
logger.attach(sim.event_bus)
# ... run simulation ...
logger.flush()
logger.detach()
```

Features:
- **Log levels:** DEBUG, INFO, WARN, ERROR
- **Writers:** ConsoleWriter (stdout), JsonWriter (JSONL file)
- **Filtering:** By event type, source ID, time range

## Account Models

Accounts are reactive models that subscribe to IncomeEvent and ExpenseEvent:

```python
from system.financesim_cpp import CheckingAccount, SavingsAccount, Schedule

checking = CheckingAccount("checking1", "Primary", initial_balance=5000.0)
savings = SavingsAccount("savings1", "High-Yield", apy=0.045, initial_balance=10000.0)
```

### Schedule Configuration

Accounts accept an optional `Schedule` parameter for customizing update behavior:

```python
# Daily interest compounding instead of monthly
daily_schedule = Schedule()
daily_schedule.rate = 1.0  # Update every day

savings = SavingsAccount("savings1", "Daily Savings", apy=0.045,
                         initial_balance=10000.0, schedule=daily_schedule)
```

Default schedules:
- **CheckingAccount:** `rate=0` (event-driven only, no periodic updates)
- **SavingsAccount:** `rate=30` (monthly interest calculation)

### Routing Mechanism

Income/expense events have an optional `target_account` field:
- If empty, routes to account with `routing_tag = "default"` (CheckingAccount default)
- If specified, routes to account with matching `routing_tag`

Account types:
- **CheckingAccount:** Default income recipient, no interest
- **SavingsAccount:** Earns APY interest, compounded monthly

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
10. Added Logging Framework (Logger, LogLevel, ConsoleWriter, JsonWriter)
11. Added Account Models (AccountBase, CheckingAccount, SavingsAccount)
12. Added AccountEvent and TransferEvent types
13. Added target_account field to IncomeEvent and ExpenseEvent for routing
14. Account models use Schedule struct for configurable update rates
15. Updated scheduler to skip models with rate <= 0 (event-driven only)
16. Created `scenarios/with_accounts.py` test scenario
17. Added `docs/scheduling.md` - Explains how the scheduling system works (Schedule struct, rates, event-driven vs scheduled)
18. Added `docs/adding_a_model.md` - Beginner-friendly tutorial for creating new models with complete examples
