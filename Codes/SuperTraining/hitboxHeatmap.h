
#ifndef PROJECTMCODES_HITBOXHEATMAP_H
#define PROJECTMCODES_HITBOXHEATMAP_H

#include <Wii/GX/GX.h>
#include "Containers/vector.h"

struct HitboxData {
  HitboxData(double radius, float scaleMatrix[3][4], float pos1[3], float pos2[3], unsigned int colour1, unsigned int colour2, float viewingMatrix[3][4]) {
    this->radius = radius;
    for (int i = 0; i < 3; i++) {
      this->pos1[i] = pos1[i];
      this->pos2[i] = pos2[i];
    }
    OSReport("col1: %08x; col2: %08x\n", colour1, colour2);
    this->colour1 = GXColor(colour1);
    this->colour2 = GXColor(colour2);
    for (int i = 0; i < 3; i++) {
      for (int j = 0; j < 4; j++) {
        this->scaleMatrix[i][j] = scaleMatrix[i][j];
      }
    }
  }
  double radius;
  float scaleMatrix[3][4];
  float pos1[3];
  float pos2[3];
  GXColor colour1;
  GXColor colour2;
};

struct HitboxDataFrame {
  vector<HitboxData*> hitboxes;
  int frame;
  int life;
};

void renderAllStoredHitboxes();
void storedHitboxTick();
// void storeRenderingData();
// void storePlayerData();

#endif // PROJECTMCODES_HITBOXHEATMAP_H