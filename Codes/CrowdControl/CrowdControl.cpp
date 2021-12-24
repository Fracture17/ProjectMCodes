//
// Created by Ilir on 2021-12-13.
//

#include "CrowdControl.h"
#include "EXIPacket.h"
#include "EffectItemHandler.h"
#include "EffectStatusHandler.h"
#include "Brawl/GF/gfPadSystem.h"

namespace FrameLogic {

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

    // called at the beginning of the logic in a frame
    void BeginFrame() {

        // halfword vector for sending into to the game
        // first entry is effect id (EFFECT_NOT_CONNECTED = 0, EFFECT_NONE = 1, EFFECT_UNKNOWN = 2, EFFECT_ACTUAL >= 3
        // second and above entries are parameters for the effect
        EXIPacket requestPckt = EXIPacket(5*sizeof(u16));
        requestPckt.Receive();
        u16* effectRequest = (u16*)requestPckt.source;

        EXIStatus exiStatus = STATUS_UNKNOWN;
        unsigned int scene = getScene();

        if (scene == SCENE_TYPE::SCENE_VS || scene == SCENE_TYPE::SCENE_TRAINING_MODE_MMS) {
            int numPlayers = FIGHTER_MANAGER->getEntryCount();

            switch (effectRequest[0]) {
                case EFFECT_ITEM_SPAWN_REGULAR:
                    exiStatus = effectItemSpawn(effectRequest[1], effectRequest[2]);
                    break;
                case EFFECT_STATUS_METAL:
                    exiStatus = effectStatusGiveMetal(numPlayers, effectRequest[1], 1, effectRequest[2]);
                    break;
                case EFFECT_STATUS_CURRY:
                    exiStatus = effectStatusGiveCurry(numPlayers, effectRequest[1], 1);
                    break;
                case EFFECT_STATUS_HAMMER:
                    exiStatus = effectStatusGiveHammer(numPlayers, effectRequest[1], effectRequest[2]);
                    break;
                case EFFECT_STATUS_SUPERSTAR:
                    exiStatus = effectStatusGiveSuperStar(numPlayers, effectRequest[1], 1);
                    break;
                case EFFECT_STATUS_FLOWER:
                    exiStatus = effectStatusGiveFlower(numPlayers, effectRequest[1], 1, effectRequest[2],
                                                       effectRequest[3]);
                    break;
                case EFFECT_STATUS_HEART:
                    exiStatus = effectStatusGiveHeart(numPlayers, effectRequest[1], effectRequest[2], 1);
                    break;
                case EFFECT_STATUS_SLOW:
                    exiStatus = effectStatusGiveSlow(numPlayers, effectRequest[1], 1, effectRequest[2]);
                    break;
                case EFFECT_STATUS_MUSHROOM:
                    exiStatus = effectStatusGiveMushroom(numPlayers, effectRequest[1], 1, effectRequest[2]);
                    break;
                case EFFECT_STATUS_EQUIP:
                    exiStatus = effectStatusGiveEquip(numPlayers, effectRequest[1], effectRequest[2]);
                    break;
                case EFFECT_NOT_CONNECTED:
                case EFFECT_NONE:
                case EFFECT_UNKNOWN:
                    break;
                default:
                    exiStatus = RESULT_EFFECT_FAILURE;
                    break;
            }

            // for testing effects
            gfPadSystem* padSystem = PAD_SYSTEM;

            if (padSystem->pads[0].buttons.LeftDPad) {
                //effectStatusGiveCurry(numPlayers, 0, 0);
                effectStatusGiveMushroom(numPlayers, 0, 1, 1);
            }
            else if (padSystem->pads[0].buttons.RightDPad) {
                //effectStatusGiveCurry(numPlayers, 0, 1);
                //effectItemSpawn(0x0, 1); //0x78, 1); // 0x2A - Pokeball
                //effectStatusGiveBury(numPlayers, 0, 1);
                //effectStatusGiveScrewAttack(numPlayers, 0);
                effectStatusGiveMushroom(numPlayers, 0, 1, 0);
            }

        }

        if (exiStatus >= 5) {
            EXIPacket responsePckt = EXIPacket(exiStatus, nullptr, 0);
            responsePckt.Send();
        }

    }

    // called at the end of the game logic in a frame
    //void EndFrame() {

    //}

    SIMPLE_INJECTION(startGame, 0x806dd5f4, "mr r3, r19") { SendGameStatus(EXIStatus::STATUS_GAME_STARTED); } // when booting up
    SIMPLE_INJECTION(startMatch, 0x800dc590, "li r9, 0x2") { SendGameStatus(EXIStatus::STATUS_MATCH_STARTED); } // when starting match
    SIMPLE_INJECTION(endMatch, 0x806d4844, "li r4, 0") { SendGameStatus(EXIStatus::STATUS_MATCH_ENDED); } // when exiting match
    SIMPLE_INJECTION(beginFrame, 0x80147394, "li r0, 0x1") { BeginFrame(); }
    //SIMPLE_INJECTION(endFrame,   0x801473a0, "li r0, 0x0") { EndFrame(); }
}