#include "server/include/id_generator.h"

uint32_t IdGenerator::nextId = 1;
std::set<uint32_t> IdGenerator::freeIds;

uint32_t IdGenerator::getNextId() {
    if (!freeIds.empty()) {
        auto it = freeIds.begin();
        uint32_t id = *it;
        freeIds.erase(it);
        return id;
    }
    return nextId++;
}

void IdGenerator::releaseId(uint32_t id) { freeIds.insert(id); }
