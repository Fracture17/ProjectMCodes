
#include "WeightedDie.h"

void WeightedDie::addValue(WeightedValue weight) {
  // OSReport("AddValue (currSize: %d) (weight: %d, %.3f)\n", weights.size(), weight.value, weight.weight);
  if (weightsLength + 1 > weightsSize) {
    OSReport("WEIGHT ALLOCATION FAILED");
    return;
  }
  if (weight.weight <= 0) {
    return;
  }
  for (int i = 0; i < weightsLength; i++) {
    if (weights[i].value == weight.value) {
      // OSReport("found value\n");
      weights[i].weight += weight.weight;
      if (weights[i].weight < 0) weights[i].weight = 0;
      return;
    } 
  }
  // OSReport("adding new\n");
  weights[weightsLength] = weight;
  weightsLength += 1;
}

void WeightedDie::clear() {
  weightsLength = 0;
}

float WeightedDie::roll() {
  float value = _randf() * getWeight();
  for (int i = 0; i < weightsLength; i++) {
    value -= weights[i].weight;
    if (value < 0) {
      return weights[i].value;
    }
  }
  return weights[weightsLength - 1].value;
}

float WeightedDie::rollAndRemove() {
  float value = _randf() * getWeight();
  for (int i = 0; i < weightsLength; i++) {
    value -= weights[i].weight;
    if (value < 0) {
      float out = weights[i].value;
      memmove(&weights[i], &weights[i + 1], sizeof(WeightedValue) * (weightsLength - i));
      weightsLength -= 1;
      return out;
    }
  }
  float out = weights[weightsLength - 1].value;
  weightsLength -= 1;
  return out;
}

float WeightedDie::getWeight() {
  float out = 0;
  for (int i = 0; i < weightsLength; i++) {
    out += weights[i].weight;
  }
  return out;
} 

float WeightedDie::getSize() {
  return weightsLength;
}