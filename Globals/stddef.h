#pragma once

#define VTABLE_METHOD(THISTYPE, RET_TYPE, FN_NAME) \
/* inline */ RET_TYPE FN_NAME() { return ((RET_TYPE (*)(THISTYPE self)) this->vtable->fn__ ## FN_NAME)(this); }
#define VTABLE_METHOD_1ARG(THISTYPE, RET_TYPE, FN_NAME, TYPE1, NAME1) \
/* inline */ RET_TYPE FN_NAME(TYPE1 NAME1) { return ((RET_TYPE (*)(THISTYPE self, TYPE1 NAME1)) this->vtable->fn__ ## FN_NAME)(this, NAME1); }
#define VTABLE_METHOD_2ARG(THISTYPE, RET_TYPE, FN_NAME, TYPE1, NAME1, TYPE2, NAME2) \
/* inline */ RET_TYPE FN_NAME(TYPE1 NAME1, TYPE2 NAME2) { return ((RET_TYPE (*)(THISTYPE self, TYPE1 NAME1, TYPE2 NAME2)) this->vtable->fn__ ## FN_NAME)(this, NAME1, NAME2); }

#define STATIC_METHOD(ADDRESS, SELF_TYPE, RET_TYPE, FN_NAME) \
/* inline */ RET_TYPE FN_NAME() { return ((RET_TYPE (*)(SELF_TYPE * self)) ADDRESS)(this); }
#define STATIC_METHOD_1ARG(ADDRESS, SELF_TYPE, RET_TYPE, FN_NAME, TYPE1, NAME1) \
/* inline */ RET_TYPE FN_NAME(TYPE1 NAME1) { return ((RET_TYPE (*)(SELF_TYPE * self, TYPE1 NAME1)) ADDRESS)(this, NAME1); }
#define STATIC_METHOD_3ARG(ADDRESS, SELF_TYPE, RET_TYPE, FN_NAME, TYPE1, NAME1, TYPE2, NAME2, TYPE3, NAME3) \
/* inline */ RET_TYPE FN_NAME(TYPE1 NAME1, TYPE2 NAME2, TYPE3 NAME3) { return ((RET_TYPE (*)(SELF_TYPE * self, TYPE1 NAME1, TYPE2 NAME2, TYPE3 NAME3)) ADDRESS)(this, NAME1, NAME2, NAME3); }
// #define _setGrCollisisonMode ((void (*)(soGroundModuleImpl * self, int collisionMode)) 0x80730a04)


union xyDouble {
    double asDouble;
    struct {
        float xPos = 0;
        float yPos = 0;
    };
};

typedef signed char s8;
typedef signed short s16;
typedef signed int s32;
typedef long long s64;

typedef unsigned char u8;
typedef unsigned short u16;
typedef unsigned int u32;
typedef unsigned long long u64;

typedef volatile signed char vs8;
typedef volatile signed short vs16;
typedef volatile signed int vs32;
typedef volatile long long vs64;

typedef volatile unsigned char vu8;
typedef volatile unsigned short vu16;
typedef volatile unsigned int vu32;
typedef volatile unsigned long long vu64;

typedef float f32;
typedef double f64;

typedef volatile float vf32;
typedef volatile double vf64;

typedef unsigned int size_t;

struct Position2D {
    f32 x;
    f32 y;

    constexpr Position2D& operator*=(float rhs) {
        x *= rhs;
        y *= rhs;
        return *this;
    }
};

struct Position3D {
    f32 x;
    f32 y;
    f32 z;
    
    constexpr Position3D& operator-=(Position3D rhs) {
        x -= rhs.x; y -= rhs.y; z -= rhs.z;
        return *this;
    }
    constexpr Position3D& operator+=(Position3D rhs) {
        x += rhs.x; y += rhs.y; z += rhs.z;
        return *this;
    }
    constexpr Position3D& operator*=(float rhs) {
        x *= rhs; y *= rhs; z *= rhs;
        return *this;
    }
};

#define offsetof(type, member) (__builtin_offsetof(type, member))
#define _randf ((double (*)()) 0x8003fb64)

#define _dynamicCast_addr ((void* (*)(void* toCast, int vtableOffset, void* sourceDescriptor, void* targetDescriptor, bool checkForBadCast)) 0x803f0f44)

inline void* DynamicCast(void* toCast, int vtableOffset, void* sourceDescriptor, void* targetDescriptor) {
  return _dynamicCast_addr(toCast, vtableOffset, sourceDescriptor, targetDescriptor, true);
}
// #define NOP() "\x60\x00\x00\x00"

// template<class> class function; // not defined
 
// template<class R, class... ArgTypes>
// class function<R(ArgTypes...)> {
// public:
// using result_type = R;

// // construct/copy/destroy
// function() noexcept;
// function(const function&);
// function(function&&) noexcept;

// ~function();

// // function modifiers
// void swap(function&) noexcept;

// // function capacity
// explicit operator bool() const noexcept;

// // function invocation
// R operator()(ArgTypes...) const;

// // function target access
// template<class T>       T* target() noexcept;
// template<class T> const T* target() const noexcept;
// };

// template<class R, class... ArgTypes>
// function(R(*)(ArgTypes...)) -> function<R(ArgTypes...)>;