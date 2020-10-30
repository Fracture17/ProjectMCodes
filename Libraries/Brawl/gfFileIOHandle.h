//
// Created by johno on 7/24/2020.
//

#ifndef PROJECTMCODES_GFFILEIOHANDLE_H
#define PROJECTMCODES_GFFILEIOHANDLE_H


#include "stddef.h"
#include "gfFileIORequest.h"

struct gfFileIOHandle {
    //0
    gfFileIORequest* request;

    bool checkFileRequest(char* filePath);
};


#define _checkFileRequest_GF_FILE_IO_HANDLE ((bool (*)(gfFileIOHandle* self, char* filePath)) 0x80020c88)


#endif //PROJECTMCODES_GFFILEIOHANDLE_H
