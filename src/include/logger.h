#pragma once

namespace Velocity {

#define VELOCITY_COLOR_PRINT

enum class LogLevel { Debug, Info, Warning, Error, Fatal };

void LogDebug(const char *formatStr, ...);
void LogInfo(const char *formatStr, ...);
void LogWarning(const char *formatStr, ...);
void LogError(const char *formatStr, ...);
void LogFatal(const char *formatStr, ...);

} // namespace Velocity
