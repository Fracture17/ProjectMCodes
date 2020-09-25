//
// Created by johno on 9/24/2020.
//

#ifndef PROJECTMCODES_GFFILEIOMANAGER_H
#define PROJECTMCODES_GFFILEIOMANAGER_H


struct gfFileIOManager {
    //1 is replay, 0 is normal match?
    void setSyncMode(int syncMode);
};


#define FILE_MANAGER_PTR ((gfFileIOManager**) 0x8059fff4)

#define _setSyncMode_gfFileIOManager ((void (*)(gfFileIOManager* self, int syncMode)) 0x800237a0)

#endif //PROJECTMCODES_GFFILEIOMANAGER_H
