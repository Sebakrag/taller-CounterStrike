#ifndef WINDOWCONFIG_H
#define WINDOWCONFIG_H

#include <cstdint>

struct WindowConfig {
public:
    const int width;
    const int height;
    const uint32_t flags;

    WindowConfig(const int w, const int h, const uint32_t flags):
            width(w), height(h), flags(flags) {}
};

#endif  // WINDOWCONFIG_H
