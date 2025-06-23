#include "client/include/model/Shop.h"

#include "client/include/model/Graphics.h"

using SDL2pp::NullOpt;

Shop::Shop(Graphics& graphics, const ShopInfo& shop_info, const std::string& font_filename,
           const int font_size):
        graphics(graphics) {
    const Font font(font_filename, font_size);
    createWeaponItems(shop_info, font);
    createAmmoItems(shop_info, font);
}

void Shop::handleEvent(const SDL_Event& e, Client& client) {
    if (e.type == SDL_MOUSEBUTTONDOWN && e.button.button == SDL_BUTTON_LEFT) {
        int x = e.button.x, y = e.button.y;

        for (size_t i = 0; i < items.size(); ++i) {
            if (items[i].rect.Contains(x, y)) {
                selectedItemIdx = i;
                for (size_t j = 0; j < items.size(); ++j) items[j].selected = (j == i);
                return;
            }
        }

        if (primaryAmmoBtn.Contains(x, y)) {
            selectedPrimaryAmmo = !selectedPrimaryAmmo;
        } else if (secondaryAmmoBtn.Contains(x, y)) {
            selectedSecondaryAmmo = !selectedSecondaryAmmo;
        } else if (buyBtn.Contains(x, y)) {
            if (selectedItemIdx != -1) {
                client.sendBuyWeapon(items[selectedItemIdx].weapon);
            }
            if (selectedPrimaryAmmo)
                client.sendBuyAmmo(AmmoType::Primary);
            if (selectedSecondaryAmmo)
                client.sendBuyAmmo(AmmoType::Secondary);
            reset();
        }
    }
}

void Shop::render() {
    for (auto& item: weapons) {
        graphics.draw(item.texture, NullOpt, item.rect);
    }
    for (const auto& item: ammos) {
        graphics.draw(item.texture, NullOpt, item.rect);
    }
    SDL_RenderCopy(renderer, buyButtonTexture, nullptr, &buyButtonRect);
}

// void Shop::render() {
//     for (const auto& [weapon, price, rect, selected] : items) {
//         Color fill = selected ? Color(0, 128, 0, 255) : Color(64, 64, 64, 255);
//         renderer.SetDrawColor(fill);
//         renderer.FillRect(rect);
//         Texture text(renderer, font.RenderText_Blended("Buy: " + getWeaponName(weapon) + " ($" +
//         std::to_string(price) + ")", textColor)); renderer.Copy(text, SDL2pp::NullOpt, rect);
//     }
//
//     auto drawButton = [&](Rect& rect, const std::string& label, bool selected) {
//         Color fill = selected ? Color(0, 128, 128, 255) : Color(64, 64, 64, 255);
//         renderer.SetDrawColor(fill);
//         renderer.FillRect(rect);
//         SDL2pp::Texture text(renderer, font.RenderText_Blended(label, textColor));
//         renderer.Copy(text, SDL2pp::NullOpt, rect);
//     };
//
//     drawButton(primaryAmmoBtn, "Primary Ammo", selectedPrimaryAmmo);
//     drawButton(secondaryAmmoBtn, "Secondary Ammo", selectedSecondaryAmmo);
//     drawButton(buyBtn, "Comprar", false);
// }

void Shop::reset() {
    selectedItemIdx = -1;
    for (auto& item: items) item.selected = false;
    selectedPrimaryAmmo = false;
    selectedSecondaryAmmo = false;
}

// helper (añadir en utils si hace falta)
std::string getWeaponName(Weapon w) {
    switch (w) {
        case Weapon::Ak47:
            return "AK-47";
        case Weapon::Awp:
            return "AWP";
        case Weapon::M3:
            return "M3";
        default:
            return "Unknown";
    }
}

void Shop::createWeaponItems(const ShopInfo& shopInfo, const Font& font) {
    const auto weaponPrices = shopInfo.weaponPrices;
    weapons.reserve(weaponPrices.size());

    int x = 100, y = 100;
    for (const auto& [w, price]: weaponPrices) {
        weapons.push_back({w, price, Rect(x, y, 150, 40)});
        y += 50;
    }

    for (auto& item: weapons) {
        item.texture = createText(renderer, font, item.label, {255, 255, 255, 255});
        int w, h;
        SDL_QueryTexture(item.texture, nullptr, nullptr, &w, &h);
        item.rect = {x, y, w, h};
        y += h + 10;
    }
}

void Shop::createAmmoItems(const ShopInfo& shopInfo, const Font& font) {
    ammos = {{AmmoType::Primary, "Primary Ammo - $200", 200},
             {AmmoType::Secondary, "Secondary Ammo - $100", 100}};

    int x = 400, y = 100;
    for (auto& item: ammos) {
        item.texture = createText(renderer, font, item.label, {255, 255, 255, 255});
        int w, h;
        SDL_QueryTexture(item.texture, nullptr, nullptr, &w, &h);
        item.rect = {x, y, w, h};
        y += h + 10;
    }
}
// SDL_Texture* Shop::createText(SDL_Renderer& renderer, TTF_Font& font, const std::string& text,
// SDL_Color color) {
Texture createText(const Font& font, const std::string& text, const Color& color) {
    SDL_Surface* surface = TTF_RenderText_Blended(&font, text.c_str(), color);
    SDL_Texture* texture = SDL_CreateTextureFromSurface(&renderer, surface);
    SDL_FreeSurface(surface);
    return texture;
}
