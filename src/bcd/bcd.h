#ifndef BCD_H
#define BCD_H

#include "types.h"

inline u8 bcd2dec(u8 bcd) { return ((bcd >> 4) * 10) + (bcd & 0x0f); }
inline u8 dec2bcd(u8 dec) { return ((dec / 10) << 4) + (dec % 10); }

#endif