# FinanceSim {#mainpage}

A modular, event-driven financial simulation framework for modeling personal and institutional finances over time.

## Overview

FinanceSim provides a flexible architecture for building financial simulations where independent models communicate through a central event bus. This design enables:

- **Modularity:** Add, remove, or swap financial models without changing other components
- **Determinism:** Simulations are fully reproducible given the same inputs
- **Observability:** All model interactions are logged as events for analysis and debugging
- **Extensibility:** Create custom models by implementing the Model interface

## Architecture

The framework is built around three core concepts:

### EventBus
The central nervous system of the simulation. All communication between models flows through the EventBus using a publish/subscribe pattern. Models never directly reference each other - they publish events and subscribe to events they care about.

### Models
Self-contained units of financial logic. Each model:
- Has a unique ID and schedule
- Initializes by subscribing to relevant events
- Updates at scheduled intervals
- Emits events to communicate state changes

### Events
Immutable messages that carry information between models. Events are timestamped and logged, creating a complete audit trail of the simulation.

## Table of Contents

### Getting Started
- @subpage building_and_running "Building and Running"
- @subpage adding_a_model "Adding a New Model" - Step-by-step tutorial for creating custom models
- @subpage scheduling "How Scheduling Works" - Understanding model execution timing

### Core Framework
- @ref financesim::EventBus "EventBus" - Central pub/sub message system
- @ref financesim::Model "Model" - Base interface for all models
- @ref financesim::Event "Event" - Base class for all events
- @ref financesim::Schedule "Schedule" - Model execution timing

### Financial Models

#### Income Models
- @ref financesim::IncomeBase "IncomeBase" - Base class for income sources
- @subpage income_models "Income Model Descriptions"

#### Expense Models
- @ref financesim::ExpensesBase "ExpensesBase" - Base class for expense tracking
- @subpage expense_models "Expense Model Descriptions"

#### Asset Models
- @ref financesim::AssetsBase "AssetsBase" - Base class for asset management
- @subpage asset_models "Asset Model Descriptions"

#### Liability Models
- @ref financesim::LiabilitiesBase "LiabilitiesBase" - Base class for debt/liability tracking
- @subpage liability_models "Liability Model Descriptions"

### Events Reference
- @ref financesim::IncomeEvent "IncomeEvent" - Income received
- @ref financesim::ExpenseEvent "ExpenseEvent" - Expense incurred
- @ref financesim::AssetEvent "AssetEvent" - Asset value change
- @ref financesim::LiabilityEvent "LiabilityEvent" - Liability value change

## Quick Start

```cpp
#include "core/event_bus.hpp"
#include "models/income_base.hpp"
#include "models/expenses_base.hpp"

// Create the event bus
financesim::EventBus bus;

// Create and initialize models
auto income = std::make_shared<MyIncomeModel>();
auto expenses = std::make_shared<MyExpenseModel>();

income->initialize(bus);
expenses->initialize(bus);

// Run simulation loop
for (SimTime t = 0; t < end_time; ++t) {
    income->update(t);
    expenses->update(t);
}

// Analyze results via event log
for (const auto& event : bus.event_log()) {
    // Process events...
}
```

## Python Bindings

FinanceSim includes Python bindings via pybind11, allowing you to use the framework from Python:

```python
import financesim_cpp as fs

bus = fs.EventBus()
# ... configure and run simulation
```

## Building

```bash
./buildSim      # Build the library
./buildDocs     # Generate this documentation
```
