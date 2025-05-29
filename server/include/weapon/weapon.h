#ifndef WEAPON_H
#define WEAPON_H

class Weapon_ {
protected:
    int damage;

public:
    explicit Weapon_(const int damage): damage(damage) {}
    virtual ~Weapon_() = default;

    virtual int use() = 0;
};

#endif
