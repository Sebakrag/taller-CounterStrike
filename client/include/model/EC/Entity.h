#ifndef ENTITY_H
#define ENTITY_H

#include <cstdint>

using Entity = uint32_t;
constexpr Entity INVALID_ENTITY = UINT32_MAX;
constexpr size_t MAX_ENTITIES = 1000;  // TODO: Is it enough? Be careful!

#endif  // ENTITY_H
