//
// Created by dareb on 7/30/2020.
//
#include "_TrainingOptionDefs.h"


#if SHOULD_INCLUDE_AI == 1 || SHOULD_INCLUDE_MOVEMENT_TRACKER == 1
#include "Assembly.h"
#include "stddef.h"

#include "Brawl/FT/ftManager.h"
#include "Brawl/FT/ftEntryManager.h"
#include "Brawl/FT/Fighter.h"
#include "Brawl/FT/ftOwner.h"
#include "Brawl/FT/ftEntry.h"
#include "Brawl/FT/ftInput.h"
#include "Brawl/AI/aiScriptData.h"
#include "Brawl/AI/aiMgr.h"
#include "Brawl/AI/aiStat.h"
#include "Brawl/AI/aiInput.h"
#include "Brawl/SC/scMelee.h"


#include "Containers/Vec3f.h"
#include "Containers/vector.h"

#include "MovementTracker.h"
#include "FudgeMenu.h"


#define OSReport ((void (*)(const char* text, ...)) 0x801d8600)
#define _stRayCheck_vec3f ((int (*)(Vec3f* start, Vec3f* dest, Vec3f* retValue, Vec3f* normalVec, int unkTrue, int unk0, int unk0_1, int unk1)) 0x809326d8)
#define _length_vec3f ((double (*)(Vec3f* vector)) 0x8070b94c)
#define _GetPlayerNo_aiChrIdx ((int (*)(char* chrIdx)) 0x808fd68c)
#define _IsPlayer_aiChrIdx ((bool (*)(char* chrIdx)) 0x808fd720)

extern u8 getPlayerCount();

unsigned int playerCount = 0;
MovementTracker movementTrackers[4] = {
    MovementTracker(),
    MovementTracker(),
    MovementTracker(),
    MovementTracker()
};
extern GlobalCustomData GCD;

INJECTION("TRACK_ACTION", 0x8077f9d8, R"(
    SAVE_REGS
    mr r3, r4
    mr r4, r5
    bl trackActionChange
    RESTORE_REGS
    cmpwi r4, -1
)")

// INJECTION("TRACK_SUBACTION", 0x80726e58, R"(
//     SAVE_REGS
//     bl trackSubactionChange
//     RESTORE_REGS
//     mr r31, r4
// )")

void updateData(soModuleAccessor * accesser, int toRecord, bool isAction) {

    // necessary because this will also pick up articles
    // checking the base type for the "ft" prefix ensures it's an actual fighter
    // 0x6674 = "ft"
    if (****((short****)accesser->paramCustomizeModule) != 0x6674) return;
    AiInput* aiInput = ((Fighter*) accesser->owningObject)->getOwner()->aiInputPtr;
    // alloy check
    auto ChId = aiInput->charId;
    if (ChId == CHAR_ID::Zakoboy || ChId == CHAR_ID::Zakogirl || ChId == CHAR_ID::Zakochild || ChId == CHAR_ID::Zakoball) return;
    // obligatory nana check
    if (aiInput->ftEntryPtr->isSubFighter(accesser)) return;
    auto pNum = _GetPlayerNo_aiChrIdx(&aiInput->cpuIdx);
    if (pNum >= 4) return;

    // get YDistFloor (+1)
    u8 yDistFloor = 0;
    Vec3f startPos = {};
    accesser->groundModule->getCorrectPos(&startPos);

    Vec3f destPos {
        0,
        -255,
        0
    };

    Vec3f ret1 {-1,-1,-1};
    Vec3f ret2 {-1,-1,-1};

    int rayResult = _stRayCheck_vec3f(&startPos, &destPos, &ret1, &ret2, true, 0, 1, 1);
    
    if (rayResult) {
        startPos.f1 -= ret1.f1;
        startPos.f2 -= ret1.f2;
        yDistFloor = _length_vec3f(&startPos) + 1;
    }

    // retroactively unrecord jump if wavedashing
    if (accesser->statusModule->previousAction == 0xB && toRecord == 0x21 && yDistFloor < 10 && accesser->postureModule->prevYPos < accesser->postureModule->yPos) {
        movementTrackers[pNum].undoLastAction();
    }

    if (getPlayerCount() == 2 && accesser->groundModule != nullptr) {
        auto oEntryID = FIGHTER_MANAGER->getEntryId(aiInput->aiTarget);
        auto oPNum = FIGHTER_MANAGER->getPlayerNo(oEntryID);
        auto oFighter = FIGHTER_MANAGER->getFighter(oEntryID);
        if (oFighter == nullptr) return;
        auto otherGroundModule = oFighter->modules->groundModule;
        if (otherGroundModule == nullptr) return;
        float other = otherGroundModule->getDownPos().xPos;
        float self = accesser->groundModule->getDownPos().xPos;
        float dist = other - self;
        if (dist < 0) dist *= -1;
        self += (self<0)*self*-2;
        self *= 0.5;

        u8 ownAttackChance = movementTrackers[pNum].approxChance(50, MOV_ATTACK) * 200;
        u8 ownShieldChance = movementTrackers[pNum].approxChance(50, MOV_SHIELD) * 200;

        if (toRecord != -1) movementTrackers[pNum].trackAction(toRecord, isAction);
        movementTrackers[pNum].updateData(yDistFloor, (u8) self, (u8) dist, ownShieldChance, ownShieldChance, movementTrackers[oPNum].getCurrentMov());
    } else {
        float xPos = accesser->groundModule->getDownPos().xPos;
        xPos += (xPos<0)*xPos*-2;
        xPos *= 0.5;
        if (toRecord != -1) movementTrackers[pNum].trackAction(toRecord, isAction);
        movementTrackers[pNum].updateData(yDistFloor, (u8) xPos);
    }
}

extern "C" {
    bool recordSubaction = false;
    void recordData(unsigned char toRecord, soModuleAccessor * accesser, bool isAction) {
        updateData(accesser, isAction ? toRecord : 0, isAction);
    }
    // void trackSubactionChange(int* subactionPtr, soMotionModule * motionModule) {
    //     if (recordSubaction) {
    //         // 0x48 to 0x6B for attacks
    //         // 0x6C to 0x6E for grabs
    //         // 0x1CE+ for specials
    //         // 0x1CE - 0x6E (- 1) = 0x15F = plenty of room (noice)
    //         int subaction = *subactionPtr;
    //         if (subaction >= 0x1CE) subaction -= 0x15F;
    //         recordData(subaction, motionModule->accesser, false);
    //     }
    //     recordSubaction = false;
    // }
    void trackActionChange(int action, soModuleAccessor * accesser) {
        if (action == -1 || (action >= 0x10E && action <= 0x110)) return;
        recordData(action, accesser, true);
        // if (action == 0x33) recordSubaction = true;
        // else recordData(action, accesser, true);817ceaa8
    }
}

#endif

#if SHOULD_INCLUDE_AI == 1

#include "Brawl/GF/gfApplication.h"
#include "Brawl/GF/GameGlobal.h"
#include "Brawl/FT/ftCancelModule.h"
#include "Brawl/FT/ftControllerModuleImpl.h"
#include "Brawl/FT/ftCommonDataAccessor.h"
#include "Brawl/IT/itManager.h"
#include "Brawl/Weapon.h"
#include "Wii/MATH.h"
#include "Graphics/Drawable.h"

#include "PatternManager.h"
#include "FudgeMenu.h"
#include "WeightedDie.h"

#define REP0(X)
#define REP1(X) X,
#define REP2(X) REP1(X) X,
#define REP3(X) REP2(X) X,
#define REP4(X) REP3(X) X,
#define REP5(X) REP4(X) X,
#define REP6(X) REP5(X) X,
#define REP7(X) REP6(X) X,
#define REP8(X) REP7(X) X,
#define REP9(X) REP8(X) X,
#define REP10(X) REP9(X) X,

#define snprintf ((int (*)(char* buffer, size_t maxSize, const char* format, ...)) 0x803f8924)
#define _stRayCheck_vec3f ((int (*)(Vec3f* start, Vec3f* dest, Vec3f* retValue, Vec3f* normalVec, int unkTrue, int unk0, int unk0_1, int unk1)) 0x809326d8)
#define _length_vec3f ((double (*)(Vec3f* vector)) 0x8070b94c)
#define OSReport ((void (*)(const char* text, ...)) 0x801d8600)
#define _calcArraivePosX_aiStat ((double (*)(double time, AiStat * stat)) 0x80916884)
#define _calcArraivePosY_aiStat ((double (*)(double time, AiStat * stat)) 0x809168c8)
#define _danger_check_aiInput ((void (*)(AiInput* aiInputPtr, unsigned int unk1, unsigned int unk2)) 0x80906378)
#define _act_change ((void (*)(AiScriptData * self, unsigned int nextScript, char* nextTargetAIChrIdx, int unk2, int unk3)) 0x80918554)
#define _target_check_aiInput ((void (*)(AiInput* self)) 0x80907ba4)

// 0x80903348 = return statement area
// 0x809017A0 = AIAct script execution
// 0x809014CC = various checks
INJECTION("POKE_AIINPUT_UPDATE", 0x808FE598, R"(
    SAVE_REGS
    mr r3, r31
    bl initAIFrame
    RESTORE_REGS
    lis r12, 0x8090
    ori r12, r12, 0x17A0
    mtctr r12
    bctr
)");

INJECTION("POKE_AIINPUT_UPDATE_2", 0x809017B0, R"(
    addi r3, r1, 0x168
    bl stickModMul
    addi r3, r1, 0x168
    lis r12, 0x8090
    ori r12, r12, 0x5824
    mtctr r12
    bctrl
    lis r12, 0x8090
    ori r12, r12, 0x3188
    mtctr r12
    bctr
)")

// INJECTION("POKE_AIINPUT_UPDATE", 0x808FE598, R"(
//     SAVE_REGS
//     mr r3, r31
//     bl initAIFrame
//     RESTORE_REGS
//     lis r12, 0x8090
//     ori r12, r12, 0x17A0
//     mtctr r12
//     bctr
// )");

// INJECTION("POKE_AIACT_UPDATE", 0x80918264, R"(
//     lis r12, 0x8091
//     ori r12, r12, 0x84FC
//     mtctr r12
//     bctr
// )")

extern "C" void stickModMul(Vec2f* thing) {
    thing->x *= 80;
    thing->y *= 80;
}

extern "C" void initAIFrame(AiInput * aiInput) {
    if (aiInput->aiActPtr->aiStatPtr->character != CHAR_ID::Nana && (aiInput->aiActPtr->aiScript < 0x8000 || (aiInput->aiMd != 0x1))) {
        aiInput->aiMd = 1;
        _act_change(aiInput->aiActPtr, 0x8100, &aiInput->aiTarget, 0, 0);
    }
    
    aiInput->aiTarget = 0;
    _target_check_aiInput(aiInput);
    // OSReport("AI Script: %04x\n", aiInput->aiActPtr->aiScript);
    
    // OSReport("TARGET: %d\n", aiInput->aiTarget);
}


// Raycast ignore Nonetype collision [Eon]
// that gal is a witch (in a good/cool way) I swear to god
INJECTION("RAYCAST_IGNORE_NONETYPE_COLLISION", 0x80138154, R"(
    lhz r0, 0xE(r3)
    andi. r0, r0, 0xF
    lbz r0, 0x10(r3)
    bne _RAYCAST_IGNORE_END_
    lis r12, 0x8013
    ori r12, r12, 0x8310
    mtctr r12
    bctr
_RAYCAST_IGNORE_END_:
)")

INJECTION("FORCE_AI_TAUNT_ROUTINE", 0x809112cc, R"(
    SAVE_REGS
    mr r3, r26
    bl forceAiTauntRoutine
    RESTORE_REGS
    lis r3, 0x8091
    ori r3, r3, 0x1464
    mtctr r3
    bctr
)");

#define change_md_aiInput ((void (*)(AiInput *aiInputInst,unsigned int newMode,unsigned char *param_3,unsigned int newOrOldAction,int param_5)) 0x809049d0)
extern "C" void forceAiTauntRoutine(AiInput *aiInputInst) {
    unsigned char dummy = 0xff;
    // setting it to md 0x1 will make it alternate between md 0x1 and md 0x14, and we don't want that.
    // therefor, we set it to 0x2 where this problem doesn't exist while still giving us control over
    // our scripts
    change_md_aiInput(aiInputInst, 0x2, &dummy, 0x8000, 0);
};


// because fu**ck hardcoded inputs :)
INJECTION("mdThreeFix", 0x80909b98, R"(
    nop
)");

// 0x60000000 = nop

// FORCED_JUMP_FIX
DATA_WRITE(0x809028dc, 0x60000000);
// FORCED_JUMP_FIX2
DATA_WRITE(0x8090272c, 0x60000000);
// FORCED_JUMP_FIX3
DATA_WRITE(0x80902110, 0x60000000);
// // FORCED_JUMP_FIX4// 
// DATA_WRITE(0x80902f9c, 0x60000000);
// FORCED_JUMP_FIX4
DATA_WRITE(0x80901db0, 0x60000000);
// FORCED_JUMP_FIX5
DATA_WRITE(0x809019cc, 0x60000000);
// FORCED_JUMP_FIX6
DATA_WRITE(0x80901a1c, 0x60000000);
// FORCED_JUMP_FIX7// 
DATA_WRITE(0x809025ec, 0x60000000);
// FORCED_JUMP_FIX8// 
DATA_WRITE(0x80902610, 0x60000000);

// FORCED_AIRDODGE_FIX
DATA_WRITE(0x80902f9c, 0x60000000);
// // FORCED_AIRDODGE_FIX2
// DATA_WRITE(0x80765758, 0x60000000);

// 0x808FE5A0
// 0x80901790

// FORCED_INPUT_FIX1
// DATA_WRITE(0x808fe93c, 0x60000000);
// FORCED_INPUT_FIX2
// DATA_WRITE(0x808ff77c, 0x60000000);
// FORCED_INPUT_FIX3
DATA_WRITE(0x809017dc, 0x60000000);
// FORCED_INPUT_FIX4
DATA_WRITE(0x80901940, 0x60000000);
// FORCED_INPUT_FIX5
DATA_WRITE(0x80902710, 0x60000000);
// FORCED_INPUT_FIX6
DATA_WRITE(0x80902674, 0x60000000);
// FORCED_INPUT_FIX7
DATA_WRITE(0x80902280, 0x60000000);
// FORCED_INPUT_FIX8
DATA_WRITE(0x809021dc, 0x60000000);
// FORCED_INPUT_FIX9
DATA_WRITE(0x80902428, 0x60000000);
// FORCED_INPUT_FIX10
DATA_WRITE(0x80902364, 0x60000000);
// FORCED_INPUT_FIX11
DATA_WRITE(0x8090293c, 0x60000000);
// FORCED_INPUT_FIX12
DATA_WRITE(0x80902e38, 0x60000000);
// FORCED_INPUT_FIX13
DATA_WRITE(0x80902f0c, 0x60000000);
// FORCED_INPUT_FIX14
DATA_WRITE(0x80902cb0, 0x60000000);
// FORCED_INPUT_FIX15
DATA_WRITE(0x80902ba0, 0x60000000);
// FORCED_INPUT_FIX16
DATA_WRITE(0x80902c50, 0x60000000);
// FORCED_INPUT_FIX17
DATA_WRITE(0x80902bf8, 0x60000000);
// FORCED_INPUT_FIX18
DATA_WRITE(0x80902d80, 0x60000000);
// FORCED_INPUT_FIX19
DATA_WRITE(0x80902d58, 0x60000000);
// FORCED_INPUT_FIX20
DATA_WRITE(0x80902f7c, 0x60000000);
// FORCED_INPUT_FIX21
DATA_WRITE(0x80903018, 0x60000000);
// FORCED_INPUT_FIX22
// DATA_WRITE(0x808fe930, 0x60000000);
// FORCED_INPUT_FIX23
// DATA_WRITE(0x808ff770, 0x60000000);
// FORCED_INPUT_FIX24
DATA_WRITE(0x809017d0, 0x60000000);
// FORCED_INPUT_FIX25
DATA_WRITE(0x80901934, 0x60000000);
// FORCED_INPUT_FIX26
DATA_WRITE(0x809021d0, 0x60000000);
// FORCED_INPUT_FIX27
DATA_WRITE(0x80902230, 0x60000000);
// FORCED_INPUT_FIX28
DATA_WRITE(0x80902274, 0x60000000);
// FORCED_INPUT_FIX29
DATA_WRITE(0x809022b4, 0x60000000);
// FORCED_INPUT_FIX30
DATA_WRITE(0x80902358, 0x60000000);
// FORCED_INPUT_FIX31
DATA_WRITE(0x80902418, 0x60000000);
// FORCED_INPUT_FIX32
DATA_WRITE(0x80902668, 0x60000000);
// FORCED_INPUT_FIX33
DATA_WRITE(0x80902704, 0x60000000);
// FORCED_INPUT_FIX34
DATA_WRITE(0x80902930, 0x60000000);
// FORCED_INPUT_FIX35
DATA_WRITE(0x80902a34, 0x60000000);
// FORCED_INPUT_FIX36
DATA_WRITE(0x80902a40, 0x60000000);
// FORCED_INPUT_FIX37
DATA_WRITE(0x80902a74, 0x60000000);
// FORCED_INPUT_FIX38
DATA_WRITE(0x80902a80, 0x60000000);
// FORCED_INPUT_FIX39
DATA_WRITE(0x80902b68, 0x60000000);
// FORCED_INPUT_FIX40
DATA_WRITE(0x80902b9c, 0x60000000);
// FORCED_INPUT_FIX41
DATA_WRITE(0x80902bf4, 0x60000000);
// FORCED_INPUT_FIX42
DATA_WRITE(0x80902c4c, 0x60000000);
// FORCED_INPUT_FIX43
DATA_WRITE(0x80902d54, 0x60000000);
// FORCED_INPUT_FIX44
DATA_WRITE(0x80902d70, 0x60000000);
// FORCED_INPUT_FIX45
DATA_WRITE(0x80902d7c, 0x60000000);
// FORCED_INPUT_FIX46
DATA_WRITE(0x8090300c, 0x60000000);
// FORCED_INPUT_FIX47
DATA_WRITE(0x80903094, 0x60000000);
// FORCED_INPUT_FIX48
DATA_WRITE(0x809030a0, 0x60000000);

// FORCED MD FIX
// DATA_WRITE(0x808fe864, 0x60606060);

// SELF_TARGET_CHANGE_FIX1
// DATA_WRITE(0x808fe5f8, 0x60000000);
// SELF_TARGET_CHANGE_FIX2
DATA_WRITE(0x809073dc, 0x60000000);
// SELF_TARGET_CHANGE_FIX3
DATA_WRITE(0x809188e0, 0x60000000);
// SELF_TARGET_CHANGE_FIX4
// DATA_WRITE(0x80900cb8, 0x60000000);



