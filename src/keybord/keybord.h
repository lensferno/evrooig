#ifndef KEYBORD_H
#define KEYBORD_H

#include "types.h"
#include "pins_energia.h"

/**
 *     C1 C2 C3 C4
 *　R1  *  *  *  *
 *　R2  *  *  *  *
 *　R3  *  *  *  *
 *　R4  *  *  *  *
 */

enum Keys {
  NUM7,   NUM8,   NUM9,   KEY_ADD,
  NUM4,   NUM5,   NUM6,   KEY_MINUS,
  NUM1,   NUM2,   NUM3,   KEY_OK,
  NUM0,   KEY_L,  KEY_R,  KEY_CANCEL,
  NO_KEY = -1,
};

static const u8 KEY_NUM = 4;

static const u8 KEYBORD_R1 = P1_5;
static const u8 KEYBORD_R2 = P1_4;
static const u8 KEYBORD_R3 = P1_3;
static const u8 KEYBORD_R4 = P1_2;

static const u8 ROW_KEYS[] = {
    KEYBORD_R1,
    KEYBORD_R2,
    KEYBORD_R3,
    KEYBORD_R4,
};

static const u8 KEYBORD_C1 = P4_3;
static const u8 KEYBORD_C2 = P4_0;
static const u8 KEYBORD_C3 = P3_7;
static const u8 KEYBORD_C4 = P8_2;

static const u8 COLUMN_KEYS[] = {
    KEYBORD_C1,
    KEYBORD_C2,
    KEYBORD_C3,
    KEYBORD_C4,
};

void initKeys();
int readKey();
int readKeyOnReleased();

#endif