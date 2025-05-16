#ifndef WINDOWCONFIG_H
#define WINDOWCONFIG_H

#include <cstdint>

struct window_config_t {
private:
    const int widht;
    const int height;
    const uint32_t flags;

public:
    window_config_t(int w, int h, uint32_t flags);

    int get_widht() const;
    int get_height() const;
    uint32_t get_flags() const;
};

#endif  // WINDOWCONFIG_H
