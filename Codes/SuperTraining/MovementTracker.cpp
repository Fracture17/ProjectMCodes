#include "MovementTracker.h"
#include "Containers/vector.h"

void MovementTracker::reset() {
  for (int i = 0; i < MOV_LEN; i++) actionCache[i] = MOV_CACHE_RESET;
  for (int i = 0; i < ACTION_COUNT; i++) {
    actionTracker[i] = 0;
    timeTracker[i] = 0;
  }
  idx = 0;
};

unsigned char actionToMov(int action) {
  if ((0x24 <= action && action <= 0x33) || action > 0x112) return MOV_ATTACK;
  switch (action) {
    case 0x0: return MOV_IDLE;
    // case 0x1: return MOV_WALK;
    case 0x3: return MOV_DASH;
    case 0x4: return MOV_RUN;
    // case 0x7: return MOV_DASHTURN;
    case 0x11: return MOV_CROUCH;
    case 0xB: return MOV_JUMP;
    case 0xC:
    case 0xD: return MOV_DJUMP;
    case 0xE:
    case 0xF: return MOV_FALL;
    case 0x1A: return MOV_SHIELD;
    case 0x21: return MOV_AIRDODGE;
    case 0x1E:
    case 0x1F:
    case 0x20: return MOV_ROLL;
    case 0x60: 
    case 0x61: return MOV_TECH;
    case 0x34:
    case 0x36:
    case 0x38: return MOV_GRAB;
    default:
      return MOV_NONE;
  }
};

// called each frame
void MovementTracker::incrementTimer() {
  for (int i = 0; i < MOV_LEN; i++) actionCache[i] = MOV_CACHE_RESET;
  timeTracker[idx] += 1;
  if (timeTracker[idx] >= 0xFF) {
    copyLatest(actionTracker[idx]);
  }
}

void MovementTracker::copyLatest(char copy) {
  if (copy != MOV_NONE) {
    idx += 1;
    if (idx >= ACTION_COUNT) idx = 0;
    actionTracker[idx] = copy;
    timeTracker[idx] = 0;
  }
};

void MovementTracker::trackAction(int action) {
  char MOV = actionToMov(action);
  if (MOV != MOV_NONE) {
    idx += 1;
    if (idx >= ACTION_COUNT) idx = 0;
    actionTracker[idx] = MOV;
    timeTracker[idx] = 0;
  }
};

