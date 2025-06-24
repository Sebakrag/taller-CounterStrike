#ifndef ACTION_H
#define ACTION_H

#include <map>
#include <string>

#include "enums/action_type.h"

class Action {
private:
    ActionType type;
    std::map<std::string, int> params;

public:
    explicit Action(const ActionType type, std::map<std::string, int> params = {});

    ActionType getType() const;
    int getParam(const std::string& key, const int defaultValue = 0) const;
};

#endif
