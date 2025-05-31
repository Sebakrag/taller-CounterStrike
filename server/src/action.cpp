#include "../include/action.h"

Action::Action(const ActionType type, std::map<std::string, int> params):
        type(type), params(std::move(params)) {}

ActionType Action::getType() const { return type; }

int Action::getParam(const std::string& key, const int defaultValue) const {
    const auto it = params.find(key);
    return (it != params.end()) ? it->second : defaultValue;
}
