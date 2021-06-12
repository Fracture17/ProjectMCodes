//
// Created by johno on 7/2/2020.
//

#ifndef PROJECTMCODES_CSTRING_H
#define PROJECTMCODES_CSTRING_H


#define _strcat ((char* (*)(char* destination, const char* source)) 0x803fa384)
#define _strcpy ((char* (*)(char* destination, const char* source)) 0x803fa280)
#define _strcpylen ((char* (*)(char* destination, const char* source, int size)) 0x803fa340)
#define sprintf ((int (*)(char* buffer, const char* format, ...)) 0x803f89fc)
#define _strstr ((char* (*)(const char* buffer, const char* target)) 0x803fa798)


char* strcat(char* destination, const char* source);
char* strcpy(char* destination, const char* source);
char* strcpy(char* destination, const char* source, int length);
//return ptr to first occurance of target in buffer, or nullptr if not found
char* strstr(const char* buffer, const char* target);

#endif //PROJECTMCODES_CSTRING_H
