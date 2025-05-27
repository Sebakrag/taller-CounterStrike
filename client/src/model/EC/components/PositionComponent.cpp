#include "client/include/model/EC/components/PositionComponent.h"

PositionComponent::PositionComponent(const float x, const float y): position(x, y) {}

void PositionComponent::init(const float x, const float y) { position.set(x, y); }

void PositionComponent::move(const Vec2D& dir) { position += dir; }

Vec2D PositionComponent::getPosition() const { return position; }
