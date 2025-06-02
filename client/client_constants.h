#ifndef CLIENTCONSTANTS_H
#define CLIENTCONSTANTS_H

#include <string>

// Window config (esto debemos recibirlo desde el server ya que esta definido en el archivo de
// config del server)
#define SCREEN_WIDTH 640
#define SCREEN_HEIGHT 400

// Paths

#define PLAYER_PATH std::string("../client/assets/player")
#define TILE_SETS_PATH std::string("../client/assets/tiles")

#define PHOENIX_IMG (PLAYER_PATH + "/t_phoenix.bmp")
#define L377_KREW_IMG (PLAYER_PATH + "/t_L377_krew.bmp")
#define ARTIC_AVENGER_IMG (PLAYER_PATH + "/t_artic_avenger.bmp")
#define GUERRILLA_IMG (PLAYER_PATH + "/t_guerrilla.bmp")
#define SEAL_FORCE_IMG (PLAYER_PATH + "/ct_seal_force.bmp")
#define GERMAN_GSG_9_IMG (PLAYER_PATH + "/ct_german_csg_9.bmp")
#define UK_SAS_IMG (PLAYER_PATH + "/ct_uk_sas.bmp")
#define FRENCH_GIGN_IMG (PLAYER_PATH + "/ct_french_gign.bmp")

#define DESERT_TILE_SET_IMG (TILE_SETS_PATH + "/desert_tiles.png")
#define AZTEC_TILE_SET_IMG (TILE_SETS_PATH + "/aztec_tiles.png")
#define TRAINING_TILE_SET_IMG (TILE_SETS_PATH + "/training_tiles.png")

// Map
#define TILE_SIZE 32  // in pixels

#endif  // CLIENTCONSTANTS_H
