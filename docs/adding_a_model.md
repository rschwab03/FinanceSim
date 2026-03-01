@page adding_a_model Adding a New Model

# Adding a New Model to FinanceSim

This guide walks you through creating a new model from scratch. By the end, you'll understand how all the pieces fit together and be able to create your own custom financial models.

## Prerequisites

Before starting, make sure you can:
- Build the project with `./buildSim`
- Run a basic simulation (see @subpage building_and_running "Building and Running")

## Understanding the Architecture

Before we write code, let's understand how FinanceSim works at a high level.

### The Event-Driven Design

FinanceSim uses an **event-driven architecture**. This means:

1. **Models don't talk to each other directly.** Instead, they communicate through messages called **events**.

2. **The EventBus is the mailroom.** When a model wants to tell others something happened (like "I received income"), it publishes an event to the EventBus. Other models that care about that event will receive it.

3. **Models are independent.** You can add, remove, or swap models without changing other models. They only know about events, not about each other.

### The Model Lifecycle

Every model goes through four phases:

```
┌─────────────┐     ┌─────────────┐     ┌─────────────┐     ┌─────────────┐
│ CONSTRUCT   │ --> │ INITIALIZE  │ --> │   UPDATE    │ --> │  FINALIZE   │
│             │     │             │     │  (repeated) │     │             │
│ Create the  │     │ Subscribe   │     │ Do your     │     │ Clean up    │
│ object      │     │ to events   │     │ logic       │     │ resources   │
└─────────────┘     └─────────────┘     └─────────────┘     └─────────────┘
```

1. **Construct**: The model object is created with its parameters (ID, name, etc.)
2. **Initialize**: Called once before the simulation runs. Subscribe to events here.
3. **Update**: Called repeatedly during the simulation according to the model's schedule.
4. **Finalize**: Called once when the simulation ends. Clean up any resources.

There's also a **reset()** method that returns the model to its initial state (for replaying simulations).

## Deciding What Kind of Model to Create

FinanceSim provides several **base classes** you can inherit from:

| Base Class | Use When You're Modeling... | Example |
|------------|----------------------------|---------|
| `IncomeBase` | Money coming in | Salary, dividends, rental income |
| `ExpensesBase` | Money going out | Bills, subscriptions, purchases |
| `AssetsBase` | Things you own | Stocks, real estate, vehicles |
| `LiabilitiesBase` | Money you owe | Mortgages, loans, credit cards |
| `AccountBase` | Where money is stored | Checking, savings accounts |

Each base class provides common functionality so you don't have to rewrite boilerplate code.

**Which should you choose?** Think about what your model represents:
- Does it generate money? --> `IncomeBase`
- Does it spend money? --> `ExpensesBase`
- Does it hold value? --> `AssetsBase`
- Is it a debt? --> `LiabilitiesBase`
- Does it store money and respond to income/expenses? --> `AccountBase`

## Step-by-Step: Creating an Income Model

Let's create a **rental income** model as an example. This model will:
- Generate monthly rent payments
- Start on a specific day (when the lease begins)

### Step 1: Create the Header File

Create a new file: `src/models/income/rental_income.hpp`

```cpp
#pragma once

#include "models/income/income_base.hpp"

namespace financesim {

/// @brief Rental income model for property investments
///
/// Generates monthly rent payments starting from a specified lease date.
/// Emits IncomeEvent with category "rent" on each payment date.
class RentalIncome : public IncomeBase {
public:
    /// @brief Create a rental income model
    /// @param id Unique identifier (e.g., "rental1")
    /// @param name Human-readable name (e.g., "123 Main St Apartment")
    /// @param monthly_rent Monthly rent amount in dollars
    /// @param lease_start_day Day when the lease begins (default: 0)
    RentalIncome(std::string id, std::string name,
                 double monthly_rent, SimTime lease_start_day = 0.0);

    /// @brief Get the monthly rent amount
    double monthly_rent() const { return monthly_rent_; }

    // Override the methods we need to customize
    void update(SimTime time) override;
    void reset() override;

private:
    double monthly_rent_;
};

} // namespace financesim
```

