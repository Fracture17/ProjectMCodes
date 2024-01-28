#include "MovementTracker.h"
#include "Containers/vector.h"

void MovementTracker::reset() {
  for (int i = 0; i < MOV_LEN; i++) {
    weightCache[i] = MOV_CACHE_RESET;
    chanceCache[i] = MOV_CACHE_RESET;
  }
  for (int i = 0; i < ACTION_COUNT; i++) {
    actionTracker[i] = 0;
    timeTracker[i] = 0;
    yDistFloorTracker[i] = 0;
    distanceTracker[i] = 0;
    attackChanceTracker[i] = 0;
    shieldChanceTracker[i] = 0;
    xPosTracker[i] = 0;
    movTracker[i] = -1;
  }
  idx = 0;
  weightCacheTotal = 0;
};

void MovementTracker::undoLastAction() {
  idx -= 1;
  if (idx < 0) idx = ACTION_COUNT - 1;
}

s16 MovementTracker::getCurrentMov() {
  return actionTracker[idx];
}

unsigned char actionToMov(int action) {
  if ((0x24 <= action && action <= 0x33) || action >= 0x112) {
    return MOV_ATTACK; // (action > 0x112) ? MOV_ATTACK : action;
  } else if (action >= 0x3D && action <= 0x4C) {
    return MOV_HITSTUN;
  } else if (action >= 0x4E && action <= 0x52) {
    return MOV_ROLL;
  } else if (action == 0x53) {
    return MOV_ATTACK;
  } else if (action >= 0x46 && action <= 0x55) {
    return MOV_MISSEDTECH;
  }
  switch (action) {
    case 0x0:
    // technically 0x1 is "walk" but oh well
    case 0x1: return MOV_IDLE;
    case 0x3: return MOV_DASH;
    case 0x4: return MOV_RUN;
    // case 0x7: return MOV_DASHTURN;
    // fall disabled
    case 0x10: return MOV_IDLE;
    case 0x11: return MOV_CROUCH;
    case 0xB: return MOV_JUMP;
    case 0xC:
    case 0xD: return MOV_DJUMP;
    case 0xE:
    case 0xF: return MOV_FALL;
    // landings
    
    case 0x16:
    case 0x17:
    case 0x18:
    case 0x19: return MOV_IDLE;
    // 
    case 0x1A: 
    case 0x1B: 
    case 0x1C: return MOV_SHIELD;
    case 0x21: return MOV_AIRDODGE;
    case 0x1E:
    case 0x1F:
    case 0x20: return MOV_ROLL;
    case 0x60: 
    case 0x61: return MOV_ROLL;
    case 0x34:
    case 0x36:
    case 0x38: return MOV_GRAB;
    case 0x75: return MOV_IDLE;
    case 0x79: return MOV_JUMP; 
    case 0x53:
    case 0x76:
    case 0x7F: return MOV_ATTACK;
    case 0x77:
    case 0x78: return MOV_ROLL;
    default:
      return MOV_NONE;
  }
};

