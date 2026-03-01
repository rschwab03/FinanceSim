@page scheduling How Scheduling Works

# How Scheduling Works

This page explains how FinanceSim decides when to run each model during a simulation. Understanding scheduling is important when creating new models because it determines how often your model's logic executes.

## The Big Picture

Imagine you're building a financial simulation with several models:
- A **job** that pays you twice a month
- A **savings account** that calculates interest monthly
- A **bill** that charges you every 30 days

Each of these models needs to run at different times. The **scheduler** is the part of FinanceSim that keeps track of when each model should run and calls them at the right times.

## Key Concepts

### Simulation Time

FinanceSim measures time in **days**. When you run a simulation for 365 days, that represents one year. Time starts at 0 and increases as the simulation progresses.

```python
sim.run(365)  # Run for one year (365 days)
```

The simulation does NOT run in real-time. It jumps from one scheduled event to the next, which makes simulations run very fast.

### The Schedule Struct

Every model has a **Schedule** that tells the scheduler when to call it. A Schedule has four fields:

| Field | Type | Description | Default |
|-------|------|-------------|---------|
| `start_time` | double | Day when the model starts executing | 0.0 |
| `stop_time` | double | Day when the model stops (-1 means never) | -1.0 |
| `rate` | double | How many days between executions | 1.0 |
| `timing` | enum | When in the period to execute | StartOfPeriod |

Here's what each field means in plain English:

- **start_time**: "Don't run me until day X"
- **stop_time**: "Stop running me after day Y" (or -1 for "run forever")
- **rate**: "Run me every N days"
- **timing**: "Run at the start or end of my period"

### Example Schedules

**Daily execution (the default):**
```cpp
Schedule daily;
daily.rate = 1.0;  // Every 1 day
// start_time = 0, stop_time = -1 (defaults)
```

**Semi-monthly (like a paycheck):**
```cpp
Schedule semiMonthly;
semiMonthly.rate = 365.0 / 24.0;  // About 15.2 days
```

**Monthly:**
```cpp
Schedule monthly;
monthly.rate = 30.0;  // Every 30 days
```

**Event-driven only (no scheduled updates):**
```cpp
Schedule eventOnly;
eventOnly.rate = 0.0;  // Never scheduled automatically
```

**Starts after 90 days:**
```cpp
Schedule delayed;
delayed.start_time = 90.0;  // Begin on day 90
delayed.rate = 7.0;         // Then run weekly
```

**Temporary (runs for one year only):**
```cpp
Schedule temporary;
temporary.start_time = 0.0;
temporary.stop_time = 365.0;  // Stop after one year
temporary.rate = 30.0;        // Monthly while active
```

## How the Scheduler Works

The scheduler maintains a **priority queue** of upcoming model executions, sorted by time. Here's the process:

1. **Registration**: When you add a model to the simulation, the scheduler calculates when it should first run based on its Schedule.

2. **Running**: The scheduler repeatedly:
   - Looks at the next scheduled execution
   - Advances simulation time to that moment
   - Calls the model's `update(time)` method
   - Schedules the model's next execution (if applicable)

3. **Jumping**: Time doesn't tick second-by-second. It **jumps** directly to the next scheduled event. If nothing is scheduled between day 10 and day 25, time jumps from 10 to 25 instantly.

### Visual Example

Imagine three models:
- Model A: runs daily (rate = 1)
- Model B: runs every 3 days (rate = 3)
- Model C: runs weekly (rate = 7)

The execution order would be:
```
Day 0: A runs, B runs, C runs
Day 1: A runs
Day 2: A runs
Day 3: A runs, B runs
Day 4: A runs
Day 5: A runs
Day 6: A runs, B runs
Day 7: A runs, C runs
...
```

## Event-Driven vs Scheduled Models

FinanceSim supports two types of model behavior, and understanding when to use each is crucial for building effective simulations.

### Scheduled Models (rate > 0)

These models run automatically at regular intervals. Examples:
- A job that pays every 15 days
- A savings account calculating interest monthly

The scheduler calls their `update()` method according to their schedule. You implement your logic in `update()` and it gets called automatically.

### Event-Driven Models (rate = 0 or rate <= 0)

These models only run when something happens. Examples:
- A checking account that responds to income/expense events
- A notification system that triggers on certain conditions

When `rate <= 0`, the scheduler never automatically calls `update()`. Instead, the model responds to events it subscribes to via the EventBus.