// FIX_SWING_CHK_RESET
DATA_WRITE(0x80905668, 0x4e800020);
// FIX_SWING_CHK_SET
DATA_WRITE(0x80905690, 0x4e800020);

// FIX_AUTO_THROW_ROUTINE
// DATA_WRITE(0x808fe89c, 0x60000000);

// THESE DO NOT WORK :pensive:
// INJECTION("FORCE_PICK_AI", 0x80685830, R"(
//     bl validCheck
//     stw r3, 0x0044 (sp)
// )");

// INJECTION("colorslot_override_1", 0x8068584c, R"(
//     li r3, 1
// )");

// INJECTION("colorslot_override_2", 0x80684914, R"(
//     li r3, 1
// )");

// FIX_AUTO_ROUTINE_SWITCH
// DATA_WRITE(0x80928620, 0x4e800020);
// DATA_WRITE(0x80928b1c, 0x4e800020);
// DATA_WRITE(0x80928c60, 0x4e800020);
// DATA_WRITE(0x809293d0, 0x4e800020);

// DATA_WRITE(0x8092a524, 0x4e800020);
// DATA_WRITE(0x8092a548, 0x4e800020);
// DATA_WRITE(0x8092a56c, 0x4e800020);
// DATA_WRITE(0x8092a590, 0x4e800020);

signed char disabledMd[4] = {-1, -1, -1, -1};

INJECTION("CPUForceMd", 0x80905204, R"(
    SAVE_REGS
    mr r3, r26
    mr r4, r27
    mr r5, r28
    bl CPUForceMd
    RESTORE_REGS
)");

#define _GetPlayerNo_aiChrIdx ((int (*)(char* chrIdx)) 0x808fd68c)
#define _IsPlayer_aiChrIdx ((bool (*)(char* chrIdx)) 0x808fd720)
extern "C" void CPUForceMd(AiInput * aiInput, unsigned int intent, int newAction) {
    // OSReport("ADDR: %08x\n", aiInput);

    if (aiInput->aiActPtr->aiStatPtr->character == CHAR_ID::Nana) {
        if (intent == aimd_value::md_nana_return) {
            intent = aimd_value::md_nana_grabbing;
        }
        aiInput->aiMd = intent;
        // OSReport("nana intent: %02x\n", intent);
    }
    else if (aiInput->ftEntryPtr != nullptr) {
        OSReport("-- MD CHANGE --\n");
        OSReport("current action: %04x; ", aiInput->aiActPtr->aiScript);
        OSReport("new action?: %04x;\n", newAction);
        OSReport("current md: %02x; ", aiInput->aiMd);
        OSReport("new md: %02x\n", intent);

        char pNum = _GetPlayerNo_aiChrIdx(&aiInput->cpuIdx);
        if (pNum >= 4) pNum = 0;
        if (intent <= 0xFFFF && pNum != -1 && intent != disabledMd[pNum] && intent != 0x2 && intent != 0x5 && intent != 0x0) {
            aiInput->aiMd = intent;
        }
    }
}

bool disabledSwitch[4] = {false, false, false, false};
bool debugSwitch[4] = {true, true, true, true};

// CURRENT CODE REGION
// 0x808FE5A0
// 0x80901790

// INJECTION("PREVENT_AUTO_CALL1", 0x80900ff8, "bl filterMDCall");
// INJECTION("PREVENT_AUTO_CALL2", 0x80900eb4, "bl filterMDCall");
// INJECTION("PREVENT_AUTO_CALL3", 0x808fe638, "bl filterMDCall");
// INJECTION("PREVENT_AUTO_CALL4", 0x80901084, "bl filterMDCall");
// INJECTION("PREVENT_AUTO_CALL5", 0x80900d38, "bl filterMDCall");
// INJECTION("PREVENT_AUTO_CALL6", 0x80900e54, "bl filterMDCall");
// INJECTION("PREVENT_AUTO_CALL7", 0x80900f60, "bl filterMDCall");
// INJECTION("PREVENT_AUTO_CALL8", 0x80900fa4, "bl filterMDCall");
// INJECTION("PREVENT_AUTO_CALL9", 0x80900fd4, "bl filterMDCall");
// INJECTION("PREVENT_AUTO_CALL10", 0x8090137c, "bl filterMDCall");
// INJECTION("PREVENT_AUTO_CALL11", 0x808feb2c, "bl filterMDCall");

// INJECTION("PREVENT_AUTO_CALL12", 0x80907b18, "bl filterMDCall");
// INJECTION("PREVENT_AUTO_CALL13", 0x80907b18, "bl filterMDCall");
extern "C" void filterMDCall(AiInput *aiInputInst,unsigned int newMode,unsigned char *param_3,unsigned int newOrOldAction,int param_5) {
    int pNum = _GetPlayerNo_aiChrIdx(&aiInputInst->cpuIdx);
    if (pNum >= 4) pNum = 0;
    if (!disabledSwitch[pNum]) {
        change_md_aiInput(aiInputInst, newMode, param_3, newOrOldAction, param_5);
    }
    return;
}

bool autoDefend[4] = {false, false, false, false};

// INJECTION("PREVENT_AUTO_DEFEND", 0x80900c60, "bl preventAutoDefend");
// INJECTION("PREVENT_AUTO_DANGER_CHECK", 0x80901514, "bl preventDangerCheck");

extern "C" void preventAutoDefend(AiInput *aiInputInst,unsigned int newMode,unsigned char *param_3,unsigned int newOrOldAction,int param_5) {
    // if (autoDefend[_GetPlayerNo_aiChrIdx(&aiInputInst->cpuIdx)]) {
    //     change_md_aiInput(aiInputInst, newMode, param_3, newOrOldAction, param_5);
    // }
    return;
}
extern "C" void preventDangerCheck(AiInput *aiInputInst,unsigned int thing1, unsigned int thing2) {
    int pNum = _GetPlayerNo_aiChrIdx(&aiInputInst->cpuIdx);
    if (pNum >= 4) pNum = 0;
    if (autoDefend[pNum]) {
        _danger_check_aiInput(aiInputInst, thing1, thing2);
    }
    return;
}

INJECTION("PREVENT_MD_CHANGE_ACT", 0x809051e0, R"(
    cmpwi r27, 0x6
    beq PREVENTED_1
    cmpwi r27, 0x7
    beq PREVENTED_1
    cmpwi r27, 0x15
    beq PREVENTED_1
    lis r12, 0x8091
    ori r12, r12, 0x8554
    mtctr r12
    bctrl

PREVENTED_1:
)");
INJECTION("PREVENT_MD_CHANGE_ACT", 0x80905248, R"(
    cmpwi r27, 0x6
    beq PREVENTED_2
    cmpwi r27, 0x7
    beq PREVENTED_2
    cmpwi r27, 0x15
    beq PREVENTED_2
    lis r12, 0x8091
    ori r12, r12, 0x8554
    mtctr r12
    bctrl

PREVENTED_2:
)");


// INJECTION("mdThreeFixTwoElectricBugaloo", 0x809013f8, R"(
//     nop
// )");

// fuck this one in particular
// because it caused me 4 hours of agony for no payoff
// INJECTION("mxThreeFixTheThird", 0x8090137c, R"(
//     nop
// )");

INJECTION("CUSTOM_AI_FUNCTIONS", 0x8091e104, R"(
    SAVE_REGS
    mr r4, r31
    mr r5, r30
    mr r6, r1
    mr r7, r2
    fmr f1, f10
    bl aiFunctionHandlers
    RESTORE_REGS
    bl outputAiFunctionResult
    cmpwi r12, 0
    beq _CUSTOM_AI_FUNCTIONS_CONTINUE
    lis r12, 0x8091
    ori r12, r12, 0xE440
    mtctr r12
    bctr

_CUSTOM_AI_FUNCTIONS_CONTINUE:
    subi r0,r31,0x1000
)" );

int fn_shouldReturnResult = 0;
extern float ai_customFnInjection[0x10];
extern bool ai_customFnInjectionToggle[0x10];
extern TrainingData playerTrainingData[];
extern Menu* fudgeMenu;
extern u8 getPlayerCount();

PatternManager rpsManagers[0x10] = {
    PatternManager(),
    PatternManager(),
    PatternManager(),
    PatternManager(),
    PatternManager(),
    PatternManager(),
    PatternManager(),
    PatternManager(),
    PatternManager(),
    PatternManager(),
    PatternManager(),
    PatternManager(),
    PatternManager(),
    PatternManager(),
    PatternManager(),
    PatternManager()
};

#define FUNCTIONAL_STACK_LEN 0x18
float functionalStack[FUNCTIONAL_STACK_LEN] = {};
int functionalStackPtr = -1;
// inverted functionality; last in --> first read; persistant until cleared/next frame/fighter 
#define LTF_STACK_LEN 0x20
float LTFStack[LTF_STACK_LEN] = {};
int LTFStackPtr = -1;

WeightedDie dynamicDice[2] = {
    WeightedDie(),
    WeightedDie()
};

extern bool isMMS;
extern "C" {
    double fn_result = 0;
    void aiFunctionHandlers(float unk_f10, AiParam* targetAiParam, unsigned int switchCase, AiScriptData* selfAi, u32 sp, u32 rtoc) {
        fn_shouldReturnResult = 0;


        // OSReport("ADDR FTINPUT: %08x\n", &targetAiParam);
        // OSReport("TARGET NUM: %d; OWN NUM: %d\n", targetAiParam->ftInput->ftEntryPtr->playerNum, selfAi->aiInputPtr->aiParam.ftInput->ftEntryPtr->playerNum);

        // if (((switchCase - 0x1000) & 0xFF) == 0xFF) {
        //     fn_result = md_customFnInjection;
        //     fn_shouldReturnResult = 1;
        //     return;
        // }

        FtEntry * targetFighterEntry;
        Fighter * targetFighter;
        char targetPlayerNo = -1;
        bool shouldGetAiTarget = (switchCase & 0x0100) >> 8;

        // OSReport("aiParam: %08x\n", &targetAiParam);
        // OSReport("aiInput: %08x\n", &targetAiParam->aiInput);
        // OSReport("cpuIdx: %08x\n", &targetAiParam->aiInput->cpuIdx);
        // OSReport("&targetAiParam->aiInput->cpuIdx: %08x\n", &targetAiParam->aiInput->cpuIdx);
        // OSReport("&selfAi->aiInputPtr->cpuIdx: %08x\n", &selfAi->aiInputPtr->cpuIdx);

        if (shouldGetAiTarget) {
            switchCase -= 0x100;
            targetPlayerNo = _GetPlayerNo_aiChrIdx(&targetAiParam->aiInput->cpuIdx);
            EntryID eID = FIGHTER_MANAGER->getEntryId(targetPlayerNo);
            targetFighterEntry = _getEntity_ftEntryManager(FIGHTER_MANAGER->entryManager, eID);
            bool getChild = targetAiParam->aiInput->aiActPtr->aiStatPtr->character == CHAR_ID::Nana;
            targetFighter = FIGHTER_MANAGER->getFighter(eID, getChild);
        } else {
            targetPlayerNo = _GetPlayerNo_aiChrIdx(&selfAi->aiInputPtr->cpuIdx);
            EntryID eID = FIGHTER_MANAGER->getEntryId(targetPlayerNo);
            targetFighterEntry = _getEntity_ftEntryManager(FIGHTER_MANAGER->entryManager, eID);
            bool getChild = selfAi->aiInputPtr->aiActPtr->aiStatPtr->character == CHAR_ID::Nana;
            targetFighter = FIGHTER_MANAGER->getFighter(eID, getChild);
        }
        switchCase -= 0x1000;

        if (switchCase == 0x62) {
            fn_result = SC_MELEE->stageLoader->stage->framesPassed;
            // OSReport("counter = %.3f; %08x; \n", fn_result, fn_result);
            fn_shouldReturnResult = 1;
            return;
        }

        if (isMMS) {
            auto* modules = FIGHTER_MANAGER->getFighter(FIGHTER_MANAGER->getEntryId(0), false)->modules;
            if (modules->statusModule->action == 0xBD || modules->statusModule->desiredAction == 0xBD) {
                fn_result = -1;
                fn_shouldReturnResult = 1;
                return;
            }
        }

        // functional stack pop
        if (switchCase == 0x5E) {
            if (functionalStackPtr >= 0) {
                fn_result = functionalStack[functionalStackPtr];
                fn_shouldReturnResult = 1;
                functionalStackPtr --;
                return;
            } else {
                OSReport("========ERROR: TRIED TO ACCESS STACK VARIABLE THAT DIDN'T EXIST========\n");
            }
        }

        if (switchCase == 0x5F) {
            LTFStackPtr++;
            if (LTFStackPtr >= 0) {
                fn_result = LTFStack[LTFStackPtr];
                fn_shouldReturnResult = 1;
                return;
            } else {
                OSReport("========ERROR: TRIED TO ACCESS LTF STACK VARIABLE THAT DIDN'T EXIST========\n");
            }
        }

        // hack to ensure MMS doesn't die
        if (targetFighterEntry == nullptr || targetFighter == nullptr || targetFighter->modules == nullptr) {
            OSReport("no target!!: %02x; shouldGetTarget? %d; ftEntry: %08x; targetPlayerNo: %d; targetFighter: %08x\n", switchCase, shouldGetAiTarget, targetFighterEntry, targetPlayerNo, targetFighter);
            fn_result = 0;
            fn_shouldReturnResult = 1;
            return;
        }

        if (switchCase == 0x41) {
            fn_result = targetFighter->modules->ftStopModule->hitlagMaybe;
            fn_shouldReturnResult = 1;
            return;
        }

        if (switchCase == 0x22) {
            auto entry = selfAi->aiStatPtr->aiInput->ftEntryPtr;
            if (entry != nullptr && entry->ftStageObject != nullptr) {
                soModuleAccessor* accessorPtr = entry->ftStageObject->modules;
                auto& pcModule = *accessorPtr->paramCustomizeModule;
                float currXVel = selfAi->aiStatPtr->xVel;
                float absCurrXVel = currXVel - 2 * currXVel * (currXVel < 0);
                float lostMomentum = (pcModule.jumpSquatFrames * pcModule.groundFriction * 2);
                float adjustedXVel = 
                    (currXVel == 0) ? 0 
                    : (currXVel + (lostMomentum - lostMomentum * 2 * (currXVel > 0)));
                if (absCurrXVel < lostMomentum) {
                    adjustedXVel = 0;
                }
                fn_result = adjustedXVel;
            } else {
                fn_result = 1;
            }
            fn_shouldReturnResult = 1;
            return;
        }
        // 0x817d13f0, f4, f8

        // 0x8081c874
        // 0x817d10d0
        // 0x817d0fa4        
        if (switchCase == 0x27 || switchCase == 0x28) {
            AiStat& stat = *(switchCase == 0x28 ? targetAiParam->aiInput->aiActPtr->aiStatPtr : selfAi->aiStatPtr);
            auto entry = stat.aiInput->ftEntryPtr;
            if (entry != nullptr && entry->ftStageObject != nullptr) {
                soModuleAccessor* accessorPtr = entry->ftStageObject->modules;
                if (accessorPtr != nullptr) {
                    soModuleAccessor& accessor = *accessorPtr;
                    fn_result = stat.hurtboxSize * 2;
                    if (!stat.floorInfoBitfield.infos.isTouchingGround || (accessor.statusModule->action == 0xB && accessor.motionModule->getFrame() <= 10)) {
                        fn_result -= (stat.topNPos.f2 - stat.position.f2);
                    }
                } else {
                    fn_result = 5;
                }
            } else {
                fn_result = 5;
            }
            fn_shouldReturnResult = 1;
            return;
        }

        if (switchCase == 0x4C) { // IsOnStage
            fn_shouldReturnResult = 1;
            if (targetFighter->modules->groundModule == nullptr) {
                fn_result = 0;
                return;
            }

            Vec3f startPos = {};
            targetFighter->modules->groundModule->getCorrectPos(&startPos);

            Vec3f destPos {
                0,
                -500,
                0
            };

            Vec3f ret1 {-1,-1,-1};
            Vec3f ret2 {-1,-1,-1};

            int rayResult = _stRayCheck_vec3f(&startPos, &destPos, &ret1, &ret2, true, 0, 1, 1);
            
            fn_result = -1;
            if (rayResult) {
                fn_result = 1;
                return;
            }

            fn_result = 0;
            return;
        }

        if (switchCase == 0x4B) { // YDistFloor
            if (targetFighter->modules->groundModule == nullptr) {
                fn_result = -1;
                return;
            }
            Vec3f startPos = {};
            xyDouble dPos = targetFighter->modules->groundModule->getDownPos();
            startPos.f1 = dPos.xPos;
            startPos.f2 = dPos.yPos;
            startPos.f3 = 0;
            Vec3f destPos {
                0,
                -500,
                0
            };

            Vec3f ret1 {-1,-1,-1};
            Vec3f ret2 {-1,-1,-1};

            int rayResult = _stRayCheck_vec3f(&startPos, &destPos, &ret1, &ret2, true, 0, 1, 1);
            
            fn_result = -1;
            if (rayResult) {
                startPos.f1 -= ret1.f1;
                startPos.f2 -= ret1.f2;
                fn_result = _length_vec3f(&startPos);
                // OSReport("NORMALIZED RESULT: %.3f\n", res);
            }

            fn_shouldReturnResult = 1;
            return;
        }

        if (switchCase == 0x51) {
            fn_result = FIGHTER_MANAGER->getInput(targetFighterEntry->entryId)->aiMd;
            fn_shouldReturnResult = 1;
            return;
        }
        if (switchCase == 0x52) {
            fn_result = targetPlayerNo;
            fn_shouldReturnResult = 1;
            return;
        }
        if (switchCase == 0x53) {
            if (targetFighterEntry == nullptr) {
                fn_result = -1;
            } else {
                fn_result = 0;
    //            OSReport("move id: %08x\n", targetFighterEntry->owner->ownerDataPtr->currStaleQueueNum);
                for (int i = 0; i < 9; i++) {
    //                OSReport("i: %d; moveNum: %08x\n", i, targetFighterEntry->owner->ownerDataPtr->staleMoveQueue[i].attack);
                    if (targetFighterEntry->owner->ownerDataPtr->staleMoveQueue[i].attack == targetFighterEntry->owner->ownerDataPtr->currStaleQueueNum) {
                        fn_result++;
                    }
                }
            }
            fn_shouldReturnResult = 1;
            return;
        }
        if (switchCase == 0x54) {
            if (targetFighter->modules->groundModule == nullptr) fn_result = 0;
            else fn_result = targetFighter->modules->groundModule->isPassableGround(0);
            fn_shouldReturnResult = 1;
            return;
        }

        if (switchCase == 0x55) {
//            OSReport("ftEntry Address: %08x\n", targetFighterEntry);
//            OSReport("ftSo Address: %08x\n", targetFighter);
//            OSReport("UnkFtPtr Address: %08x\n", targetFighter->modFnAccessor);
//            OSReport("Supposed cancelModule address: %08x\n", targetFighter->modFnAccessor->getFtCancelModule(targetFighter));
            if (targetFighter == nullptr) {
                fn_result = 0;
                return;
            }
            fn_result = targetFighter
                    ->getCancelModule()
                    ->isEnableCancel();

            fn_shouldReturnResult = 1;
            return;
        }

        // weight
        if (switchCase == 0x56) {
            fn_result = targetFighter->modules->paramCustomizeModule->weight;
            fn_shouldReturnResult = 1;
            return;
        }
        // gravity
        if (switchCase == 0x57) {
            fn_result = targetFighter->modules->paramCustomizeModule->gravity;
            fn_shouldReturnResult = 1;
            return;
        }
        // fastfallspeed
        if (switchCase == 0x58) {
            fn_result = targetFighter->modules->paramCustomizeModule->fastFallSpeed;
            fn_shouldReturnResult = 1;
            return;
        }
        // endframe
        if (switchCase == 0x59) {

            // fn_result = 1;
            // fn_shouldReturnResult = 1;
            auto motionModule = targetFighter->modules->motionModule;
            if (motionModule != nullptr && motionModule->mainAnimationData.resPtr != nullptr) {
                fn_result = motionModule->getEndFrame();
                fn_shouldReturnResult = 1;
            }
            return;
        }
        // width
        if (switchCase == 0x5A) {
            auto groundModule = targetFighter->modules->groundModule;
            if (groundModule == nullptr) fn_result = 0;
            else fn_result = (groundModule->getRightPos().xPos - groundModule->getLeftPos().xPos) * 1.5;
            fn_shouldReturnResult = 1;
            return;
        }
        // ECB Center X
        if (switchCase == 0x5B) {
            auto groundModule = targetFighter->modules->groundModule;
            if (groundModule == nullptr) fn_result = 0;
            else fn_result = (groundModule->getRightPos().xPos + groundModule->getLeftPos().xPos) / 2;
            fn_shouldReturnResult = 1;
            return;
        }
        // ECB Center Y
        if (switchCase == 0x5C) {
            auto groundModule = targetFighter->modules->groundModule;
            if (groundModule == nullptr) fn_result = 0;
            else fn_result = (groundModule->getUpPos().yPos + groundModule->getDownPos().yPos) / 2;
            fn_shouldReturnResult = 1;
            return;
        }

        // player count
        if (switchCase == 0x5D) {
            fn_result = getPlayerCount();
            fn_shouldReturnResult = 1;
            return;
        } // 80623560

        // shieldStunRemain
        if (switchCase == 0x60) {
            auto RABasicsArr = (*(int (*)[targetFighter->modules->workModule->RAVariables->basicsSize])targetFighter->modules->workModule->RAVariables->basics);
            fn_result = RABasicsArr[0x5];
            fn_shouldReturnResult = 1;
            return;
        }

        // scale
        if (switchCase == 0x61) {
            fn_result = targetFighter->getOwner()->scale;
            fn_shouldReturnResult = 1;
            return;
        }
        
        // action category
        if (switchCase == 0x63) {
            fn_result = targetFighterEntry->ftInput->aiInputMain->aiActPtr->aiStatPtr->aiActionCategory;
            fn_shouldReturnResult = 1;
            return;
        }

        if (switchCase == 0x70) {
            fn_result = targetFighter->modules->animCmdModule->threadList->instanceUnitFullPropertyArrayVector.threadUnion.ActionMain.cmdInterpreter->logicalFrame;
            fn_shouldReturnResult = 1;
            return;
        }

        // throw frame
        if (switchCase == 0x71) {
            fn_result = playerTrainingData[targetPlayerNo < 4 ? targetPlayerNo : 0].debug.psaData.throwFrame;
            fn_shouldReturnResult = 1;
            return;
        }

        if (switchCase >= 0x80 && switchCase <= 0x8B && targetPlayerNo >= 4) {
            switchCase -= 0x80;
            fn_shouldReturnResult = 1;
            fn_result = 1;
            switch(switchCase) {
                case 0x0: fn_result = 1.75; return;
                case 0x1: fn_result = 0.6; return;
                case 0x2: fn_result = 0.3; return;
                case 0x3: fn_result = 0.6; return;
                case 0x4: fn_result = 0.8; return;
                case 0x5: fn_result = 0.2; return;
                case 0x6: fn_result = 0.3; return;
                case 0x7: fn_result = 0.4; return;
                case 0x8: fn_result = 0.35; return;
                case 0x9: fn_result = 1; return;
                case 0xA: fn_result = 0.75; return;
                case 0xB: fn_result = 1; return;
            }
            return;
        }
        fn_shouldReturnResult = 1;
        switch (switchCase) {
            case 0x80: fn_result = playerTrainingData[targetPlayerNo].options.AI.aggression; return;
            case 0x81: fn_result = playerTrainingData[targetPlayerNo].options.AI.bait_dashAwayChance; return;
            case 0x82: fn_result = playerTrainingData[targetPlayerNo].options.AI.bait_wdashAwayChance; return;
            case 0x83: fn_result = playerTrainingData[targetPlayerNo].options.AI.baitChance; return;
            case 0x84: fn_result = playerTrainingData[targetPlayerNo].options.AI.braveChance; return;
            case 0x85: fn_result = playerTrainingData[targetPlayerNo].options.AI.circleCampChance; return;
            case 0x86: fn_result = playerTrainingData[targetPlayerNo].options.AI.djumpiness; return;
            case 0x87: fn_result = playerTrainingData[targetPlayerNo].options.AI.jumpiness; return;
            case 0x88: fn_result = playerTrainingData[targetPlayerNo].options.AI.platChance; return;
            case 0x89: fn_result = playerTrainingData[targetPlayerNo].options.AI.SDIChance; return;
            case 0x8A: fn_result = playerTrainingData[targetPlayerNo].options.AI.wall_chance; return;
            case 0x8B: fn_result = playerTrainingData[targetPlayerNo].options.AI.reactionTime; return;
            default:
                fn_shouldReturnResult = 0;
        }

        if (switchCase == 0xFD) {
            fn_result = playerTrainingData[targetPlayerNo].options.AI.debugValue;
            fn_shouldReturnResult = 1;
            return;
        }

        if (switchCase == 0xFE) {
            fn_result = -1;
            if (SCENE_NAME == 0x71547261) {
                fn_result = *TRAINING_MODE_OPTION;
            }
            fn_shouldReturnResult = 1;
            return;
        }
        if (switchCase == 0xFF) {
            fn_result = targetPlayerNo < 4 ? playerTrainingData[targetPlayerNo].options.AI.debug : false;
            fn_shouldReturnResult = 1;
            return;
        }
    };
    void outputAiFunctionResult() {
        asm(R"(
            lfd f1, 0x00(%0)
            mr 12, %1
        )"
        :
        : "r" (&fn_result), "r" (fn_shouldReturnResult));
    };
}

