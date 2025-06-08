#ifndef WINDOWCONFIG_H
#define WINDOWCONFIG_H

#include <cstdint>

struct WindowConfig {
    int width;   // en px
    int height;  // en px
    uint32_t flags;

    WindowConfig() {}

    WindowConfig(const int w, const int h, const uint32_t flags):
            width(w), height(h), flags(flags) {}
};

#endif  // WINDOWCONFIG_H
