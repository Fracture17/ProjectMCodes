#ifndef PROJECTMCODES_GFTASK_H
#define PROJECTMCODES_GFTASK_H

struct gfTaskVTable {
    void** gfTask_descriptor_8059c4f0;
    void* offset_fix;
    void** fn_processDefault;
    void** fn_processBegin;
    void** fn_processAnim;
    void** fn_processUpdate;
    void** fn_processPreMapCorrection;
    void** fn_processMapCorrection;
    void** fn_processFixPosition;
    void** fn_processPreCollision;
    void** fn_processCollision;
    void** fn_processCatch;
    void** fn_processHit;
    void** fn_processCamera;
    void** fn_processFixCamera;
    void** fn_processEffect;
    void** fn_processGameProc;
    void** fn_processEnd;
    void** fn_renderPre;
    void** fn_renderOpa;
    void** fn_renderXlu;
    void** fn_processDebug;
    void** fn_renderDebug;
    void** fn_init;
    void** fn___dt;
};

struct gfTask {
    void processBegin();
    void processAnim();
    void processUpdate();
    void processPreMapCorrection();
    void processMapCorrection();
    void processFixPosition();
    void processPreCollision();
    void processCollision();
    void processCatch();
    void processHit();
    void processCamera();
    void processFixCamera();
    void processEffect();
    void processGameProc();
    void processEnd();
    void renderPre();
    void renderOpa();
    void renderXlu();
    void processDebug();
    void renderDebug();
    void init();
    void __dt();

    // 0x0
    char** taskName;
    // 0x4
    gfTask* nextTask;
    // 0x8
    gfTask* prevTask;

    char _spacer[0x28 - 0x8 - 4];
    
    int taskID;
    
    char _spacer2[0x3c - 0x28 - 4];
    
    gfTaskVTable* vtable; // points to a struct with functions seen above
};

#endif //PROJECTMCODES_GFTASK_H