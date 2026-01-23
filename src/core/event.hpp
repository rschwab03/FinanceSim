#pragma once

#include <string>
#include <memory>
#include "core/time.hpp"

namespace financesim {

/// Base class for all typed events in the simulation
class Event {
public:
    virtual ~Event() = default;

    /// Returns the type name of this event (for debugging/logging)
    virtual const char* type_name() const = 0;

    /// Time at which this event was generated
    SimTime timestamp() const { return timestamp_; }

    /// Source model ID that generated this event
    const std::string& source_id() const { return source_id_; }

protected:
    Event(SimTime timestamp, std::string source_id)
        : timestamp_(timestamp), source_id_(std::move(source_id)) {}

private:
    SimTime timestamp_;
    std::string source_id_;
};

using EventPtr = std::shared_ptr<const Event>;

// ============================================================================
// Typed Events for Financial Models
// ============================================================================

/// Event emitted when income is received
class IncomeEvent : public Event {
public:
    IncomeEvent(SimTime timestamp, std::string source_id,
                double amount, std::string category)
        : Event(timestamp, std::move(source_id))
        , amount_(amount)
        , category_(std::move(category)) {}

    const char* type_name() const override { return "IncomeEvent"; }

    double amount() const { return amount_; }
    const std::string& category() const { return category_; }

private:
    double amount_;
    std::string category_;
};

/// Event emitted when an expense occurs
class ExpenseEvent : public Event {
public:
    ExpenseEvent(SimTime timestamp, std::string source_id,
                 double amount, std::string category)
        : Event(timestamp, std::move(source_id))
        , amount_(amount)
        , category_(std::move(category)) {}

    const char* type_name() const override { return "ExpenseEvent"; }

    double amount() const { return amount_; }
    const std::string& category() const { return category_; }

private:
    double amount_;
    std::string category_;
};

/// Event emitted when asset value changes
class AssetEvent : public Event {
public:
    AssetEvent(SimTime timestamp, std::string source_id,
               std::string asset_id, double value, double delta)
        : Event(timestamp, std::move(source_id))
        , asset_id_(std::move(asset_id))
        , value_(value)
        , delta_(delta) {}

    const char* type_name() const override { return "AssetEvent"; }

    const std::string& asset_id() const { return asset_id_; }
    double value() const { return value_; }
    double delta() const { return delta_; }

private:
    std::string asset_id_;
    double value_;
    double delta_;
};

/// Event emitted when liability value changes
class LiabilityEvent : public Event {
public:
    LiabilityEvent(SimTime timestamp, std::string source_id,
                   std::string liability_id, double value, double delta)
        : Event(timestamp, std::move(source_id))
        , liability_id_(std::move(liability_id))
        , value_(value)
        , delta_(delta) {}

    const char* type_name() const override { return "LiabilityEvent"; }

    const std::string& liability_id() const { return liability_id_; }
    double value() const { return value_; }
    double delta() const { return delta_; }

private:
    std::string liability_id_;
    double value_;
    double delta_;
};

} // namespace financesim
