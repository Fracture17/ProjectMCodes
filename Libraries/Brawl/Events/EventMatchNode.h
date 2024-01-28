#ifndef PROJECTMCODES_EVENTMATCHNODE_H
#define PROJECTMCODES_EVENTMATCHNODE_H

#include "stddef.h"


// struct EventMatchNode {
//   // 0x0
//   u32 hordeSetting; // aka event extension; can be 0, 1, or 2
//   char _spacer0[0x8 - 0x0 - 4];
//   // 0x8
//   u8 matchType;
//   char _spacer1[0xC - 0x8 - 1];
//   // 0xC
//   u32 timeLimitFrames;
//   // 0x10
//   struct _bits_x10 {
//     flag_0 : 1;
//     flag_1 : 1;
//     flag_2 : 1;
//     flag_3 : 1;
//     flag_4 : 1;
//     timerVisible : 1; // meleeInitData teamAttackByte (0x2)
//     hideCountdown : 1; // meleeInitData 0x1 b1
//     used_flag_7 : 1; // meleeInitData 0x1 b2 ; only used in sqAdventure::setAdventureCondition ?
//   } bits_x10;
//   char _spacer1[0x14 - 0x10 - 1];
//   // 0x14
//   s32 unknown_x14; // meleeInitData 0x44
//   // 0x18
//   struct _bits_x18 {
//     flag_0 : 1;
//     flag_1 : 1;
//     flag_2 : 1;
//     flag_3 : 1;
//     flag_4 : 1;
//     flag_5 : 1;
//     flag_6 : 1; 
//     hideDamageValues : 1; // meleeInitData 0x3
//   } bits_x18;
//   bool isTeamGame; // meleeInitData 0xB
//   struct _bits_x1A {
//     flag_0 : 1;
//     flag_1 : 1;
//     flag_2 : 1;
//     flag_3 : 1;
//     flag_4 : 1;
//     flag_5 : 1;
//     flag_6 : 1;
//     used_flag_7 : 1; // meleeInitData teamAttackByte (0x2)
//   } bits_x1A;
//   char _spacer2[0x1B - 0x19 - 1];
//   // 0x1B
//   u8 itemFrequency;
//   char _spacer3[0x1E - 0x1B - 1];
//   // 0x1E
//   s16 stageId;
//   // 0x20
//   u8 subStageId;
//   // 0x21
//   struct _bits_x21 {
//     numPlayersOnScreen : 3;
//     flag_3 : 1;
//     flag_4 : 1;
//     teamAttackByteStuff_b5 : 3; // meleeInitData teamAttackByte (0x2 b345)
//   } bits_x21;
//   char _spacer4[0x38 - 0x21 - 1];
//   // 0x38
//   float gameSpeed;
//   // 0x3C
//   float cameraShakeControl;
//   struct _bits_x40 {
//     flag_0 : 1;
//     flag_1 : 1;
//     flag_2 : 1;
//     flag_3 : 1;
//     flag_4 : 1;
//     flag_5 : 1;
//     flag_6 : 1;
//     used_flag_7 : 1; // meleeInitData 0x7 b2
//   } bits_x40;
//   char _spacer5[0x44 - 0x40 - 1];
//   // 0x44
//   u32 songId;
//   // 0x46
//   s16 globalOffsenseRatio;
//   // 0x48
//   s16 globalDefenseRatio;
//   char _spacer6[0x50 - 0x48 - 2];
// };

// void thing () {
//   EventMatchNode n;
//   n.bits_x10.flag_0;
// }

#endif