
// #include "Brawl/IT/itResourceModuleImpl.h"
#include "Brawl/IT/BaseItem.h"
#include "Brawl/IT/itManager.h"
#include "Brawl/FT/ftManager.h"
#include "./TrainingDataInit.h"
#include "./MovementTracker.h"

extern MovementTracker movementTrackers[4]; 

TrainingData playerTrainingData[] = {
  TrainingData(),
  TrainingData(),
  TrainingData(),
  TrainingData()
};