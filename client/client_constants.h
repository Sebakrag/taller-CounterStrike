#ifndef CLIENTCONSTANTS_H
#define CLIENTCONSTANTS_H

#include <string>


// Paths (graphics)
#define PLAYER_PATH std::string("client/assets/gfx/player")
#define TILE_SETS_PATH std::string("client/assets/gfx/tiles")
#define WEAPON_PATH std::string("client/assets/gfx/weapons")
#define HUD_PATH std::string("client/assets/gfx/hud")
#define FONT_PATH std::string("client/assets/gfx/fonts")

// Paths (sounds)
#define PLAYER_SOUNDS_PATH std::string("client/assets/sfx/player")
#define RADIO_SOUNDS_PATH std::string("client/assets/sfx/radio")
#define WEAPON_SOUNDS_PATH std::string("client/assets/sfx/weapon")
#define MUSIC_SOUNDS_PATH std::string("client/assets/sfx/music")

// Player skins
#define RANDOM_PLAYER_IMG (PLAYER_PATH + "/randomplayer.png")
#define PHOENIX_IMG (PLAYER_PATH + "/t_phoenix_spritesheet.png")
#define L377_KREW_IMG (PLAYER_PATH + "/t_L377_krew_spritesheet.png")
#define ARTIC_AVENGER_IMG (PLAYER_PATH + "/t_artic_avenger_spritesheet.png")
#define GUERRILLA_IMG (PLAYER_PATH + "/t_guerrilla_spritesheet.png")
#define SEAL_FORCE_IMG (PLAYER_PATH + "/ct_seal_force_spritesheet.png")
#define GERMAN_GSG_9_IMG (PLAYER_PATH + "/ct_german_csg_9_spritesheet.png")
#define UK_SAS_IMG (PLAYER_PATH + "/ct_uk_sas_spritesheet.png")
#define FRENCH_GIGN_IMG (PLAYER_PATH + "/ct_french_gign_spritesheet.png")

// Weapons skin
#define AK47_IMG (WEAPON_PATH + "/ak47_spritesheet.png")
#define AWP_IMG (WEAPON_PATH + "/awp_spritesheet.png")
#define GLOCK_IMG (WEAPON_PATH + "/glock_spritesheet.png")
#define KNIFE_IMG (WEAPON_PATH + "/knife_spritesheet.png")
#define M3_IMG (WEAPON_PATH + "/m3_spritesheet.png")
#define BULLET_IMG (WEAPON_PATH + "/bullet.bmp")
#define BOMB_IMG (WEAPON_PATH + "/bomb_spritesheet.png")

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
#define DESERT_TILE_SET_IMG (TILE_SETS_PATH + "/desert_tiles.png")
#define AZTEC_TILE_SET_IMG (TILE_SETS_PATH + "/aztec_tiles.png")
#define TRAINING_TILE_SET_IMG (TILE_SETS_PATH + "/training_tiles.png")

// HUD
#define HUD_NUMS (HUD_PATH + "/hud_nums.bmp")
#define HUD_SYMB (HUD_PATH + "/hud_symbols.bmp")

// Map
#define TILE_SIZE 32  // in pixels

// Sprites
#define SPRITE_SIZE 32        // in pixels
#define SPRITE_LONG_WIDTH 42  // in pixels para los sprites de weapons mas largas
#define RENDER_SCALAR 1       // va a dibujar los sprites al doble de su tamaño original


// Sounds
// Player (sounds)
// #define STEP_SOUND (PLAYER_SOUNDS_PATH + "/dirt_walk_step.wav")
#define STEP_SOUND (PLAYER_SOUNDS_PATH + "/dirt_walk_two_steps.wav")
#define PICKUP_SOUND (PLAYER_SOUNDS_PATH + "/pickup.wav")
#define DROP_WEAPON_SOUND (PLAYER_SOUNDS_PATH + "/drop.wav")
#define CHANGE_WEAPON_SOUND \
    (PLAYER_SOUNDS_PATH + "/pickup.wav")  // Changing and picking up a weapon produce the same sound
#define SHOOT_SOUND (WEAPON_SOUNDS_PATH + "/glock18.wav")  // TODO: Esto estaria de mas
#define TAKE_DAMAGE_SOUND (PLAYER_SOUNDS_PATH + "/hit.wav")
#define DIE_SOUND (PLAYER_SOUNDS_PATH + "/die.wav")


// Weapon (sounds)
// #define KNIFE_HIT_SOUND (WEAPON_SOUNDS_PATH + "/knife_hit.wav")
#define KNIFE_SLASH_SOUND (WEAPON_SOUNDS_PATH + "/knife_slash.wav")
#define GLOCK_SOUND (WEAPON_SOUNDS_PATH + "/glock18.wav")
#define AK47_SOUND (WEAPON_SOUNDS_PATH + "/ak47.wav")
#define AWP_SOUND (WEAPON_SOUNDS_PATH + "/awp.wav")
#define M3_SOUND (WEAPON_SOUNDS_PATH + "/m3.wav")
#define BOMB_BEEP_SOUND (WEAPON_SOUNDS_PATH + "/bomb_beep.wav")
#define BOMB_EXPLOSION_SOUND (WEAPON_SOUNDS_PATH + "/bomb_explosion.wav")

// Radio (sounds)
#define BOMB_PLANTED_SOUND \
    (RADIO_SOUNDS_PATH + "/bombpl.ogg")  // Mas que un sound es un mensaje que dice la radio
#define BOMB_DEFUSED_SOUND (RADIO_SOUNDS_PATH + "/bombdef.ogg")
#define GO_SOUND (RADIO_SOUNDS_PATH + "/go.ogg")

// Shop
#define SHOP_FONT_FILE_NAME (FONT_PATH + "/OpenSans-Light.ttf")
#define SHOP_FONT_SIZE 16
#define SHOP_MUSIC_FILE_NAME (MUSIC_SOUNDS_PATH + "/unrealsoftware.wav")


#endif  // CLIENTCONSTANTS_H
