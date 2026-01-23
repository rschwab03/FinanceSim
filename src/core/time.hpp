#pragma once

namespace financesim {

/// Represents a point in simulation time (continuous, in days)
using SimTime = double;

/// Configuration for when a model executes within its period
enum class ExecutionTiming {
    StartOfPeriod,
    EndOfPeriod
};

/// Schedule configuration for a model
/// The Python scheduler uses this to determine when to call step()
struct Schedule {
    SimTime start_time = 0.0;      // When the model starts executing (days)
    SimTime stop_time = -1.0;      // When the model stops (-1 = never)
    SimTime rate = 1.0;            // Execution interval in days
    ExecutionTiming timing = ExecutionTiming::StartOfPeriod;
};

} // namespace financesim
