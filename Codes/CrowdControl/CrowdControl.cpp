//
// Created by Ilir on 2021-12-13.
//

#include "CrowdControl.h"
#include "EXIPacket.h"
#include "EffectItemHandler.h"
#include "EffectStatusHandler.h"
#include "EffectActionHandler.h"
#include "EffectGameHandler.h"
#include "EffectPositionHandler.h"
#include "EffectModeHandler.h"
#include "EffectAttributeHandler.h"
#include "Brawl/GF/gfPadSystem.h"

namespace FrameLogic {
    // setup global variables
    DebugData data = DebugData();
    DebugData* debugData = &data;
    Menu* myMenu;
    bool visible = false;

    u32 testWaitDuration = 0;

    //hacky way to check if in game
    u32 getScene() {
        u32* ptr = (u32*) (0x805b4fd8 + 0xd4);
        ptr = (u32*) *ptr;
        if(ptr < (u32*)0xA0000000) {
            ptr = (u32*) *(ptr + (0x10 / 4));
            if(ptr != nullptr) {
                u32 scene = *(ptr + (8 / 4));
                return scene;
            }
        }
        return false;
    }

    // called when game starts
    void SendGameStatus(EXIStatus exiStatus) {
        EXIPacket responsePckt = EXIPacket(exiStatus, nullptr, 0);
        responsePckt.Send();
    }

