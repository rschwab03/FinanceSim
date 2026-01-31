#include "core/logger.hpp"

namespace financesim {

Logger::Logger() = default;

Logger::~Logger() {
    detach();
    flush();
    for (auto& writer : writers_) {
        writer->close();
    }
}

void Logger::set_level(LogLevel level) {
    level_ = level;
}

void Logger::add_writer(LogWriterPtr writer) {
    writers_.push_back(std::move(writer));
}

void Logger::clear_writers() {
    for (auto& writer : writers_) {
        writer->close();
    }
    writers_.clear();
}

void Logger::add_type_filter(const std::string& type_name) {
    type_filters_.insert(type_name);
}

void Logger::clear_type_filters() {
    type_filters_.clear();
}

void Logger::add_source_filter(const std::string& source_id) {
    source_filters_.insert(source_id);
}

void Logger::clear_source_filters() {
    source_filters_.clear();
}

void Logger::set_time_range(SimTime start, SimTime end) {
    time_start_ = start;
    time_end_ = end;
    time_filter_active_ = true;
}

void Logger::clear_time_range() {
    time_filter_active_ = false;
}

void Logger::attach(EventBus& bus) {
    if (subscription_id_ != 0) {
        detach();
    }
    bus_ = &bus;
    subscription_id_ = bus_->subscribe_all(
        [this](const EventPtr& event) { on_event(event); }
    );
}

void Logger::detach() {
    if (bus_ && subscription_id_ != 0) {
        bus_->unsubscribe(subscription_id_);
    }
    bus_ = nullptr;
    subscription_id_ = 0;
}

void Logger::flush() {
    for (auto& writer : writers_) {
        writer->flush();
    }
}

void Logger::on_event(const EventPtr& event) {
    if (!event) return;
    if (!passes_filters(*event)) return;

    LogLevel event_level = classify_event(*event);
    if (event_level < level_) return;

    for (auto& writer : writers_) {
        writer->write(*event, event_level);
    }
}

bool Logger::passes_filters(const Event& event) const {
    // Type filter
    if (!type_filters_.empty()) {
        if (type_filters_.find(event.type_name()) == type_filters_.end()) {
            return false;
        }
    }

    // Source filter
    if (!source_filters_.empty()) {
        if (source_filters_.find(event.source_id()) == source_filters_.end()) {
            return false;
        }
    }

    // Time filter
    if (time_filter_active_) {
        SimTime t = event.timestamp();
        if (t < time_start_ || t > time_end_) {
            return false;
        }
    }

    return true;
}

LogLevel Logger::classify_event(const Event& event) const {
    // For now, all events are INFO level
    // Could be extended to classify different event types at different levels
    return LogLevel::INFO;
}

} // namespace financesim