**What's happening here:**

- We include `income_base.hpp` to get the `IncomeBase` class
- We declare our class inside the `financesim` namespace (all FinanceSim code uses this)
- We inherit from `IncomeBase` with `public` inheritance
- The constructor takes the required parameters
- We provide a getter for `monthly_rent_` (optional but useful) - see note below
- We override `update()` because that's where our logic goes
- We override `reset()` to properly reset any custom state

#### A Note on Getters and Naming Conventions

Look at these two lines in the code above:

```cpp
private:
    double monthly_rent_;    // The actual variable (private)

public:
    double monthly_rent() const { return monthly_rent_; }  // The getter method (public)
```

This might look confusing at first. Here's what's happening:

**The private variable `monthly_rent_`** (with a trailing underscore) stores the actual data. It's `private`, meaning code outside the class cannot access it directly. The underscore at the end is a common C++ convention to mark private member variables.

**The public method `monthly_rent()`** (without underscore) is a "getter" - a method that returns the value of the private variable. This allows outside code to *read* the value without being able to *change* it.

```cpp
// From outside the class:
RentalIncome rental("r1", "Apt", 1500.0);
double rent = rental.monthly_rent();  // OK - calls the getter, returns 1500.0
rental.monthly_rent_ = 2000.0;        // ERROR - can't access private variable
```

**Why not call it `get_monthly_rent()`?**

You might expect getter methods to be named `get_something()` (like in Java). Both conventions exist:

| Convention | Getter | Setter | Used By |
|------------|--------|--------|---------|
| Java-style | `getMonthlyRent()` | `setMonthlyRent(value)` | Java, C# |
| C++-style | `monthly_rent()` | `set_monthly_rent(value)` | C++, FinanceSim |

FinanceSim uses the C++ style throughout. You'll see this pattern in existing models:
- `CareerJob::annual_salary()` - not `get_annual_salary()`
- `SavingsAccount::apy()` - not `get_apy()`
- `AccountBase::balance()` - not `get_balance()`

Neither convention is "wrong" - just be consistent with the project you're working in.

### Step 2: Create the Implementation File

Create: `src/models/income/rental_income.cpp`

```cpp
#include "models/income/rental_income.hpp"
#include "core/event.hpp"

namespace financesim {

// Monthly = approximately 30 days
namespace {
    constexpr double MONTHLY_DAYS = 30.0;
}

RentalIncome::RentalIncome(std::string id, std::string name,
                           double monthly_rent, SimTime lease_start_day)
    : IncomeBase(
          std::move(id),
          std::move(name),
          Schedule{lease_start_day, -1.0, MONTHLY_DAYS, ExecutionTiming::StartOfPeriod}
      )
    , monthly_rent_(monthly_rent)
{
    // Constructor body - additional initialization if needed
}

void RentalIncome::update(SimTime time) {
    // This is called every 30 days by the scheduler.
    // Emit an income event for the rent payment.
    emit<IncomeEvent>(time, monthly_rent_, "rent");
}

void RentalIncome::reset() {
    // Call base class reset first
    IncomeBase::reset();
    // Reset any custom state here (we don't have any in this simple example)
}

} // namespace financesim
```

**What's happening here:**

1. **Constructor initialization list**: We pass parameters to `IncomeBase`:
   - `id` and `name` are passed through
   - We create a `Schedule` struct inline:
     - `start_time = lease_start_day` (when rent starts)
     - `stop_time = -1.0` (runs forever)
     - `rate = 30.0` (every 30 days)
     - `timing = StartOfPeriod` (rent due at start of month)

2. **update()**: This is called by the scheduler every 30 days. We use `emit<IncomeEvent>()` to publish an event. The `emit` function is provided by `IncomeBase` and:
   - Creates the event with the current time and our model's ID
   - Publishes it to the EventBus
   - Any model subscribed to `IncomeEvent` will receive it

