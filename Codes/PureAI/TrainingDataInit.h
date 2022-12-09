
#ifndef PROJECTMCODES_FUDGEMENU_H
#define PROJECTMCODES_FUDGEMENU_H

#include "Containers/vector.h"

#define sprintf ((int (*)(char* buffer, const char* format, ...)) 0x803f89fc)
#define OSReport ((void (*)(const char* text, ...)) 0x801d8600)

struct AIPersonality {
  float braveChance = 0;
  float baitChance = 0;
  float aggression = 0;
  float wall_chance = 0;
  float circleCampChance = 0;
  float bait_dashAwayChance = 0;
  float bait_wdashAwayChance = 0;
  float jumpiness = 0;
  float djumpiness = 0;
  float platChance = 0;
  float SDIChance = 0;
  float reactionTime = 0;
};

struct AIData {
  bool AIDebug = false;
  AIPersonality personality;
};

struct TrainingData {
  AIData aiData;
};

#endif // PROJECTMCODES_FUDGEMENU_H