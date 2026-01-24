#include "models/assets/assets_base.hpp"

namespace financesim {

AssetsBase::AssetsBase(std::string id, std::string name, Schedule schedule)
    : id_(std::move(id))
    , name_(std::move(name))
    , schedule_(schedule) {}

void AssetsBase::initialize(EventBus& bus) {
    bus_ = &bus;
    // TODO: Subscribe to relevant events
}

void AssetsBase::update(SimTime time) {
    // TODO: Implement in derived classes
}

void AssetsBase::finalize() {
    // TODO: Cleanup
}

void AssetsBase::reset() {
    bus_ = nullptr;
    // TODO: Reset state
}

} // namespace financesim