3. **reset()**: Always call the base class `reset()` first, then reset your own state.

### Step 3: Add to CMakeLists.txt

Open `src/CMakeLists.txt` and add your source file to `financesim_models`:

```cmake
add_library(financesim_models STATIC
    core/log_writer.cpp
    core/logger.cpp
    models/income/income_base.cpp
    models/income/career_job.cpp
    models/income/rental_income.cpp    # <-- Add this line
    models/expenses/expenses_base.cpp
    # ... rest of the files
)
```

### Step 4: Add Python Bindings

FinanceSim models are written in C++ for performance, but we use them from Python for convenience. **pybind11** is a library that creates a bridge between the two languages. The "bindings" file tells pybind11 how to expose our C++ classes to Python.

Open `src/bindings/bindings.cpp` and add bindings for your model.

#### Step 4a: Add the Include

At the top of the file, add an include for your header:
```cpp
#include "models/income/rental_income.hpp"
```

This tells the C++ compiler where to find your class definition.

#### Step 4b: Add the Class Binding

Find the `PYBIND11_MODULE` block (this is where all bindings are defined) and add your binding after the other income models:

```cpp
py::class_<RentalIncome, IncomeBase, std::shared_ptr<RentalIncome>>(m, "RentalIncome")
    .def(py::init<std::string, std::string, double, SimTime>(),
         py::arg("id"), py::arg("name"), py::arg("monthly_rent"),
         py::arg("lease_start_day") = 0.0)
    .def("monthly_rent", &RentalIncome::monthly_rent);
```

This looks complicated! Let's break it down piece by piece.

#### Understanding the Binding Code

**Line 1: Declaring the class to Python**
```cpp
py::class_<RentalIncome, IncomeBase, std::shared_ptr<RentalIncome>>(m, "RentalIncome")
```

| Part | Meaning |
|------|---------|
| `py::class_<...>` | "I want to expose a C++ class to Python" |
| `RentalIncome` | The C++ class we're exposing |
| `IncomeBase` | The parent class (so Python knows about inheritance) |
| `std::shared_ptr<RentalIncome>` | How Python should manage the object's memory |
| `(m, "RentalIncome")` | `m` is the Python module; `"RentalIncome"` is the name Python will use |

After this line, Python will know there's a class called `RentalIncome`.

**Lines 2-4: Defining the constructor**
```cpp
    .def(py::init<std::string, std::string, double, SimTime>(),
         py::arg("id"), py::arg("name"), py::arg("monthly_rent"),
         py::arg("lease_start_day") = 0.0)
```

| Part | Meaning |
|------|---------|
| `.def(...)` | "I want to define something on this class" |
| `py::init<...>` | "This is a constructor" (how to create new objects) |
| `std::string, std::string, double, SimTime` | The C++ types of each parameter, in order |
| `py::arg("id")` | "Call the first parameter `id` in Python" |
| `py::arg("name")` | "Call the second parameter `name` in Python" |
| `py::arg("monthly_rent")` | "Call the third parameter `monthly_rent` in Python" |
| `py::arg("lease_start_day") = 0.0` | "The fourth parameter is named `lease_start_day` and defaults to 0.0" |

This lets Python users create objects like:
```python
# Using positional arguments
rental = RentalIncome("r1", "My Apartment", 1500.0, 30.0)

# Using keyword arguments (clearer!)
rental = RentalIncome(id="r1", name="My Apartment", monthly_rent=1500.0)

# Mixing (positional first, then keyword)
rental = RentalIncome("r1", "My Apartment", monthly_rent=1500.0, lease_start_day=30.0)
```

**Line 5: Exposing a method**
```cpp
    .def("monthly_rent", &RentalIncome::monthly_rent);
```

This line exposes our getter method to Python. Let's understand the two parts:

| Part | What It Is | Purpose |
|------|------------|---------|
| `"monthly_rent"` | A string | The name Python will use to call this method |
| `&RentalIncome::monthly_rent` | A pointer to the C++ method | Tells pybind11 which C++ function to call |

