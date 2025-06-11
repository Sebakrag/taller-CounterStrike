#ifndef ID_GENERATOR_H
#define ID_GENERATOR_H

#include <cstdint>
#include <set>

class IdGenerator {
private:
    static uint32_t nextId;
    static std::set<uint32_t> freeIds;

public:
    static uint32_t getNextId();
    static void releaseId(uint32_t id);
};

#endif