bool rq_shouldReturnResult = false;
INJECTION("CUSTOM_AI_REQUIREMENTS", 0x8091ed20, R"(
    SAVE_REGS
    mr r3,r23
    mr r4,r24
    bl customAiRequirements
    RESTORE_REGS
    lbz r0,0x0(r24)
    bl customAiRequirementOutput
)");

// INJECTION("OUTPUT_CUSTOM_REQUIREMENTS", 0x8091ed44, R"(
//     mr r3,r28
// )");
int originScript = 0;

int* gotoCmdStack[0x11] = {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}; 
int gotoCmdScripts[0x11] = {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0};
int* gotoCmdScriptHeads[0x11] = {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0};
int gotoCmdStackPtr = 0;
#define _get_script_value_aiScriptData ((double (*)(AiScriptData * self, int soughtValue, int isPartOfVector)) 0x8091dfc4)
#define _get_script_tag_scriptpac ((int* (*)(int* aice, int* commonce, int target)) 0x8091dedc)
extern "C" {
    bool rq_result = false;
    void customAiRequirements(AiScriptData* aiActInst, const int* args) {
        rq_shouldReturnResult = false;
        if (args[1] < 0x1000) return;
        rq_result = false;
        rq_shouldReturnResult = true;
        // catches weird loading things
        // if (aiActInst == nullptr
        //     || aiActInst->aiInputPtr == nullptr
        //     || aiActInst->aiInputPtr->ftEntryPtr == nullptr
        //     || aiActInst->aiInputPtr->ftEntryPtr->owner == nullptr) return;
        switch(args[1]) {
            case 0x1022: {
                double traitVal = _get_script_value_aiScriptData(aiActInst, *(int *) &args[2], 0);
                double multiplier = _get_script_value_aiScriptData(aiActInst, *(int *) &args[3], 0);
                rq_result = (bool)(_randf() <= (traitVal * multiplier));
                break;
            }
            case 0x1023: {
                double traitVal = _get_script_value_aiScriptData(aiActInst, *(int *) &args[2], 0);
                double multiplier = _get_script_value_aiScriptData(aiActInst, *(int *) &args[3], 0);
                rq_result = (bool)(_randf() >= (traitVal * multiplier));
                break;
            }
            // CalledFrom 817cead4
            case 0x1024: {
                unsigned int scriptVal = *(unsigned int *) &args[2];
                for (int i = 0; i < gotoCmdStackPtr; i++) {
                    if (*gotoCmdScriptHeads[i] == scriptVal) {
                        rq_result = true;
                        break;
                    }
                }
                break;
                // 0x817d16c4, 8, c, 
                // 0x8081c874 
            }
            // HasTurbo (curry)
            case 0x1025: {
                auto* ftEntry = aiActInst->aiInputPtr->ftEntryPtr;
                auto ChId = aiActInst->aiInputPtr->charId;
                if (ftEntry == nullptr || ftEntry->owner == nullptr || (isMMS && (ChId >= CHAR_ID::Zakoboy && ChId <= CHAR_ID::Zakoball))) {
                    rq_result = false;
                } else {
                    rq_result = ftEntry->owner->hasCurry();
                }
                break;
            }
            // 0x805b561c, 20
            // 0x805b5660, 64

            case 0x1026: {
                // 0x9017F378 = stamina/300% mode flag
                // 0x9017F385 = blastzones on/off flag
                rq_result = !((*(u8*)0x9017F378) == 2 && (*(u8*)0x9017F385) != 2);
                break;
            }
            case 0x1027: {
                AiInput* targetAiInput = FIGHTER_MANAGER->getOwner(FIGHTER_MANAGER->getEntryId(aiActInst->aiInputPtr->aiTarget))->aiInputPtr;
                if (targetAiInput != nullptr) {
                    auto targetFighterEntry = targetAiInput->ftEntryPtr;
                    bool getChild = targetFighterEntry == nullptr;
                    int targetPlayerNo = _GetPlayerNo_aiChrIdx(&targetAiInput->cpuIdx);
                    if (targetPlayerNo >= 4) {
                        rq_result = false;
                        return;
                    }
                    rq_result = playerTrainingData[targetPlayerNo].debug.psaData.shouldTechThrow;
                } else {
                    rq_result = false;
                }
                break;
            }
            // 0x817d199c, a0, a4, a8
            // 0x817d18b4
            // 0x817d16c4

            // 0x8055cc38
            // 0x8055ce64
            // 0x8055ccec

            // 0x8081c874, 78
            // 0x805b561c
            case 0x1028: { // NoJumpLastFrame
                rq_shouldReturnResult = true;
                auto ChId = aiActInst->aiInputPtr->charId;
                if (ChId >= CHAR_ID::Zakoboy && ChId <= CHAR_ID::Zakoball) {
                    rq_result = true;
                } else {
                    auto* ftEntry = aiActInst->aiInputPtr->ftEntryPtr;
                    if (ftEntry != nullptr && ftEntry->ftStageObject != nullptr && ((int) ftEntry->ftStageObject) > 0x80000000)
                        rq_result = !((ftControllerModuleImpl*)(ftEntry->ftStageObject->modules->controllerModule))->controller.prevInputs.jump;
                    else
                        rq_result = true;
                }
                break;
            }
            case 0x1029: { // GettingThrown
                rq_shouldReturnResult = true;
                auto ChId = aiActInst->aiInputPtr->charId;
                if (ChId >= CHAR_ID::Zakoboy && ChId <= CHAR_ID::Zakoball) {
                    rq_result = false;
                } else {
                    auto* ftEntry = aiActInst->aiInputPtr->ftEntryPtr;
                    if (ftEntry != nullptr && ftEntry->ftStageObject != nullptr && ((int) ftEntry->ftStageObject) > 0x80000000) {
                        int action = ftEntry->ftStageObject->modules->statusModule->action;
                        rq_result = (action == 0x42 || (action >= 0xE6 && action <= 0xEA) || action == 0xEE || action == 0xD7 || action == 0xF0);
                    }
                    else {
                        rq_result = false;
                    }
                }
                break;
            }
            default: {
                rq_shouldReturnResult = false;
            }
        }
        return;
    };
    void customAiRequirementOutput() {
        if (rq_shouldReturnResult) {
            asm("mr r28, %0"
                :
                : "r" (rq_result ? 0x1 : 0x0));
            rq_shouldReturnResult = false;
        }
    }
}

INJECTION("AI_GOTO_WITH_STACK_PUSH", 0x80917620, R"(
    SAVE_REGS
    mr r3, r26
    mr r4, r30
    bl aiGotoPush
    RESTORE_REGS
    mr r29, r30
)");
INJECTION("AI_GOTO_WITH_STACK_POP", 0x809174c4, R"(
    mr r3, r26
    bl aiGotoPop
    mr r29, r3
    mr r30, r3
)");
INJECTION("AI_GOTO_WITH_STACK_FIX1", 0x80917548, R"(
    mr r3, r26
    bl clearGotoStack
    lhz r0, 0x2(r30)
)");
INJECTION("AI_GOTO_WITH_STACK_FIX2", 0x80917470, R"(
    mr r3, r26
    bl clearGotoStack
    lhz r0, 0x7e(r26)
)");
INJECTION("AI_GOTO_WITH_STACK_FIX3", 0x80917604, R"(
    mr r3, r26
    bl clearGotoStack
    stw r6, 0xa8(r26)
)");
INJECTION("AI_GOTO_WITH_STACK_FIX4", 0x80917c34, R"(
    mr r3, r26
    bl clearGotoStack
    lbz r0,0x1(r30)
)");
INJECTION("AI_GOTO_WITH_STACK_FIX5", 0x80918148, R"(
    mr r3, r26
    bl clearGotoStack
    lhz r4,0x7a(r26)
)");
INJECTION("CLEAR_DYNAMIC_DICE", 0x80918500, R"(
    mr r3, r26
    bl clearDynamicDice
    lfd f31, 0x100(r1)
)")

// THIS TAKES PLACE IN IF_CHK ON THE REQUIREMENT SIDE OF THINGS
INJECTION("CUSTOM_AI_XGOTO_REQ_FIX", 0x8091ea68, R"(
    bl aiReqCalledAsFix
)");

vector<const void*> NoRepeatInstructions = vector<const void*>();
#define _get_fix_target_aiInput ((char (*)(AiInput* self, int zero)) 0x80908e8c)
SIMPLE_INJECTION(clearNoRepeatInstruction, 0x809171f4, "li r31, 0x0") {
    NoRepeatInstructions.reallocate(0);
    NoRepeatInstructions.reallocate(1);
    dynamicDice[0].clear();
    dynamicDice[1].clear();
    functionalStackPtr = -1;
    LTFStackPtr = -1;
}

int* forcedNextInstruction = nullptr;
bool forceContinue = false;
extern "C" {
    void aiReqCalledAsFix() {
        if (originScript != 0) {
            asm("mr r0, %0"
                :
                : "r" (originScript));
        } else {
            // otherwise use the default code
            asm("lhz r0,0x78(r23)");
        }
    }
    void clearDynamicDice(AiScriptData* aiActInst) { 
        dynamicDice[0].clear();
        dynamicDice[1].clear();
    }
    // SOMETHING CAUSED AISCRIPT TO BE SET TO 0 AND IDK WHAT IT IS
    // BUT AT LEAST THE CORRECT ONE IS STORED IN AISCRIPT
    // IT'S SO WEIRD BUT I MEAN HEY IT WORKS I GUESS
    void aiGotoPush(AiScriptData* aiActInst, int* nextPlace) {
        if (aiActInst->aiScript == 0) aiActInst->aiScript = *(int*)aiActInst->constPtr;
        int scriptToGet = aiActInst->aiScript;
        // skips "execution" of the label targeted by Goto statement
        // forcedNextInstruction += 2;

        gotoCmdStack[gotoCmdStackPtr] = nextPlace + 2;
        gotoCmdScripts[gotoCmdStackPtr] = 0;
        gotoCmdScriptHeads[gotoCmdStackPtr] = aiActInst->constPtr;
        // OSReport("Goto: (%08x) ==> %d: %08x\n", scriptToGet, gotoCmdStackPtr, aiActInst->constPtr);
        gotoCmdStackPtr += 1;
    };
    void aiXGotoPush(AiScriptData* aiActInst, int* nextPlace, int nextScript) {
        if (aiActInst->aiScript == 0) aiActInst->aiScript = *(int*)aiActInst->constPtr;
        if (originScript == 0) originScript = aiActInst->aiScript;
        // offsets by 1 command
        gotoCmdStack[gotoCmdStackPtr] = nextPlace + 2;
        gotoCmdScripts[gotoCmdStackPtr] = aiActInst->aiScript;
        gotoCmdScriptHeads[gotoCmdStackPtr] = aiActInst->constPtr;
        int pNum = _GetPlayerNo_aiChrIdx(&aiActInst->aiInputPtr->cpuIdx);
        if (nextScript != 0x8505 && pNum < 4) {
            auto& scriptPath = playerTrainingData[pNum].aiData.scriptPath;
            scriptPath.push(new AIScriptCache {(unsigned short) nextScript, (char) gotoCmdStackPtr + 1});
        }
        // OSReport("XGoto: (%08x) ==> %d: %08x: [\n", aiActInst->aiScript, gotoCmdStackPtr, aiActInst->constPtr);
        // for (int i = 0; i < gotoCmdStackPtr; i++) {
        //     OSReport("%08x, ", gotoCmdScriptHeads[i]);
        // }
        // OSReport("]\n");
        gotoCmdStackPtr += 1;
    };
    char dummy = 0xFF;
    int* aiGotoPop(AiScriptData* aiActInst) {
        // OSReport("POP-PRE: (%08x) <== %d: %08x\n", aiActInst->aiScript, gotoCmdStackPtr, aiActInst->constPtr);
        // OSReport("POP-PRE: (%08x) <== %d: %08x: [\n", aiActInst->aiScript, gotoCmdStackPtr, aiActInst->constPtr);
        // for (int i = 0; i < gotoCmdStackPtr; i++) {
        //     OSReport("%08x, ", gotoCmdStack[i]);
        // }
        // OSReport("]\n");
        if (gotoCmdStackPtr > 0) {
            gotoCmdStackPtr -= 1;
            if (gotoCmdScripts[gotoCmdStackPtr] > 0) {
                int pNum = _GetPlayerNo_aiChrIdx(&aiActInst->aiInputPtr->cpuIdx);
                if (gotoCmdScripts[gotoCmdStackPtr] != 0 && aiActInst->aiScript != 0x8505 && pNum < 4) {
                    auto& scriptPath = playerTrainingData[pNum].aiData.scriptPath;
                    auto thing = new AIScriptCache {(unsigned short) gotoCmdScripts[gotoCmdStackPtr], (char) -(gotoCmdStackPtr) - 1};
                    scriptPath.push(thing);
                }

                aiActInst->aiScript = 0;
                _act_change(aiActInst, gotoCmdScripts[gotoCmdStackPtr], &dummy, 0, 0);
                aiActInst->constPtr = gotoCmdScriptHeads[gotoCmdStackPtr];
                
            } else if (gotoCmdScripts[gotoCmdStackPtr] == -1) {
                
            }
            // OSReport("POP: (%08x) <== %d: %08x\n", gotoCmdScripts[gotoCmdStackPtr], gotoCmdStackPtr, gotoCmdStack[gotoCmdStackPtr]);
            if (gotoCmdStackPtr == 0) originScript = 0;
            forcedNextInstruction = gotoCmdStack[gotoCmdStackPtr];
            return forcedNextInstruction;
        }
        if (gotoCmdStackPtr == 0) originScript = 0;
        forcedNextInstruction = nullptr;
        return forcedNextInstruction;
    };
    void clearGotoStack(AiScriptData* aiActInst) {
        asm("SAVE_REGS");
        // OSReport("CLEARED AT: %08x\n", aiActInst->currentInstruction);
        // OSReport("CLEARED GOTO STACK\n");
        aiActInst->aiScript = *(int*)aiActInst->constPtr;
        asm("RESTORE_REGS");
        functionalStackPtr = -1;
        gotoCmdStackPtr = 0;
        // gotoCmdStack[gotoCmdStackPtr] = nextPlace + 2;
        // gotoCmdScripts[gotoCmdStackPtr] = aiActInst->aiScript;
        // gotoCmdScriptHeads[gotoCmdStackPtr] = aiActInst->constPtr;
        originScript = 0;
    };
}

