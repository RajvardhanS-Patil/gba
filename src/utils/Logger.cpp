// ═══════════════════════════════════════════════════════════════════════════
// RUNTIME: The Last Algorithm
// Logger — Implementation
// ═══════════════════════════════════════════════════════════════════════════

#include "utils/Logger.h"

Logger& Logger::instance() {
    static Logger instance;
    return instance;
}

Logger::Logger()
    : minLevel_(LogLevel::INFO)
    , consoleOutput_(true)
{
}

Logger::~Logger() {
    if (logFile_.is_open()) {
        logFile_.close();
    }
}

void Logger::setLogFile(const std::string& filepath) {
    std::lock_guard<std::mutex> lock(mutex_);
    if (logFile_.is_open()) {
        logFile_.close();
    }
    logFile_.open(filepath, std::ios::app);
    if (!logFile_.is_open()) {
        std::cerr << "[Logger] Failed to open log file: " << filepath << std::endl;
    }
}

void Logger::setMinLevel(LogLevel level) {
    std::lock_guard<std::mutex> lock(mutex_);
    minLevel_ = level;
}

void Logger::setConsoleOutput(bool enabled) {
    std::lock_guard<std::mutex> lock(mutex_);
    consoleOutput_ = enabled;
}

void Logger::log(LogLevel level, const std::string& message,
                 const std::string& file, int line) {
    if (level < minLevel_) return;

    std::lock_guard<std::mutex> lock(mutex_);

    std::string timestamp = getTimestamp();
    std::string levelStr = levelToString(level);

    std::ostringstream oss;
    oss << "[" << timestamp << "] [" << levelStr << "] " << message;

    if (!file.empty()) {
        // Extract just the filename from the full path
        size_t pos = file.find_last_of("/\\");
        std::string filename = (pos != std::string::npos) ? file.substr(pos + 1) : file;
        oss << " (" << filename << ":" << line << ")";
    }

    std::string formatted = oss.str();

    if (consoleOutput_) {
        if (level == LogLevel::ERROR_LOG) {
            std::cerr << formatted << std::endl;
        } else {
            std::cout << formatted << std::endl;
        }
    }

    if (logFile_.is_open()) {
        logFile_ << formatted << std::endl;
        logFile_.flush();
    }
}

void Logger::debug(const std::string& msg, const std::string& file, int line) {
    log(LogLevel::DEBUG, msg, file, line);
}

void Logger::info(const std::string& msg, const std::string& file, int line) {
    log(LogLevel::INFO, msg, file, line);
}

void Logger::warn(const std::string& msg, const std::string& file, int line) {
    log(LogLevel::WARN, msg, file, line);
}

void Logger::error(const std::string& msg, const std::string& file, int line) {
    log(LogLevel::ERROR_LOG, msg, file, line);
}

std::string Logger::levelToString(LogLevel level) const {
    switch (level) {
        case LogLevel::DEBUG:     return "DEBUG";
        case LogLevel::INFO:      return "INFO ";
        case LogLevel::WARN:      return "WARN ";
        case LogLevel::ERROR_LOG: return "ERROR";
        default:                  return "?????";
    }
}

std::string Logger::getTimestamp() const {
    auto now = std::chrono::system_clock::now();
    auto time = std::chrono::system_clock::to_time_t(now);
    auto ms = std::chrono::duration_cast<std::chrono::milliseconds>(
        now.time_since_epoch()) % 1000;

    std::ostringstream oss;
    oss << std::put_time(std::localtime(&time), "%H:%M:%S");
    oss << "." << std::setfill('0') << std::setw(3) << ms.count();
    return oss.str();
}
