#ifndef PROJECTMCODES_MOVEMENTTRACKER_H
#define PROJECTMCODES_MOVEMENTTRACKER_H

#include "Wii/GX/GX.h"
// the name for each value
#define MOV_NONE 0
#define MOV_IDLE 1
#define MOV_WALK 2
#define MOV_RUN 3
#define MOV_DASH 4
#define MOV_DASHTURN 5
#define MOV_CROUCH 6
#define MOV_JUMP 7
#define MOV_DJUMP 8
#define MOV_FALL 9
#define MOV_SHIELD 10
#define MOV_AIRDODGE 11
#define MOV_ROLL 12
#define MOV_TECH 13
#define MOV_ATTACK 14
#define MOV_GRAB 15
#define MOV_LEN 16
#define MOV_CACHE_RESET -1

// number of actions to be tracked
#define ACTION_COUNT 0xFF

template<class T>
struct KVPair {
  const char* key;
  T value;
  GXColor color;
};

class MovementTracker {
public:
  MovementTracker() {};

  void reset();
  void trackAction(int action);
  float approxChance(float levelValue);
  float approxChance(float levelValue, char actionType);
  void incrementTimer();

private:
  // will multiply alongside time for each tracked action to get resulting weight
  // 100 = a multiplier of 1 (because chars consume less space than floats)
  static constexpr unsigned char weights[0x10] = {
    20, // NONE
    90, // IDLE
    110, // WALK
    80, // RUN
    120, // DASH
    0, // DASHTURN
    100, // CROUCH
    120, // JUMP
    130, // DJUMP
    100, // FALL
    170, // SHIELD 
    70, // AIRDODGE 
    130, // ROLL 
    130, // TECH 
    170, // ATTACK 
    190, // GRAB 
  };

  void copyLatest(char copy);
  unsigned short idx = 0;
  unsigned char actionTracker[ACTION_COUNT] = {};
  unsigned char timeTracker[ACTION_COUNT] = {};
  float actionCache[MOV_LEN] = {};
};

unsigned char actionToMov(int action);

#endif // PROJECTMCODES_MOVEMENTTRACKER_H