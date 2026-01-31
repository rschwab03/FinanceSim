#pragma once

#include <string>
#include <memory>
#include <fstream>
#include "core/event.hpp"
#include "core/log_level.hpp"

namespace financesim {

/// Interface for log output writers
class LogWriter {
public:
    virtual ~LogWriter() = default;

    /// Write an event to the output
    virtual void write(const Event& event, LogLevel level) = 0;

    /// Flush any buffered output
    virtual void flush() = 0;

    /// Close the writer and release resources
    virtual void close() = 0;

protected:
    LogWriter() = default;
};

using LogWriterPtr = std::shared_ptr<LogWriter>;

/// Writes formatted log output to console (stdout)
class ConsoleWriter : public LogWriter {
public:
    ConsoleWriter() = default;
    ~ConsoleWriter() override = default;

    void write(const Event& event, LogLevel level) override;
    void flush() override;
    void close() override;
};

/// Writes events as JSON lines to a file
class JsonWriter : public LogWriter {
public:
    explicit JsonWriter(const std::string& filename);
    ~JsonWriter() override;

    void write(const Event& event, LogLevel level) override;
    void flush() override;
    void close() override;

private:
    std::ofstream file_;
    std::string filename_;
};

} // namespace financesim
