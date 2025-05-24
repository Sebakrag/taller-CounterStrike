#ifndef ACTION_H
#define ACTION_H

#include "../enums/action_type.h"
#include <map>
#include <string>

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