bool forcedReturnStatement = false;
INJECTION("CUSTOM_AI_COMMANDS", 0x80917450, R"(
    lbz r4, 0x00(r30)
    SAVE_REGS
    mr r3, r26
    mr r4, r30
    mr r5, r28
    bl aiCommandHandlers
    RESTORE_REGS
    bl FORCE_RETURN
)");

INJECTION("CUSTOM_XGOTO_RELOCATE_FIX", 0x809181d4, R"(
    bl RELOCATE_INSTRUCTION
    cmpwi r30, 0x1
    bne normal
    lis r12, 0x8091
    ori r12, r12, 0x84fc
    mtctr r12
    bctr

normal:
    bl FORCE_XGOTO_CONTINUE
)")

// FIXME: remove later

// old position: 0x80904958

// # <gutteral screaming>
// timingFloat:
//     .float 5
//     .float 20
// timingCheck:
//     lis r12, timingFloat@ha
//     ori r12, r12, timingFloat@l
//     lfs f2, 0x0(r12)

//     # let pivot grab exist
//     # lets nana pivot grab
//     fcmpo cr0, f1, f2
//     ble notThrow

//     lfs f2, 0x4(r12)
//     # keeps nana from buffering dash
//     fcmpo cr0, f1, f2 
//     bge notThrow

// cmpwi r3, 0x36 # = "catchDash" (action)
//     beq- timingCheck
//     cmpwi r3, 0x38 # = "catchTurn" (action)
//     beq- timingCheck

// INJECTION("nana_throw_routine_correction", 0x80903440, R"(
//     # last line before nana child_update call
//     lwz r3, 0x0034 (r31)
    
//     # r23 = AiInput
//     # r24 = stick outputs
//     # 0x8(r1) = button output
//     # r30 = AiStat

// check_if_grabbing:
//     # clears nana inputs
//     # li r3, 0x0
//     # stw r3, 0x0(r24) # ? xCoord
//     # stw r3, 0x4(r24) # ? yCoord
//     # stw r3, 0x8(r1)

//     # AI action
//     # checks if between 0x34 and 0x3B which covers all "grabbing/holding" states
//     lwz r3, 0xB4(r30)
//     # 0x34 = "Catch" (action)
//     cmpwi r3, 0x34
//     blt+ notThrow
//     # 0x3B = "CatchCut" (action)
//     cmpwi r3, 0x3B
//     bgt+ notThrow
//     b setupGrab

// setupGrab:
//     # grab or setup
//     li r3, 0x0
//     stw r3, 0x0(r24) # stickX
//     stw r3, 0x4(r24) # stickY
//     stw r3, 0x8(r1)
//     lwz r4, 0x8(r1) # buttons
//     rlwinm r4, r4, 0, 23, 21
//     stw r4, 0x8(r1) 
    
//     lwz r3, 0x44(r23) # r3 = AiAct
//     lhz r4, 0x78(r3) # r4 = aiScript
//     cmpwi r4, 0x6100 # if aiScript = 6100
//     beq- inThrowRoutine

//     # setup throw routine?
//     li r3, 0x0
//     stw r3, 0x0(r24) # ? xCoord
//     stw r3, 0x4(r24) # ? yCoord
//     stw r3, 0xD0(r1) # stick XCoord
//     stw r3, 0xD4(r1) # stick YCoord
//     # copy button input bitfield from current AiInput
//     lwz r3, 0xC(r23) # \tell nana inputs should not change since previous logic
//     stw r3, 0x8(r1)  # /
//     b changeToThrowRoutine

// inThrowRoutine:
//     # relies on AI script to tell it when the throw is done for some reason?
//     # this will only branch if AI var23 is set to 0 :thonk:
//     lwz r4, 0x5C(r3)
//     cmpwi r4, 0x0
//     bne- notThrow

//     lwz r4, 0x8(r1)                 # \
//     andi. r3, r4, 0x100             # |
//     cmpwi r3, 0x100                 # | if cstick is pressed, tell nana to forget about it
//     # rlwinm r4, r4, 0, 23, 21      # | (<-- eliminate cstick input)
//     beq changeToThrowRoutine        # |
//     stw r3, 0x8(r1)                 # /

//     b notThrow

// changeToThrowRoutine:
//     #act_change to 0x6100
//     #args: (AiAct*, unsigned int nextScript, char* nextTargetAIChrIdx, int unk1, int unk2)
    
//     lwz r3, 0x44(r23) # r3 = AiAct
//     li r5, 0x1
//     stw r5, 0x5C(r3)      # sets internal AI variable (var23) to 1 for some reason 
//     lhz r4, 0x78(r3)

//     li r4, 0x6100         # next script
//     lis r5, -1            #\ initialise this variable that PMDT never did
//     stb r5, 0x24(r1)      #/ (pointer to 0xFF, effectively randomizes the AI target)
//     li r6, 0 # unk1 arg
//     li r7, 0 # unk2 arg

//     # calls act_change(this, 0x6100, &0xFF, 0, 0)
//     lis r12, 0x8090
//     ori r12, r12, 0x3F4C
//     mtctr r12
//     bctr

//     #exit
// notThrow:
//     lwz r3,0x0(r24)
// )")

// #################################
// nana throws without considering human input [Bero, v1.2 by Eon, dual-hook fix by fudgepop01]
// #################################
// INJECTION("NANA_FIX_1", 0x80903440, R"(
//     # update::aiInput (specifically right before calling aiInput::child_update)
    
//     # last line before nana child_update call
//     lwz r3, 0x0034 (r31)
    
//     # r3 = childAi (AiInput)
//     # r4 = nana stick ptr
//     # r5 = nana buttons
//     # r6 = aiChrIdxPtr
//     # r7 = is tap jump enabled

// check_if_grabbing:
//     mr r14, r3 # AiInput
//     lwz r15, 0x44(r14) # AiInput->AiAct r15 = AiAct
//     lwz r16, 0x74(r15) # AiAct->AiStat r16 = AiStat
//     # AI action
//     # checks if between 0x34 and 0x3B which covers all "grabbing/holding" states
//     lwz r17, 0xB4(r16)
//     # 0x34 = "Catch" (action)
//     cmpwi r17, 0x34
//     blt+ notThrow
//     # 0x3B = "CatchCut" (action)
//     cmpwi r17, 0x3B
//     bgt+ notThrow
//     b setupGrab

// setupGrab:
//     # grab or setup
//     li r17, 0x0
//     stw r17, 0x0(r4) # stickX
//     stw r17, 0x4(r4) # stickY
//     mr r5, r17 # buttons
//     # li r17, 0x79 # forces nana into mode that allows ai scripts
//     li r17, 0x7e # forces nana into mode that allows ai scripts
//     stw r17, 0x48(r14)
//     li r17, 0xA
//     stb r17, 0x1ba(r16)
    
//     lhz r17, 0x78(r15) # r4 = aiScript
//     cmpwi r17, 0x6100 # if aiScript = 6100
//     beq- inThrowRoutine
//     b changeToThrowRoutine

// inThrowRoutine:
//     # relies on AI script to tell it when the throw is done for some reason?
//     # this will only branch if AI var23 is set to 0 :thonk:
//     lwz r17, 0x5C(r15)
//     cmpwi r17, 0x0
//     bne- notThrow

//     # li r17, 0x60 # forces nana back into default AI mode
//     # stw r17, 0x48(r14)
//     lwz r17, 0x8(r5)                 # \
//     andi. r18, r17, 0x100            # |
//     cmpwi r18, 0x100                 # | if cstick is pressed, tell nana to forget about it
//     beq changeToThrowRoutine         # |
//     stw r18, 0x8(r5)                 # /

//     b notThrow

// changeToThrowRoutine:

//     #act_change to 0x6100
//     #args: (AiAct*, unsigned int nextScript, char* nextTargetAIChrIdx, int vanilla ai routine, int unk2)
    
//     mr r3, r15 # r3 = AiAct
//     li r5, 0x1
//     stw r5, 0x5C(r3)      # sets internal AI variable (var23) to 1 for some reason 
//     lhz r4, 0x78(r3)

//     li r4, 0x6100         # next script
    
//     li r0, -0x1             #\ initialise this variable that PMDT never did
//     stb r0, 0x24(r1)        #| (stb + addi make it a pointer to 0xFF, -
//     addi r5, r1, 0x24       #/ - effectively randomizes the AI target)
//     li r6, 0x6100 # unk1 arg
//     li r7, 0 # unk2 arg

//     # calls act_change(this, 0x6100, &0xFF, 0, 0)
//     lis r12, 0x8091
//     ori r12, r12, 0x8554
//     mtctr r12
//     bctr
//     # the bctr means it just branches and will not return here at the next blr.
//     # Thus: exit

// notThrow:
// )")

// INJECTION("NANA_FIX_2", 0x8090366c, R"(
//     # FNTE = Force Nana Throw Execution
//     # child_update::aiInput (specifically right before the nana distance check)

// FNTE_begin:
//     lwz r14, 0x44(r23) # AiInput->AiAct r14 = AiAct
//     lwz r15, 0x74(r14) # AiAct->AiStat r15 = AiStat
    
//     # AI action (see previous HOOK for explaination)
//     lwz r16, 0xB4(r15)
//     # 0x34 = "Catch" (action)
//     cmpwi r16, 0x34
//     blt+ FNTE_normalExec
//     # 0x3B = "CatchCut" (action)
//     cmpwi r16, 0x3B
//     bgt+ FNTE_normalExec
//     bl FNTE_cache_large_dist
//     b FNTE_normalExec

// FNTE_big_long_dist:
//     .float 9999

// FNTE_cache_large_dist:
//     mflr r12 # lr is set by the bl here...
//     lfs f31, 0x4(r12) # ...which lets us grab the "FNTE_big_long_dist" float based on an offset
//     blr

// FNTE_normalExec:
//     fcmpo 0, f31, f29
// )");

// INJECTION("NANA_FIX_3", 0x809188B0, R"(
//     # forces nana to ignore calls to script 0x1120 while grabbing 
//     # r26 = next script
//     # r25 = AiActPtr
// nana_check:
//     lwz r14, 0x74(r25) # r14 = AiStat
//     lbz r15, 0xAF(r14) # r15 = character ID
//     cmpwi r15, 0x10
//     bne nana_fix3_end

// action_check:
//     lwz r15, 0xB4(r14) # r15 = action ID
//     cmpwi r15, 0x39 # r39 = grab idle
//     bne nana_fix3_end

// check_if_6100:
//     lhz r15, 0x78(r25)
//     cmpwi r15, 0x6100
//     beq ignore_if_6100

// nana_fix3_end:
//     sth r26, 120(r25)

// ignore_if_6100:
// )");

// // # this was the culprit; by eliminating the call to change_md altogether nana sometimes just derps around in unexpected ways
// // # op nop @ $809036c

// // # ...so instead I perform a check to see if nana is grabbing and ONLY THEN do I ignore it
// INJECTION("NANA_FIX_4", 0x809036cc, R"(
//     lwz r14, 0x44(r23) # AiInput->AiAct r14 = AiAct
//     lwz r15, 0x74(r14) # AiAct->AiStat r15 = AiStat

//     # AI action (see previous HOOK for explaination)
//     lwz r16, 0xB4(r15)
//     # 0x34 = "Catch" (action)
//     cmpwi r16, 0x34
//     blt+ KeepMD_normalExec
//     # 0x3B = "CatchCut" (action)
//     cmpwi r16, 0x3B
//     bgt+ KeepMD_normalExec
//     b KeepMD_end

// KeepMD_normalExec:
//     lis r12, 0x8090
//     ori r12, r12, 0x49d0
//     mtctr r12
//     bctrl

// KeepMD_end:
// )");

// INJECTION("analyze_script_value_output", 0x8091e440, R"(
//     lwz	r0, 0x0024 (sp)
//     SAVE_REGS
//     mr r3, r26
//     bl scriptValueCheck
//     RESTORE_REGS
// )");

// INJECTION("analyze_stick_output", 0x80904104, R"(
//     SAVE_REGS
//     mr r3, r24
//     bl stickValueCheck
//     RESTORE_REGS
//     lis r3, 0x805A
// )");

