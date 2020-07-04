//
// Created by johno on 7/2/2020.
//

#ifndef PROJECTMCODES_CSTRING_H
#define PROJECTMCODES_CSTRING_H


#define _strcat ((char* (*)(char* destination, const char* source)) 0x803fa384)
#define _strcpy ((char* (*)(char* destination, const char* source)) 0x803fa280)



char* strcat(char* destination, const char* source);
char* strcpy(char* destination, const char* source);

#endif //PROJECTMCODES_CSTRING_H
