#ifndef CONSTANTS_PROTOCOL_H_
#define CONSTANTS_PROTOCOL_H_

#define BYTE_INIT_MSG 0X01

// codigos de los enums

// Weapon { None, Glock, Ak47, M3, Awp };
#define BYTE_WEAPON_NONE 0x00
#define BYTE_WEAPON_GLOCK 0X01
#define BYTE_WEAPON_AK_47 0X02
#define BYTE_WEAPON_M3 0x03
#define BYTE_WEAPON_AWP 0x04

// TypeWeapon { Primary, Secondary, Knife, Bomb };
#define BYTE_TYPE_PRIMARY 0X11
#define BYTE_TYPE_SECONDARY 0x12
#define BYTE_TYPE_KNIFE 0X13
#define BYTE_TYPE_BOMB 0x14

// Team { Terrorist, CounterTerrorist };
#define BYTE_TEAM_TERRORIST 0x01
#define BYTE_TEAM_COUNTERTERRORIST 0x02

// GamePhase { Preparation, Combat, EndOfMatch };
#define BYTE_PHASE_PREPARATION 0x20
#define BYTE_PHASE_COMBAT 0x21
#define BYTE_PHASE_END_OF_MATCH 0x22

// PlayerState { Idle, Walking, Attacking, TakingDamage, PickingUp, Dead };
#define BYTE_STATE_IDLE 0xE1
#define BYTE_STATE_WALKING 0xE2
#define BYTE_STATE_ATTACKING 0xE3
#define BYTE_STATE_TAKING_DAMAGE 0xE4
#define BYTE_STATE_PICKING_UP 0xE5
#define BYTE_STATE_DEAD 0xE6

// PlayerSkin { ... }
#define BYTE_SKIN_TERRORIST_1 0x01
#define BYTE_SKIN_TERRORIST_2 0x02
#define BYTE_SKIN_TERRORIST_3 0x03
#define BYTE_SKIN_TERRORIST_4 0x04
#define BYTE_SKIN_COUNTERTERRORIST_1 0x05
#define BYTE_SKIN_COUNTERTERRORIST_2 0x06
#define BYTE_SKIN_COUNTERTERRORIST_3 0x07
#define BYTE_SKIN_COUNTERTERRORIST_4 0x08


// TypeItem { Coin, Glock, Ak47, M3, Awp, Bomb };
#define BYTE_ITEM_COIN 0x31
#define BYTE_ITEM_GLOCK 0x32
#define BYTE_ITEM_AK47 0x33
#define BYTE_ITEM_M3 0x34
#define BYTE_ITEM_AWP 0x35
#define BYTE_ITEM_BOMB 0x36

// SpriteType { ... }
#define BYTE_SPRITE_ARTIC_AVENGER 0x01
#define BYTE_SPRITE_L337_KREW 0x02
#define BYTE_SPRITE_GUERRILLA 0x03
#define BYTE_SPRITE_PHOENIX 0x04
#define BYTE_SPRITE_SEAL_FORCE 0x05
#define BYTE_SPRITE_GERMAN_GSG_9 0x06
#define BYTE_SPRITE_UK_SAS 0x07
#define BYTE_SPRITE_FRENCH_GIGN 0x08


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
#define BYTE_ROTATE 0X08

// Acciones del servidor:
//--------------------------
#define BYTE_OK 0X06
#define BYTE_FAIL 0X15

#define BYTE_TRUE 0X01
#define BYTE_FALSE 0X00

#define BYTE_TILE_MAP 0xC0
#define BYTE_MATCH_LIST 0xC1    // server envía al cliente
#define BYTE_PLAYERS_LIST 0xC2  // server envía al cliente
#define BYTE_GAME_INFO 0xC3

#endif  // CONSTANTS_PROTOCOL_H_
