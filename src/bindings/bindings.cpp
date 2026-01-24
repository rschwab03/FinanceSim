#include <pybind11/pybind11.h>
#include <pybind11/stl.h>
#include <pybind11/functional.h>

#include "core/time.hpp"
#include "core/event.hpp"
#include "core/event_bus.hpp"
#include "core/model.hpp"
#include "models/income/income_base.hpp"
#include "models/income/career_job.hpp"
#include "models/expenses/expenses_base.hpp"
#include "models/assets/assets_base.hpp"
#include "models/liabilities/liabilities_base.hpp"

namespace py = pybind11;
using namespace financesim;

PYBIND11_MODULE(financesim_cpp, m) {
    m.doc() = "Financial simulation C++ models";

    // Time types
    py::enum_<ExecutionTiming>(m, "ExecutionTiming")
        .value("StartOfPeriod", ExecutionTiming::StartOfPeriod)
        .value("EndOfPeriod", ExecutionTiming::EndOfPeriod);

    py::class_<Schedule>(m, "Schedule")
        .def(py::init<>())
        .def_readwrite("start_time", &Schedule::start_time)
        .def_readwrite("stop_time", &Schedule::stop_time)
        .def_readwrite("rate", &Schedule::rate)
        .def_readwrite("timing", &Schedule::timing);

    // Event base class
    py::class_<Event, std::shared_ptr<Event>>(m, "Event")
        .def("type_name", &Event::type_name)
        .def("timestamp", &Event::timestamp)
        .def("source_id", &Event::source_id);

    // Typed events
    py::class_<IncomeEvent, Event, std::shared_ptr<IncomeEvent>>(m, "IncomeEvent")
        .def(py::init<SimTime, std::string, double, std::string>())
        .def("amount", &IncomeEvent::amount)
        .def("category", &IncomeEvent::category);

    py::class_<ExpenseEvent, Event, std::shared_ptr<ExpenseEvent>>(m, "ExpenseEvent")
        .def(py::init<SimTime, std::string, double, std::string>())
        .def("amount", &ExpenseEvent::amount)
        .def("category", &ExpenseEvent::category);

    py::class_<AssetEvent, Event, std::shared_ptr<AssetEvent>>(m, "AssetEvent")
        .def(py::init<SimTime, std::string, std::string, double, double>())
        .def("asset_id", &AssetEvent::asset_id)
        .def("value", &AssetEvent::value)
        .def("delta", &AssetEvent::delta);

    py::class_<LiabilityEvent, Event, std::shared_ptr<LiabilityEvent>>(m, "LiabilityEvent")
        .def(py::init<SimTime, std::string, std::string, double, double>())
        .def("liability_id", &LiabilityEvent::liability_id)
        .def("value", &LiabilityEvent::value)
        .def("delta", &LiabilityEvent::delta);

    // Event bus
    py::class_<EventBus>(m, "EventBus")
        .def(py::init<>())
        .def("subscribe_all", &EventBus::subscribe_all)
        .def("unsubscribe", &EventBus::unsubscribe)
        .def("event_log", &EventBus::event_log, py::return_value_policy::reference)
        .def("clear_log", &EventBus::clear_log)
        .def("reset", &EventBus::reset);

    // Model base class
    py::class_<Model, std::shared_ptr<Model>>(m, "Model")
        .def("id", &Model::id)
        .def("name", &Model::name)
        .def("schedule", &Model::schedule, py::return_value_policy::reference)
        .def("initialize", &Model::initialize)
        .def("update", &Model::update)
        .def("finalize", &Model::finalize)
        .def("reset", &Model::reset);

    // Base model classes
    py::class_<IncomeBase, Model, std::shared_ptr<IncomeBase>>(m, "IncomeBase")
        .def(py::init<std::string, std::string, Schedule>());

    py::class_<CareerJob, IncomeBase, std::shared_ptr<CareerJob>>(m, "CareerJob")
        .def(py::init<std::string, std::string, double, SimTime>(),
             py::arg("id"), py::arg("name"), py::arg("annual_salary"), py::arg("start_day") = 0.0)
        .def("annual_salary", &CareerJob::annual_salary)
        .def("payment_amount", &CareerJob::payment_amount);

    py::class_<ExpensesBase, Model, std::shared_ptr<ExpensesBase>>(m, "ExpensesBase")
        .def(py::init<std::string, std::string, Schedule>());

    py::class_<AssetsBase, Model, std::shared_ptr<AssetsBase>>(m, "AssetsBase")
        .def(py::init<std::string, std::string, Schedule>());

    py::class_<LiabilitiesBase, Model, std::shared_ptr<LiabilitiesBase>>(m, "LiabilitiesBase")
        .def(py::init<std::string, std::string, Schedule>());
}
