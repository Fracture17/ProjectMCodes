
#include "WeightedDie.h"

void WeightedDie::addValue(WeightedValue weight) {
  // OSReport("AddValue (currSize: %d) (weight: %d, %.3f)\n", weights.size(), weight.value, weight.weight);
  if (weight.weight <= 0) {
    return;
  }
  for (int i = 0; i < weights.size(); i++) {
    if (weights[i]->value == weight.value) {
      // OSReport("found value\n");
      weights[i]->weight += weight.weight;
      if (weights[i]->weight < 0) weights[i]->weight = 0;
      return;
    } 
  }
  // OSReport("adding new\n");
  weights.push(new WeightedValue(weight.value, weight.weight));
}

void WeightedDie::clear() {
  weights.clear();
}

float WeightedDie::roll() {
  float value = _randf() * getWeight();
  for (int i = 0; i < weights.size(); i++) {
    value -= weights[i]->weight;
    if (value < 0) {
      return weights[i]->value;
    }
  }
  return weights[weights.size() - 1]->value;
}

float WeightedDie::rollAndRemove() {
  float value = _randf() * getWeight();
  for (int i = 0; i < weights.size(); i++) {
    value -= weights[i]->weight;
    if (value < 0) {
      float out = weights[i]->value;
      weights.erase(i);
      return out;
    }
  }
  float out = weights[weights.size() - 1]->value;
  weights.erase(weights.size() - 1);
  return out;
}

float WeightedDie::getWeight() {
  float out = 0;
  for (int i = 0; i < weights.size(); i++) {
    out += weights[i]->weight;
  }
  return out;
} 

float WeightedDie::getSize() {
  return weights.size();
}