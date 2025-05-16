#include "windowConfig.h"

window_config_t::window_config_t(int w, int h, uint32_t flags): widht(w), height(h), flags(flags) {}

int window_config_t::get_widht() const { return widht; }

int window_config_t::get_height() const { return height; }

uint32_t window_config_t::get_flags() const { return flags; }
