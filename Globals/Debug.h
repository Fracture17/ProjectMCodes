//
// Created by johno on 9/23/2020.
//

#ifndef PROJECTMCODES_DEBUG_H
#define PROJECTMCODES_DEBUG_H


//use ASSERT macro in code, might have extra logic later to remove it in release builds or something
//use b assert in GDB to break on failed asserts
//use b _assert to break on any assert statement
void _assert(bool condition);
void assert();

#define ASSERT(condition) (_assert(condition))

#endif //PROJECTMCODES_DEBUG_H
