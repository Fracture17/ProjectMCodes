#ifndef PROJECTMCODES_ARRAYVECTOR_H
#define PROJECTMCODES_ARRAYVECTOR_H

struct ArrayVecVTable {
  char _spacer[0x8];
  void** isNullFn;
  void** atFn1;
  void** atFn2;
  void** sizeFn;
  void** isEmptyFn;
  void** __dtFn;
  void** shiftFn;
  void** popFn;
  void** clearFn;
  void** unshiftFn;
  void** pushFn;
  void** insertFn;
  void** eraseFn;
  void** capacityFn;
  void** isFullFn;
  void** setFn;
};

struct ArrayVecVTable2 {
  char _spacer[0x8];
    
};

template<class T>
struct ArrayVector {
  bool isNull();
  T* at(int index);
  // T* at();
  int size();
  bool isEmpty();
  ~ArrayVector();
  T* shift();
  T* pop();
  void clear();
  void unshift(T* element);
  void push(T* element);
  void insert(T* element, int index);
  void erase(int index);
  int capacity();
  bool isFull();
  // these parameters are just a guess
  void set(int index, T* element);

  ArrayVecVTable* vtable1;
  ArrayVecVTable2* vtable2;
  int maxLen;
};

template<class T>
bool ArrayVector<T>::isNull() {
  return ((bool (*)(void* self)) this->vtable1->isNullFn)(this);
}

template<class T>
T* ArrayVector<T>::at(int index) {
  return ((T* (*)(void* self, int index)) this->vtable1->atFn2)(this, index);
}

// template<class T>
// T* ArrayVector<T>::at() {
//   return ((T* (*)(void* self, int index)) this->vtable1->isNullFn)(this);
// }

template<class T>
int ArrayVector<T>::size() {
  return ((int (*)(void* self)) this->vtable1->sizeFn)(this);
}

template<class T>
bool ArrayVector<T>::isEmpty() {
  return ((bool (*)(void* self)) this->vtable1->isEmptyFn)(this);
}

template<class T>
ArrayVector<T>::~ArrayVector() {
  ((void (*)(void* self)) this->vtable1->__dtFn)(this);
}

template<class T>
T* ArrayVector<T>::shift() {
  return ((T* (*)(void* self)) this->vtable1->shiftFn)(this);
}

template<class T>
T* ArrayVector<T>::pop() {
  return ((T* (*)(void* self)) this->vtable1->popFn)(this);
}

template<class T>
void ArrayVector<T>::clear() {
  return ((T* (*)(void* self)) this->vtable1->clearFn)(this);
}

template<class T>
void ArrayVector<T>::unshift(T* element) {
  return ((void (*)(void* self)) this->vtable1->unshiftFn)(this, element);
}

template<class T>
void ArrayVector<T>::push(T* element) {
  return ((void (*)(void* self)) this->vtable1->pushFn)(this, element);
}

template<class T>
void ArrayVector<T>::insert(T* element, int index) {
  return ((void (*)(void* self)) this->vtable1->insertFn)(this, element, index);
}

template<class T>
void ArrayVector<T>::erase(int index) {
  return ((void (*)(void* self)) this->vtable1->eraseFn)(this, index);
}

template<class T>
int ArrayVector<T>::capacity() {
  return ((int (*)(void* self)) this->vtable1->capacityFn)(this);
}

template<class T>
void ArrayVector<T>::set(int index, T* element) {
  return ((void (*)(void* self)) this->vtable1->setFn)(this, index, element);
}

#endif