**What's the difference between `"monthly_rent"` and `&RentalIncome::monthly_rent`?**

This is a key concept:

- **`"monthly_rent"`** is just text - a string that becomes the method name in Python. You could change it to `"get_rent"` and Python users would call `rental.get_rent()` instead.

- **`&RentalIncome::monthly_rent`** is a *pointer to a function*. The `&` means "address of" and `RentalIncome::monthly_rent` specifies which method (the `monthly_rent` method in the `RentalIncome` class). This tells pybind11: "when Python calls this method, run THIS C++ code."

Think of it like a phone directory:
- `"monthly_rent"` is the name you look up ("John Smith")
- `&RentalIncome::monthly_rent` is the actual phone number to call

You could even expose the same C++ method under multiple Python names:
```cpp
.def("monthly_rent", &RentalIncome::monthly_rent)      // rental.monthly_rent()
.def("get_monthly_rent", &RentalIncome::monthly_rent)  // rental.get_monthly_rent() - same function!
.def("rent", &RentalIncome::monthly_rent);             // rental.rent() - also same function!
```

#### The Complete Binding Explained Visually

```
C++ Side                                    Python Side
─────────────────────────────────────────────────────────────────────

class RentalIncome                          class RentalIncome:

    RentalIncome(id, name, rent, start)  →      def __init__(self, id, name,
         │                                                    monthly_rent,
         │                                                    lease_start_day=0.0)
         └── py::init<...> connects these

    double monthly_rent()                →      def monthly_rent(self):
         │                                          return <value>
         └── .def("monthly_rent", &...) connects these
```

#### Why Do We Need Bindings?

You might wonder: why can't Python just use C++ classes directly?

C++ and Python are very different languages:
- C++ compiles to machine code, Python is interpreted
- C++ requires you to declare types, Python figures them out
- C++ manages memory manually, Python has garbage collection

pybind11 handles all these differences. It:
1. Translates Python calls into C++ calls
2. Converts Python types (like `str`) to C++ types (like `std::string`)
3. Manages memory so objects don't get deleted while Python is using them
4. Translates C++ exceptions into Python exceptions

The binding code you write is the "instruction manual" for pybind11 to do this translation correctly.

### Step 5: Build and Test

Build the project:
```bash
./buildSim
```

Test your model in Python:
```python
from system.financesim_cpp import RentalIncome
from system.simulation import Simulation

# Create simulation
sim = Simulation()

# Add your rental income model
rental = RentalIncome("rental1", "Downtown Apartment", monthly_rent=1500.0)
sim.add_model(rental)

# Run for one year
sim.initialize()
sim.run(365)

# Check the events
for event in sim.event_bus.event_log():
    print(f"Day {event.timestamp():.0f}: {event.type_name()} - {event.source_id()}")

sim.finalize()
```

You should see approximately 12 `IncomeEvent` entries (one per month).

## Understanding the Schedule

The Schedule you pass to the base class controls when `update()` is called. See @ref scheduling "How Scheduling Works" for a detailed explanation.

Quick reference:
- `rate = 1.0` --> daily
- `rate = 7.0` --> weekly
- `rate = 30.0` --> monthly
- `rate = 365.0 / 24.0` (~15.2) --> semi-monthly (like paychecks)
- `rate = 0.0` --> never scheduled (event-driven only)

## Creating an Event-Driven Model

Some models don't run on a schedule - they only respond to events. Let's look at how `AccountBase` does this.

### Subscribing to Events

In your `initialize()` method, subscribe to the events you care about:

```cpp
void MyModel::initialize(EventBus& bus) {
    // IMPORTANT: Call base class initialize first!
    BaseClass::initialize(bus);

    // Subscribe to IncomeEvent
    income_subscription_ = bus.subscribe<IncomeEvent>(
        [this](const IncomeEvent& event) {
            // This lambda is called whenever an IncomeEvent is published
            handle_income(event);
        }
    );
}
```

