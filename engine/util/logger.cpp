#include <cstdarg>
#include <cstdio>
#include <cstdlib>
#include <sstream>

#include "logger.h"

namespace Velocity {
static LogLevel currentLevel = LogLevel::Debug;

void SetLogLevel(LogLevel level) { currentLevel = level; }

static void Logf(LogLevel level, const char *formatStr, va_list args) {
    if (level < currentLevel)
        return;

    std::stringstream ss;

#ifdef VELOCITY_COLOR_PRINT
    const char *ANSI_RED = "\033[0;31m";
    const char *ANSI_YELLOW = "\033[0;33m";
    const char *ANSI_BOLD_RED = "\033[1;31m";
    switch (level) {
        case LogLevel::Warning:
            printf("%s", ANSI_YELLOW);
            break;
        case LogLevel::Error:
            printf("%s", ANSI_RED);
            break;
        case LogLevel::Fatal:
            printf("%s", ANSI_BOLD_RED);
            break;
        default:
            break;
    }
#endif // VELOCITY_COLOR_PRINT

    switch (level) {
        case LogLevel::Debug:
            ss << "[DEBUG] ";
            break;
        case LogLevel::Info:
            ss << "[INFO] ";
            break;
        case LogLevel::Warning:
            ss << "[WARNING] ";
            break;
        case LogLevel::Error:
            ss << "[ERROR] ";
            break;
        case LogLevel::Fatal:
            ss << "[PANIC] ";
            break;
        default:
            break;
    }

    ss << formatStr << "\n";

    vprintf(ss.str().c_str(), args);
    fflush(stdout);

#ifdef VELOCITY_COLOR_PRINT
    const char *ANSI_RESET = "\033[0m";
    switch (level) {
        case LogLevel::Warning:
        case LogLevel::Error:
        case LogLevel::Fatal:
            printf("%s", ANSI_RESET);
            break;
        default:
            break;
    }
#endif // VELOCITY_COLOR_PRINT

    if (level == LogLevel::Fatal)
        std::exit(EXIT_FAILURE);
}

void LogDebug(const char *formatStr, ...) {
    va_list args;
    va_start(args, formatStr);
    Logf(LogLevel::Debug, formatStr, args);
    va_end(args);
}

void LogInfo(const char *formatStr, ...) {
    va_list args;
    va_start(args, formatStr);
    Logf(LogLevel::Info, formatStr, args);
    va_end(args);
}

void LogWarning(const char *formatStr, ...) {
    va_list args;
    va_start(args, formatStr);
    Logf(LogLevel::Warning, formatStr, args);
    va_end(args);
}

void LogError(const char *formatStr, ...) {
    va_list args;
    va_start(args, formatStr);
    Logf(LogLevel::Error, formatStr, args);
    va_end(args);
}

void LogFatal(const char *formatStr, ...) {
    va_list args;
    va_start(args, formatStr);
    Logf(LogLevel::Fatal, formatStr, args);
    va_end(args);
}

} // namespace Velocity
