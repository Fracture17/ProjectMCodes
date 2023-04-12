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
#define ACTION_COUNT 0x80

template<class T>
struct KVPair {
  const char* key;
  T value;
  GXColor color;
};

struct MarkovInput {
  u8 tolerence;
  float weight;
};

struct MarkovInputs {
  static constexpr MarkovInput time = { 200, 2 }; 
  static constexpr MarkovInput yDistFloor = { 35, 1 }; 
  static constexpr MarkovInput xDist = { 75, 4 }; 
  // these two add a little extra "YOMI"
  // the AI's own chance of being in an attacking state
  static constexpr MarkovInput attackChance = { 125, 2 }; 
  // the AI's own chance of being in an defending state
  static constexpr MarkovInput shieldChance = { 125, 2 }; 
  static constexpr MarkovInput xPos = { 250, 3 }; 
};

class MovementTracker {
public:
  MovementTracker() {};

  void reset();
  void trackAction(int action, bool isAction, u8 yDistFloor, u8 xPos, u8 distance = 0, u8 attackChance = 0, u8 shieldChance = 0);
  float approxChance(float levelValue);
  float approxChance(float levelValue, char actionType);
  void incrementTimer();

private:
  // will multiply alongside time for each tracked action to get resulting weight
  // 100 = a multiplier of 1 (because chars consume less space than floats)
  static constexpr unsigned char weights[0x10] = {
    20, // NONE
    90, // IDLE
    0, // WALK
    60, // RUN
    120, // DASH
    0, // DASHTURN
    100, // CROUCH
    120, // JUMP
    130, // DJUMP
    100, // FALL
    160, // SHIELD 
    80, // AIRDODGE 
    115, // ROLL 
    110, // TECH 
    150, // ATTACK 
    190, // GRAB 
  };

  void copyLatest(char copy);
  unsigned short idx = 0;
  unsigned char actionTracker[ACTION_COUNT] = {};
  unsigned char timeTracker[ACTION_COUNT] = {};
  unsigned char yDistFloorTracker[ACTION_COUNT] = {};
  unsigned char distanceTracker[ACTION_COUNT] = {};
  unsigned char attackChanceTracker[ACTION_COUNT] = {};
  unsigned char shieldChanceTracker[ACTION_COUNT] = {};
  unsigned char xPosTracker[ACTION_COUNT] = {};
  float actionCache[MOV_LEN] = {};
  unsigned char attackGuess = 0;
};

unsigned char actionToMov(int action);

#endif // PROJECTMCODES_MOVEMENTTRACKER_H