#pragma once

#include <string>
#include <memory>
#include "core/time.hpp"
#include "core/event_bus.hpp"

namespace financesim {

/// Base interface for all financial models
/// Models are deterministic and communicate only via the EventBus
class Model {
public:
    virtual ~Model() = default;

    /// Unique identifier for this model instance
    virtual const std::string& id() const = 0;

    /// Human-readable name/description
    virtual const std::string& name() const = 0;

    /// Get the model's execution schedule
    virtual const Schedule& schedule() const = 0;

    /// Called once before simulation starts
    /// Use for setup, subscribing to events, allocating resources
    virtual void initialize(EventBus& bus) = 0;

    /// Called at each scheduled execution time
    /// Must be deterministic given the same inputs and time
    virtual void update(SimTime time) = 0;

    /// Called once after simulation ends
    /// Use for cleanup, final calculations, releasing resources
    virtual void finalize() = 0;

    /// Reset model to initial state (for replay)
    virtual void reset() = 0;

protected:
    Model() = default;
};

using ModelPtr = std::shared_ptr<Model>;

} // namespace financesim