#### How Event-Driven Updates Work

Event-driven models use the **publish/subscribe** pattern. Here's the flow:

```
┌─────────────┐         ┌─────────────┐         ┌─────────────┐
│  Model A    │ ──────> │  EventBus   │ ──────> │  Model B    │
│ (publisher) │ publish │             │ deliver │ (subscriber)│
│             │  event  │             │  event  │             │
└─────────────┘         └─────────────┘         └─────────────┘
```

1. **Model A** publishes an event (e.g., `IncomeEvent`)
2. **EventBus** receives the event and looks up all subscribers
3. **Model B** (which subscribed to `IncomeEvent`) has its callback invoked immediately

#### Setting Up Event Subscriptions

You subscribe to events in your model's `initialize()` method:

```cpp
void MyAccount::initialize(EventBus& bus) {
    // Save the bus pointer for later use
    bus_ = &bus;

    // Subscribe to IncomeEvent - the lambda is called whenever one is published
    income_subscription_ = bus.subscribe<IncomeEvent>(
        [this](const IncomeEvent& event) {
            // This code runs immediately when ANY model publishes an IncomeEvent
            on_income_received(event);
        }
    );

    // You can subscribe to multiple event types
    expense_subscription_ = bus.subscribe<ExpenseEvent>(
        [this](const ExpenseEvent& event) {
            on_expense_occurred(event);
        }
    );
}
```

**Key points about subscriptions:**

- **Immediate execution**: When an event is published, your callback runs right away, before the publishing model's `update()` even returns
- **Save the subscription ID**: You need it to unsubscribe later
- **Capture `this`**: The lambda captures `this` so you can access your model's member variables
- **Const reference**: Events are passed as `const&` - you cannot modify them

#### Handling Events

Your event handler can do anything: update internal state, publish new events, or just record information:

```cpp
void MyAccount::on_income_received(const IncomeEvent& event) {
    // Check if this event is meant for us
    if (event.target_account() != routing_tag_ &&
        !(event.target_account().empty() && routing_tag_ == "default")) {
        return;  // Not our event, ignore it
    }

    // Update our state
    balance_ += event.amount();

    // Optionally publish our own event to notify others
    emit<AccountEvent>(event.timestamp(), id_, balance_, event.amount(), "deposit");
}
```

#### Cleaning Up Subscriptions

Always unsubscribe in `reset()` to prevent dangling callbacks:

```cpp
void MyAccount::reset() {
    if (bus_) {
        bus_->unsubscribe(income_subscription_);
        bus_->unsubscribe(expense_subscription_);
    }
    bus_ = nullptr;
    income_subscription_ = 0;
    expense_subscription_ = 0;

    // Reset other state...
}
```

**Why this matters**: If you don't unsubscribe and the simulation resets, the old callbacks still exist and point to potentially invalid memory.

### Hybrid Models: Both Scheduled AND Event-Driven

A model can be BOTH scheduled AND event-driven. This is common and powerful. For example, a savings account might:
- Respond immediately to deposit events (event-driven)
- Also calculate interest monthly (scheduled with rate = 30)

```cpp
// SavingsAccount is hybrid - it does both
SavingsAccount::SavingsAccount(...)
    : AccountBase(..., Schedule{0.0, -1.0, 30.0, ...})  // rate=30 for monthly interest
{
}

void SavingsAccount::initialize(EventBus& bus) {
    AccountBase::initialize(bus);  // Sets up event subscriptions
    // The scheduler will ALSO call update() every 30 days
}

void SavingsAccount::on_income(const IncomeEvent& event) {
    // Event-driven: called immediately when income arrives
    accrue_interest_up_to(event.timestamp());  // Catch up on interest first
    deposit(event.timestamp(), event.amount(), event.category());
}

void SavingsAccount::update(SimTime time) {
    // Scheduled: called every 30 days by the scheduler
    accrue_interest_up_to(time);  // Calculate monthly interest
}
```

This hybrid approach lets the savings account:
1. **React instantly** to deposits (no waiting for the next scheduled update)
2. **Ensure interest is calculated** even if no deposits happen that month

### When to Use Each Approach

| Use Case | Approach | Why |
|----------|----------|-----|
| Regular payments (salary, rent) | Scheduled | Predictable timing, no trigger needed |
| Responding to transactions | Event-driven | Need immediate reaction |
| Interest calculation | Hybrid | Regular calculation + catch-up on transactions |
| Alerts/notifications | Event-driven | Only act when something happens |
| Daily market prices | Scheduled | Updates regardless of other activity |

