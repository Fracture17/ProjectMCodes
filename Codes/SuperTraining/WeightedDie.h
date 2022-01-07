#ifndef PROJECTMCODES_WEIGHTEDDIE_H
#define PROJECTMCODES_WEIGHTEDDIE_H

#include "Containers/vector.h"

struct WeightedValue {
  WeightedValue(int value, float weight) : value(value), weight(weight) {};
  int value;
  float weight;
};

class WeightedDie {

  vector<WeightedValue*> weights = vector<WeightedValue*>();

public:
  WeightedDie() {};

  void addValue(WeightedValue value);
  void clear();

  int roll();
  int rollAndRemove();
  float getWeight();
};


#endif // PROJECTMCODES_WEIGHTEDDIE_H