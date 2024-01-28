#ifndef PROJECTMCODES_WEIGHTEDDIE_H
#define PROJECTMCODES_WEIGHTEDDIE_H

#include "Containers/vector.h"

struct WeightedValue {
  WeightedValue(float value, float weight) : value(value), weight(weight) {};
  float value;
  float weight;
};

class WeightedDie {

  vector<WeightedValue*> weights = vector<WeightedValue*>();

public:
  WeightedDie() {};

  void addValue(WeightedValue value);
  void clear();
  float getSize();

  float roll();
  float rollAndRemove();
  float getWeight();
};


#endif // PROJECTMCODES_WEIGHTEDDIE_H