#define _getButtonMask_soController ((unsigned int (*)(int btn)) 0x8076544c)
extern "C" {
    // void scriptValueCheck(double output, AiScriptData* aiActInst) {
    //     OSReport("output: %.3f; constPtr: %08x; currInst: %08x\n", output, aiActInst->constPtr, aiActInst->currentInstruction);
    // }
    // void stickValueCheck(Vec2f* stick) {
    //     OSReport("stick: [%.3f, %.3f]\n", stick->x, stick->y);
    // }
    // // # r3 = childAi (AiInput)
    // // # r4 = nana stick ptr
    // // # r5 = nana buttons
    // // # r6 = aiChrIdxPtr
    // // # r7 = is tap jump enabled
    // void nanaCheck(AiInput* nanaAi, Vec2f* nanaStick, Inputs nanaButtons, void* aiChrIdxPtr, bool isTapJumpEnabled) {
    //     OSReport("currScript: %04x; currMd: %02x; passed stick: [%.3f, %.3f]\nbuttons: %04x, isTapJump?: %d\n", nanaAi->aiActPtr->aiScript, nanaAi->aiMd, nanaStick->x, nanaStick->y, nanaButtons, isTapJumpEnabled);
    // }
    // float checkNana(AiStat* nanaStat) {
    //     return FIGHTER_MANAGER->getFighter(FIGHTER_MANAGER->getEntryId(_GetPlayerNo_aiChrIdx(&nanaStat->aiIndex)), true)->modules->motionModule->getFrame();
    // }

    //Special Offensive Collision: ID=0, Damage=0, ShieldDamage=0, direction=23, WeightKnockback=0, KnockbackGrowth=5, BaseKnockback=70, Size=8.0, Bone=0, X Offset=0.0, Y Offset=0.0, Z Offset=0.0, TripRate=0.0, HitlagMultiplier=1.0, SDIMultiplier=1.0, Flags=30080, RehitRate=14, SpecialFlags=600F4FC0
    // function calculateKnockback(percent, damage, bkb, kbg, weight, isWeightDependent) {
    //     if (isWeightDependent) {
    //         return ((((bkb * 10 / 20) + 1) * 1.4 * (200 / (weight + 100)) + 18) * (kbg / 100)) + 0;
    //     }
    //     // var kb = ((0.01 * growth) * ((1.4 * (((0.05 * (damageunstaled * (damagestaled + Math.floor(percent)))) + (damagestaled + Math.floor(percent)) * 0.1) * (2.0 - (2.0 * (weight * 0.01)) / (1.0 + (weight * 0.01))))) + 18) + base);

    //     return ((0.01 * kbg) * ((1.4 * (((0.05 * (damage * (damage + Math.floor(percent)))) + (damage + Math.floor(percent)) * 0.1) * (2.0 - (2.0 * (weight * 0.01)) / (1.0 + (weight * 0.01))))) + 18) + bkb);
    // }


    float calculateKnockback(float percent, float damage, float bkb, float kbg, float weight, bool isWeightDependent) {
        if (isWeightDependent) {
            return ((((bkb * 10 / 20) + 1) * 1.4 * (200 / (weight + 100)) + 18) * (kbg / 100)) + 0;
        }
        // var kb = ((0.01 * growth) * ((1.4 * (((0.05 * (damageunstaled * (damagestaled + Math.floor(percent)))) + (damagestaled + Math.floor(percent)) * 0.1) * (2.0 - (2.0 * (weight * 0.01)) / (1.0 + (weight * 0.01))))) + 18) + base);

        return ((0.01 * kbg) * ((1.4 * (((0.05 * (damage * (damage + _floor(percent)))) + (damage + _floor(percent)) * 0.1) * (2.0 - (2.0 * (weight * 0.01)) / (1.0 + (weight * 0.01))))) + 18) + bkb);
    }
    float calculatePercentForKnockback(float target, float damage, float bkb, float kbg, float weight, bool isWeightDependent) {
        // big shoutouts to https://www.symbolab.com/solver/solve-for-equation-calculator/solve%20for%20p%2C%20t%20%3D%20%5Cleft(%5Cleft(0.01%20%5Ccdot%20k%5Cright)%20%5Ccdot%5Cleft(%5Cleft(1.4%20%5Ccdot%5Cleft(%5Cleft(%5Cleft(0.05%20%5Ccdot%5Cleft(d%20%5Ccdot%5Cleft(d%20%2B%20p%5Cright)%5Cright)%5Cright)%20%2B%20%5Cleft(d%20%2B%20p%5Cright)%20%5Ccdot%200.1%5Cright)%20%5Ccdot%5Cleft(2.0%20-%20%5Cleft(2.0%20%5Ccdot%5Cleft(w%20%5Ccdot%200.01%5Cright)%5Cright)%20%2F%20%5Cleft(1.0%20%2B%20%5Cleft(w%20%5Ccdot%200.01%5Cright)%5Cright)%5Cright)%5Cright)%5Cright)%20%2B%2018%5Cright)%20%2B%20b%5Cright)?or=input
        // t = ((0.01 * k) * ((1.4 * (((0.05 * (d * (d + p))) + (d + p) * 0.1) * (2.0 - (2.0 * (w * 0.01)) / (1.0 + (w * 0.01))))) + 18) + b)
        // percent = (((target * weight + 100 * target - weight * bkb - 100 * bkb - 0.18 * kbg * weight - 18 * kbg) / (2.8 * kbg - 0.028 * kbg * weight)) - 0.05 * damage * damage - 0.1 * damage) * 0.05 * damage + 0.1
        if (isWeightDependent) {
            f32 kb = ((((bkb * 10 / 20) + 1) * 1.4 * (200 / (weight + 100)) + 18) * (kbg / 100));
            return (kb > target) ? 0 : 1000;
        }
        return  (((target * weight + 100 * target - weight * bkb - 100 * bkb - 0.18 * kbg * weight - 18 * kbg) / (2.8 * kbg - 0.028 * kbg * weight)) - 0.05 * damage * damage - 0.1 * damage) * 0.05 * damage + 0.1;
    }
    void RELOCATE_INSTRUCTION() {
        if (forcedNextInstruction != nullptr) {
            asm("mr r30, %0"
                :
                : "r" (forcedNextInstruction));
        }
    }
    void FORCE_RETURN() {
        if (forcedReturnStatement) {
            forcedReturnStatement = false;
            asm("li r4, 0x4");
        }
    }
    void FORCE_XGOTO_CONTINUE() {
        if (forceContinue) {
            asm("cmpwi r30, 1");
            forceContinue = false;
        } else {
            asm("cmpwi r30, 0");
        }
    }
    // 0x8081c878
    void aiCommandHandlers(AiScriptData* aiActInst, int* args, unsigned int* buttons) {
        int cmd = (args[0] & 0xFF000000) >> 24;

            forcedNextInstruction = nullptr;
        // if (forcedNextInstruction != nullptr) {
        //     if (cmd == 0x3) {
        //         aiActInst->currentInstruction = args;
        //         args = aiActInst->currentInstruction + 2;
        //         cmd = (args[0] & 0xFF000000) >> 24;
        //     }
        // }

        // char targetChrIdx = (aiActInst) ? : ;
        if (isMMS) {
            auto* modules = FIGHTER_MANAGER->getFighter(FIGHTER_MANAGER->getEntryId(0), false)->modules;
            // forcibly escapes from the function
            if (modules->statusModule->action == 0xBD || modules->statusModule->desiredAction == 0xBD) {
                forcedNextInstruction = (int*) 0x1; //0x817d119c
                return;
            }
        } else if (aiActInst->aiInputPtr == nullptr) {
            forcedNextInstruction = (int*) 0x1;
            return;
        }
        

        if (cmd < 0x35) return;

        // starts at 0x35
        static void* dispatchTable[0xFF - 0x34] = {
            REP6(&&_getPSAVariable)
            // 0x3B
            &&_YDistFloorOffset,
            // 0x3C
            &&_YDistFloorAbsolute,
            // 0x3E - 0x3C - 1 = 1
            nullptr,
            // 0x3E
            &&_isTeammateCloser,
            // 0x3F
            &&_getAttribute,
            // 0x40
            &&_taunt,
            // 0x41
            &&_setAIMD,
            // 0x42
            &&_switchTarget,
            // 0x43
            &&_calcKnockback,
            // 0x44
            &&_calcYChange,
            // 0x45
            &&_setAutoDefend,
            // 0x46
            &&_setDisableMd,
            // 0x47
            &&_setDisabledSwitch,
            // 0x48
            &&_retrieveFullATKD,
            // 0x49 (xcoord)
            &&_estXYCoord,
            // 0x4A (ycoord)
            &&_estXYCoord,
            // 0x4B (relative)
            &&_getCollisionDistance,
            // 0x4C (absolute)
            &&_getCollisionDistance,
            // 0x4D
            &&_setDebugMode,
            // 0x4E
            &&_calcPercentForKnockback,
            // 0x4F
            &&_calcMoveWeight,
            // 0x50
            &&_trackOAction,
            // 0x51
            &&_predictOOption,
            // 0x52
            &&_predictionConfidence,
            // 0x53
            &&_predictAverage,
            // 0x54
            &&_incrementPrediction,
            // 0x55
            &&_getCurrentPredictValue,
            // 0x58 - 0x55 - 1 = 2
            REP2(nullptr)
            // 0x58
            &&_getCommitPredictChance,
            // 0x59
            &&_predictOMov,
            // 0x60 - 0x59 - 1 = 6
            REP6(nullptr)
            // 0x60
            &&_bitOR,
            // 0x61
            &&_bitAND,
            // 0x62
            &&_bitLSHIFT,
            // 0x63
            &&_bitRSHIFT,
            // 0x64
            &&_mathMOD,
            // 0x65
            &&_mathCOS,
            // 0x66
            &&_mathSIN,
            // 0x67
            &&_setVarByNum,
            // 0x68
            &&_copyVarByNum,
            // 0x69
            &&_STACK_PUSH,
            // 0x6A
            &&_GotoByValue,
            // 0x6B
            &&_SeekNoCommit,
            // 0x6C
            &&_RESET_LTF_STACK_PTR,
            // 0x6D
            &&_STACK_TOSS,
            // 0x70 - 0x6D - 1 = 2
            REP2(nullptr)
            // 0x70
            &&_GetItemLocFromIdx,
            // 0x71 (loc)
            &&_GetArticleOfTypeAtTargetLocSpeed,
            // 0x72 (speed)
            &&_GetArticleOfTypeAtTargetLocSpeed,
            // 0x80 - 0x72 - 1 = 13 (0xD)
            REP10(nullptr)
            REP3(nullptr)
            // 0x80
            &&_CallI,
            // 0x81
            &&_XGoto,
            // 0x82
            &&_NoRepeat,
            // 0x83
            &&_XReciever,
            // 0x84
            &&_DynamicDiceAdd,
            // 0x85
            &&_DynamicDiceRoll,
            // 0x86
            &&_DynamicDiceClear,
            // 0x87
            &&_DynamicDiceSize,
            // 0x88
            &&_OXGoto,
            // 0x90 - 0x88 - 1 = 7
            REP7(nullptr) 
            // 0x90
            &&_ADJUST_PERSONALITY,
            // 0x91
            &&_SetVarAttackData,
            // 0xB0 - 0x91 - 1 = 30 (0x1E)
            REP10(nullptr)
            REP10(nullptr)
            REP10(nullptr)
            // 0xB0 through 0xBF
            REP10(&&_SAFE_WRITE_0toF)
            REP6(&&_SAFE_WRITE_0toF)
            // 0xC0 through 0xCF
            REP10(&&_SAFE_INJECT_0toF)
            REP6(&&_SAFE_INJECT_0toF)
            // 0xD0
            &&_DrawDebugPoint,
            // 0xD1
            &&_DrawDebugLine,
            // 0xD2
            &&_DrawDebugRectOutline,
            // 0xD3
            &&_DrawDebugCircle,
            REP10(nullptr)
            REP2(nullptr)
            // 0xE0
            &&_OverlayStuff,
            // 0xE1
            &&_OverlayStuff,
            // 0xE2
            &&_OverlayStuff,
            REP10(nullptr)
            REP3(nullptr)
            // 0xF0
            &&_LOGVAL_NL,
            // 0xF1
            &&_LOGSTR_NL,
            // 0xF2
            &&_LOGVAL,
            // 0xF3
            &&_LOGSTR,
            // 0xF4
            &&_PRINTLN,
            // 0xF5
            &&_PAUSE,
            REP10(nullptr)
        };

        int pNum = _GetPlayerNo_aiChrIdx(&aiActInst->aiInputPtr->cpuIdx);
        int oPNum = _GetPlayerNo_aiChrIdx(&aiActInst->aiInputPtr->aiTarget);
        bool debugEnabled = pNum < 4 && debugSwitch[pNum];
        // OSReport("switch: %s; debugEnabled: %s\n", debugSwitch[pNum] ? "T" : "F", debugEnabled ? "T" : "F");
        if (cmd >= 0xD0 && !debugEnabled) {
            return;
        }
        // OSReport("Src: %02x\n DispatchTable entry: %02x\n result: 0x%08x\n", cmd, cmd - 0x35, dispatchTable[cmd - 0x35]);
        goto *dispatchTable[cmd - 0x35];

        _handlers: {
            _getPSAVariable: { // OSReport("getPSAVariable\n");
                int varToMod = args[1];
                double index = _get_script_value_aiScriptData(aiActInst, *(int *) &args[2], 0);
                bool shouldGetTarget = _get_script_value_aiScriptData(aiActInst, *(int *) &args[3], 0);

                soWorkManageModuleImpl* targetWorkModule;
                if (shouldGetTarget) {
                    auto targetPlayerNum = AI_MANAGER->getAiCpuTarget(_GetPlayerNo_aiChrIdx(&aiActInst->aiInputPtr->cpuIdx));
                    Fighter* target = FIGHTER_MANAGER->getFighter(FIGHTER_MANAGER->getEntryId(targetPlayerNum));
                    if (target == nullptr) return;
                    targetWorkModule = target->modules->workModule;
                } else {
                    targetWorkModule = FIGHTER_MANAGER->getFighter(aiActInst->aiInputPtr->fighterId)->modules->workModule;
                }

                switch (cmd) {
                    case 0x35:
                        aiActInst->variables[varToMod] = _isFlag_soWorkManageModuleImpl(targetWorkModule, 0x20000000 | 0x02000000 |((int) index)) ? 1 : 0;
                        return;
                    case 0x36:
                        aiActInst->variables[varToMod] = (float) (*(int (*)[targetWorkModule->RAVariables->basicsSize]) (targetWorkModule->RAVariables->basics))[(int) index];
                        return;
                    case 0x37:
                        aiActInst->variables[varToMod] = (float) (*(float (*)[targetWorkModule->RAVariables->floatsSize]) (targetWorkModule->RAVariables->floats))[(int) index];
                        return;
                    case 0x38:
                        aiActInst->variables[varToMod] = _isFlag_soWorkManageModuleImpl(targetWorkModule, 0x10000000 | 0x02000000 |((int) index)) ? 1 : 0;
                        return;
                    case 0x39:
                        aiActInst->variables[varToMod] = (float) (*(int (*)[targetWorkModule->LAVariables->basicsSize]) (targetWorkModule->LAVariables->basics))[(int) index];
                        return;
                    case 0x3A:
                        aiActInst->variables[varToMod] = (float) (*(float (*)[targetWorkModule->LAVariables->floatsSize]) (targetWorkModule->LAVariables->floats))[(int) index];
                        return;
                }
                return;
            }
            _YDistFloorOffset: { // OSReport("YDistFloorOffset\n");
                int varToMod = args[1];
                double xOffset = _get_script_value_aiScriptData(aiActInst, *(int *) &args[2], 0);
                double yOffset = _get_script_value_aiScriptData(aiActInst, *(int *) &args[3], 0);
                bool shouldGetTarget = _get_script_value_aiScriptData(aiActInst, *(int *) &args[4], 0);

                soGroundModuleImpl* targetGroundModule;
                if (shouldGetTarget) {
                    auto targetPlayerNum = AI_MANAGER->getAiCpuTarget(_GetPlayerNo_aiChrIdx(&aiActInst->aiInputPtr->cpuIdx));
                    Fighter* target = FIGHTER_MANAGER->getFighter(FIGHTER_MANAGER->getEntryId(targetPlayerNum));
                    if (target == nullptr) return;
                    targetGroundModule = target->modules->groundModule;
                } else {
                    targetGroundModule = FIGHTER_MANAGER->getFighter(aiActInst->aiInputPtr->fighterId)->modules->groundModule;
                }
                if (targetGroundModule == nullptr) {
                    aiActInst->variables[varToMod] = (float) -1;
                    return;
                }

                Vec3f startPos = {};
                targetGroundModule->getCorrectPos(&startPos);
                startPos.f1 += xOffset;
                startPos.f2 += yOffset;

                Vec3f destPos {
                    0,
                    -500,
                    0
                };

                Vec3f ret1 {-1,-1,-1};
                Vec3f ret2 {-1,-1,-1};

                int rayResult = _stRayCheck_vec3f(&startPos, &destPos, &ret1, &ret2, true, 0, 1, 1);
                // OSReport("YDISTFLOOR OFFSET RESULT: %.3f, %.3f\n", ret1.f1, ret1.f2);
                // OSReport("YDISTFLOOR OFFSET RET2: %.3f, %.3f\n", ret2.f1, ret2.f2);

                float res = -1;
                if (rayResult) {
                    startPos.f1 -= ret1.f1;
                    startPos.f2 -= ret1.f2;
                    res = _length_vec3f(&startPos);
                    // OSReport("NORMALIZED RESULT: %.3f\n", res);
                }

                aiActInst->variables[varToMod] = (float) res;
                return;
            }
            _YDistFloorAbsolute: { // OSReport("YDistFloorAbsolute\n");
                int varToMod = args[1];
                double xPos = _get_script_value_aiScriptData(aiActInst, *(int *) &args[2], 0);
                double yPos = _get_script_value_aiScriptData(aiActInst, *(int *) &args[3], 0);

                double res = -1;

                Vec3f startPos {
                        (float) xPos,
                        (float) yPos,
                        0
                };

                Vec3f destPos {
                    0,
                    -500,
                    0
                };

                Vec3f ret1 {-1,-1,-1};
                Vec3f ret2 {-1,-1,-1};

                int rayResult = _stRayCheck_vec3f(&startPos, &destPos, &ret1, &ret2, true, 0, 1, 1);
                
                if (rayResult) {
                    startPos.f1 -= ret1.f1;
                    startPos.f2 -= ret1.f2;
                    res = _length_vec3f(&startPos);
                }

                aiActInst->variables[varToMod] = (float) res;
                return;
            }
            _isTeammateCloser: { // OSReport("isTeammateCloser\n");
                int varToMod = args[1];

                aiActInst->variables[varToMod] = 0;
                Fighter* target = FIGHTER_MANAGER->getFighter(FIGHTER_MANAGER->getEntryId(aiActInst->aiInputPtr->aiTarget));
                // OSReport("(myRes, mgrRes): (%08x, %08x)\n", aiActInst->ftInputPtr->aiTarget);
                int targetXPos = target->modules->postureModule->xPos;

                if (GM_GLOBAL_MODE_MELEE->meleeInitData.teamAttackOnOff) {
                    auto entryCount = FIGHTER_MANAGER->getEntryCount();
                    for (int i = 0; i < entryCount; i++) {
                        auto id = FIGHTER_MANAGER->getEntryIdFromIndex(i);
                        FtOwner* fighterOwner = FIGHTER_MANAGER->getOwner(id);
                        // OSReport("(self team, t team): (%08x, %08x)\n", fighterOwner->ftInputPtr->ftEntryPtr->team, aiActInst->ftInputPtr->ftEntryPtr->team);
                        if (id != aiActInst->aiInputPtr->fighterId
                            && fighterOwner->ownerDataPtr->team == aiActInst->aiInputPtr->ftEntryPtr->owner->ownerDataPtr->team
                            && !(FIGHTER_MANAGER->getFighter(id)->modules->statusModule->action >= 0x3E
                            && FIGHTER_MANAGER->getFighter(id)->modules->statusModule->action <= 0x48)) {
                            auto xDiffTeammate = FIGHTER_MANAGER->getFighter(id)->modules->postureModule->xPos - targetXPos;
                            auto xDiffSelf = aiActInst->aiInputPtr->ftEntryPtr->ftStageObject->modules->postureModule->xPos - targetXPos;
                            auto xDiffSelfAbs = (xDiffSelf < 0) ? xDiffSelf * -1 : xDiffSelf;
                            auto xDiffTeammateAbs = (xDiffTeammate < 0) ? xDiffTeammate * -1 : xDiffTeammate;
                            if ((xDiffTeammate > 0 && xDiffSelf > 0 && xDiffTeammate < xDiffSelf)
                                || (xDiffTeammate < 0 && xDiffSelf < 0 && xDiffTeammate > xDiffSelf)
                                || (xDiffTeammateAbs < xDiffSelfAbs && xDiffSelfAbs - xDiffTeammateAbs <= 30)) {
                                aiActInst->variables[varToMod] = 1;
                                break;
                            }
                        }
                    }
                }
                return;
            }
            _getAttribute: { // OSReport("getAttribute\n");
                int varToMod = args[1];
                int attributeIndex = _get_script_value_aiScriptData(aiActInst, *(int *) &args[2], 0);
                bool fromTarget = _get_script_value_aiScriptData(aiActInst, *(int *) &args[3], 0);

                Fighter* targetFighter;
                if (fromTarget) {
                    targetFighter = FIGHTER_MANAGER->getFighter(FIGHTER_MANAGER->getEntryId(AI_MANAGER->getAiCpuTarget(_GetPlayerNo_aiChrIdx(&aiActInst->aiInputPtr->cpuIdx))));
                } else {
                    targetFighter = FIGHTER_MANAGER->getFighter(aiActInst->aiInputPtr->fighterId);
                }
                // OSReport("fromTarget? %d; targetFighter: %08x\n", fromTarget, targetFighter);
                aiActInst->variables[varToMod] = *(float*)((int)&targetFighter->modules->paramCustomizeModule->walkInitVel + (attributeIndex - 0x18));
                return;
            }
            _taunt: { // OSReport("taunt\n");
                int tauntInput = _get_script_value_aiScriptData(aiActInst, *(int *) &args[1], 0);
                switch (tauntInput) {
                    case 1:
                        *buttons |= _getButtonMask_soController(6); // utaunt
                        break;
                    case 2:
                        *buttons |= _getButtonMask_soController(8); // staunt
                        break;
                    case 3:
                        *buttons |= _getButtonMask_soController(7); // dtaunt
                        break;
                }
                return;
            }
            _setAIMD: { // OSReport("setAIMD\n");
                int newMode = _get_script_value_aiScriptData(aiActInst, *(int *) &args[1], 0);
                int newScript = _get_script_value_aiScriptData(aiActInst, *(int *) &args[2], 0);

                unsigned char dummy = 0xff;
                change_md_aiInput(aiActInst->aiInputPtr, newMode, &dummy, newScript, 0);
                return;
            }
            _switchTarget: { // OSReport("switchTarget\n"); // 8055b284
                // OSReport("================\n");
                // OSReport("SWITCHING TARGET\n");
                // OSReport("SWITCHING TARGET\n");
                // OSReport("SWITCHING TARGET\n");
                // _target_check_aiInput(aiActInst->aiInputPtr);
                // OSReport("SWITCHING TARGET\n");
                // OSReport("SWITCHING TARGET\n");
                // OSReport("SWITCHING TARGET\n");
                // OSReport("================\n");
                return;
            }
            _calcKnockback: { // OSReport("calcKnockback\n");
                int varToMod = args[1];
                float percent = _get_script_value_aiScriptData(aiActInst, *(int *) &args[2], 0);
                float damage = _get_script_value_aiScriptData(aiActInst, *(int *) &args[3], 0);
                float bkb = _get_script_value_aiScriptData(aiActInst, *(int *) &args[4], 0);
                float kbg = _get_script_value_aiScriptData(aiActInst, *(int *) &args[5], 0);
                float weight = _get_script_value_aiScriptData(aiActInst, *(int *) &args[6], 0);
                int isWeightDependent = _get_script_value_aiScriptData(aiActInst, *(int *) &args[7], 0);

                aiActInst->variables[varToMod] = calculateKnockback(percent, damage, bkb, kbg, weight, isWeightDependent); ;
                return;
            }
            _calcYChange: { // OSReport("calcYChange\n");
                int varToMod = args[1];
                int frameCount = _get_script_value_aiScriptData(aiActInst, *(int *) &args[2], 0);
                float ySpeed = _get_script_value_aiScriptData(aiActInst, *(int *) &args[3], 0);
                float gravity = _get_script_value_aiScriptData(aiActInst, *(int *) &args[4], 0);
                float maxFallSpeed = _get_script_value_aiScriptData(aiActInst, *(int *) &args[5], 0) * -1;
                float fastFallSpeed = _get_script_value_aiScriptData(aiActInst, *(int *) &args[6], 0) * -1;
                bool fastFallImmediate = _get_script_value_aiScriptData(aiActInst, *(int *) &args[7], 0);
                
                float accumulator = 0;
                float tracker = ySpeed;
                s32 remainder = frameCount;
                while (remainder > 0) {
                    remainder --;
                    accumulator += tracker;
                    tracker -= gravity;
                    if (tracker < fastFallSpeed || (tracker <= 0 && fastFallImmediate)) {
                        tracker = fastFallSpeed;
                        break;
                    }
                    else if (tracker < maxFallSpeed) {
                        tracker = maxFallSpeed;
                        break;
                    }
                }
                accumulator += remainder * tracker;
                aiActInst->variables[varToMod] = accumulator;
            }
            _setAutoDefend: { // OSReport("setAutoDefend\n");
                if (pNum >= 4) return;
                bool onOff = _get_script_value_aiScriptData(aiActInst, *(int *) &args[1], 0);
                autoDefend[_GetPlayerNo_aiChrIdx(&aiActInst->aiInputPtr->cpuIdx)] = onOff;
                return;
            }
            _setDisableMd: { // OSReport("setDisableMd\n");
                if (pNum >= 4) return;
                int mdValue = _get_script_value_aiScriptData(aiActInst, *(int *) &args[1], 0);
                disabledMd[_GetPlayerNo_aiChrIdx(&aiActInst->aiInputPtr->cpuIdx)] = mdValue;
                return;
            }
            _setDisabledSwitch: { // OSReport("setDisabledSwitch\n");
                if (pNum >= 4) pNum = 0;
                bool onOff = _get_script_value_aiScriptData(aiActInst, *(int *) &args[1], 0);
                disabledSwitch[pNum] = onOff;
                return;
            }
            _retrieveFullATKD: { // OSReport("retrieveFullATKD\n");
                int unkVar = args[1];
                int startVar = args[2];
                int endVar = args[3];
                int xMinVar = args[4];
                int xMaxVar = args[5];
                int yMinVar = args[6];
                int yMaxVar = args[7];
                int subaction = _get_script_value_aiScriptData(aiActInst, *(int *) &args[8], 0);
                bool shouldGetOpponent = _get_script_value_aiScriptData(aiActInst, *(int *) &args[9], 0);
                if (subaction == -1) {
                    aiActInst->variables[unkVar] = -1;
                    return;
                }

                AiParam targetAIParam;
                if (shouldGetOpponent) {
                    targetAIParam = FIGHTER_MANAGER->getOwner(FIGHTER_MANAGER->getEntryId(aiActInst->aiInputPtr->aiTarget))->aiInputPtr->aiParam;
                } else {
                    targetAIParam = aiActInst->aiInputPtr->aiParam;
                }
                ATKDContent* data = targetAIParam.searchAtkData(subaction);
                if (data == nullptr) {
                    aiActInst->variables[unkVar] = -1;
                    return;
                }

                if (unkVar >= 0) aiActInst->variables[unkVar] = data->unk;
                if (startVar >= 0) aiActInst->variables[startVar] = data->start;
                if (endVar >= 0) aiActInst->variables[endVar] = data->end;
                if (xMinVar >= 0) aiActInst->variables[xMinVar] = data->xmin;
                if (xMaxVar >= 0) aiActInst->variables[xMaxVar] = data->xmax;
                if (yMinVar >= 0) aiActInst->variables[yMinVar] = data->ymin;
                if (yMaxVar >= 0) aiActInst->variables[yMaxVar] = data->ymax;
                return;
            }
            // 2
            _estXYCoord: { // OSReport("estXYCoord\n");
                double time = _get_script_value_aiScriptData(aiActInst, *(int *) &args[2], 0);
                
                double result;
                if (cmd == 0x49) result = _calcArraivePosX_aiStat(time, aiActInst->aiStatPtr);
                else result = _calcArraivePosY_aiStat(time, aiActInst->aiStatPtr);

                aiActInst->variables[args[1]] = (float) result;
                return;
            }
            // 2
            _getCollisionDistance: { // OSReport("getCollisionDistance\n");
                int retX = args[1];
                int retY = args[2];
                // int grTypeTarget = args[2];
                double xPos = _get_script_value_aiScriptData(aiActInst, *(int *) &args[3], 0);
                double yPos = _get_script_value_aiScriptData(aiActInst, *(int *) &args[4], 0);
                double xDest = _get_script_value_aiScriptData(aiActInst, *(int *) &args[5], 0);
                double yDest = _get_script_value_aiScriptData(aiActInst, *(int *) &args[6], 0);
                bool detectPlats = _get_script_value_aiScriptData(aiActInst, *(int *) &args[7], 0);

                Vec3f startPos {
                    (float) xPos,
                    (float) yPos,
                    0
                };

                // THIS IS RELATIVE TO THE STARTPOS
                Vec3f destPos {
                    (float) ((cmd == 0x4C) ? (xDest - xPos) : xDest),
                    (float) ((cmd == 0x4C) ? (yDest - yPos) : yDest),
                    0
                };

                Vec3f ret1 {-1,-1,-1};
                Vec3f ret2 {-1,-1,-1};

                int rayResult = _stRayCheck_vec3f(&startPos, &destPos, &ret1, &ret2, detectPlats, 0, 1, 1);
                if (rayResult) {
                    aiActInst->variables[retX] = (float) ret1.f1;
                    aiActInst->variables[retY] = (float) ret1.f2;
                } else {
                    aiActInst->variables[retX] = (float) -1;
                    aiActInst->variables[retY] = (float) -1;
                }
                return;
            }
            _setDebugMode: { // OSReport("setDebugMode\n");
                if (pNum >= 4) return;
                bool value = _get_script_value_aiScriptData(aiActInst, *(int *) &args[1], 0);
                debugSwitch[_GetPlayerNo_aiChrIdx(&aiActInst->aiInputPtr->cpuIdx)] = value;
                return;
            }
            _calcPercentForKnockback: { // OSReport("calcPercentForKnockback\n");
                int varToMod = args[1];
                float target = _get_script_value_aiScriptData(aiActInst, *(int *) &args[2], 0);
                float damage = _get_script_value_aiScriptData(aiActInst, *(int *) &args[3], 0);
                float bkb = _get_script_value_aiScriptData(aiActInst, *(int *) &args[4], 0);
                float kbg = _get_script_value_aiScriptData(aiActInst, *(int *) &args[5], 0);
                float weight = _get_script_value_aiScriptData(aiActInst, *(int *) &args[6], 0);
                int isWeightDependent = _get_script_value_aiScriptData(aiActInst, *(int *) &args[7], 0);

                aiActInst->variables[varToMod] = calculatePercentForKnockback(target, damage, bkb, kbg, weight, isWeightDependent);
                return;
            }
            _calcMoveWeight: { // OSReport("calcMoveWeight\n");
                int varToMod = args[1];
                float ODamage = _get_script_value_aiScriptData(aiActInst, *(int *) &args[2], 0);
                float OWeight = _get_script_value_aiScriptData(aiActInst, *(int *) &args[3], 0);
                float moveAngle = _get_script_value_aiScriptData(aiActInst, *(int *) &args[4], 0);
                float moveBaseKnockback = _get_script_value_aiScriptData(aiActInst, *(int *) &args[5], 0);
                float moveKnockbackGrowth = _get_script_value_aiScriptData(aiActInst, *(int *) &args[6], 0);
                float moveHitFrame = _get_script_value_aiScriptData(aiActInst, *(int *) &args[7], 0);
                float moveDuration = _get_script_value_aiScriptData(aiActInst, *(int *) &args[8], 0);
                float moveIASA = _get_script_value_aiScriptData(aiActInst, *(int *) &args[9], 0);
                float moveDamage = _get_script_value_aiScriptData(aiActInst, *(int *) &args[10], 0);
                float moveIsWeightDependent = _get_script_value_aiScriptData(aiActInst, *(int *) &args[11], 0);
                
                // xterminalvel
                // jumpVel
                // OEndLag
                // EstOTopNX
                // TopNX
                // TopBZ
                // xBZ
                // stageWidth
                // EstOTopNY
                float XTerminalVelocity = _get_script_value_aiScriptData(aiActInst, *(int *) &args[12], 0);
                float jumpVelocity = _get_script_value_aiScriptData(aiActInst, *(int *) &args[13], 0);
                float OEndLag = _get_script_value_aiScriptData(aiActInst, *(int *) &args[14], 0);
                float EstOTopNX = _get_script_value_aiScriptData(aiActInst, *(int *) &args[15], 0);
                float TopNX = _get_script_value_aiScriptData(aiActInst, *(int *) &args[16], 0);
                float TopBZ = _get_script_value_aiScriptData(aiActInst, *(int *) &args[17], 0);
                float xBZ = _get_script_value_aiScriptData(aiActInst, *(int *) &args[18], 0);
                float stageWidth = _get_script_value_aiScriptData(aiActInst, *(int *) &args[19], 0);
                float EstOTopNY = _get_script_value_aiScriptData(aiActInst, *(int *) &args[20], 0);
                float EstOYDistFloor = _get_script_value_aiScriptData(aiActInst, *(int *) &args[21], 0);

                if (EstOTopNX < 0) EstOTopNX *= -1;
                if (TopNX < 0) TopNX *= -1;
                float moveXVelMultiplier = math_cos(moveAngle * math_rad);
                if (moveXVelMultiplier < 0) moveXVelMultiplier *= -1;            
                float moveYVelMultiplier = 1 - moveXVelMultiplier;
                if (debugEnabled) OSReport("XVelMul= %.3f; YVelMul= %.3f; ", moveXVelMultiplier, moveYVelMultiplier);
                if (moveXVelMultiplier == 0) moveXVelMultiplier = 0.00001;
                if (moveYVelMultiplier == 0) moveYVelMultiplier = 0.00001;

                stageWidth += 10;

                // y = v * 0.03 - 0.051 * x
                // 0 = v * 0.03 - 0.051 * x
                // 0.051 * x = v * 0.03
                // x = (v * 0.03) / 0.051
                // v = (0.051 * x) / 0.03

                aiActInst->variables[varToMod] = ((moveDamage < 5) ? 5 : moveDamage) * 10;

                constexpr float kbRatio = (0.051 * 0.75) / 0.03;
                constexpr float invKbRatio = 1 / kbRatio;

                float xVelRequired = ((xBZ - EstOTopNX) * kbRatio) * 0.73;
                // behold, perhaps the only time i've used the XOR operator in an if statement
                // if ((moveAngle > 90 && moveAngle < 270) ^ (EstOTopNX < TopNX))
                //     xVelRequired = ((xBZ + EstOTopNX) * kbRatio) * 0.73;
                // else
                //     xVelRequired = ;
                float yVelRequired = ((TopBZ - EstOTopNY) * kbRatio) * 0.77;
                
                
                float multiplier = (xVelRequired > yVelRequired && moveAngle <= 180) ? moveYVelMultiplier : moveXVelMultiplier;
                float moveCurrKnockback = calculateKnockback(ODamage, moveDamage, moveBaseKnockback, moveKnockbackGrowth, OWeight, moveIsWeightDependent);
                if (debugEnabled) OSReport("; KBX: %.3f, KBY: %.3f", moveCurrKnockback * moveXVelMultiplier, moveCurrKnockback * moveYVelMultiplier);
                if (debugEnabled) OSReport(";(KILL): XVelReq: %.3f; YVelReq: %.3f", xVelRequired, yVelRequired);
                
                float hitstun = moveCurrKnockback * 0.4;
                // if (GCD.superTurbo && FIGHTER_MANAGER->getOwner(FIGHTER_MANAGER->getEntryId(pNum))->hasCurry()) {
                //     moveCurrKnockback *= (0.5 + (playerTrainingData[oPNum].turboData.comboHitCount + 1) * 0.15);
                //     hitstun *= (2 / (playerTrainingData[oPNum].turboData.comboHitCount * 0.25 + 1));
                // }

                float endlag = moveIASA - ((moveDuration / 2) + moveHitFrame);
                aiActInst->variables[varToMod] += (hitstun - endlag) * 0.1;
                // OEndLag += 2;
                if (((OEndLag + 5) > moveHitFrame || (stageWidth / 2 - EstOTopNX < 0) || _randf() < 0.3) && (xVelRequired < moveCurrKnockback * moveXVelMultiplier || yVelRequired < moveCurrKnockback * moveYVelMultiplier) && (moveAngle <= 210 || moveAngle >= 330)) {
                    if (debugEnabled) OSReport("; KILLING");
                    aiActInst->variables[varToMod] += 10000;
                    return;
                }

                // will a move launch the target offstage
                xVelRequired = (((stageWidth + 2) / 2 - EstOTopNX) * kbRatio);
                if (debugEnabled) OSReport("; Launch XVelReq: %.3f", xVelRequired);
                if (xVelRequired > 0) {
                    if ((moveAngle >= 75 && moveAngle <= 105) && moveYVelMultiplier * moveCurrKnockback > 100) {
                        if (debugEnabled) OSReport("; (+ launch-high)");
                        aiActInst->variables[varToMod] += 30;
                    } 
                    if ((moveAngle >= 230 && moveAngle <= 310) && moveYVelMultiplier * moveCurrKnockback > 70 && EstOYDistFloor > 20) {
                        if (debugEnabled) OSReport("; (+ launch-low)");
                        aiActInst->variables[varToMod] += 15;
                    }
                    float approxKB = moveCurrKnockback * moveXVelMultiplier;
                    if (xVelRequired < approxKB && moveCurrKnockback > 110) {
                        if (debugEnabled) OSReport("; LAUNCHING");
                        aiActInst->variables[varToMod] += 80 + (approxKB - xVelRequired) * 1.5;
                        return;
                    }
                } else {
                    if (!(moveAngle > 75 && moveAngle < 105)) {
                        aiActInst->variables[varToMod] += moveCurrKnockback * 0.45; 
                        if (((moveAngle >= 210 && moveAngle <= 340) || moveAngle < 20) && moveCurrKnockback > 50) {
                            if (debugEnabled) OSReport("; (+ launch-low)");
                            aiActInst->variables[varToMod] *= 50; 
                        }
                    }
                    return;
                }

                if (moveHitFrame < (OEndLag - 8)) {
                    aiActInst->variables[varToMod] += 5 * ((OEndLag - 8) - moveHitFrame);
                }

                // will a move combo
                // if (moveCurrKnockback > 90) {
                //     endlag -= 10;
                // }
                // endlag *= 1.65;
                if (EstOYDistFloor < 5) {
                    hitstun *= 0.7;
                }
                if (endlag < hitstun) {
                    if ((moveAngle >= 220 && moveAngle <= 320) || (moveAngle > 75 && moveAngle < 105)) {
                        if (jumpVelocity * hitstun * 20 > moveYVelMultiplier * moveCurrKnockback && XTerminalVelocity * hitstun * 3 > moveXVelMultiplier * moveCurrKnockback) {
                            if (debugEnabled) OSReport("; COMBOING (vert)");
                            aiActInst->variables[varToMod] += 25;
                            if ((moveAngle > 75 && moveAngle < 105)) {
                                aiActInst->variables[varToMod] += 30;
                                aiActInst->variables[varToMod] *= 1 + ((hitstun - endlag) / 5);
                                return;
                            }
                            if ((moveAngle >= 220 && moveAngle <= 320)) {
                                aiActInst->variables[varToMod] += (EstOYDistFloor) * 0.45;
                                if (EstOYDistFloor < 10) {
                                    aiActInst->variables[varToMod] += 25;
                                    aiActInst->variables[varToMod] *= 1 + ((hitstun - endlag) / 20);
                                    return;
                                }
                            }
                        }      
                    } else if (hitstun > 8) {
                        if (XTerminalVelocity * hitstun * 15 > moveXVelMultiplier * moveCurrKnockback) {
                            if (debugEnabled) OSReport("; COMBOING (horiz)");
                            if (moveYVelMultiplier * moveCurrKnockback > 20 || moveCurrKnockback <= 80) aiActInst->variables[varToMod] += 5;
                            else if (!(moveAngle <= 20 && moveAngle >= 160)) aiActInst->variables[varToMod] += 25;
                            else aiActInst->variables[varToMod] += 20;
                            aiActInst->variables[varToMod] += 10 * (1 - (moveXVelMultiplier * moveCurrKnockback / 100));
                            aiActInst->variables[varToMod] += 15;
                        }
                    }
                    aiActInst->variables[varToMod] *= 1 + ((hitstun - endlag) / 25);
                }
                return;
            }
            _trackOAction: { // OSReport("trackOAction\n");
                int managerNum = _get_script_value_aiScriptData(aiActInst, *(int *) &args[1], 0);
                if (0 <= managerNum && managerNum < 0x10) {
                    if (oPNum >= 4) oPNum = 0;
                    rpsManagers[managerNum].pushNew(oPNum, (int) _get_script_value_aiScriptData(aiActInst, *(int *) &args[2], 0));
                }
                return;
            }
            _predictOOption: { // OSReport("predictOOption\n");
                if (oPNum >= 4) oPNum = 0;
                int varToMod = args[1];
                int managerNum = _get_script_value_aiScriptData(aiActInst, *(int *) &args[2], 0);
                int lookAmount = aiActInst->aiInputPtr->aiParam.levelValue / 7;
                aiActInst->variables[varToMod] = rpsManagers[managerNum].calcOption(oPNum, lookAmount);
                return;
            }
            _predictionConfidence: { // OSReport("predictionConfidence\n");
                if (oPNum >= 4) oPNum = 0;
                int varToMod = args[1];
                int managerNum = _get_script_value_aiScriptData(aiActInst, *(int *) &args[2], 0);
                int lookAmount = aiActInst->aiInputPtr->aiParam.levelValue / 7;
                aiActInst->variables[varToMod] = rpsManagers[managerNum].calcOptionConfidence(oPNum, lookAmount);
                return;
            }
            _predictAverage: { // OSReport("predictAverage\n");
                if (oPNum >= 4) oPNum = 0;
                int varToMod = args[1];
                int managerNum = _get_script_value_aiScriptData(aiActInst, *(int *) &args[2], 0);
                int lookAmount = aiActInst->aiInputPtr->aiParam.levelValue / 7;
                aiActInst->variables[varToMod] = rpsManagers[managerNum].average(oPNum, lookAmount);
                return;
            }
            _incrementPrediction: { // OSReport("incrementPrediction\n");
                if (oPNum >= 4) oPNum = 0;
                int managerNum = _get_script_value_aiScriptData(aiActInst, *(int *) &args[1], 0);
                rpsManagers[managerNum].plusOne(oPNum);
                return;
            }
            _getCurrentPredictValue: { // OSReport("getCurrentPredictValue\n");
                if (oPNum >= 4) oPNum = 0;
                int varToMod = args[1];
                int managerNum = _get_script_value_aiScriptData(aiActInst, *(int *) &args[2], 0);
                aiActInst->variables[varToMod] = rpsManagers[managerNum].get(oPNum);
                return;
            }
            _getCommitPredictChance: { // OSReport("getCommitPredictChance\n");
                if (oPNum >= 4) oPNum = 0;
                int varToMod = args[1];
                int levelValue = aiActInst->aiInputPtr->aiParam.levelValue;
                float result = 0;
                result = movementTrackers[oPNum].approxCommit(levelValue);
                aiActInst->variables[varToMod] = result;
                return;
            }
            _predictOMov: { // OSReport("predictOMov\n");
                if (oPNum >= 4) oPNum = 0;
                int varToMod = args[1];
                int actToGet = _get_script_value_aiScriptData(aiActInst, *(int *) &args[2], 0);
                int levelValue = aiActInst->aiInputPtr->aiParam.levelValue;
                aiActInst->variables[varToMod] = movementTrackers[oPNum].approxChance(levelValue, actToGet);
                return;
            }
            _bitOR: { // OSReport("bitOR\n");
                int varToMod = args[1];
                int op1 = _get_script_value_aiScriptData(aiActInst, *(int *) &args[2], 0);
                int op2 = _get_script_value_aiScriptData(aiActInst, *(int *) &args[3], 0);
                aiActInst->variables[varToMod] = op1 | op2; 
                return;
            }
            _bitAND: { // OSReport("bitAND\n");
                int varToMod = args[1];
                int op1 = _get_script_value_aiScriptData(aiActInst, *(int *) &args[2], 0);
                int op2 = _get_script_value_aiScriptData(aiActInst, *(int *) &args[3], 0);
                aiActInst->variables[varToMod] = op1 & op2; 
                return;
            }
            _bitLSHIFT: { // OSReport("bitLSHIFT\n");
                int varToMod = args[1];
                int op1 = _get_script_value_aiScriptData(aiActInst, *(int *) &args[2], 0);
                int op2 = _get_script_value_aiScriptData(aiActInst, *(int *) &args[3], 0);
                aiActInst->variables[varToMod] = op1 << op2; 
                return;
            }
            _bitRSHIFT: { // OSReport("bitRSHIFT\n");
                int varToMod = args[1];
                int op1 = _get_script_value_aiScriptData(aiActInst, *(int *) &args[2], 0);
                int op2 = _get_script_value_aiScriptData(aiActInst, *(int *) &args[3], 0);
                aiActInst->variables[varToMod] = op1 >> op2; 
                return;
            }
            _mathMOD: { // OSReport("mathMOD\n");
                int varToMod = args[1];
                int op1 = _get_script_value_aiScriptData(aiActInst, *(int *) &args[2], 0);
                int op2 = _get_script_value_aiScriptData(aiActInst, *(int *) &args[3], 0);
                if (op2 < 1) op2 = 1;
                aiActInst->variables[varToMod] = op1 % op2; 
                return;
            }
            _mathCOS: { // OSReport("mathCOS\n");
                int varToMod = args[1];
                float angleValue = _get_script_value_aiScriptData(aiActInst, *(int *) &args[2], 0);
                aiActInst->variables[varToMod] = math_cos(angleValue * math_rad);
                return;
            }
            _mathSIN: { // OSReport("mathSIN\n");
                int varToMod = args[1];
                float angleValue = _get_script_value_aiScriptData(aiActInst, *(int *) &args[2], 0);            
                aiActInst->variables[varToMod] = math_sin(angleValue * math_rad);
                return;
            }
            _setVarByNum: { // OSReport("setVarByNum\n");
                int varNum = _get_script_value_aiScriptData(aiActInst, *(int *) &args[1], 0);
                float value = _get_script_value_aiScriptData(aiActInst, *(int *) &args[2], 0);
                if (varNum >= 24) {
                    OSReport("========(@%08x) WARNING: TRIED TO INSERT DATA INTO VARNUM %d\n========", &args[0], varNum);
                } else {
                    aiActInst->variables[varNum] = value;
                }
                return;
            }
            _copyVarByNum: { // OSReport("copyVarByNum\n");
                int varNum = _get_script_value_aiScriptData(aiActInst, *(int *) &args[1], 0);
                int varFrom = _get_script_value_aiScriptData(aiActInst, *(int *) &args[2], 0);
                if (varNum >= 24 || varFrom >= 24) {
                    OSReport("========WARNING: TRIED OBTAIN DATA FROM VARNUM %d\n========", varNum);
                } else {
                    aiActInst->variables[varNum] = aiActInst->variables[varFrom];
                }
                return;
            }
            _STACK_PUSH: { // OSReport("STACK_PUSH\n");
                float value = _get_script_value_aiScriptData(aiActInst, *(int *) &args[1], 0);
                bool isLongTerm = _get_script_value_aiScriptData(aiActInst, *(int *) &args[2], 0);
                if (isLongTerm) {
                    LTFStackPtr++;
                    LTFStack[LTFStackPtr] = value;
                } else {
                    if (functionalStackPtr >= FUNCTIONAL_STACK_LEN) {
                        OSReport("========WARNING: FUNCTIONAL STACK OVERFLOW @ %08x (p%d)========\n", aiActInst->currentInstruction, pNum);
                    }
                    functionalStackPtr ++;
                    functionalStack[functionalStackPtr] = value;
                }
                return;
            }
            _STACK_TOSS: { // OSReport("STACK_TOSS\n");
                int numToPop = _get_script_value_aiScriptData(aiActInst, *(int *) &args[1], 0);
                functionalStackPtr -= numToPop;
                if (functionalStackPtr < 0) {
                    functionalStackPtr = 0;
                }
                return;
            }
            _GotoByValue: { // OSReport("GotoByValue\n");
                s32 toFind = _get_script_value_aiScriptData(aiActInst, *(s32 *) &args[1], 0);
                s32* item = (s32 *)((int) aiActInst->constPtr + *(s32*)(aiActInst->constPtr + 0x1));
                u32 iter = -1;
                while( true ) {
                    while (*(u8 *)item != 0x3) {
                        item = (s32 *)((s32) item + (u32)*(u16 *)((s32)item + 0x2));
                    }
                    iter++;
                    if ((*(u8 *)((s32)item + 0x1) != 0x0) && (item[0x1] == toFind)) break;
                    item = (s32 *)((s32)item + (u32)*(u16 *)((s32)item + 0x2));
                }
                aiGotoPush(aiActInst, (s32*)((s32) args));
                forcedNextInstruction = item + 2;
                return;
            }
            _SeekNoCommit: { // OSReport("SeekNoCommit\n");
                s32 toFind = args[1];
                s32* item = (s32 *)((int) aiActInst->constPtr + *(s32*)(aiActInst->constPtr + 0x1));
                u32 iter = -1;
                while( true ) {
                    while (*(u8 *)item != 0x3) {
                        item = (s32 *)((s32) item + (u32)*(u16 *)((s32)item + 0x2));
                    }
                    iter++;
                    if ((*(u8 *)((s32)item + 0x1) != 0x0) && (item[0x1] == toFind)) break;
                    item = (s32 *)((s32)item + (u32)*(u16 *)((s32)item + 0x2));
                }
                forcedNextInstruction = item + 2;
                return;
            }
            _RESET_LTF_STACK_PTR: { // OSReport("RESET_LTF_STACK_PTR\n");
                LTFStackPtr = -1;
                return;
            }
            // item stuff
            _GetItemLocFromIdx: { // OSReport("GetItemLocFromIdx\n");
                int xPosDest = args[1];
                int yPosDest = args[2];
                int itemIndex = _get_script_value_aiScriptData(aiActInst, *(int *) &args[3], 0);
                BaseItem * it = ITEM_MANAGER->getItemFromIndex(itemIndex);
                if (it == nullptr) {
                    aiActInst->variables[xPosDest] = 0;
                    aiActInst->variables[yPosDest] = 0;
                    return;
                }
                aiActInst->variables[xPosDest] = it->modules->postureModule->xPos;
                aiActInst->variables[yPosDest] = it->modules->postureModule->yPos;
                return;
            }
            _GetArticleOfTypeAtTargetLocSpeed: { // OSReport("GetArticleOfTypeAtTargetLocSpeed\n");
                int xPosDest = args[1];
                int yPosDest = args[2];
                int articleKind = _get_script_value_aiScriptData(aiActInst, *(int *) &args[3], 0);
                bool shouldGetTarget = _get_script_value_aiScriptData(aiActInst, *(int *) &args[4], 0);

                Fighter * target;
                if (shouldGetTarget) {
                    auto targetPlayerNum = AI_MANAGER->getAiCpuTarget(_GetPlayerNo_aiChrIdx(&aiActInst->aiInputPtr->cpuIdx));
                    target = FIGHTER_MANAGER->getFighter(FIGHTER_MANAGER->getEntryId(targetPlayerNum));
                    if (target == nullptr) {
                        aiActInst->variables[xPosDest] = 0;
                        aiActInst->variables[yPosDest] = 0;
                        return;
                    }
                } else {
                    target = aiActInst->aiInputPtr->ftEntryPtr->ftStageObject;
                }

                void*** rawArticle = target->modules->generateArticleManageModule->getArticle(articleKind);
                if (rawArticle == nullptr) {
                    aiActInst->variables[xPosDest] = 0;
                    aiActInst->variables[yPosDest] = 0;
                    return;
                }
                Weapon* article = (Weapon*) DynamicCast(rawArticle, 0, **rawArticle, ArticleDescriptor);

                if (article == nullptr) {
                    aiActInst->variables[xPosDest] = 0;
                    aiActInst->variables[yPosDest] = 0;
                    return;
                }

                OSReport("article: %08x\n", article);
                // 812865ac | 812865ac
                // 81286524 | 81286524
                // DYNAMIC_CAST()
                
                // asm(R"(
                //     mr r3, %1
                //     li r4, 0
                //     lwz r5, 0(r3)
                //     lwz r5, 0(r5)
                //     lis r6, 0x80AE
                //     ori r6, r6, 0xB7D8
                //     li r7, 1
                //     lis r12, 0x803f
                //     ori r12, r12, 0x0f44
                //     mtctr r12
                //     bctrl
                //     mr %0, r3
                // )"
                // : "=r" (article)
                // : "r" (article));

                // OSReport("article address: %08x\n", &article);
                // OSReport("article: %08x\n", article);
                // OSReport("article module addr: %08x\n", article->modules);
                // OSReport("article posture module addr: %08x\n", article->modules->postureModule);
                // OSReport("article module posture xpos addr: %08x\n", &article->modules->postureModule->xPos);

                soPostureModuleImpl* postureModule = article->modules->postureModule;
                if (postureModule == 0) {
                    aiActInst->variables[xPosDest] = 0;
                    aiActInst->variables[yPosDest] = 0;
                    return;
                }
                if (cmd == 0x71) {
                    aiActInst->variables[xPosDest] = postureModule->xPos;
                    aiActInst->variables[yPosDest] = postureModule->yPos;
                } else {
                    aiActInst->variables[xPosDest] = postureModule->xPos - postureModule->prevXPos;
                    aiActInst->variables[yPosDest] = postureModule->yPos - postureModule->prevYPos;
                }
                return;
            }
            _CallI: { // OSReport("CallI\n");
                forceContinue = true;
                unsigned int nextScript = args[1] & 0xffff;
                
                if (pNum < 4) {
                    auto& scriptPath = playerTrainingData[pNum].aiData.scriptPath;
                    scriptPath.push(new AIScriptCache {(unsigned short) nextScript, 0});
                }

                char dummy = 0xff;
                aiActInst->aiScript = 0;
                _act_change(aiActInst, nextScript, &dummy, 0, 0);
                // force change current instruction in memory
                clearGotoStack(aiActInst);
                aiActInst->framesSinceScriptChanged = -1;
                aiActInst->currentInstruction -= 1;
                forcedNextInstruction = (int*)((int) aiActInst->currentInstruction + 0x4);
                // aiActInst->constPtr = _get_script_tag_scriptpac((int*)aiActInst->AIScriptPac, (int*)AI_MANAGER->aiCommonCE, nextScript);
                // OSReport("FNInst (1): %08x\n", forcedNextInstruction);

                // aiActInst
                return;
            }
            _XGoto: { // OSReport("XGoto\n");
                forceContinue = true;
                unsigned int nextScript = args[1] & 0xffff;
                char dummy = 0xff;
                aiXGotoPush(aiActInst, (int*)((int) args), nextScript);
                // OSReport("StoredInstruction: %08x\n", (int*)((int) args));
                // auto currScr = aiActInst->aiScript;
                // OSReport("BEFORE CHANGE: %08x\n", aiActInst->constPtr);
                aiActInst->aiScript = 0;
                _act_change(aiActInst, nextScript, &dummy, 0, 0);
                // OSReport("AFTER CHANGE: %08x\n", aiActInst->constPtr);
                // aiActInst->aiScript = currScr;
                // OSReport("TARGET SCRIPT: %04x\n", nextScript);
                // OSReport("NEW SCRIPT: %04x\n", aiActInst->aiScript);
                // force change current instruction in memory
                aiActInst->framesSinceScriptChanged = -1;
                aiActInst->currentInstruction -= 1;
                forcedNextInstruction = (int*)((int) aiActInst->currentInstruction + 0x4);
                // OSReport("FNInst (2): %08x\n", forcedNextInstruction);
                return;
            }
            _NoRepeat: { // OSReport("NoRepeat\n");
                // OSReport("ARGS0 ADDR: %08x\n", &args[0]);
                for (int i = 0; i < (int) NoRepeatInstructions.size(); i++) {
                    // OSReport("RepInst[%d]: %08x\n", i, NoRepeatInstructions[i]);
                    if (NoRepeatInstructions[i] == &args[0]) {
                        aiGotoPop(aiActInst);
                        // OSReport("NextInstruction: %08x\n", forcedNextInstruction);
                        if (forcedNextInstruction == nullptr) {
                            forcedReturnStatement = true;
                        }
                        if (pNum < 4) {
                            auto& scriptPath = playerTrainingData[pNum].aiData.scriptPath;
                            scriptPath.pop_back();
                            scriptPath.pop_back();
                        }
                        return;
                    }
                }
                NoRepeatInstructions.push(&args[0]);
                return;
            }
            _XReciever: { // OSReport("XReciever\n"); 
                return; 
            }
            _DynamicDiceAdd: { // OSReport("DynamicDiceAdd\n");
                int slot = _get_script_value_aiScriptData(aiActInst, *(int *) &args[1], 0);
                float value = _get_script_value_aiScriptData(aiActInst, *(int *) &args[2], 0);
                float weight = _get_script_value_aiScriptData(aiActInst, *(int *) &args[3], 0);
                // OSReport("====Slot: %d, value: %d, weight: %.3f====\n", slot, value, weight);
                dynamicDice[slot].addValue({value, weight});
                // OSReport("====weight: %.3f====\n", dynamicDice[slot].getWeight());
                return;
            }
            _DynamicDiceRoll: { // OSReport("DynamicDiceRoll\n");
                int slot = _get_script_value_aiScriptData(aiActInst, *(int *) &args[1], 0);
                int varToMod = args[2];
                bool shouldRemove = _get_script_value_aiScriptData(aiActInst, *(int *) &args[3], 0);
                
                if (dynamicDice[slot].getWeight() == 0) {
                    aiActInst->variables[varToMod] = -1;
                    return;
                }
                aiActInst->variables[varToMod] = (shouldRemove) ? dynamicDice[slot].rollAndRemove() : dynamicDice[slot].roll();
                return;
            }
            _DynamicDiceClear: { // OSReport("DynamicDiceClear\n");
                int slot = _get_script_value_aiScriptData(aiActInst, *(int *) &args[1], 0);
                dynamicDice[slot].clear();
                return;
            }
            _DynamicDiceSize: { // OSReport("DynamicDiceSize\n");
                int slot = _get_script_value_aiScriptData(aiActInst, *(int *) &args[1], 0);
                int varToMod = args[2];
                aiActInst->variables[varToMod] = dynamicDice[slot].getSize();
                return;
            }
            _OXGoto: { // OSReport("OXGoto\n");
                forceContinue = true;
                unsigned int nextScript = args[1] & 0xffff;
                aiXGotoPush(aiActInst, (int*)((int) args), nextScript);
                // OSReport("StoredInstruction: %08x\n", (int*)((int) args));
                // auto currScr = aiActInst->aiScript;
                // OSReport("BEFORE CHANGE: %08x\n", aiActInst->constPtr);
                
                // force change current instruction in memory
                aiActInst->framesSinceScriptChanged = -1;
                // aiActInst->currentInstruction -= 1;
                AICEEntry* scriptEntry = _get_script_AISCRIPTPACK(_getEntity_ftEntryManager(FIGHTER_MANAGER->entryManager, FIGHTER_MANAGER->getEntryId(oPNum))->ftInput->aiInputMain->aiActPtr->AIScriptPac, AI_MANAGER->aiCommonCE, nextScript);
                aiActInst->aiScript = scriptEntry->ID;
                aiActInst->constPtr = (int*) scriptEntry;
                forcedNextInstruction = (int*)((int)scriptEntry + scriptEntry->EventsOffset);
                // OSReport("FNInst (2): %08x\n", forcedNextInstruction);
                return;
            }
            _ADJUST_PERSONALITY: { // OSReport("ADJUST_PERSONALITY\n");
                if (pNum >= 4) return;
                int index = _get_script_value_aiScriptData(aiActInst, *(int *) &args[1], 0);
                float amount = _get_script_value_aiScriptData(aiActInst, *(int *) &args[2], 0);
                float multiplier = _get_script_value_aiScriptData(aiActInst, *(int *) &args[3], 0);
                amount *= multiplier;
                int pNum = _GetPlayerNo_aiChrIdx(&aiActInst->aiInputPtr->cpuIdx);
                if (!playerTrainingData[pNum].options.AI.autoAdjust) amount = 0;
                switch(index) {
                    case 0x0: {
                        playerTrainingData[pNum].options.AI.aggression += amount;
                        if (playerTrainingData[pNum].options.AI.aggression > 10) playerTrainingData[pNum].options.AI.aggression = 10;
                        else if (playerTrainingData[pNum].options.AI.aggression < -1) playerTrainingData[pNum].options.AI.aggression = -1;
                        return;
                    }
                    case 0x1: {
                        playerTrainingData[pNum].options.AI.bait_dashAwayChance += amount;
                        if (playerTrainingData[pNum].options.AI.bait_dashAwayChance > 10) playerTrainingData[pNum].options.AI.bait_dashAwayChance = 10;
                        else if (playerTrainingData[pNum].options.AI.bait_dashAwayChance < -1) playerTrainingData[pNum].options.AI.bait_dashAwayChance = -1;
                        return;
                    }
                    case 0x10: {
                        playerTrainingData[pNum].options.AI.bait_wdashAwayChance += amount;
                        if (playerTrainingData[pNum].options.AI.bait_wdashAwayChance > 10) playerTrainingData[pNum].options.AI.bait_wdashAwayChance = 10;
                        else if (playerTrainingData[pNum].options.AI.bait_wdashAwayChance < -1) playerTrainingData[pNum].options.AI.bait_wdashAwayChance = -1;
                        return;
                    }
                    case 0x3: {
                        playerTrainingData[pNum].options.AI.baitChance += amount;
                        if (playerTrainingData[pNum].options.AI.baitChance > 10) playerTrainingData[pNum].options.AI.baitChance = 10;
                        else if (playerTrainingData[pNum].options.AI.baitChance < -1) playerTrainingData[pNum].options.AI.baitChance = -1;
                        return;
                    }
                    case 0x4: {
                        playerTrainingData[pNum].options.AI.braveChance += amount;
                        if (playerTrainingData[pNum].options.AI.braveChance > 10) playerTrainingData[pNum].options.AI.braveChance = 10;
                        else if (playerTrainingData[pNum].options.AI.braveChance < -1) playerTrainingData[pNum].options.AI.braveChance = -1;
                        return;
                    }
                    case 0x5: {
                        playerTrainingData[pNum].options.AI.circleCampChance += amount;
                        if (playerTrainingData[pNum].options.AI.circleCampChance > 10) playerTrainingData[pNum].options.AI.circleCampChance = 10;
                        else if (playerTrainingData[pNum].options.AI.circleCampChance < -1) playerTrainingData[pNum].options.AI.circleCampChance = -1;
                        return;
                    }
                    case 0x6: {
                        playerTrainingData[pNum].options.AI.djumpiness += amount;
                        if (playerTrainingData[pNum].options.AI.djumpiness > 10) playerTrainingData[pNum].options.AI.djumpiness = 10;
                        else if (playerTrainingData[pNum].options.AI.djumpiness < -1) playerTrainingData[pNum].options.AI.djumpiness = -1;
                        return;
                    }
                    case 0x7: {
                        playerTrainingData[pNum].options.AI.jumpiness += amount;
                        if (playerTrainingData[pNum].options.AI.jumpiness > 10) playerTrainingData[pNum].options.AI.jumpiness = 10;
                        else if (playerTrainingData[pNum].options.AI.jumpiness < -1) playerTrainingData[pNum].options.AI.jumpiness = -1;
                        return;
                    }
                    case 0x8: {
                        playerTrainingData[pNum].options.AI.platChance += amount;
                        if (playerTrainingData[pNum].options.AI.platChance > 10) playerTrainingData[pNum].options.AI.platChance = 10;
                        else if (playerTrainingData[pNum].options.AI.platChance < -1) playerTrainingData[pNum].options.AI.platChance = -1;
                        return;
                    }
                    case 0x9: {
                        playerTrainingData[pNum].options.AI.SDIChance += amount;
                        if (playerTrainingData[pNum].options.AI.SDIChance > 10) playerTrainingData[pNum].options.AI.SDIChance = 10;
                        else if (playerTrainingData[pNum].options.AI.SDIChance < -1) playerTrainingData[pNum].options.AI.SDIChance = -1;
                        return;
                    }
                    case 0xA: {
                        playerTrainingData[pNum].options.AI.wall_chance += amount;
                        if (playerTrainingData[pNum].options.AI.wall_chance > 10) playerTrainingData[pNum].options.AI.wall_chance = 10;
                        else if (playerTrainingData[pNum].options.AI.wall_chance < -1) playerTrainingData[pNum].options.AI.wall_chance = -1;
                        return;
                    }
                    case 0xB: {
                        playerTrainingData[pNum].options.AI.reactionTime += amount;
                        if (playerTrainingData[pNum].options.AI.reactionTime > 10) playerTrainingData[pNum].options.AI.reactionTime = 10;
                        else if (playerTrainingData[pNum].options.AI.reactionTime < -1) playerTrainingData[pNum].options.AI.reactionTime = -1;
                        return;
                    }
                }
                return;
            }
            _SetVarAttackData: { // OSReport("SetVarAttackData\n");
                float xOffset = _get_script_value_aiScriptData(aiActInst, *(int *) &args[1], 0);
                float yOffset = _get_script_value_aiScriptData(aiActInst, *(int *) &args[2], 0);
                float xRange = _get_script_value_aiScriptData(aiActInst, *(int *) &args[3], 0);
                float yRange = _get_script_value_aiScriptData(aiActInst, *(int *) &args[4], 0);
                float hitFrame = _get_script_value_aiScriptData(aiActInst, *(int *) &args[5], 0);
                float duration = _get_script_value_aiScriptData(aiActInst, *(int *) &args[6], 0);
                float IASA = _get_script_value_aiScriptData(aiActInst, *(int *) &args[7], 0);
                float damage = _get_script_value_aiScriptData(aiActInst, *(int *) &args[8], 0);
                float isWeightDependant = _get_script_value_aiScriptData(aiActInst, *(int *) &args[9], 0);
                float baseKnockback = _get_script_value_aiScriptData(aiActInst, *(int *) &args[10], 0);
                float knockbackGrowth = _get_script_value_aiScriptData(aiActInst, *(int *) &args[11], 0);

                float data[11] = {xOffset, yOffset, xRange, yRange, hitFrame, duration, IASA, damage, isWeightDependant, baseKnockback, knockbackGrowth};
                // OSReport("DATA: xOffset: %.3f, yOffset: %.3f, xRange: %.3f, yRange: %.3f, hitFrame: %.3f, duration: %.3f, IASA: %.3f, damage: %.3f, isWeightDependant: %.3f, baseKnockback: %.3f, knockbackGrowth: %.3f\n", xOffset, yOffset, xRange, yRange, hitFrame, duration, IASA, damage, isWeightDependant, baseKnockback, knockbackGrowth);
                // OSReport("VARS TO SET:");
                for (int i = 0; i < 11; i++) {
                    if (functionalStackPtr < 0) {
                        OSReport("========ERROR: TRIED TO ACCESS STACK VARIABLE THAT DIDN'T EXIST========\n");
                    }
                    int toSet = functionalStack[functionalStackPtr];
                    // OSReport("%d, ", toSet);
                    functionalStackPtr --;
                    aiActInst->variables[toSet] = data[i];
                }
                // OSReport("\n");
                return;
            }
            _SAFE_WRITE_0toF: { // OSReport("SAFE_WRITE_0toF\n");
                if (ai_customFnInjection != nullptr && !ai_customFnInjectionToggle[cmd & 0xF]) ai_customFnInjection[cmd & 0xF] = _get_script_value_aiScriptData(aiActInst, *(int *) &args[1], 0);
                return;
            }
            _SAFE_INJECT_0toF: { // OSReport("SAFE_INJECT_0toF\n");
                if (ai_customFnInjection != nullptr && ai_customFnInjectionToggle[cmd & 0xF]) aiActInst->variables[args[1]] = ai_customFnInjection[cmd & 0xF];
                return;
            }
            _DrawDebugPoint: { // OSReport("DrawDebugPoint\n");
                double x1 = _get_script_value_aiScriptData(aiActInst, *(int *) &args[1], 0);
                double y1 = _get_script_value_aiScriptData(aiActInst, *(int *) &args[2], 0);
                u8 red = _get_script_value_aiScriptData(aiActInst, *(int *) &args[3], 0);
                u8 green = _get_script_value_aiScriptData(aiActInst, *(int *) &args[4], 0);
                u8 blue = _get_script_value_aiScriptData(aiActInst, *(int *) &args[5], 0);
                u8 alpha = _get_script_value_aiScriptData(aiActInst, *(int *) &args[6], 0);
                if (alpha == 255) {
                    Point * pt = new Point{
                            0x000000FF,
                            (float) x1,
                            (float) y1,
                            42,
                            false
                    };
                    pt->autoTimer = false;
                    renderables.items.tick.push(pt);
                }
                Point * pt = new Point{
                        (red << 24) | (green << 16) | (blue << 8) | ((alpha > 255) ? 255 : alpha),
                        (float) x1,
                        (float) y1,
                        30,
                        false
                };
                pt->autoTimer = false;
                renderables.items.tick.push(pt);
                return;
            }
            _DrawDebugLine: { // OSReport("DrawDebugLine\n");
                double x1 = _get_script_value_aiScriptData(aiActInst, *(int *) &args[1], 0);
                double y1 = _get_script_value_aiScriptData(aiActInst, *(int *) &args[2], 0);
                double x2 = _get_script_value_aiScriptData(aiActInst, *(int *) &args[3], 0);
                double y2 = _get_script_value_aiScriptData(aiActInst, *(int *) &args[4], 0);
                u8 red = _get_script_value_aiScriptData(aiActInst, *(int *) &args[5], 0);
                u8 green = _get_script_value_aiScriptData(aiActInst, *(int *) &args[6], 0);
                u8 blue = _get_script_value_aiScriptData(aiActInst, *(int *) &args[7], 0);
                u8 alpha = _get_script_value_aiScriptData(aiActInst, *(int *) &args[8], 0);
                if (alpha == 255) {
                    Line * ln = new Line{
                            0x000000FF,
                            (float) x1,
                            (float) y1,
                            (float) x2,
                            (float) y2,
                            42,
                            false
                    };
                    ln->autoTimer = false;
                    renderables.items.tick.push(ln);
                }
                Line * ln = new Line{
                        (red << 24) | (green << 16) | (blue << 8) | ((alpha > 255) ? 255 : alpha),
                        (float) x1,
                        (float) y1,
                        (float) x2,
                        (float) y2,
                        30,
                        false
                };
                ln->autoTimer = false;
                renderables.items.tick.push(ln);
                return;
            }
            _DrawDebugRectOutline: { // OSReport("DrawDebugRectOutline\n");
                double x = _get_script_value_aiScriptData(aiActInst, *(int *) &args[1], 0);
                double y = _get_script_value_aiScriptData(aiActInst, *(int *) &args[2], 0);
                double width = _get_script_value_aiScriptData(aiActInst, *(int *) &args[3], 0) * 0.5;
                double height = _get_script_value_aiScriptData(aiActInst, *(int *) &args[4], 0) * 0.5;
                u8 red = _get_script_value_aiScriptData(aiActInst, *(int *) &args[5], 0);
                u8 green = _get_script_value_aiScriptData(aiActInst, *(int *) &args[6], 0);
                u8 blue = _get_script_value_aiScriptData(aiActInst, *(int *) &args[7], 0);
                u8 alpha = _get_script_value_aiScriptData(aiActInst, *(int *) &args[8], 0);
                // OSReport("RECT RGBA: %d %d %d %d\n", red, green, blue, alpha);
                if (alpha == 255) {
                    RectOutline * ro = new RectOutline{
                            0x000000FF,
                            (float) (y - height),
                            (float) (y + height),
                            (float) (x - width),
                            (float) (x + width),
                            42,
                            false
                    };
                    ro->autoTimer = false;
                    renderables.items.tick.push(ro);
                }
                RectOutline * ro = new RectOutline{
                    (red << 24) | (green << 16) | (blue << 8) | ((alpha > 255) ? 255 : alpha),
                    (float) (y - height),
                    (float) (y + height),
                    (float) (x - width),
                    (float) (x + width),
                    30,
                    false
                };
                // OSReport("OUT RGBA: %08x\n", ro->color.value);
                ro->autoTimer = false;
                renderables.items.tick.push(ro);
                return;
            }
            _DrawDebugCircle: {
                double x = _get_script_value_aiScriptData(aiActInst, *(int *) &args[1], 0);
                double y = _get_script_value_aiScriptData(aiActInst, *(int *) &args[2], 0);
                double radius = _get_script_value_aiScriptData(aiActInst, *(int *) &args[3], 0);
                u8 red = _get_script_value_aiScriptData(aiActInst, *(int *) &args[4], 0);
                u8 green = _get_script_value_aiScriptData(aiActInst, *(int *) &args[5], 0);
                u8 blue = _get_script_value_aiScriptData(aiActInst, *(int *) &args[6], 0);
                u8 alpha = _get_script_value_aiScriptData(aiActInst, *(int *) &args[7], 0);

                // (float x, float y, float radius, int vertCount, bool is2D, GXColor color, float borderThickness, GXColor borderColor)
                Circle * co = new Circle{
                    x,
                    y,
                    radius,
                    16,
                    false,
                    (red << 24) | (green << 16) | (blue << 8) | ((alpha > 255) ? 255 : alpha)
                };

                co->autoTimer = false;
                renderables.items.tick.push(co);
                return;
            }
            // _EnableDebugOverlay:
            // _DisableDebugOverlay:
            // _SetDebugOverlayColor
            _OverlayStuff: { // OSReport("OverlayStuff\n");
                auto colorModule = FIGHTER_MANAGER->getFighter(aiActInst->aiInputPtr->fighterId)->modules->colorBlendModule;
                switch (cmd) {
                    case 0xE0:
                        colorModule->isEnabled = 1;
                        return;
                    case 0xE1:
                        colorModule->isEnabled = 0;
                        return;
                    case 0xE2:
                        int red = _get_script_value_aiScriptData(aiActInst, *(int *) &args[1], 0);
                        int green = _get_script_value_aiScriptData(aiActInst, *(int *) &args[2], 0);
                        int blue = _get_script_value_aiScriptData(aiActInst, *(int *) &args[3], 0);
                        int alpha = _get_script_value_aiScriptData(aiActInst, *(int *) &args[4], 0);
                        colorModule->red = red;
                        colorModule->green = green;
                        colorModule->blue = blue;
                        colorModule->alpha = alpha;
                        return;
                }
            }
            _LOGVAL_NL: { // OSReport("LOGVAL_NL\n");
                OSReport("LOGGED VALUE: %.3f\n", _get_script_value_aiScriptData(aiActInst, *(int *) &args[1], 0));
                return;
            }
            _LOGSTR_NL: { // OSReport("LOGSTR_NL\n");
                OSReport("LOGGED STRING: ");
                for (int i = 1; i <= 5; i++) {
                    unsigned int toConvert = _get_script_value_aiScriptData(aiActInst, *(int *) &args[i], 0);
                    // OSReport("%08x", toConvert);
                    OSReport("%c%c%c",
                            (toConvert & 0xFF000000) >> 24,
                            (toConvert & 0x00FF0000) >> 16,
                            (toConvert & 0x0000FF00) >> 8);
                }
                OSReport("\n");
                return;
            }
            _LOGVAL: { // OSReport("LOGVAL\n");
                OSReport("; %.3f ", _get_script_value_aiScriptData(aiActInst, *(int *) &args[1], 0));
                return;
            }
            _LOGSTR: { // OSReport("LOGSTR\n");
                OSReport("; ");
                for (int i = 1; i <= 5; i++) {
                    unsigned int toConvert = _get_script_value_aiScriptData(aiActInst, *(int *) &args[i], 0);
                    // OSReport("%08x", toConvert);
                    OSReport("%c%c%c",
                            (toConvert & 0xFF000000) >> 24,
                            (toConvert & 0x00FF0000) >> 16,
                            (toConvert & 0x0000FF00) >> 8);
                }
                return;
            }
            _PRINTLN: { // OSReport("PRINTLN\n");
                OSReport("\n");
                return;
            }
            _PAUSE: { // OSReport("PAUSE\n");
                fudgeMenu->visible = true;
                fudgeMenu->paused = true;
                return;
            }
        }
    }
}

