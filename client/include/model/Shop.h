#ifndef SHOP_H
#define SHOP_H

#include <memory>
#include <string>
#include <vector>

#include <SDL2pp/SDL2pp.hh>

#include "common/game_info/shop_info.h"
#include "common/types.h"

using SDL2pp::Color;
using SDL2pp::Font;
using SDL2pp::Rect;
using SDL2pp::Texture;

class Graphics;

enum class ShopSelection : unsigned char {
    None,
    WeaponSelected,
    PrimaryAmmoSelected,
    SecondaryAmmoSelected
};

class Shop {
private:
    struct WeaponItem {
        Weapon weapon;
        int price;
        std::unique_ptr<Texture> texture;
        Rect rect;
    };

    struct AmmoItem {
        AmmoType ammoType;
        int price;
        std::unique_ptr<Texture> texture;
        Rect rect;
    };

    Rect backgroundRect;

    std::vector<WeaponItem> weapons;
    std::vector<AmmoItem> ammos;

    // TODO: puedo crear una abstraccion Button.
    std::unique_ptr<Texture> buyButtonTexture;
    Rect buyButtonRect;

    bool selectedPrimaryAmmo = false;
    bool selectedSecondaryAmmo = false;
    int selectedItemIdx = -1;

    Graphics& graphics;

    void createWeaponItems(const ShopInfo& shopInfo, Font& font, int& currentY, int itemX);
    void createAmmoItems(const ShopInfo& shopInfo, Font& font, int& currentY, int itemX);
    void resetSelection();
    // TODO: esta deberia ser una funcion helper (en utils).
    std::string toString(Weapon w) const;
    std::string toString(AmmoType a) const;

public:
    Shop(Graphics& graphics, const ShopInfo& shop_info, const std::string& font_filename,
         int font_size);

    void render();
    void handleClick(int mouseX, int mouseY);
    ShopSelection getSelectionType() const;
    Weapon getSelectedWeapon() const;
    AmmoType getSelectedAmmoType() const;
    bool isBuyButtonClicked(int mouseX, int mouseY) const;
};

#endif  // SHOP_H