**Key points:**
- Save the subscription ID (returned by `subscribe()`) so you can unsubscribe later
- The callback (lambda) captures `this` so you can access your model's members
- The event is passed by `const` reference - you cannot modify it

### Unsubscribing in reset()

Clean up your subscriptions in `reset()`:

```cpp
void MyModel::reset() {
    // Unsubscribe from events
    if (bus_) {
        bus_->unsubscribe(income_subscription_);
    }

    // Reset state
    income_subscription_ = 0;

    // Call base class reset
    BaseClass::reset();
}
```

### Full Example: Event-Driven Model

Here's a sketch of a model that tracks total income received:

```cpp
// income_tracker.hpp
class IncomeTracker : public Model {
public:
    IncomeTracker(std::string id, std::string name);

    const std::string& id() const override { return id_; }
    const std::string& name() const override { return name_; }
    const Schedule& schedule() const override { return schedule_; }

    void initialize(EventBus& bus) override;
    void update(SimTime time) override;
    void finalize() override;
    void reset() override;

    double total_income() const { return total_income_; }

private:
    std::string id_;
    std::string name_;
    Schedule schedule_;  // Will have rate=0 for event-driven
    EventBus* bus_ = nullptr;
    SubscriptionId sub_id_ = 0;
    double total_income_ = 0.0;
};

// income_tracker.cpp
IncomeTracker::IncomeTracker(std::string id, std::string name)
    : id_(std::move(id))
    , name_(std::move(name))
{
    schedule_.rate = 0.0;  // Event-driven only, no scheduled updates
}

void IncomeTracker::initialize(EventBus& bus) {
    bus_ = &bus;
    sub_id_ = bus.subscribe<IncomeEvent>(
        [this](const IncomeEvent& event) {
            total_income_ += event.amount();
        }
    );
}

void IncomeTracker::update(SimTime /*time*/) {
    // Nothing to do - we're event-driven
}

void IncomeTracker::finalize() {
    // Nothing to clean up
}

void IncomeTracker::reset() {
    if (bus_) {
        bus_->unsubscribe(sub_id_);
    }
    bus_ = nullptr;
    sub_id_ = 0;
    total_income_ = 0.0;
}
```

## Publishing Events

To tell other models that something happened, publish an event.

### Using the emit() Helper

If you inherit from a base class like `IncomeBase`, you get a convenient `emit()` method:

```cpp
void MyIncomeModel::update(SimTime time) {
    // emit<EventType>(time, ...constructor args...)
    emit<IncomeEvent>(time, 500.0, "bonus");
}
```

The `emit()` method:
1. Creates the event with the timestamp and your model's ID as the source
2. Publishes it to the EventBus
3. Handles the case where no bus is attached (just skips)

### Publishing Directly

If you don't have the helper, publish directly:

```cpp
void MyModel::update(SimTime time) {
    auto event = std::make_shared<IncomeEvent>(
        time,           // timestamp
        id_,            // source_id (your model's ID)
        1000.0,         // amount
        "salary"        // category
    );
    bus_->publish(event);
}
```

## Creating a New Event Type

If the existing events don't fit your needs, you can create new ones.

### Define the Event Class

In `src/core/event.hpp` (or a new header), add your event:

```cpp
/// Event emitted when a dividend is paid
class DividendEvent : public Event {
public:
    DividendEvent(SimTime timestamp, std::string source_id,
                  std::string ticker, double amount, double shares)
        : Event(timestamp, std::move(source_id))
        , ticker_(std::move(ticker))
        , amount_(amount)
        , shares_(shares) {}

    const char* type_name() const override { return "DividendEvent"; }

    const std::string& ticker() const { return ticker_; }
    double amount() const { return amount_; }
    double shares() const { return shares_; }

private:
    std::string ticker_;
    double amount_;
    double shares_;
};
```