SIMPLE_INJECTION(clearPredictions, 0x800dc590, "li r9, 2") {
    for (int i = 0; i < 0x10; i++) {
        if (i < 4) {
            movementTrackers[i].reset();
            movementTrackers[i].trackAction(0x0, true);
            disabledSwitch[i] = false;
            disabledMd[i] = -1;
            autoDefend[i] = false;
            debugSwitch[i] = true;
            // clear personality
            if (playerTrainingData[i].options.AI.unlocked) {
                playerTrainingData[i].options.AI.aggression = 0;
                playerTrainingData[i].options.AI.bait_dashAwayChance = 0;
                playerTrainingData[i].options.AI.bait_wdashAwayChance = 0;
                playerTrainingData[i].options.AI.baitChance = 0;
                playerTrainingData[i].options.AI.braveChance = 0;
                playerTrainingData[i].options.AI.circleCampChance = 0;
                playerTrainingData[i].options.AI.djumpiness = 0;
                playerTrainingData[i].options.AI.jumpiness = 0;
                playerTrainingData[i].options.AI.platChance = 0;
                playerTrainingData[i].options.AI.SDIChance = 0;
                playerTrainingData[i].options.AI.wall_chance = 0;
            }
        }
        rpsManagers[i].clearAll();
    }
    NoRepeatInstructions.reallocate(0);
    NoRepeatInstructions.reallocate(1);
    dynamicDice[0].clear();
    dynamicDice[1].clear();
    functionalStackPtr = -1;
    LTFStackPtr = -1;
    forcedNextInstruction = nullptr;
    forceContinue = false;
};

#endif