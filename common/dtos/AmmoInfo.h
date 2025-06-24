#ifndef AMMOINFO_H
#define AMMOINFO_H

struct AmmoInfo {
    int ammoLoaded;
    int totalAmmo;
    AmmoInfo() = default;
    AmmoInfo(const int ammoLoaded, const int totalAmmo):
            ammoLoaded(ammoLoaded), totalAmmo(totalAmmo) {}
};

#endif  // AMMOINFO_H