**Requirements:**
- Inherit from `Event`
- Call `Event`'s constructor with `timestamp` and `source_id`
- Override `type_name()` to return a unique string
- Make all data `private` with `const` getters (events are immutable)

### Add Python Bindings for the Event

In `bindings.cpp`:

```cpp
py::class_<DividendEvent, Event, std::shared_ptr<DividendEvent>>(m, "DividendEvent")
    .def(py::init<SimTime, std::string, std::string, double, double>(),
         py::arg("timestamp"), py::arg("source_id"),
         py::arg("ticker"), py::arg("amount"), py::arg("shares"))
    .def("ticker", &DividendEvent::ticker)
    .def("amount", &DividendEvent::amount)
    .def("shares", &DividendEvent::shares);
```

## Creating a Scenario

Once your model works, you can include it in scenarios for easy testing.

Create a file in `scenarios/`, for example `scenarios/rental_property.py`:

```python
"""Scenario testing rental income model."""
from system.financesim_cpp import RentalIncome, CheckingAccount

SCENARIO_NAME = "Rental Property Investment"


def create_models():
    """Return list of models for this scenario."""
    return [
        # Rental income deposited to checking account
        RentalIncome("rental1", "Downtown Apartment", monthly_rent=1500.0),
        RentalIncome("rental2", "Suburban House", monthly_rent=2200.0,
                     lease_start_day=15.0),  # Lease starts on day 15

        # Checking account to receive the income
        CheckingAccount("checking1", "Primary Checking", initial_balance=5000.0),
    ]
```

Run it:
```python
from system.simulation import Simulation

sim = Simulation.from_scenario("rental_property")
sim.initialize()
sim.run(365)

print(f"Ran scenario: {sim.scenario_name}")
print(f"Total events: {len(sim.event_bus.event_log())}")

sim.finalize()
```

## Checklist for Adding a Model

Use this checklist when creating a new model:

- [ ] **Header file** created in appropriate `src/models/` subdirectory
- [ ] **Implementation file** created with constructor, update(), reset()
- [ ] **CMakeLists.txt** updated to include your `.cpp` file
- [ ] **Python bindings** added in `bindings.cpp`
- [ ] **Build succeeds** with `./buildSim`
- [ ] **Basic test** passes in Python
- [ ] **Scenario file** created (optional but recommended)

## Common Mistakes

### Forgetting to call base class methods

Always call the base class version of overridden methods:

```cpp
void MyModel::initialize(EventBus& bus) {
    BaseClass::initialize(bus);  // Don't forget this!
    // Your code here
}

void MyModel::reset() {
    BaseClass::reset();  // Don't forget this!
    // Your code here
}
```

### Not saving the EventBus pointer

If you need to publish events later, save the bus pointer:

```cpp
void MyModel::initialize(EventBus& bus) {
    bus_ = &bus;  // Save for later use
}
```

### Forgetting to unsubscribe

Always unsubscribe in `reset()` to avoid dangling callbacks:

```cpp
void MyModel::reset() {
    if (bus_) {
        bus_->unsubscribe(my_subscription_);
    }
}
```

### Using the wrong schedule rate

Remember: `rate` is in **days**, not months or years.
- Monthly = 30 (not 1)
- Weekly = 7 (not 1)
- Daily = 1

## Next Steps

- Read @ref scheduling "How Scheduling Works" for advanced scheduling patterns
- Look at existing models in `src/models/` for more examples
- Check `CareerJob` for a scheduled income model
- Check `SavingsAccount` for a model that's both scheduled AND event-driven

## Summary

Creating a new model involves:

1. **Choose a base class** (IncomeBase, ExpensesBase, etc.) or implement Model directly
2. **Create header and implementation files** with your model's logic
3. **Configure the Schedule** to control when your model runs
4. **Implement update()** with your model's behavior
5. **Subscribe to events** in initialize() if you need to react to other models
6. **Publish events** using emit() to communicate with other models
7. **Add to CMakeLists.txt** and **bindings.cpp**
8. **Test** with a simple Python script or scenario
