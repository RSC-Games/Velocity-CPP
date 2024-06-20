#include "include/logger.h"

int main() {
    using namespace Velocity;
    LogDebug("Hello, world");
    LogInfo("Hello, world");
    LogWarning("This is meant to be a warning!");
    LogError("This, on the other hand, is meant to be an error");
    LogFatal("Omg, I'm going to die");

    return 0;
}
