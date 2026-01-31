#include "core/log_writer.hpp"
#include <iostream>
#include <iomanip>
#include <sstream>

namespace financesim {

// ============================================================================
// ConsoleWriter Implementation
// ============================================================================

void ConsoleWriter::write(const Event& event, LogLevel level) {
    std::cout << "[" << std::setw(5) << log_level_to_string(level) << "] "
              << "t=" << std::fixed << std::setprecision(1) << event.timestamp()
              << " [" << event.source_id() << "] "
              << event.type_name()
              << std::endl;
}

void ConsoleWriter::flush() {
    std::cout.flush();
}

void ConsoleWriter::close() {
    // Nothing to close for console
}

// ============================================================================
// JsonWriter Implementation
// ============================================================================

JsonWriter::JsonWriter(const std::string& filename)
    : filename_(filename) {
    file_.open(filename, std::ios::out | std::ios::trunc);
    if (!file_.is_open()) {
        throw std::runtime_error("Failed to open log file: " + filename);
    }
}

JsonWriter::~JsonWriter() {
    close();
}

namespace {
    // Helper to escape strings for JSON
    std::string escape_json(const std::string& s) {
        std::ostringstream o;
        for (char c : s) {
            switch (c) {
                case '"': o << "\\\""; break;
                case '\\': o << "\\\\"; break;
                case '\b': o << "\\b"; break;
                case '\f': o << "\\f"; break;
                case '\n': o << "\\n"; break;
                case '\r': o << "\\r"; break;
                case '\t': o << "\\t"; break;
                default:
                    if ('\x00' <= c && c <= '\x1f') {
                        o << "\\u" << std::hex << std::setw(4) << std::setfill('0') << static_cast<int>(c);
                    } else {
                        o << c;
                    }
            }
        }
        return o.str();
    }
}

void JsonWriter::write(const Event& event, LogLevel level) {
    if (!file_.is_open()) return;

    // Build JSON object manually (avoiding external JSON library)
    file_ << "{"
          << "\"timestamp\":" << std::fixed << std::setprecision(6) << event.timestamp() << ","
          << "\"level\":\"" << log_level_to_string(level) << "\","
          << "\"type\":\"" << event.type_name() << "\","
          << "\"source_id\":\"" << escape_json(event.source_id()) << "\"";

    // Add type-specific fields
    if (auto* income = dynamic_cast<const IncomeEvent*>(&event)) {
        file_ << ",\"amount\":" << income->amount()
              << ",\"category\":\"" << escape_json(income->category()) << "\""
              << ",\"target_account\":\"" << escape_json(income->target_account()) << "\"";
    } else if (auto* expense = dynamic_cast<const ExpenseEvent*>(&event)) {
        file_ << ",\"amount\":" << expense->amount()
              << ",\"category\":\"" << escape_json(expense->category()) << "\""
              << ",\"target_account\":\"" << escape_json(expense->target_account()) << "\"";
    } else if (auto* asset = dynamic_cast<const AssetEvent*>(&event)) {
        file_ << ",\"asset_id\":\"" << escape_json(asset->asset_id()) << "\""
              << ",\"value\":" << asset->value()
              << ",\"delta\":" << asset->delta();
    } else if (auto* liability = dynamic_cast<const LiabilityEvent*>(&event)) {
        file_ << ",\"liability_id\":\"" << escape_json(liability->liability_id()) << "\""
              << ",\"value\":" << liability->value()
              << ",\"delta\":" << liability->delta();
    } else if (auto* account = dynamic_cast<const AccountEvent*>(&event)) {
        file_ << ",\"account_id\":\"" << escape_json(account->account_id()) << "\""
              << ",\"balance\":" << account->balance()
              << ",\"delta\":" << account->delta()
              << ",\"reason\":\"" << escape_json(account->reason()) << "\"";
    } else if (auto* transfer = dynamic_cast<const TransferEvent*>(&event)) {
        file_ << ",\"from_account\":\"" << escape_json(transfer->from_account()) << "\""
              << ",\"to_account\":\"" << escape_json(transfer->to_account()) << "\""
              << ",\"amount\":" << transfer->amount()
              << ",\"reason\":\"" << escape_json(transfer->reason()) << "\"";
    }

    file_ << "}\n";
}

void JsonWriter::flush() {
    if (file_.is_open()) {
        file_.flush();
    }
}

void JsonWriter::close() {
    if (file_.is_open()) {
        file_.close();
    }
}

} // namespace financesim
