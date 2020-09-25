//
// Created by johno on 7/31/2020.
//

#include "Memory.h"
#include "Assembly.h"
#include "stddef.h"


BASIC_INJECT("TEST", 0x80017470, "lwz r0, 0xF8(r22)");


class A {
public:
    virtual int test() {return 3;}
};

class B: public A {
public:
    int test() {return 4;}
};


extern "C" void TEST() {
    B b;
    A* x = &b;
    int y = x->test();
}