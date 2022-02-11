//
// Created by Ilir on 2021-12-13.
//

#ifndef PROJECTMCODES_CROWDCONTROL_H
#define PROJECTMCODES_CROWDCONTROL_H


#include "Assembly.h"
#include "Debug.h"
#include "Wii/EXI/EXI.h"
#include "Memory.h"
#include "Wii/PAD/PADStatus.h"
#include "Brawl/GF/GameFrame.h"
#include "Brawl/FT/Fighter.h"
#include <Brawl/FT/ftManager.h>
#include "MenuPages.h"

enum CCEffect
{
    EFFECT_NOT_CONNECTED = 0,
    EFFECT_NONE = 1,
    EFFECT_UNKNOWN = 2,
    EFFECT_GIVE_DAMAGE = 3,
    EFFECT_GIVE_STOCKS = 4,
    EFFECT_GIVE_TIME = 5,
    EFFECT_ITEM_SPAWN_REGULAR = 6,
    EFFECT_ITEM_SPAWN_POKEMON = 7,
    EFFECT_ITEM_SPAWN_ASSIST = 8,
    EFFECT_ITEM_ATTACH_GOOEY = 9,
    EFFECT_ITEM_THROW = 10,
    EFFECT_STATUS_METAL = 11,
    EFFECT_STATUS_CURRY = 12,
    EFFECT_STATUS_HAMMER = 13,
    EFFECT_STATUS_SUPERSTAR = 14,
    EFFECT_STATUS_FLOWER = 15,
    EFFECT_STATUS_HEART = 16,
    EFFECT_STATUS_SLOW = 17,
    EFFECT_STATUS_MUSHROOM = 18,
    EFFECT_STATUS_EQUIP = 19,
    EFFECT_STATUS_SWAP = 20,
    EFFECT_STATUS_FINALSMASH = 21,
    EFFECT_STATUS_ACTION = 22,
    EFFECT_WARP_TOPLAYER = 23,
    EFFECT_WARP_SWAP = 24,
    EFFECT_WARP_SWITCHDIRECTIONS = 25,
    EFFECT_MODE_FLIGHT = 26,
    EFFECT_MODE_BORDERLESS = 27,
    EFFECT_MODE_ELEMENT = 28,
    EFFECT_MODE_ZTD = 29,
    EFFECT_MODE_BOMBRAIN = 30,
    EFFECT_MODE_COIN = 31,
    EFFECT_MODE_SPEED = 32,
    EFFECT_MODE_WAR = 33,
    EFFECT_MODE_RANDOMANGLE = 34,
    EFFECT_MODE_BIGHEAD = 35,
    EFFECT_MODE_HITFALL = 36,
    EFFECT_MODE_LANDINGLAG = 37,
    EFFECT_MODE_SUDDENDEATH = 38,
    EFFECT_ATTRIBUTE_SLIP = 39,
    EFFECT_ATTRIBUTE_NUMJUMPS = 40,
    EFFECT_ATTRIBUTE_JUMPSQUAT = 41,
    EFFECT_ATTRIBUTE_GROUNDFRICTION = 42,
    EFFECT_ATTRIBUTE_GRAVITY = 43,
    EFFECT_ATTRIBUTE_FASTFALLSPEED = 44,
    EFFECT_ATTRIBUTE_WEIGHT = 45,
    EFFECT_ATTRIBUTE_SIZE = 46,
    EFFECT_ATTRIBUTE_SHIELD = 47,
    EFFECT_ATTRIBUTE_ITEMTHROWSTRENGTH = 48,
    EFFECT_ATTRIBUTE_WALLJUMP = 49,
    EFFECT_ATTRIBUTE_WALK = 50,
    EFFECT_ATTRIBUTE_DASH = 51,
    EFFECT_ATTRIBUTE_JUMP = 52,
    EFFECT_ATTRIBUTE_AIRJUMP = 53,
    EFFECT_ATTRIBUTE_HIT = 54,
    EFFECT_ATTRIBUTE_SDI = 55,
    EFFECT_ATTRIBUTE_SHIELD_GLOBAL = 56,
    EFFECT_ATTRIBUTE_SHIELD_PUSHBACK = 57,
    EFFECT_ATTRIBUTE_KNOCKBACK = 58,
    EFFECT_ATTRIBUTE_LEDGE = 59,
    EFFECT_DEBUG_PAUSE = 60,
    EFFECT_DEBUG_LOCK_CAMERA = 61,
    EFFECT_DEBUG_REMOVE_HUD = 62,
    EFFECT_DEBUG_VIEW = 63,
    EFFECT_DEBUG_CHARACTER_SWITCH = 64,
    EFFECT_STAGE_WILD = 65,
    EFFECT_STAGE_BALLOONPOP = 66,
};

//hacky way to check if in game
enum SCENE_TYPE {
    SCENE_MAIN_MENU = 0x1,
    SCENE_HRC_CSS = 0x2,
    SCENE_DIFFICULTY_TRAINING_CSS = 0x4,
    SCENE_CSS_HRC = 0x5,
    SCENE_DIFFICULTY_TRAINING_SSS_EVT_CSS = 0x6,
    SCENE_SSS_BTT = 0x7,
    SCENE_TRAINING_MODE_MMS = 0x8,
    SCENE_VS = 0xA
};


#endif //PROJECTMCODES_CROWDCONTROL_H
