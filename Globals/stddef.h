#pragma once

typedef unsigned char u8;
typedef signed char s8;
typedef unsigned short u16;
typedef signed short s16;
typedef unsigned int u32;
typedef signed int s32;
typedef unsigned long long u64;
typedef long long s64;
typedef float f32;
typedef double f64;

typedef unsigned int size_t;

#define offsetof(st, m) ((size_t)&(((st *)0)->m))