## Configuring Schedules in Your Models

### In the Constructor

Most models set their schedule in the constructor by passing it to their base class:

```cpp
// Fixed schedule - always semi-monthly
CareerJob::CareerJob(std::string id, std::string name, double salary, SimTime start_day)
    : IncomeBase(std::move(id), std::move(name),
                 Schedule{start_day, -1.0, 15.2, ExecutionTiming::EndOfPeriod})
    // ...
{
}
```

### Accepting a Custom Schedule

For flexibility, you can accept a Schedule as a constructor parameter:

```cpp
SavingsAccount::SavingsAccount(std::string id, std::string name,
                               double apy, double initial_balance,
                               std::string routing_tag,
                               Schedule schedule)  // User can customize
    : AccountBase(std::move(id), std::move(name),
                  std::move(routing_tag), initial_balance,
                  schedule)
    // ...
{
}
```

This lets users override the default:

```python
# Default: monthly interest (rate=30)
savings = SavingsAccount("s1", "My Savings", apy=0.045)

# Custom: daily interest calculation
daily_schedule = Schedule()
daily_schedule.rate = 1.0
savings = SavingsAccount("s1", "My Savings", apy=0.045,
                         schedule=daily_schedule)
```

## ExecutionTiming

The `timing` field controls whether the model runs at the **start** or **end** of its period. This matters when multiple models interact:

- **StartOfPeriod**: Run at the beginning of the interval (default)
- **EndOfPeriod**: Run at the end of the interval

For example, a paycheck model might use `EndOfPeriod` to represent receiving payment at the end of a pay period rather than the start.

```cpp
Schedule payday;
payday.rate = 15.0;
payday.timing = ExecutionTiming::EndOfPeriod;
```

## Common Patterns

### "Run Once at Startup"

To run exactly once when the simulation starts:
```cpp
Schedule runOnce;
runOnce.start_time = 0.0;
runOnce.stop_time = 0.0;  // Stop immediately after starting
runOnce.rate = 1.0;       // Must be > 0 to run at all
```

### "Wait for Event, Then Start"

Some models need to remain dormant until something triggers them. For example:
- A loan repayment model that starts only after a `LoanOriginationEvent`
- An investment model that activates when funds are transferred in
- A subscription service that begins after a `SignupEvent`

There are several ways to implement this pattern, each with trade-offs.

#### Approach 1: Use `schedule_at()` for One-Time Triggers

The scheduler's `schedule_at()` method lets you schedule a model to run at a specific future time. This is useful when an event should trigger a single future update:

```python
# In your event handler (Python side)
def on_signup(event):
    # User signed up - schedule the first billing 30 days from now
    scheduler.schedule_at(billing_model, event.timestamp + 30.0)
```

```cpp
// Or from C++ within an event callback
void MyModel::on_trigger_event(const TriggerEvent& event) {
    // Schedule ourselves to run at a future time
    // Note: This requires access to the scheduler, which is typically
    // managed at the Python level
    first_payment_time_ = event.timestamp() + 30.0;
    // The actual scheduling would be done from Python
}
```

**Limitation**: `schedule_at()` schedules a single execution. If you need recurring updates after the trigger, you'll need a different approach.

#### Approach 2: Start with Event-Driven, Then Switch to Scheduled

A more flexible pattern is to start as event-driven (`rate = 0`) and dynamically enable scheduling when triggered. However, FinanceSim's current Schedule is set at construction time. The workaround is to track state internally:

```cpp
class DelayedStartModel : public Model {
public:
    DelayedStartModel(std::string id)
        : id_(std::move(id))
    {
        // Start with rate > 0 so we get scheduled, but we'll skip updates
        // until activated
        schedule_.rate = 30.0;  // Check monthly
        schedule_.start_time = 0.0;
    }

    void initialize(EventBus& bus) override {
        bus_ = &bus;
        // Subscribe to the event that will activate us
        sub_id_ = bus.subscribe<ActivationEvent>(
            [this](const ActivationEvent& event) {
                activated_ = true;
                activation_time_ = event.timestamp();
            }
        );
    }

    void update(SimTime time) override {
        if (!activated_) {
            return;  // Do nothing until activated
        }

        // Now we're active - do our normal logic
        do_monthly_payment(time);
    }

private:
    bool activated_ = false;
    SimTime activation_time_ = 0.0;
    // ...
};
```

