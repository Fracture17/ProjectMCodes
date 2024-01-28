#ifndef PROJECTMCODES_PROPERTYVECTOR_H
#define PROJECTMCODES_PROPERTYVECTOR_H

#include "stddef.h"

struct PropertyVectorVTable {
  char** objDescriptor;
  int* thisPtrFixup;
  // soNullable
  void** fn__isNull;
  // soInstanceManagerFullPropertyVector<P15soKineticEnergy,12>
  void** fn__at;
  // soInstanceManagerFullPropertyVector<P15soKineticEnergy,12>
  void** fn__atIndex;
  // soInstanceManagerFullPropertyVector<P15soKineticEnergy,12>
  void** fn__getId;
  // soInstanceManagerFullPropertyVector<P15soKineticEnergy,12>
  void** fn__size;
  // soInstanceManagerFixed<P15soKineticEnergy>
  void** fn__isEmpty;
  // soInstanceManagerFullPropertyVector<P15soKineticEnergy,12>
  void** fn__isContain;
  // soInstanceManagerFullProperty<P15soKineticEnergy>
  void** fn__add;
  // soInstanceManagerFullPropertyVector<P15soKineticEnergy,12>
  void** fn__erase;
  // soInstanceManagerFullPropertyVector<P15soKineticEnergy,12>
  void** fn__clear;
  // soInstanceManagerFullPropertyVector<P15soKineticEnergy,12>
  void** fn__set;
};

template<class T>
struct PropertyVector {
  VTABLE_METHOD(PropertyVector*, void, isNull)
  VTABLE_METHOD_1ARG(PropertyVector*, T, at, int, index)
  VTABLE_METHOD(PropertyVector*, void, atIndex)
  VTABLE_METHOD(PropertyVector*, void, getId)
  VTABLE_METHOD(PropertyVector*, int, size)
  VTABLE_METHOD(PropertyVector*, bool, isEmpty)
  VTABLE_METHOD(PropertyVector*, void, isContain)
  VTABLE_METHOD(PropertyVector*, void, add)
  VTABLE_METHOD(PropertyVector*, void, erase)
  VTABLE_METHOD(PropertyVector*, void, clear)
  VTABLE_METHOD(PropertyVector*, void, set)

  PropertyVectorVTable* vtable;
};

#endif