    // called every frame
    void onUpdateFrame() {

        // initializes the menu
        if (myMenu == nullptr) {
            myMenu = new Menu();
            Page* mainPage = new MainPage(myMenu, debugData);
            myMenu->nextPage(mainPage);

        }
        printer.setup();
        printer.drawBoundingBoxes(0);

        startNormalDraw();


        Message * message = &printer.message;
        message->zPos = 0;
        message->fontScaleX = 0.1;
        message->fontScaleY = 0.1;

        printer.lineHeight = 20 * message->fontScaleY;

        char buffer[200] = {};

        message->xPos = 1;
        message->yPos = 1;
        message->zPos = 0;

        setupDrawPrimitives();


        // halfword vector for sending into to the game
        // first entry is effect id (EFFECT_NOT_CONNECTED = 0, EFFECT_NONE = 1, EFFECT_UNKNOWN = 2, EFFECT_ACTUAL >= 3
        // second and above entries are parameters for the effect
        EXIPacket requestPckt = EXIPacket(5*sizeof(u16));
        requestPckt.Receive();
        u16* effectRequest = (u16*)requestPckt.source;

        EXIStatus exiStatus = STATUS_UNKNOWN;
        u32 scene = getScene();

        // TODO: Investigate crash during classic mode

        // for testing effects
        gfPadSystem *padSystem = PAD_SYSTEM;

        if (scene == SCENE_TYPE::SCENE_VS || scene == SCENE_TYPE::SCENE_TRAINING_MODE_MMS) {
            //visible = true;

            if (testWaitDuration > 0) {
                testWaitDuration--;
            }

            int numPlayers = FIGHTER_MANAGER->getEntryCount();

            checkEffectGameDurationFinished();
            checkEffectModeDurationFinished();
            checkEffectAttributeDurationFinished(numPlayers);
            checkPositionResetCorrect();
            checkItemSpawnPokemonOrAssist();

            switch (effectRequest[0]) {
                case EFFECT_GIVE_DAMAGE:
                    exiStatus = effectGameGiveDamage(numPlayers, effectRequest[1], effectRequest[2], effectRequest[3]);
                    break;
                case EFFECT_ITEM_SPAWN_REGULAR:
                    exiStatus = effectItemSpawn(numPlayers, effectRequest[1], effectRequest[2], effectRequest[3]);
                    break;
                case EFFECT_ITEM_SPAWN_POKEMON:
                    exiStatus = effectItemPreloadPokemon(effectRequest[1], effectRequest[2]);
                    break;
                case EFFECT_ITEM_SPAWN_ASSIST:
                    exiStatus = effectItemPreloadAssist(effectRequest[1], effectRequest[2]);
                    break;
                case EFFECT_ITEM_ATTACH_GOOEY:
                    exiStatus = effectItemAttachGooey(numPlayers, effectRequest[1], effectRequest[2]);
                    break;
                case EFFECT_STATUS_METAL:
                    exiStatus = effectStatusGiveMetal(numPlayers, effectRequest[1], effectRequest[2], effectRequest[3]);
                    break;
                case EFFECT_STATUS_CURRY:
                    exiStatus = effectStatusGiveCurry(numPlayers, effectRequest[1], effectRequest[2]);
                    break;
                case EFFECT_STATUS_HAMMER:
                    exiStatus = effectStatusGiveHammer(numPlayers, effectRequest[1], effectRequest[2]);
                    break;
                case EFFECT_STATUS_SUPERSTAR:
                    exiStatus = effectStatusGiveSuperStar(numPlayers, effectRequest[1], effectRequest[2]);
                    break;
                case EFFECT_STATUS_FLOWER:
                    exiStatus = effectStatusGiveFlower(numPlayers, effectRequest[1], 1, effectRequest[2],
                                                       effectRequest[3]);
                    break;
                case EFFECT_STATUS_HEART:
                    exiStatus = effectStatusGiveHeart(numPlayers, effectRequest[1], effectRequest[2], 1);
                    break;
                case EFFECT_STATUS_SLOW:
                    exiStatus = effectStatusGiveSlow(numPlayers, effectRequest[1], effectRequest[2], effectRequest[3], effectRequest[4]);
                    break;
                case EFFECT_STATUS_MUSHROOM:
                    exiStatus = effectStatusGiveMushroom(numPlayers, effectRequest[1], effectRequest[2], effectRequest[3]);
                    break;
                case EFFECT_STATUS_EQUIP:
                    exiStatus = effectStatusGiveEquip(numPlayers, effectRequest[1], effectRequest[2]);
                    break;
                case EFFECT_STATUS_SWAP:
                    exiStatus = effectStatusGiveSwap(numPlayers, effectRequest[1], effectRequest[2], effectRequest[3], effectRequest[4]);
                    break;
                case EFFECT_STATUS_FINALSMASH:
                    exiStatus = effectStatusGiveFinalSmash(numPlayers, effectRequest[1], 1);
                    break;
                case EFFECT_STATUS_ACTION:
                    exiStatus = effectActionChangeForce(numPlayers, effectRequest[1], effectRequest[2]);
                    break;
                case EFFECT_WARP_TOPLAYER:
                    exiStatus = effectPositionWarpToPlayer(numPlayers, effectRequest[1], effectRequest[2]);
                    break;
                case EFFECT_WARP_SWAP:
                    exiStatus = effectPositionSwap(numPlayers, effectRequest[1], effectRequest[2]);
                    break;
                case EFFECT_WARP_SWITCHDIRECTIONS:
                    exiStatus = effectPositionSwitchDirection(numPlayers, effectRequest[1]);
                    break;
                case EFFECT_MODE_FLIGHT:
                    // TODO: Package the new Dolphin with increased packet size
                    exiStatus = effectModeFlight(effectRequest[1], effectRequest[2], effectRequest[3], effectRequest[4], effectRequest[5]);
                    break;
                case EFFECT_MODE_BORDERLESS:
                    exiStatus = effectModeBorderless(effectRequest[1]);
                    break;
                case EFFECT_MODE_ELEMENT:
                    exiStatus = effectModeElement(effectRequest[1]);
                    break;
                case EFFECT_MODE_ZTD:
                    exiStatus = effectModeZTD(effectRequest[1]);
                    break;
                case EFFECT_MODE_BOMBRAIN:
                    exiStatus = effectModeBombRain(effectRequest[1]);
                    break;
                case EFFECT_MODE_WILD:
                    exiStatus = effectGameWild(effectRequest[1], effectRequest[2], effectRequest[3]);
                    break;
                case EFFECT_MODE_SPEED:
                    exiStatus = effectGameSpeed(effectRequest[1], effectRequest[2]);
                    break;
                case EFFECT_MODE_WAR:
                    exiStatus = effectModeWar(effectRequest[1]);
                    break;
                case EFFECT_MODE_RANDOMANGLE:
                    exiStatus = effectModeRandomAngle(effectRequest[1]);
                    break;
                case EFFECT_MODE_BIGHEAD:
                    exiStatus = effectModeBigHead(effectRequest[1], effectRequest[2], effectRequest[3]);
                    break;
                case EFFECT_MODE_HITFALL:
                    exiStatus = effectGameHitfall(effectRequest[1]);
                    break;
                case EFFECT_MODE_LANDINGLAG:
                    exiStatus = effectGameLandingLag(numPlayers, effectRequest[1], effectRequest[2], effectRequest[3], effectRequest[4], effectRequest[5]);
                    break;
                case EFFECT_MODE_SUDDENDEATH:
                    exiStatus = effectGameSuddenDeath(numPlayers, effectRequest[1], effectRequest[2], 300.0);
                    break;
                case EFFECT_ATTRIBUTE_SLIP:
                    exiStatus = effectAttributeSlip(numPlayers, effectRequest[1], effectRequest[2], effectRequest[3], effectRequest[4]);
                    break;
                case EFFECT_ATTRIBUTE_NUMJUMPS:
                    exiStatus = effectAttributeNumJumps(numPlayers, effectRequest[1], effectRequest[2], effectRequest[3]);
                    break;
                case EFFECT_ATTRIBUTE_JUMPSQUAT:
                    exiStatus = effectAttributeJumpSquat(numPlayers, effectRequest[1], effectRequest[2], effectRequest[3]);
                    break;
                case EFFECT_ATTRIBUTE_GROUNDFRICTION:
                    exiStatus = effectAttributeGroundFriction(numPlayers, effectRequest[1], effectRequest[2], effectRequest[3]);
                    break;
                case EFFECT_ATTRIBUTE_GRAVITY:
                    exiStatus = effectAttributeGravity(numPlayers, effectRequest[1], effectRequest[2], effectRequest[3]);
                    break;
                case EFFECT_ATTRIBUTE_FASTFALLSPEED:
                    exiStatus = effectAttributeFastFallSpeed(numPlayers, effectRequest[1], effectRequest[2], effectRequest[3]);
                    break;
                case EFFECT_ATTRIBUTE_WEIGHT:
                    exiStatus = effectAttributeWeight(numPlayers, effectRequest[1], effectRequest[2], effectRequest[3]);
                    break;
                case EFFECT_ATTRIBUTE_SIZE:
                    exiStatus = effectAttributeSize(numPlayers, effectRequest[1], effectRequest[2], effectRequest[3]);
                    break;
                case EFFECT_ATTRIBUTE_SHIELD:
                    exiStatus = effectAttributeShield(numPlayers, effectRequest[1], effectRequest[2], effectRequest[3], effectRequest[4], effectRequest[5]);
                    break;
                case EFFECT_ATTRIBUTE_ITEMTHROWSTRENGTH:
                    exiStatus = effectAttributeItemThrow(numPlayers, effectRequest[1], effectRequest[2], effectRequest[3], effectRequest[4], effectRequest[5]);
                    break;
                case EFFECT_NOT_CONNECTED:
                case EFFECT_NONE:
                case EFFECT_UNKNOWN:
                    break;
                default:
                    exiStatus = RESULT_EFFECT_FAILURE;
                    break;
            }

            if (testWaitDuration == 0) {//preloadedPokemonId < 0) {

                if (padSystem->pads[0].buttons.LeftDPad) {
                    //effectStatusGiveCurry(numPlayers, 0, 0);
                    //effectStatusGiveMushroom(numPlayers, 0, 1, 0);
                    //effectActionChangeForce(numPlayers, 0, 0x10C);
                    //effectStatusGiveFinalSmash(numPlayers, 0, 0);
                    //effectStatusGiveSwap(4, 0, 1, 0, 12);
                    //effectItemSpawn(numPlayers, 0x29, 1, 3); //0x78, 1); // 0x2A - Pokeball

                    //effectItemPreloadPokemon(0x69, 1); // Deoxys
                    //effectItemPreloadAssist(0xAD, 1); // Tingle

                    //effectItemAttachGooey(numPlayers, 0, 1);
                    //effectPositionWarpToPlayer(numPlayers, 0, 1);
                    //effectPositionSwap(numPlayers, MAX_PLAYERS + 1, MAX_PLAYERS + 1);
                    //effectPositionSwap(numPlayers, 0, 1);

                    //effectModeFlight(12, 2, 2, (u16)-1, 1);
                    //effectModeBorderless(12);
                    //effectModeElement(12);
                    //effectModeZTD(12);
                    //effectModeBombRain(12);
                    //effectGameWild(12, 6, true);
                    //effectGameSpeed(12, 120);
                    //effectModeWar(12);
                    //effectModeRandomAngle(12);
                    //effectModeBigHead(12, 0, false);
                    //effectGameHitfall(12);
                    //effectGameLandingLag(numPlayers, 12, 0, false, 0, -2);
                    //effectAttributeSlip(numPlayers, 12, 0, 100, false);
                    //effectAttributeNumJumps(numPlayers, 12, 0, 1);
                    //effectAttributeJumpSquat(numPlayers, 12, 0, 15);
                    //effectAttributeGroundFriction(numPlayers, 12, 0, -9);
                    //effectAttributeGravity(numPlayers, 12, 0, -1);
                    //effectAttributeFastFallSpeed(numPlayers, 12, 0, -3);
                    //effectAttributeWeight(numPlayers, 12, 1, 20);
                    //effectAttributeSize(numPlayers, 12, 0, -5);
                    //effectPositionSwitchDirection(numPlayers, 0);
                    //effectAttributeShield(numPlayers, 12, 0, 1, 1, 1);
                    //effectAttributeItemThrow(numPlayers, 12, 0, 0, 0, 0);
                    //effectGameSuddenDeath(numPlayers, 12, 0, 300.0);

                    //OSReport("paramCustomizeModule Address: %08x\n", getFighter(0)->modules->paramCustomizeModule);
                    testWaitDuration = 60;

                } else if (padSystem->pads[0].buttons.RightDPad) {
                    //effectStatusGiveCurry(numPlayers, 0, 1);
                    //effectItemSpawn(0x2A, 1); //0x78, 1); // 0x2A - Pokeball
                    //effectStatusGiveEquip(numPlayers, 0, 0x32);
                    //effectStatusGiveMushroom(numPlayers, 0, 1, 0);
                    //effectActionChangeForce(numPlayers, 0, 0xC7);
                    //effectGameGiveDamage(numPlayers, 0, (int)15, 0);
                    //effectStatusGiveFinalSmash(numPlayers, 0, 1);
                    //effectStatusGiveSwap(4, 0, 1, 1, 720);

                    //effectItemPreloadPokemon(0x6C, 1); // Staryu
                    //effectItemPreloadAssist(0x96, 1); // Hammer Bro


                } else if (padSystem->pads[0].buttons.UpDPad) {
                    //effectItemPreloadPokemon(0x84, 1); // Suicune
                    //effectItemPreloadAssist(0xA2, 1); // Isaac


                } else if (padSystem->pads[0].buttons.DownDPad) {
                    //effectItemPreloadPokemon(0x66, 1); // Entei
                    //effectItemPreloadAssist(0xAA, 1); // Stafy

                } else if (padSystem->pads[0].buttons.Z) {
                    //effectItemPreloadPokemon(0x64, 1); // Chickorita
                    //effectItemPreloadAssist(0xAF, 1); // Waluigi

                } else if (padSystem->pads[0].buttons.B) {
                    //effectItemPreloadPokemon(0x6E, 1); // Hooh
                    //effectAssistSpawn(0x94, 1); // // Knuckle Joe

                }
                else if (padSystem->pads[0].buttons.R) {
                    //effectItemPreloadPokemon(0x7B, 1); // Metagross
                    //effectItemPreloadAssist(0x9F, 1); // Nintendog

                }
            }

            /*int capacity = ITEM_MANAGER->itKindArrayList.capacity();
            for (int i = 0; i < capacity; i++) {
                debugData->loadedPkmn[i] = 0;
            }*/

            int size = ITEM_MANAGER->itKindArrayList.size();
            for (int i = 0; i < size; i++) {
                u32* pkmnPtr = ITEM_MANAGER->itKindArrayList.at(i);
                //u32* pkmnPtr = ((u32* (*)(void* it, int i)) ITEM_MANAGER->itArchiveArrayList_vtable->at)(&ITEM_MANAGER->itArchiveArrayList_vtable, i);
                if (pkmnPtr != nullptr) debugData->loadedPkmn[i] = *pkmnPtr;
            }

        }

        if (exiStatus >= 5) {
            EXIPacket responsePckt = EXIPacket(exiStatus, nullptr, 0);
            responsePckt.Send();
        }

        // TODO: Print current active effect e.g. Random Element (and maybe time left) (should handle it in durationFinished)

        startNormalDraw();
        if (visible) {
            printer.setup();
            printer.start2D();

            message->fontScaleY = RENDER_SCALE_Y;
            message->fontScaleX = RENDER_SCALE_X;
            printer.lineHeight = 20 * message->fontScaleY;
            message->xPos = LEFT_PADDING;
            message->yPos = TOP_PADDING;
            myMenu->render(&printer, buffer);
        }

        startNormalDraw();



    }

