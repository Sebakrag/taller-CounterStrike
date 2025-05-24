#ifndef WEAPON_H
#define WEAPON_H

class Weapon {
protected:
    int damage;

public:
    explicit Weapon(const int damage) : damage(damage) {}
    virtual ~Weapon() = default;

    virtual int use() = 0;
};

#endif
