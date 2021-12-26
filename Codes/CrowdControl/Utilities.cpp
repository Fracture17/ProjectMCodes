//
// Created by Ilir on 2021-12-25.
//

#include "Utilities.h"

//Does not update random seed, so multiple uses at once return same value
int randi(int max) {
    return DEFAULT_MT_RAND->seed % max;
}

