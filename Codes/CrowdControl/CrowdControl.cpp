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

            checkEffectGameDurationFinished();
            checkEffectModeDurationFinished();
            checkPositionResetCorrect();
            checkItemSpawnPokemonOrAssist();

            int numPlayers = FIGHTER_MANAGER->getEntryCount();

            switch (effectRequest[0]) {
                case EFFECT_GIVE_DAMAGE:
                    exiStatus = effectGameGiveDamage(numPlayers, effectRequest[1], effectRequest[2], effectRequest[3]);
                    break;
                case EFFECT_ITEM_SPAWN_REGULAR:
                    exiStatus = effectItemSpawn(effectRequest[1], effectRequest[2]);
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
                    //effectStatusGiveMushroom(numPlayers, 0, 1, 1);
                    //effectActionChangeForce(numPlayers, 0, 0x10C);
                    //effectStatusGiveFinalSmash(numPlayers, 0, 0);
                    //effectStatusGiveSwap(4, 0, 1, 0, 12);
                    //effectItemSpawn(0x0, 1); //0x78, 1); // 0x2A - Pokeball

                    //effectItemPreloadPokemon(0x69); // Deoxys
                    //preloadedPokemonId = 0x69;
                    //preloadedPokemonAmount = 1;

                    //effectItemPreloadAssist(0x9D); // Little Mac
                    //preloadedAssistId = 0x9D;
                    //preloadedAssistAmount = 1;

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

                    //effectItemPreloadPokemon(0x6C); // Staryu
                    //preloadedPokemonId = 0x6C;
                    //preloadedPokemonAmount = 1;

                    //effectItemPreloadAssist(0x96); // Hammer Bro
                    //preloadedAssistId = 0x96;
                    //preloadedAssistAmount = 2;

                } else if (padSystem->pads[0].buttons.UpDPad) {
                    //effectItemPreloadPokemon(0x84); // Suicune
                    //preloadedPokemonId = 0x84;
                    //preloadedPokemonAmount = 1;

                    //effectItemPreloadAssist(0xA2); // Isaac
                    //preloadedAssistId = 0xA2;
                    //preloadedAssistAmount = 1;

                } else if (padSystem->pads[0].buttons.DownDPad) {
                    //effectItemPreloadPokemon(0x66); // Entei
                    //preloadedPokemonId = 0x66;
                    //preloadedPokemonAmount = 1;

                    //effectItemPreloadAssist(0xAA); // Stafy
                    //preloadedAssistId = 0xAA;
                    //preloadedAssistAmount = 1;
                } else if (padSystem->pads[0].buttons.Z) {
                    //effectItemPreloadPokemon(0x64); // Chickorita
                    //preloadedPokemonId = 0x64;
                    //preloadedPokemonAmount = 1;

                    //effectItemPreloadAssist(0xAF); // Waluigi
                    //preloadedAssistId = 0xAF;
                    //preloadedAssistAmount = 1;
                } else if (padSystem->pads[0].buttons.B) {
                    //effectItemPreloadPokemon(0x6E); // Hooh
                    //preloadedPokemonId = 0x6E;
                    //preloadedPokemonAmount = 1;

                    //effectAssistSpawn(0x94, 1); // // Knuckle Joe
                    //preloadedAssistId = 0x94;
                    //preloadedAssistAmount = 1;
                }
                else if (padSystem->pads[0].buttons.R) {
                    //effectItemPreloadPokemon(0x7B); // Metagross
                    //preloadedPokemonId = 0x7B;
                    //preloadedPokemonAmount = 1;

                    //effectItemPreloadAssist(0x9F); // Nintendog
                    //preloadedAssistId = 0xAA;
                    //preloadedAssistAmount = 1;
                }
            }

            int capacity = ((int (*)(void* it)) ITEM_MANAGER->itKindArrayList_vtable->capacity)(&ITEM_MANAGER->itKindArrayList_vtable);
            for (int i = 0; i < capacity; i++) {
                debugData->loadedPkmn[i] = 0;
            }

            int size = ((int (*)(void* it)) ITEM_MANAGER->itKindArrayList_vtable->size)(&ITEM_MANAGER->itKindArrayList_vtable);
            for (int i = 0; i < size; i++) {
                u32* pkmnPtr = ((u32* (*)(void* it, int i)) ITEM_MANAGER->itKindArrayList_vtable->at)(&ITEM_MANAGER->itKindArrayList_vtable, i);
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
    SIMPLE_INJECTION(startMatch, 0x800dc590, "li r9, 0x2") { SendGameStatus(EXIStatus::STATUS_MATCH_STARTED); } // when starting match
    SIMPLE_INJECTION(endMatch, 0x806d4844, "li r4, 0") {
        resetEffectGame();
        resetEffectMode();
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