    //void onEndMatch() {

    //}

    // called at the end of the game logic in a frame
    //void EndFrame() {

    //}

    SIMPLE_INJECTION(startGame, 0x806dd5f4, "mr r3, r19") { SendGameStatus(EXIStatus::STATUS_GAME_STARTED); } // when booting up
    SIMPLE_INJECTION(startMatch, 0x800dc590, "li r9, 0x2") {
        saveEffectGame();
        saveEffectMode();
        SendGameStatus(EXIStatus::STATUS_MATCH_STARTED); } // when starting match
    SIMPLE_INJECTION(endMatch, 0x806d4844, "li r4, 0") {
        resetEffectGame();
        resetEffectMode();
        resetEffectAttribute();
        SendGameStatus(EXIStatus::STATUS_MATCH_ENDED); } // when exiting match
    /*INJECTION("frameUpdate", 0x8001792c, R"(
    bl onUpdateFrame
    addi r3, r30, 280
)");*/

    SIMPLE_INJECTION(beginFrame, 0x80147394, "li r0, 0x1") { onUpdateFrame(); }
    //SIMPLE_INJECTION(endFrame,   0x801473a0, "li r0, 0x0") { EndFrame(); }


    INJECTION("forceVisMemPool", 0x80025dc8, R"(
    cmpwi r3, 69
)");

}