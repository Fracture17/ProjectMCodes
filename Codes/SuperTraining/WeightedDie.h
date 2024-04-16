#ifndef PROJECTMCODES_WEIGHTEDDIE_H
#define PROJECTMCODES_WEIGHTEDDIE_H

#include "Containers/vector.h"

struct WeightedValue {
  WeightedValue() {}
  WeightedValue(float value, float weight) : value(value), weight(weight) {};
  float value = 0;
  float weight = 0;
};

constexpr const int weightsSize = 0x30;
class WeightedDie {

  WeightedValue weights[weightsSize];
  char weightsLength = 0;

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