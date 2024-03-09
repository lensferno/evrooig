#ifndef types_h
#define types_h

#include "stdint.h"

typedef int8_t i8;
typedef uint8_t u8;

typedef int16_t i16;
typedef uint16_t u16;

typedef int32_t i32;
typedef uint32_t u32;

typedef int64_t i64;
typedef uint64_t u64;

typedef float f32;
typedef double f64;

typedef uint8_t gpio;

static const u32 TIME_SECOND = 1000;
static const u32 TIME_MINUTE = 60 * TIME_SECOND;
static const u32 TIME_HOUR = 60 * TIME_MINUTE;

#endif