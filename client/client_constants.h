#ifndef CLIENTCONSTANTS_H
#define CLIENTCONSTANTS_H

#include <string>

// Window config (esto debemos recibirlo desde el server ya que esta definido en el archivo de
// config del server)
#define SCREEN_WIDTH 640
#define SCREEN_HEIGHT 400

// Paths
#define PLAYER_PATH std::string("client/assets/player")
#define TILE_SETS_PATH std::string("client/assets/tiles")
#define WEAPON_PATH std::string("client/assets/weapons")
#define HUD_PATH std::string("client/assets/hud")

#define PHOENIX_IMG (PLAYER_PATH + "/t_phoenix.bmp")
#define L377_KREW_IMG (PLAYER_PATH + "/t_L377_krew.bmp")
#define ARTIC_AVENGER_IMG (PLAYER_PATH + "/t_artic_avenger.bmp")
#define GUERRILLA_IMG (PLAYER_PATH + "/t_guerrilla.bmp")
#define SEAL_FORCE_IMG (PLAYER_PATH + "/ct_seal_force.bmp")
#define GERMAN_GSG_9_IMG (PLAYER_PATH + "/ct_german_csg_9.bmp")
#define UK_SAS_IMG (PLAYER_PATH + "/ct_uk_sas.bmp")
#define FRENCH_GIGN_IMG (PLAYER_PATH + "/ct_french_gign.bmp")

// Weapons
#define AK47_IMG (WEAPON_PATH + "/ak47_spritesheet.png")
#define AWP_IMG (WEAPON_PATH + "/awp_spritesheet.png")
#define GLOCK_IMG (WEAPON_PATH + "/glock_spritesheet.png")
#define KNIFE_IMG (WEAPON_PATH + "/knife_spritesheet.bmp")
#define M3_IMG (WEAPON_PATH + "/m3_spritesheet.png")

// Maps
#define DESERT_TILE_SET_IMG (TILE_SETS_PATH + "/tileset_desierto_(4x11).png")
#define AZTEC_TILE_SET_IMG (TILE_SETS_PATH + "/aztec_tiles.png")
#define TRAINING_TILE_SET_IMG (TILE_SETS_PATH + "/training_tiles.png")

// HUD:
#define HUD_NUMS (HUD_PATH + "/hud_nums.bmp")
#define HUD_SYMB (HUD_PATH + "/hud_symbols.bmp")

// Map
#define TILE_SIZE 32  // in pixels

// Sprites
#define SPRITE_SIZE 32   // in pixels
#define RENDER_SCALAR 1  // va a dibujar los sprites al doble de su tamaño original

// Stencil (Field Of View)
#define FOV_CIRCLE_RADIUS \
    32                // in pixels (before assigning a value, consider the player sprite size)
#define FOV_ANGLE 40  // in degrees
#define PERCENTAGE_OF_TRANSPARENCY 0.75
#define STENCIL_ALPHA static_cast<int>(255 * PERCENTAGE_OF_TRANSPARENCY)


#endif  // CLIENTCONSTANTS_H