// in the end we will get a value from 0 to 1 representing the odds of that
// action happening based upon the prior actions of the target
// to accomplish this there is a max score that is tracked throughout.
// we effectively achieve a weighted average, where weights for actions are 
// defined as a global array, with the weight decreasing as more elements are scanned.
// 
// this is not meant to be perfect, rather it is meant to be
// speed/size-efficient (and simple enough for me to figure out without 
// technically knowing true ML techniques)
float MovementTracker::approxChance(float CPULevel, char actionType) {
  // score is now a float to account for the varying weights
  if (actionCache[actionType] != MOV_CACHE_RESET) {
    return actionCache[actionType];
  }
  float score = 0;
  float totalScore = 0;
  // this is how the AI will be limited based on its level. 
  // higher levels have more access to the wider array, and as such can take
  // more information into account
  int lookAmount = (CPULevel / 100) * ACTION_COUNT;
  // we start at the most recent action and move backwards, decrementing the base
  // score throughout
  int tracker = idx;
  // this loop is responsible for simply looping through the array and finding 
  // the past occurences of the target action type, then storing them in the vector below
  vector<int> offsets;
  for (int i = 0; i < lookAmount; i++) {
    // because we use a circular array, we can just loop back around
    if (tracker < 0) tracker = ACTION_COUNT - 1;
    // if MOV_NONE is reached, then there are no more elements and it is pointless to continue
    if (actionTracker[tracker] == MOV_NONE) break;
    // 
    if (actionTracker[tracker] == actionType && tracker != idx) offsets.push(tracker);
    tracker --;
  }

  // to prevent the AI from reacting instantly
  int reactionTime = (1 - (CPULevel / 100)) * 45 + 12 + 7 * _randf();
  if (CPULevel == 101) reactionTime = 0;

  // create an offset to use instead of 
  int reactionPatchTime = timeTracker[idx];
  int reactionPatchIdx = idx;
  if (offsets.size() > 0) {
    while(reactionTime > 0) {
      reactionPatchTime = timeTracker[reactionPatchIdx];
      int difference = timeTracker[reactionPatchIdx] - reactionTime;
      reactionTime -= timeTracker[reactionPatchIdx];
      if (reactionTime <= 0) {
        reactionPatchTime -= difference;
      } else {
        reactionPatchIdx -= 1;
        if (reactionPatchIdx < 0) reactionPatchIdx += ACTION_COUNT;
      }
    }
  }

  // just so we don't look through the full array AGAIN if the level is high
  lookAmount *= 0.3;
  // for each offset in the array, we'll compare it to the current series of actions
  // and look for patterns in the actions taken and the timing of those actions
  // OSReport("offsets size: %d; idx: %d\n", offsets.size(), idx);
  for (int i = 0; i < offsets.size(); i++) {
    int startTracker = reactionPatchIdx;
    // because reasons
    int looked = 0;
    if (actionTracker[startTracker] == actionType) {
      // looked = 1;
      startTracker -= 1;
    }
    int offsetTracker = offsets[i] - 1;
    
    // scoreMultiplier decrements each time to a fraction of lookAmount. This makes the
    // influence of patterns deteriorate over time
    float scoreMultiplier = 1;
    while(offsetTracker != idx && looked < lookAmount) {
      if (i != offsets.size() && offsetTracker == offsets[i + 1]) break;
      if (startTracker < 0) startTracker = ACTION_COUNT - 1;
      if (offsetTracker < 0) offsetTracker = ACTION_COUNT - 1;
      // pointless to continue because MOV_NONE effectively means uninitalized
      if (actionTracker[offsetTracker] == MOV_NONE) break;

      // OSReport("i: %d; mov @ idx: %d; mov @ tracker: %d\n", i, actionTracker[startTracker], actionTracker[offsetTracker]);

      // a value (toAdd) is calculated based on the weight and the scoremultiplier
      float toAdd = ((float) weights[actionTracker[offsetTracker]] / 100);
      // skips over if inconsequential
      // if (toAdd * timeTracker[offsetTracker] < 30 && actionTracker[startTracker] != actionTracker[offsetTracker]) {
      //   offsetTracker++;
      //   continue;
      // }
      toAdd *= scoreMultiplier;

      // ...the difference in time is then calculated, and the absolute value is taken
      float timeDifference = ((looked == 0) ? reactionPatchTime : timeTracker[startTracker]) - timeTracker[offsetTracker];
      if (timeDifference < 0) timeDifference *= -1;
      // finally, toAdd is MULTIPLIED by 80 MINUS the time difference times the scoreMultiplier (again)
      // This effecitvely means "take this into account if the time difference is less than 80 frames",
      // and also says "the less the difference in time, the greater the score increase"
      toAdd *= (200 - timeDifference) * scoreMultiplier;
      // this here effectively limits the influence of a time difference greater than 80, which would
      // be negative
      if (toAdd < 0) toAdd = 0;

      // if the action is the same, then add it to the score
      if (actionTracker[startTracker] == actionTracker[offsetTracker]) {
        score += toAdd;
      }
      // no matter what, we'll add the score to the tracked total
      totalScore += toAdd;
      
      // this just sets the score multiplier to a value less than it was before, spaced evenly down to 0
      scoreMultiplier = (float) (lookAmount - looked) / (float) lookAmount;
      startTracker -= 1;
      offsetTracker -= 1;
      looked += 1;
    }
  }
  // we don't want to divide by 0 - that would break the universe and we don't want that
  // - so we have this check to preven that
  if (totalScore == 0) return 0;
  // finally, we divide the score by the total score to get our prediction!
  actionCache[actionType] = score / totalScore;
  return score / totalScore;
};

float MovementTracker::approxChance(float CPULevel) {
  return approxChance(CPULevel, MOV_ATTACK);
}