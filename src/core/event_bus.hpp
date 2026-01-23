#pragma once

#include <vector>
#include <functional>
#include <unordered_map>
#include <typeindex>
#include <memory>
#include "core/event.hpp"

namespace financesim {

/// Callback type for event subscribers
using EventCallback = std::function<void(const EventPtr&)>;

/// Subscription handle for unsubscribing
using SubscriptionId = std::size_t;

/// Central pub/sub event bus for model communication
/// All communication between models goes through this bus
class EventBus {
public:
    EventBus() = default;

    // Non-copyable, non-movable (one per simulation)
    EventBus(const EventBus&) = delete;
    EventBus& operator=(const EventBus&) = delete;

    /// Subscribe to a specific event type
    /// Returns a subscription ID that can be used to unsubscribe
    template<typename EventType>
    SubscriptionId subscribe(std::function<void(const EventType&)> callback) {
        auto wrapper = [callback](const EventPtr& event) {
            if (auto typed = std::dynamic_pointer_cast<const EventType>(event)) {
                callback(*typed);
            }
        };

        auto type_idx = std::type_index(typeid(EventType));
        SubscriptionId id = next_subscription_id_++;
        subscriptions_[type_idx].push_back({id, wrapper});
        return id;
    }

    /// Subscribe to all events
    SubscriptionId subscribe_all(EventCallback callback) {
        SubscriptionId id = next_subscription_id_++;
        global_subscribers_.push_back({id, std::move(callback)});
        return id;
    }

    /// Unsubscribe using subscription ID
    void unsubscribe(SubscriptionId id) {
        for (auto& [type, subs] : subscriptions_) {
            subs.erase(
                std::remove_if(subs.begin(), subs.end(),
                    [id](const Subscription& s) { return s.id == id; }),
                subs.end()
            );
        }
        global_subscribers_.erase(
            std::remove_if(global_subscribers_.begin(), global_subscribers_.end(),
                [id](const Subscription& s) { return s.id == id; }),
            global_subscribers_.end()
        );
    }

    /// Publish an event to all subscribers
    template<typename EventType>
    void publish(std::shared_ptr<const EventType> event) {
        event_log_.push_back(event);

        auto type_idx = std::type_index(typeid(EventType));
        auto it = subscriptions_.find(type_idx);
        if (it != subscriptions_.end()) {
            for (const auto& sub : it->second) {
                sub.callback(event);
            }
        }

        for (const auto& sub : global_subscribers_) {
            sub.callback(event);
        }
    }

    /// Get all events in chronological order (for replay/debugging)
    const std::vector<EventPtr>& event_log() const { return event_log_; }

    /// Clear the event log
    void clear_log() { event_log_.clear(); }

    /// Reset the bus (clear all subscriptions and log)
    void reset() {
        subscriptions_.clear();
        global_subscribers_.clear();
        event_log_.clear();
        next_subscription_id_ = 0;
    }

private:
    struct Subscription {
        SubscriptionId id;
        EventCallback callback;
    };

    std::unordered_map<std::type_index, std::vector<Subscription>> subscriptions_;
    std::vector<Subscription> global_subscribers_;
    std::vector<EventPtr> event_log_;
    SubscriptionId next_subscription_id_ = 0;
};

} // namespace financesim