// called each frame
void MovementTracker::incrementTimer() {
  for (int i = 0; i < MOV_LEN; i++) {
    weightCache[i] = MOV_CACHE_RESET;
    chanceCache[i] = MOV_CACHE_RESET;
  }
  weightCacheTotal = 0;
  timeTracker[idx] += 1;
  if (timeTracker[idx] >= 254) {
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

float getMovDiffMultiplier(int source, int comparison) {
  switch(source) {
    // mostly idle
    case MOV_NONE:
    case MOV_IDLE:
    case MOV_MISSEDTECH:
    case MOV_RUN:
    case MOV_CROUCH:
      switch (comparison) {
        case MOV_NONE:
        case MOV_IDLE:
        case MOV_MISSEDTECH:
        case MOV_RUN:
        case MOV_CROUCH:
          return 1;
        case MOV_DASH:
          return 0.5;
        default:
          return 0;
      }
    case MOV_DASH:
    case MOV_HITSTUN:
      switch (comparison) {
        case MOV_HITSTUN:
          return 1;
        default:
          return 0;
      }
    case MOV_JUMP:
      switch (comparison) {
        case MOV_DJUMP:
          return 0.5;
      }
    case MOV_DJUMP:
    case MOV_FALL:
      switch (comparison) {
        case MOV_JUMP:
        case MOV_DJUMP:
        case MOV_FALL:
          return 1;
      }
    case MOV_SHIELD:
    case MOV_AIRDODGE:
      break;
    case MOV_ROLL:
    case MOV_TECH:
      switch (comparison) {
        case MOV_ROLL:
        case MOV_TECH:
          return 1;
        case MOV_SHIELD:
          return 0.5;
      }
    case MOV_ATTACK:
    case MOV_GRAB:
      break;
  }
  return (float)(source == comparison);
}

void MovementTracker::trackAction(int actionOrSubaction, bool isAction) {
  int MOV = actionToMov(actionOrSubaction);
  if (MOV != MOV_NONE) {
    idx += 1;
    if (idx >= ACTION_COUNT) idx = 0;
    actionTracker[idx] = MOV;
    timeTracker[idx] = 0;
  }
};

void MovementTracker::updateData(u8 yDistFloor, u8 xPos, u8 distance, u8 attackChance, u8 shieldChance, s16 otherMov) {
  yDistFloorTracker[idx] = yDistFloor;
  distanceTracker[idx] = distance;
  attackChanceTracker[idx] = attackChance;
  shieldChanceTracker[idx] = shieldChance;
  xPosTracker[idx] = xPos;
  movTracker[idx] = otherMov;

  for (int i = 0; i < MOV_LEN; i++) {
    calcWeight(i);
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
void MovementTracker::calcWeight(char actionType) {
  // score is now a float to account for the varying weights
  float score = 0;
  float totalScore = 0;
  // this is how the AI will be limited based on its level. 
  // higher levels have more access to the wider array, and as such can take
  // more information into account
  int lookAmount = ACTION_COUNT;
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
  // int reactionTime = (1 - (CPULevel / 100)) * 45 + 12 + 7 * _randf();
  // if (CPULevel == 101) reactionTime = 0;
  int reactionTime = 0;

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
      // if (i + 1 != offsets.size() && offsetTracker == offsets[i + 1]) break;
      if (startTracker < 0) startTracker = ACTION_COUNT - 1;
      if (offsetTracker < 0) offsetTracker = ACTION_COUNT - 1;
      // pointless to continue because MOV_NONE effectively means uninitalized
      if (actionTracker[offsetTracker] == MOV_NONE) break;

      // OSReport("i: %d; mov @ idx: %d; mov @ tracker: %d\n", i, actionTracker[startTracker], actionTracker[offsetTracker]);

      // a value (toAdd) is calculated based on the weight and the scoremultiplier
      float toAdd = ((float) (200 - weights[actionTracker[offsetTracker]]) / 100);

      toAdd *= scoreMultiplier;
      totalScore += toAdd;

      // if the action is the same, then add it to the score
      float diffMultiplier = getMovDiffMultiplier(actionTracker[startTracker], actionTracker[offsetTracker]);
      if (diffMultiplier > 0) {
        s16 timeDiff = ((looked == 0) ? reactionPatchTime : timeTracker[startTracker]) - timeTracker[offsetTracker];
        s16 yDistFloorDiff = yDistFloorTracker[startTracker] - yDistFloorTracker[offsetTracker];
        s16 xDistDiff = distanceTracker[startTracker] - distanceTracker[offsetTracker];
        s16 attackChanceDiff = attackChanceTracker[startTracker] - attackChanceTracker[offsetTracker];
        s16 shieldChanceDiff = shieldChanceTracker[startTracker] - shieldChanceTracker[offsetTracker];
        s16 xPosDiff = xPosTracker[startTracker] - xPosTracker[offsetTracker];
        bool movDiff = movTracker[startTracker] != movTracker[offsetTracker];
        // branchless programming go brr :3
        timeDiff *= 1 - ((timeDiff < 0)*2);
        yDistFloorDiff *= 1 - ((yDistFloorDiff < 0)*2);
        xDistDiff *= 1 - ((xDistDiff < 0)*2);
        attackChanceDiff *= 1 - ((attackChanceDiff < 0)*2);
        shieldChanceDiff *= 1 - ((shieldChanceDiff < 0)*2);
        // if (actionType == MOV_ATTACK) OSReport("PRE %d: xPosDiff: %d\n", i, xPosDiff);
        xPosDiff *= 1 - ((xPosDiff < 0)*2);
        // if (actionType == MOV_ATTACK) OSReport("POST %d: xPosDiff: %d\n", i, xPosDiff);
        movDiff *= 1 - ((movDiff < 0)*2);
      
        using m = MarkovInputs;
        constexpr float totalWeight = m::time.weight + m::xDist.weight + m::yDistFloor.weight + m::attackChance.weight + m::shieldChance.weight + m::xPos.weight + m::mov.weight;
        #define BIAS(name) m::name.weight * (float)(m::name.tolerence - name ## Diff) / m::name.tolerence
        toAdd *= (
            BIAS(time)
          + BIAS(yDistFloor)
          + BIAS(xDist)
          + BIAS(attackChance)
          + BIAS(shieldChance)
          + BIAS(xPos)
          + BIAS(mov)
        ) / totalWeight;
        #undef BIAS
        if (toAdd < 0) toAdd = 0;
        score += toAdd * diffMultiplier;
      }

      // this just sets the score multiplier to a value less than it was before, spaced evenly down to 0
      scoreMultiplier = (float) (lookAmount - looked) / (float) lookAmount;
      startTracker -= 1;
      offsetTracker -= 1;
      looked += 1;
    }
  }
  
  float result = (totalScore == 0) ? 0 : score / totalScore;
  
  // finally, we divide the score by the total score to get our prediction!
  weightCache[actionType] = result;
  weightCacheTotal += result;
}


float MovementTracker::approxChance(int CPULevel, char actionType) {
  float chance;
  if (chanceCache[actionType] != MOV_CACHE_RESET) {
    chance = chanceCache[actionType];
  } else {
    float actionWeight = weightCache[actionType];
    chance = actionWeight / (weightCacheTotal + (weightCacheTotal == 0));
    chanceCache[actionType] = chance;
  }
  // CPULevel Fuzz
  if (CPULevel <= 100) {
    float chanceFuzz = ((95 - CPULevel) * 0.01) * chance * _randf();
    chanceFuzz *= 0.35;
    chance -= chanceFuzz;
  } 
  return chance;
};

float MovementTracker::approxCommit(int CPULevel) {
  float actionWeight = weightCache[MOV_ATTACK] + weightCache[MOV_GRAB];
  float chance = actionWeight / (weightCacheTotal + (weightCacheTotal == 0));
  // CPULevel Fuzz
  if (CPULevel <= 100) {
    float chanceFuzz = ((95 - CPULevel) * 0.01) * chance * _randf();
    chanceFuzz * 0.35;
    chance -= chanceFuzz;
    // I am very lazy :)
    // chance *= 1.5;
  } 
  return chance;
};

/**
 * CURRENT_PROCESS:
 * total = 0
 * score = 0
 * 
 * for each segment:
 *   for each action:
 *     max = action weight
 *     currDiff = diff of now + then
 *     curr *= max * currDiff
 *     if action then == now:
 *       score += curr
 *     total += max
 * 
 * out = score / total
 * 
 * | GOAL |
 * combine each segment somehow, then evalueate curr against that 
 * 
 * FASTER_PLAN:
 * total = 0
 * score = 0
 * 
 * for each segment:
 *   for each action:
 *     max = action weight
 *     if action then == now:
 *       currDiff = diff of now + then
 *       curr *= max * currDiff
 *       score += curr
 *     total += max
 * 
 * out = score / total
*/