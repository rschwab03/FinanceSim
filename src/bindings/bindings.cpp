#include <pybind11/pybind11.h>
#include <pybind11/stl.h>
#include <pybind11/functional.h>

#include "core/time.hpp"
#include "core/event.hpp"
#include "core/event_bus.hpp"
#include "core/model.hpp"
#include "core/log_level.hpp"
#include "core/log_writer.hpp"
#include "core/logger.hpp"
#include "models/income/income_base.hpp"
#include "models/income/career_job.hpp"
#include "models/expenses/expenses_base.hpp"
#include "models/assets/assets_base.hpp"
#include "models/liabilities/liabilities_base.hpp"
#include "models/accounts/account_base.hpp"
#include "models/accounts/checking_account.hpp"
#include "models/accounts/savings_account.hpp"

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
        .def(py::init<SimTime, std::string, double, std::string, std::string>(),
             py::arg("timestamp"), py::arg("source_id"), py::arg("amount"),
             py::arg("category"), py::arg("target_account") = "")
        .def("amount", &IncomeEvent::amount)
        .def("category", &IncomeEvent::category)
        .def("target_account", &IncomeEvent::target_account);

    py::class_<ExpenseEvent, Event, std::shared_ptr<ExpenseEvent>>(m, "ExpenseEvent")
        .def(py::init<SimTime, std::string, double, std::string, std::string>(),
             py::arg("timestamp"), py::arg("source_id"), py::arg("amount"),
             py::arg("category"), py::arg("target_account") = "")
        .def("amount", &ExpenseEvent::amount)
        .def("category", &ExpenseEvent::category)
        .def("target_account", &ExpenseEvent::target_account);

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

    py::class_<AccountEvent, Event, std::shared_ptr<AccountEvent>>(m, "AccountEvent")
        .def(py::init<SimTime, std::string, std::string, double, double, std::string>(),
             py::arg("timestamp"), py::arg("source_id"), py::arg("account_id"),
             py::arg("balance"), py::arg("delta"), py::arg("reason"))
        .def("account_id", &AccountEvent::account_id)
        .def("balance", &AccountEvent::balance)
        .def("delta", &AccountEvent::delta)
        .def("reason", &AccountEvent::reason);

    py::class_<TransferEvent, Event, std::shared_ptr<TransferEvent>>(m, "TransferEvent")
        .def(py::init<SimTime, std::string, std::string, std::string, double, std::string>(),
             py::arg("timestamp"), py::arg("source_id"), py::arg("from_account"),
             py::arg("to_account"), py::arg("amount"), py::arg("reason"))
        .def("from_account", &TransferEvent::from_account)
        .def("to_account", &TransferEvent::to_account)
        .def("amount", &TransferEvent::amount)
        .def("reason", &TransferEvent::reason);

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

    // Logging framework
    py::enum_<LogLevel>(m, "LogLevel")
        .value("DEBUG", LogLevel::DEBUG)
        .value("INFO", LogLevel::INFO)
        .value("WARN", LogLevel::WARN)
        .value("ERROR", LogLevel::ERROR);

    py::class_<LogWriter, LogWriterPtr>(m, "LogWriter")
        .def("flush", &LogWriter::flush)
        .def("close", &LogWriter::close);

    py::class_<ConsoleWriter, LogWriter, std::shared_ptr<ConsoleWriter>>(m, "ConsoleWriter")
        .def(py::init<>());

    py::class_<JsonWriter, LogWriter, std::shared_ptr<JsonWriter>>(m, "JsonWriter")
        .def(py::init<const std::string&>(), py::arg("filename"));

    py::class_<Logger>(m, "Logger")
        .def(py::init<>())
        .def("set_level", &Logger::set_level)
        .def("level", &Logger::level)
        .def("add_writer", &Logger::add_writer)
        .def("clear_writers", &Logger::clear_writers)
        .def("add_type_filter", &Logger::add_type_filter)
        .def("clear_type_filters", &Logger::clear_type_filters)
        .def("add_source_filter", &Logger::add_source_filter)
        .def("clear_source_filters", &Logger::clear_source_filters)
        .def("set_time_range", &Logger::set_time_range)
        .def("clear_time_range", &Logger::clear_time_range)
        .def("attach", &Logger::attach)
        .def("detach", &Logger::detach)
        .def("flush", &Logger::flush)
        .def("is_attached", &Logger::is_attached);

    // Account models
    py::class_<AccountBase, Model, std::shared_ptr<AccountBase>>(m, "AccountBase")
        .def(py::init<std::string, std::string, std::string, double, Schedule>(),
             py::arg("id"), py::arg("name"), py::arg("routing_tag"),
             py::arg("initial_balance") = 0.0,
             py::arg("schedule") = make_account_schedule())
        .def("routing_tag", &AccountBase::routing_tag)
        .def("balance", &AccountBase::balance);

    py::class_<CheckingAccount, AccountBase, std::shared_ptr<CheckingAccount>>(m, "CheckingAccount")
        .def(py::init<std::string, std::string, double, std::string, Schedule>(),
             py::arg("id"), py::arg("name"),
             py::arg("initial_balance") = 0.0,
             py::arg("routing_tag") = "default",
             py::arg("schedule") = make_account_schedule());

    py::class_<SavingsAccount, AccountBase, std::shared_ptr<SavingsAccount>>(m, "SavingsAccount")
        .def(py::init<std::string, std::string, double, double, std::string, Schedule>(),
             py::arg("id"), py::arg("name"), py::arg("apy"),
             py::arg("initial_balance") = 0.0,
             py::arg("routing_tag") = "savings",
             py::arg("schedule") = make_savings_schedule())
        .def("apy", &SavingsAccount::apy);
}
