#ifndef CONSTANTS_PROTOCOL_H_
#define CONSTANTS_PROTOCOL_H_

#define BYTE_INIT_MSG 0X01

#define BYTE_WEAPON_NONE 0x00
#define BYTE_WEAPON_GLOCK 0X01
#define BYTE_WEAPON_AK_47 0X02
#define BYTE_WEAPON_M3 0x03
#define BYTE_WEAPON_AWP 0x04

#define BYTE_TYPE_PRIMARY 0X11
#define BYTE_TYPE_SECONDARY 0x12
#define BYTE_TYPE_KNIFE 0X13
#define BYTE_TYPE_BOMB 0x14

#define BYTE_TEAM_TERRORIST 0x01
#define BYTE_TEAM_COUNTERTERRORIST 0x02

// Acciones del cliente:
//--------------------------
// MenuActions
#define BYTE_CREATE_MATCH 0XA1
#define BYTE_JOIN_MATCH 0XA2
#define BYTE_LIST_MATCHS 0XA3  // cliente pide al server
#define BYTE_EXIT 0XF0

// LobbyActions
#define BYTE_QUIT_MATCH 0XF1
#define BYTE_START_MATCH 0XB1
#define BYTE_LIST_PLAYERS 0XB2

// GameActions
#define BYTE_BUY_WEAPON 0X02
#define BYTE_BUY_AMMO 0X03
#define BYTE_ATTACK 0X04
#define BYTE_WALK 0X05
#define BYTE_CHANGE_WEAPON 0X06
#define BYTE_PICK_UP 0X07

// Acciones del servidor:
//--------------------------
#define BYTE_OK 0X06
#define BYTE_FAIL 0X15

#define BYTE_TILE_MAP 0xC0
#define BYTE_MATCH_LIST 0xC1    // server envía al cliente
#define BYTE_PLAYERS_LIST 0xC2  // server envía al cliente
#define BYTE_GAME_INFO 0xC3

#endif  // CONSTANTS_PROTOCOL_H_
