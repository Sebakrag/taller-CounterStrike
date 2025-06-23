#ifndef SHOP_H
#define SHOP_H

#include <vector>

#include <SDL2pp/SDL2pp.hh>

#include "client/include/client.h"  // TODO : NO deberia tener contecto directo con client.
#include "common/types.h"

using SDL2pp::Color;
using SDL2pp::Font;
using SDL2pp::Rect;
using SDL2pp::Texture;

class Graphics;

class Shop {
private:
    struct WeaponItem {
        Weapon weapon;
        // std::string label;
        int price;
        Texture texture;
        Rect rect;
    };

    struct AmmoItem {
        AmmoType weapon;
        // std::string label;
        int price;
        Texture texture;
        Rect rect;
    };

    std::vector<WeaponItem> weapons;
    std::vector<AmmoItem> ammos;

    bool selectedPrimaryAmmo = false;
    bool selectedSecondaryAmmo = false;

    Texture buyButtonTexture;
    Rect buyButtonRect;

    Rect primaryAmmoBtn;
    Rect secondaryAmmoBtn;
    Rect buyBtn;

    int selectedItemIdx = -1;

    Graphics& graphics;

    void createWeaponItems(const ShopInfo& shopInfo, const Font& font);
    void createAmmoItems(const ShopInfo& shopInfo, const Font& font);
    Texture createText(const Font& font, const std::string& text, const Color& color);

public:
    Shop(Graphics& graphics, const ShopInfo& shop_info, const std::string& font_filename,
         int font_size);

    void handleEvent(const SDL_Event& e, Client& client);
    void render();
    void reset();
};

#endif  // SHOP_H
