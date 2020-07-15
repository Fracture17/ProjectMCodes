//
// Created by johno on 7/5/2020.
//

#ifndef PROJECTMCODES_SETUP_H
#define PROJECTMCODES_SETUP_H






struct Injection {
    unsigned int injectionAddress;
    unsigned int injectedFunctionAddress;
    unsigned int returnBranchAddress;
};

struct CodeFile {
    char name[4];
    unsigned int address;
};


struct SetupFile {
    Injection injections[];
    CodeFile fileNames[];
};


#endif //PROJECTMCODES_SETUP_H
