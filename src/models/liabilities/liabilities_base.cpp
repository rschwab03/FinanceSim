#include "models/liabilities/liabilities_base.hpp"

namespace financesim {

LiabilitiesBase::LiabilitiesBase(std::string id, std::string name, Schedule schedule)
    : id_(std::move(id))
    , name_(std::move(name))
    , schedule_(schedule) {}

void LiabilitiesBase::initialize(EventBus& bus) {
    bus_ = &bus;
    // TODO: Subscribe to relevant events
}

void LiabilitiesBase::update(SimTime time) {
    // TODO: Implement in derived classes
}

void LiabilitiesBase::finalize() {
    // TODO: Cleanup
}

void LiabilitiesBase::reset() {
    bus_ = nullptr;
    // TODO: Reset state
}

} // namespace financesim
