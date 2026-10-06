#pragma once
// ═══════════════════════════════════════════════════════════════════════════
// RUNTIME: The Last Algorithm
// Logger — Structured debug/info/warning/error logging
// ═══════════════════════════════════════════════════════════════════════════

#include <string>
#include <fstream>
#include <iostream>
#include <sstream>
#include <mutex>
#include <chrono>
#include <iomanip>

enum class LogLevel {
    DEBUG,
    INFO,
    WARN,
    ERROR_LOG  // Avoid conflict with ERROR macro on Windows
};

class Logger {
public:
    // Singleton access
    static Logger& instance();

    // Delete copy/move
    Logger(const Logger&) = delete;
    Logger& operator=(const Logger&) = delete;

    // Configuration
    void setLogFile(const std::string& filepath);
    void setMinLevel(LogLevel level);
    void setConsoleOutput(bool enabled);

    // Logging methods
    void log(LogLevel level, const std::string& message, 
             const std::string& file = "", int line = 0);

    void debug(const std::string& msg, const std::string& file = "", int line = 0);
    void info(const std::string& msg, const std::string& file = "", int line = 0);
    void warn(const std::string& msg, const std::string& file = "", int line = 0);
    void error(const std::string& msg, const std::string& file = "", int line = 0);

private:
    Logger();
    ~Logger();

    std::string levelToString(LogLevel level) const;
    std::string getTimestamp() const;

    std::ofstream  logFile_;
    LogLevel       minLevel_;
    bool           consoleOutput_;
    std::mutex     mutex_;
};

// ─── Convenience macros ─────────────────────────────────────────────────────
#ifdef RUNTIME_DEBUG
    #define LOG_DEBUG(msg) Logger::instance().debug(msg, __FILE__, __LINE__)
#else
    #define LOG_DEBUG(msg) ((void)0)
#endif

#define LOG_INFO(msg)  Logger::instance().info(msg, __FILE__, __LINE__)
#define LOG_WARN(msg)  Logger::instance().warn(msg, __FILE__, __LINE__)
#define LOG_ERROR(msg) Logger::instance().error(msg, __FILE__, __LINE__)
