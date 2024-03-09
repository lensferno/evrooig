#include "keybord.h"

#include "Arduino.h"

/**
 *     C1 C2 C3 C4
 *　R1  *  *  *  *
 *　R2  *  *  *  *
 *　R3  *  *  *  *
 *　R4  *  *  *  *
 */

void initKeys() {
  // 键盘行
  for (int r = 0; r < KEY_NUM; r++) {
    pinMode(ROW_KEYS[r], OUTPUT);
    digitalWrite(ROW_KEYS[r], LOW);
  }

  // 键盘列
  for (int c = 0; c < KEY_NUM; c++) {
    pinMode(COLUMN_KEYS[c], INPUT);
    digitalWrite(ROW_KEYS[c], LOW);
  }
}

int readKey() {
  for (int r = 0; r < KEY_NUM; r++) {
    digitalWrite(ROW_KEYS[r], HIGH);
    for (int c = 0; c < KEY_NUM; c++) {
      if (digitalRead(COLUMN_KEYS[c]) == HIGH) {
        digitalWrite(ROW_KEYS[r], LOW);
        return (KEY_NUM * r) + c;
      }
    }
    digitalWrite(ROW_KEYS[r], LOW);
  }

  return NO_KEY;
}

int prevKey = NO_KEY;
int currKey = NO_KEY;

int readKeyOnReleased() { 
  currKey = readKey(); 
  if (prevKey != NO_KEY && currKey == NO_KEY) {
    int p = prevKey;
    prevKey = NO_KEY;
    return p;
  }

  prevKey = currKey;
  return NO_KEY;
}
