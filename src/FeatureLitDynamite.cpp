#include "FeatureLitDynamite.h"

#include "Engine.h"
#include "Explosion.h"
#include "Map.h"
#include "Fov.h"

void LitDynamite::newTurn() {
  turnsLeftToExplosion_--;
  if(turnsLeftToExplosion_ <= 0) {
    eng.explosionMaker->runExplosion(pos_);
    eng.gameTime->eraseFeatureMob(this, true);
  }
}

void LitFlare::newTurn() {
  life_--;
  if(life_ <= 0) {
    eng.gameTime->eraseFeatureMob(this, true);
  }
}

LitFlare::LitFlare(Feature_t id, Pos pos, Engine& engine,
                   DynamiteSpawnData* spawnData) :
  FeatureMob(id, pos, engine), life_(spawnData->turnsLeftToExplosion_) {
}

void LitFlare::addLight(bool light[MAP_W][MAP_H]) const {
  bool myLight[MAP_W][MAP_H];
  eng.basicUtils->resetArray(myLight, false);
  const int RADI = FOV_STANDARD_RADI_INT; //getLightRadius();
  Pos x0y0(max(0, pos_.x - RADI), max(0, pos_.y - RADI));
  Pos x1y1(min(MAP_W - 1, pos_.x + RADI),
           min(MAP_H - 1, pos_.y + RADI));
  bool visionBlockers[MAP_W][MAP_H];
  for(int y = x0y0.y; y <= x1y1.y; y++) {
    for(int x = x0y0.x; x <= x1y1.x; x++) {
      visionBlockers[x][y] =
        eng.map->cells[x][y].featureStatic->isVisionPassable() == false;
    }
  }

  eng.fov->runFovOnArray(visionBlockers, pos_, myLight, false);
  for(int y = x0y0.y; y <= x1y1.y; y++) {
    for(int x = x0y0.x; x <= x1y1.x; x++) {
      if(myLight[x][y]) {
        light[x][y] = true;
      }
    }
  }
}



