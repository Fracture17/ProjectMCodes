#ifndef PROJECTMCODES_MOVEMENTTRACKER_H
#define PROJECTMCODES_MOVEMENTTRACKER_H

#include "Wii/GX/GX.h"
// the name for each value
#define MOV_NONE 0
#define MOV_IDLE 1
#define MOV_MISSEDTECH 2
#define MOV_RUN 3
#define MOV_DASH 4
#define MOV_HITSTUN 5
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
  // if the difference between the incoming value and the value its compared to
  // is greater than this, the influence of this particular weight is 0
  u8 tolerence;
  float weight;
};

// { tolerence, weight }
struct MarkovInputs {
  // time spent in a particular action "category" (ex - attacking, running, grabbing)
  static constexpr MarkovInput time = { 60, 2 }; 
  // Y Distance to the ground beneath a fighter
  static constexpr MarkovInput yDistFloor = { 35, 3 };
  // X distance between the AI and the opponent 
  static constexpr MarkovInput xDist = { 75, 6 }; 
  // these two add a little extra "YOMI"
  // the AI's own chance of being in an attacking state
  static constexpr MarkovInput attackChance = { 125, 4 }; 
  // the AI's own chance of being in an defending state
  static constexpr MarkovInput shieldChance = { 125, 4 };
  // the x position of the fighter 
  static constexpr MarkovInput xPos = { 250, 4 }; 
  // the particular action category in question (attack/run/grab/etc)
  static constexpr MarkovInput mov = { 1, 2 }; 
};

class MovementTracker {
public:
  MovementTracker() {};

  void reset();
  void trackAction(int action, bool isAction);
  void updateData(u8 yDistFloor, u8 xPos, u8 distance = 0, u8 attackChance = 0, u8 shieldChance = 0, s16 otherMov = 0);
  float approxChance(int levelValue, char actionType);
  float approxCommit(int levelValue);
  void incrementTimer();
  void undoLastAction();
  s16 getCurrentMov();

private:
  // will multiply alongside time for each tracked action to get resulting weight
  // 100 = a multiplier of 1 (because chars consume less space than floats)
  static constexpr unsigned char weights[0x10] = {
    20, // NONE
    40, // IDLE
    0, // WALK
    60, // RUN
    120, // DASH
    100, // HITSTUN
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

  void calcWeight(char actionType);
  void copyLatest(char copy);
  unsigned short idx = 0;
  unsigned char actionTracker[ACTION_COUNT] = {};
  unsigned char timeTracker[ACTION_COUNT] = {};
  unsigned char yDistFloorTracker[ACTION_COUNT] = {};
  unsigned char distanceTracker[ACTION_COUNT] = {};
  unsigned char attackChanceTracker[ACTION_COUNT] = {};
  unsigned char shieldChanceTracker[ACTION_COUNT] = {};
  unsigned char xPosTracker[ACTION_COUNT] = {};
  char movTracker[ACTION_COUNT] = {};
  float weightCache[MOV_LEN] = {};
  float weightCacheTotal = 0;
  float chanceCache[MOV_LEN] = {};
  u8 actionCacheEntries = 0;
  unsigned char attackGuess = 0;
};

unsigned char actionToMov(int action);
float getMovDiffMultiplier(int source, int comparison);

#endif // PROJECTMCODES_MOVEMENTTRACKER_H