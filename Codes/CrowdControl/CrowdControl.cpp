//
// Created by Ilir on 2021-12-13.
//

#include "CrowdControl.h"
#include "EXIPacket.h"
#include "EffectItemHandler.h"
#include "EffectStatusHandler.h"
#include "EffectActionHandler.h"
#include "EffectGameHandler.h"
#include "Brawl/GF/gfPadSystem.h"

namespace FrameLogic {
    // setup global variables
    DebugData data = DebugData();
    DebugData* debugData = &data;
    Menu* myMenu;
    bool visible = false;

    const int MIN_PRELOADED_ITEM_FRAMES = 60;
    int preloadedItemId = -1;
    int preloadedItemWaitFrames = 0;
    int preloadedItemAmount = 0;

    //hacky way to check if in game
    unsigned int getScene() {
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
        unsigned int scene = getScene();

        // TODO: Investigate crash during classic mode

        // for testing effects
        gfPadSystem *padSystem = PAD_SYSTEM;

        if (scene == SCENE_TYPE::SCENE_VS || scene == SCENE_TYPE::SCENE_TRAINING_MODE_MMS) {
            //visible = true;

            if (preloadedItemId >= 0) { // TODO: Tell to crowd control to wait if currently preloading
                preloadedItemWaitFrames++;
                if (preloadedItemWaitFrames > MIN_PRELOADED_ITEM_FRAMES) {
                    effectItemSpawn(preloadedItemId, preloadedItemAmount);
                    preloadedItemId = -1;
                    preloadedItemWaitFrames = 0;
                    preloadedItemAmount = 0;
                }
            }

            int numPlayers = FIGHTER_MANAGER->getEntryCount();

            switch (effectRequest[0]) {
                case EFFECT_GIVE_DAMAGE:
                    exiStatus = effectGameGiveDamage(numPlayers, effectRequest[1], effectRequest[2], effectRequest[3]);
                    break;
                case EFFECT_ITEM_SPAWN_REGULAR:
                    exiStatus = effectItemSpawn(effectRequest[1], effectRequest[2]);
                    break;
                case EFFECT_ITEM_SPAWN_POKEMON:
                    if (preloadedItemId < 0) {
                        exiStatus = effectPokemonPreload(effectRequest[1]);
                        preloadedItemId = effectRequest[1];
                        preloadedItemAmount = effectRequest[2];
                    }
                    else exiStatus = RESULT_EFFECT_RETRY;
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
                case EFFECT_NOT_CONNECTED:
                case EFFECT_NONE:
                case EFFECT_UNKNOWN:
                    break;
                default:
                    exiStatus = RESULT_EFFECT_FAILURE;
                    break;
            }

            if (preloadedItemId < 0) {

                if (padSystem->pads[0].buttons.LeftDPad) {
                    //effectStatusGiveCurry(numPlayers, 0, 0);
                    //effectStatusGiveMushroom(numPlayers, 0, 1, 1);
                    //effectActionChangeForce(numPlayers, 0, 0x10C);
                    //effectStatusGiveFinalSmash(numPlayers, 0, 0);
                    //effectStatusGiveSwap(4, 0, 1, 0, 720);
                    effectItemSpawn(0x2A, 1); //0x78, 1); // 0x2A - Pokeball

                    //effectPokemonPreload(0x69); // Deoxys
                    //preloadedItemId = 0x69;
                    //preloadedItemAmount = 1;

                    //effectAssistPreload(0x96); // Hammer Bro
                    //preloadedItemId = 0x96;
                } else if (padSystem->pads[0].buttons.RightDPad) {
                    //effectStatusGiveCurry(numPlayers, 0, 1);
                    //effectItemSpawn(0x2A, 1); //0x78, 1); // 0x2A - Pokeball
                    //effectStatusGiveEquip(numPlayers, 0, 0x32);
                    //effectStatusGiveMushroom(numPlayers, 0, 1, 0);
                    //effectActionChangeForce(numPlayers, 0, 0xC7);
                    //effectGameGiveDamage(numPlayers, 0, (int)15, 0);
                    //effectStatusGiveFinalSmash(numPlayers, 0, 1);
                    //effectStatusGiveSwap(4, 0, 1, 1, 720);

                    effectPokemonPreload(0x6C); // Staryu
                    preloadedItemId = 0x6C;
                    preloadedItemAmount = 1;

                    //effectAssistPreload(0x9D); // Little Mac
                    //preloadedItemId = 0x9D;
                } else if (padSystem->pads[0].buttons.UpDPad) {
                    effectPokemonPreload(0x84); // Suicune
                    preloadedItemId = 0x84;
                    preloadedItemAmount = 1;

                    //effectAssistPreload(0xA2); // Isaac
                    //preloadedItemId = 0xA2;

                } else if (padSystem->pads[0].buttons.DownDPad) {
                    effectPokemonPreload(0x66); // Entei
                    preloadedItemId = 0x66;
                    preloadedItemAmount = 1;

                    //effectAssistPreload(0xAA); // Stafy
                    //preloadedItemId = 0xAA;
                } else if (padSystem->pads[0].buttons.Z) {
                    //effectPokemonPreload(0x64); // Chickorita
                    //preloadedItemId = 0x64;
                    //preloadedItemAmount = 1;

                    //effectAssistPreload(0xAF); // Waluigi
                    //preloadedItemId = 0xAF;
                } else if (padSystem->pads[0].buttons.B) {
                    //effectPokemonPreload(0x6E); // Hooh
                    //preloadedItemId = 0x6E;
                    //preloadedItemAmount = 1;

                    //effectAssistSpawn(0x94, 1); // // Knuckle Joe
                }
                else if (padSystem->pads[0].buttons.R) {
                    //effectPokemonPreload(0x7B); // Metagross
                    //preloadedItemId = 0x7B;
                    //preloadedItemAmount = 1;

                    //effectAssistSpawn(0x94, 1); // // Knuckle Joe
                }
            }

            int capacity = ((int (*)(void* it)) ITEM_MANAGER->itKindArrayList_vtable->capacity)(&ITEM_MANAGER->itKindArrayList_vtable);
            for (int i = 0; i < capacity; i++) {
                debugData->loadedPkmn[i] = 0;
            }

            int size = ((int (*)(void* it)) ITEM_MANAGER->itKindArrayList_vtable->size)(&ITEM_MANAGER->itKindArrayList_vtable);
            for (int i = 0; i < size; i++) {
                int* pkmnPtr = ((int* (*)(void* it, int i)) ITEM_MANAGER->itKindArrayList_vtable->at)(&ITEM_MANAGER->itKindArrayList_vtable, i);
                //int* pkmnPtr = ((int* (*)(void* it, int i)) ITEM_MANAGER->itArchiveArrayList_vtable->at)(&ITEM_MANAGER->itArchiveArrayList_vtable, i);
                if (pkmnPtr != nullptr) debugData->loadedPkmn[i] = *pkmnPtr;
            }

        }

        if (exiStatus >= 5) {
            EXIPacket responsePckt = EXIPacket(exiStatus, nullptr, 0);
            responsePckt.Send();
        }


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
    SIMPLE_INJECTION(endMatch, 0x806d4844, "li r4, 0") { SendGameStatus(EXIStatus::STATUS_MATCH_ENDED); } // when exiting match
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