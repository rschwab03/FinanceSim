#pragma once

#include "core/model.hpp"

namespace financesim {

/// Base class for asset models
/// Derive from this to create specific asset types (stocks, real estate, etc.)
class AssetsBase : public Model {
public:
    AssetsBase(std::string id, std::string name, Schedule schedule);
    virtual ~AssetsBase() = default;

    const std::string& id() const override { return id_; }
    const std::string& name() const override { return name_; }
    const Schedule& schedule() const override { return schedule_; }

    void initialize(EventBus& bus) override;
    void update(SimTime time) override;
    void finalize() override;
    void reset() override;

protected:
    EventBus& bus() { return *bus_; }

    template<typename EventType, typename... Args>
    void emit(SimTime time, Args&&... args) {
        auto event = std::make_shared<EventType>(
            time, id_, std::forward<Args>(args)...
        );
        bus_->publish(event);
    }

private:
    std::string id_;
    std::string name_;
    Schedule schedule_;
    EventBus* bus_ = nullptr;
};

} // namespace financesim
