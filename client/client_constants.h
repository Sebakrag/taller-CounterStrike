#ifndef CLIENTCONSTANTS_H
#define CLIENTCONSTANTS_H

#include <string>


// Paths
#define PLAYER_PATH std::string("client/assets/player")
#define TILE_SETS_PATH std::string("client/assets/tiles")
#define WEAPON_PATH std::string("client/assets/weapons")
#define HUD_PATH std::string("client/assets/hud")

#define PHOENIX_IMG (PLAYER_PATH + "/t_phoenix_spritesheet.png")
#define L377_KREW_IMG (PLAYER_PATH + "/t_L377_krew_spritesheet.png")
#define ARTIC_AVENGER_IMG (PLAYER_PATH + "/t_artic_avenger_spritesheet_.png")
#define GUERRILLA_IMG (PLAYER_PATH + "/t_guerrilla_spritesheet.png")
#define SEAL_FORCE_IMG (PLAYER_PATH + "/ct_seal_force_spritesheet.png")
#define GERMAN_GSG_9_IMG (PLAYER_PATH + "/ct_german_csg_9_spritesheet.png")
#define UK_SAS_IMG (PLAYER_PATH + "/ct_uk_sas_spritesheet.png")
#define FRENCH_GIGN_IMG (PLAYER_PATH + "/ct_french_gign_spritesheet.png")

// Weapons
#define AK47_IMG (WEAPON_PATH + "/ak47_spritesheet.png")
#define AWP_IMG (WEAPON_PATH + "/awp_spritesheet.png")
#define GLOCK_IMG (WEAPON_PATH + "/glock_spritesheet.png")
#define KNIFE_IMG (WEAPON_PATH + "/knife_spritesheet.png")
#define M3_IMG (WEAPON_PATH + "/m3_spritesheet.png")
#define BULLET_IMG (WEAPON_PATH + "/bullet.bmp")

// weapons Version2. Estos sprites usa blending para suavizar los bordes con el fondo.
#define AK47_IMG_2 (WEAPON_PATH + "/ak47_spritesheet (42x64).png")
#define AWP_IMG_2 (WEAPON_PATH + "/awp_spritesheet (42x64).png")
#define GLOCK_IMG_2 (WEAPON_PATH + "/glock_spritesheet (32x64).png")
#define KNIFE_IMG_2 (WEAPON_PATH + "/knife_spritesheet.png")
#define M3_IMG_2 (WEAPON_PATH + "/m3_spritesheet (32x64).png")
#define BULLET_IMG_2 (WEAPON_PATH + "/bullet (32x32).png")
#define BOMB_IMG_2 (WEAPON_PATH + "/bomb_spritesheet (32x64).png")
#define BOMB_PLANTED_IMG_2 (WEAPON_PATH + "/bomb_planted_spritesheet (32x128).png")

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
#define SPRITE_SIZE 32        // in pixels
#define SPRITE_LONG_WIDTH 42  // in pixels para los sprites de weapons mas largas
#define RENDER_SCALAR 1       // va a dibujar los sprites al doble de su tamaño original


#endif  // CLIENTCONSTANTS_H
