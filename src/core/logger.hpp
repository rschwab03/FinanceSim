#pragma once

#include <vector>
#include <memory>
#include <string>
#include <set>
#include "core/event_bus.hpp"
#include "core/log_level.hpp"
#include "core/log_writer.hpp"

namespace financesim {

/// Logger service that subscribes to EventBus and writes events to configured writers
/// The Logger is a service, not a Model - it doesn't need scheduled execution
class Logger {
public:
    Logger();
    ~Logger();

    // Non-copyable
    Logger(const Logger&) = delete;
    Logger& operator=(const Logger&) = delete;

    /// Set the minimum log level (events below this level are ignored)
    void set_level(LogLevel level);

    /// Get the current log level
    LogLevel level() const { return level_; }

    /// Add a writer for output (Logger takes ownership via shared_ptr)
    void add_writer(LogWriterPtr writer);

    /// Remove all writers
    void clear_writers();

    /// Add event type filter (only log these types)
    /// If no filters are set, all event types are logged
    void add_type_filter(const std::string& type_name);

    /// Clear all type filters (log all types)
    void clear_type_filters();

    /// Add source ID filter (only log events from these sources)
    /// If no filters are set, all sources are logged
    void add_source_filter(const std::string& source_id);

    /// Clear all source filters (log all sources)
    void clear_source_filters();

    /// Set time range filter
    void set_time_range(SimTime start, SimTime end);

    /// Clear time range filter
    void clear_time_range();

    /// Attach to an EventBus (starts logging)
    void attach(EventBus& bus);

    /// Detach from the current EventBus (stops logging)
    void detach();

    /// Flush all writers
    void flush();

    /// Check if attached to an EventBus
    bool is_attached() const { return subscription_id_ != 0; }

private:
    void on_event(const EventPtr& event);
    bool passes_filters(const Event& event) const;
    LogLevel classify_event(const Event& event) const;

    LogLevel level_ = LogLevel::INFO;
    std::vector<LogWriterPtr> writers_;
    std::set<std::string> type_filters_;
    std::set<std::string> source_filters_;
    SimTime time_start_ = -1.0;
    SimTime time_end_ = -1.0;
    bool time_filter_active_ = false;

    EventBus* bus_ = nullptr;
    SubscriptionId subscription_id_ = 0;
};

} // namespace financesim