**Pros**: Simple to understand, model is always in the schedule
**Cons**: Model's `update()` is called even when inactive (minor performance cost)

#### Approach 3: Purely Event-Driven with Internal Scheduling

For complex scenarios, you can manage your own timing entirely through events:

```cpp
class EventTriggeredRecurring : public Model {
public:
    EventTriggeredRecurring(std::string id, SimTime interval)
        : id_(std::move(id)), interval_(interval)
    {
        schedule_.rate = 0.0;  // Purely event-driven
    }

    void initialize(EventBus& bus) override {
        bus_ = &bus;

        // Listen for the activation trigger
        activation_sub_ = bus.subscribe<StartEvent>(
            [this](const StartEvent& event) {
                if (!active_) {
                    active_ = true;
                    next_execution_ = event.timestamp() + interval_;
                }
            }
        );

        // Listen for time-advance events (if your sim publishes them)
        // Or use a scheduled "clock" model that publishes TimeTickEvent
        time_sub_ = bus.subscribe<TimeTickEvent>(
            [this](const TimeTickEvent& event) {
                if (active_ && event.timestamp() >= next_execution_) {
                    do_work(event.timestamp());
                    next_execution_ += interval_;
                }
            }
        );
    }

private:
    bool active_ = false;
    SimTime next_execution_ = 0.0;
    SimTime interval_;
    // ...
};
```

**Pros**: Complete control over timing, truly dormant until activated
**Cons**: More complex, requires a time-tick mechanism

#### Approach 4: Use `start_time` for Known Activation Times

If you know when the model should activate at construction time, simply set `start_time`:

```cpp
// Loan starts 30 days after simulation begins (e.g., closing date is known)
Schedule loan_schedule;
loan_schedule.start_time = 30.0;  // Start on day 30
loan_schedule.rate = 30.0;        // Monthly payments after that
```

This is the simplest approach when the activation time is predetermined.

#### Choosing the Right Approach

| Scenario | Recommended Approach |
|----------|---------------------|
| Activation time known at construction | Set `start_time` |
| One-time future action after event | `schedule_at()` |
| Recurring actions after unknown trigger | Approach 2 (conditional update) |
| Complex timing logic | Approach 3 (internal scheduling) |

#### Example: Loan That Starts After Funding

Here's a complete example of a loan model that remains dormant until it receives a funding event:

```cpp
class Loan : public Model {
public:
    Loan(std::string id, double principal, double rate, int term_months)
        : id_(std::move(id))
        , principal_(principal)
        , interest_rate_(rate)
        , term_months_(term_months)
    {
        // Monthly payments, but don't start yet
        schedule_.rate = 30.0;
        monthly_payment_ = calculate_payment(principal, rate, term_months);
    }

    void initialize(EventBus& bus) override {
        bus_ = &bus;

        // Wait for funding event
        funding_sub_ = bus.subscribe<LoanFundedEvent>(
            [this](const LoanFundedEvent& event) {
                if (event.loan_id() == id_ && !funded_) {
                    funded_ = true;
                    balance_ = principal_;
                    // First payment due 30 days after funding
                    first_payment_due_ = event.timestamp() + 30.0;
                }
            }
        );
    }

    void update(SimTime time) override {
        if (!funded_) {
            return;  // Not funded yet, do nothing
        }

        if (time < first_payment_due_) {
            return;  // Not time for first payment yet
        }

        if (balance_ <= 0) {
            return;  // Loan paid off
        }

        // Make a payment
        make_payment(time);
    }

private:
    bool funded_ = false;
    SimTime first_payment_due_ = 0.0;
    double balance_ = 0.0;
    // ...
};
```

This pattern keeps the model simple while handling the "wait for event" requirement cleanly.

### "Run During Specific Period"

To model something like a 2-year contract:
```cpp
Schedule contract;
contract.start_time = 30.0;   // Start on day 30
contract.stop_time = 760.0;   // End after ~2 years
contract.rate = 30.0;         // Monthly payments
```

## Summary

- **Schedule** controls when a model's `update()` is called
- **rate** is the interval in days (0 = event-driven only)
- **start_time** and **stop_time** bound when the model is active
- The scheduler **jumps** between events (no fixed time step)
- Models can be scheduled, event-driven, or both

For a complete guide on creating new models that use scheduling, see @ref adding_a_model "Adding a New Model".
