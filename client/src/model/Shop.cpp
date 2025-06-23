#include "client/include/model/Shop.h"

#include <sstream>

#include "client/include/model/Graphics.h"
#include "model/utils/TextureManager.h"

using SDL2pp::NullOpt;

namespace {
constexpr float MENU_WIDTH_RATIO = 0.4f;
constexpr float MENU_HEIGHT_RATIO = 0.6f;
constexpr int PADDING = 10;
constexpr Color SEMI_TRANSPARENT_BLACK = {0, 0, 0, 180};  // background color
constexpr Color WHITE = {255, 255, 255, 255};             // text color
constexpr Color BLUE_GREY = {35, 50, 65, 255};            // items color
constexpr Color SELECTED_COLOR = {85, 80, 50, 255};       // Brown color
constexpr Color DARK_GREEN = {45, 65, 25, 255};           // Buy button color
}  // namespace

Shop::Shop(Graphics& graphics, const ShopInfo& shop_info, const std::string& font_filename,
           const int font_size):
        graphics(graphics) {
    // Creo el rectangulo del fondo de la tienda.
    const Vec2D windowDimensions(graphics.getDrawableWindowDimension());
    const float winWidth = windowDimensions.getX();
    const float winHeight = windowDimensions.getY();
    const int menuWidth = static_cast<int>(winWidth * MENU_WIDTH_RATIO);
    const int menuHeight = static_cast<int>(winHeight * MENU_HEIGHT_RATIO);

    const int centerX = (winWidth - menuWidth) / 2;
    const int centerY = (winHeight - menuHeight) / 2;
    backgroundRect = Rect(centerX, centerY, menuWidth, menuHeight);

    // Posiciones relativas al fondo
    int currentY = backgroundRect.GetY() + PADDING;
    const int itemX = backgroundRect.GetX() + PADDING;

    Font font(font_filename, font_size);
    createWeaponItems(shop_info, font, currentY, itemX);
    createAmmoItems(shop_info, font, currentY, itemX);

    // Creo boton comprar.
    auto buyText = TextureManager::createTextTexture("Comprar", font, WHITE);
    auto [w, h] = buyText->GetSize();
    buyButtonRect = Rect(itemX, currentY + PADDING, w, h);
    buyButtonTexture = std::move(buyText);
}

void Shop::createWeaponItems(const ShopInfo& shopInfo, Font& font, int& currentY, const int itemX) {
    const auto weaponPrices = shopInfo.weaponPrices;
    weapons.reserve(weaponPrices.size());

    for (const auto& [weapon, price]: weaponPrices) {
        std::ostringstream oss;
        oss << toString(weapon) << " - $" << price;
        auto text = TextureManager::createTextTexture(oss.str(), font, WHITE);
        auto [w, h] = text->GetSize();
        const Rect rect(itemX, currentY, w,
                        h);  // TODO: si no queda bien el w y h puedo ponerle constantes.
        weapons.push_back({weapon, price, std::move(text), rect});
        currentY += h + PADDING;
    }
}

void Shop::createAmmoItems(const ShopInfo& shopInfo, Font& font, int& currentY, const int itemX) {
    const auto ammoPrices = shopInfo.ammoPrices;
    ammos.reserve(ammoPrices.size());

    for (const auto& [ammoType, price]: ammoPrices) {
        std::ostringstream oss;
        oss << toString(ammoType) << " - $" << price;
        auto text = TextureManager::createTextTexture(oss.str(), font, WHITE);
        auto [w, h] = text->GetSize();
        const Rect rect(itemX, currentY, w,
                        h);  // TODO: si no queda bien el w y h puedo ponerle las constantes.
        ammos.push_back({ammoType, price, std::move(text), rect});
        currentY += h + PADDING;
    }
}

void Shop::render() {
    graphics.fillRect(backgroundRect, SEMI_TRANSPARENT_BLACK);

    for (size_t i = 0; i < weapons.size(); ++i) {
        auto& item = weapons[i];
        Color color = (i == static_cast<size_t>(selectedItemIdx)) ? SELECTED_COLOR : BLUE_GREY;

        graphics.fillRect(item.rect, color);
        graphics.draw(*item.texture, NullOpt, item.rect);
    }

    for (const auto& item: ammos) {
        bool selected = (item.ammoType == AmmoType::Primary && selectedPrimaryAmmo) ||
                        (item.ammoType == AmmoType::Secondary && selectedSecondaryAmmo);
        Color color = selected ? SELECTED_COLOR : BLUE_GREY;

        graphics.fillRect(item.rect, color);
        graphics.draw(*item.texture, NullOpt, item.rect);
    }

    graphics.fillRect(buyButtonRect, DARK_GREEN);
    graphics.draw(*buyButtonTexture, NullOpt, buyButtonRect);
}

void Shop::handleClick(const int mouseX, const int mouseY) {
    resetSelection();

    // Check weapon click
    for (size_t i = 0; i < weapons.size(); ++i) {
        if (weapons[i].rect.Contains(mouseX, mouseY)) {
            selectedItemIdx = static_cast<int>(i);
            return;
        }
    }

    // Check ammo click
    for (const auto& ammo: ammos) {
        if (ammo.rect.Contains(mouseX, mouseY)) {
            if (ammo.ammoType == AmmoType::Primary)
                selectedPrimaryAmmo = true;
            else if (ammo.ammoType == AmmoType::Secondary)
                selectedSecondaryAmmo = true;
            return;
        }
    }
}

ShopSelection Shop::getSelectionType() const {
    if (selectedItemIdx >= 0)
        return ShopSelection::WeaponSelected;
    if (selectedPrimaryAmmo)
        return ShopSelection::PrimaryAmmoSelected;
    if (selectedSecondaryAmmo)
        return ShopSelection::SecondaryAmmoSelected;
    return ShopSelection::None;
}

Weapon Shop::getSelectedWeapon() const {
    if (selectedItemIdx >= 0 && selectedItemIdx < static_cast<int>(weapons.size()))
        return weapons[selectedItemIdx].weapon;
    return Weapon::None;
}

AmmoType Shop::getSelectedAmmoType() const {
    if (selectedPrimaryAmmo)
        return AmmoType::Primary;
    if (selectedSecondaryAmmo)
        return AmmoType::Secondary;
    return AmmoType::None;
}

bool Shop::isBuyButtonClicked(const int mouseX, const int mouseY) const {
    return buyButtonRect.Contains(mouseX, mouseY);
}

void Shop::resetSelection() {
    selectedItemIdx = -1;
    selectedPrimaryAmmo = false;
    selectedSecondaryAmmo = false;
}

std::string Shop::toString(const Weapon w) const {
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

std::string Shop::toString(const AmmoType a) const {
    switch (a) {
        case AmmoType::Primary:
            return "Primary ammo";
        case AmmoType::Secondary:
            return "Secondary ammo";
        default:
            return "Unknown";
    }
}
