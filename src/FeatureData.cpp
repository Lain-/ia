#include "FeatureData.h"

#include <cassert>

#include "Engine.h"
#include "Colors.h"

void FeatureData::resetDef(FeatureDef& d) {
  d.id = feature_empty;
  d.spawnType = featureSpawnType_static;
  d.glyph = ' ';
  d.tile = tile_empty;
  d.color = clrYellow;
  d.colorBg = clrBlack;
  for(unsigned int i = 0; i < endOfMoveType; i++) {
    d.isMovePassable[i] = true;
  }
  d.isShootPassable = true;
  d.isVisionPassable = true;
  d.isSmokePassable = true;
  d.canHaveBlood = true;
  d.canHaveGore = true;
  d.canHaveCorpse = true;
  d.canHaveStaticFeature = true;
  d.canHaveItem = true;
  d.isBottomless = false;
  d.materialType = materialType_hard;
  d.name_a = "";
  d.name_the = "";
  d.messageOnPlayerBlocked = "The way is blocked.";
  d.messageOnPlayerBlockedBlind = "I bump into something.";
  d.dodgeModifier = 0;
  d.shockWhenAdjacent = 0;
  d.themedFeatureSpawnRules.reset();
  d.featuresOnDestroyed.resize(0);
}

void FeatureData::addToListAndReset(FeatureDef& d) {
  featureDefs[d.id] = d;
  resetDef(d);
}

void FeatureData::makeList() {
  FeatureDef d;
  resetDef(d);
  addToListAndReset(d);

  /*---------------------------------------------*/
  d.id = feature_stoneFloor;
  d.name_a = "stone floor";
  d.name_the = "the stone floor";
  d.glyph = '.';
  d.color = clrGray;
  d.tile = tile_floor;
  d.isShootPassable = true;
  d.isVisionPassable = true;
  d.canHaveBlood = true;
  d.canHaveGore = true;
  d.canHaveCorpse = true;
  d.canHaveStaticFeature = true;
  d.canHaveItem = true;
  addToListAndReset(d);
  /*---------------------------------------------*/
  d.id = feature_stoneWall;
  d.spawnType = featureSpawnType_other;
  d.name_a = "a stone wall";
  d.name_the = "the stone wall";
  d.glyph = eng->config->WALL_SYMBOL_FULL_SQUARE == false ? '#' : 10;
  d.color = clrGray;
  d.tile = tile_wallTop;
  d.isMovePassable[moveType_walk] = false;
  d.isMovePassable[moveType_ethereal] = true;
  d.isMovePassable[moveType_fly] = false;
  d.isMovePassable[moveType_ooze] = false;
  d.isShootPassable = false;
  d.isVisionPassable = false;
  d.isSmokePassable = false;
  d.canHaveBlood = true;
  d.canHaveGore = false;
  d.canHaveCorpse = false;
  d.canHaveStaticFeature = false;
  d.canHaveItem = false;
  d.featuresOnDestroyed.push_back(feature_rubbleHigh);
  d.featuresOnDestroyed.push_back(feature_rubbleLow);
  d.featuresOnDestroyed.push_back(feature_stoneFloor);
  addToListAndReset(d);
  assert(featureDefs[feature_stoneWall].id == feature_stoneWall);
  /*---------------------------------------------*/
//  d.id = feature_stoneWallSlimy;
//  d.name_a = "a slimy stone wall";
//  d.name_the = "the slimy stone wall";
//  d.glyph = eng->config->WALL_SYMBOL_FULL_SQUARE == false ? '#' : 10;
//  d.color = clrGreen;
//  d.tile = tile_wall;
//  d.isMovePassable[moveType_walk] = false;
//  d.isMovePassable[moveType_ethereal] = true;
//  d.isMovePassable[moveType_fly] = false;
//  d.isMovePassable[moveType_ooze] = false;
//  d.isShootPassable = false;
//  d.isVisionPassable = false;
//  d.isSmokePassable = false;
//  d.canHaveBlood = true;
//  d.canHaveGore = false;
//  d.canHaveCorpse = false;
//  d.canHaveStaticFeature = false;
//  d.canHaveItem = false;
//  d.featuresOnDestroyed.push_back(feature_rubbleHigh);
//  d.featuresOnDestroyed.push_back(feature_rubbleLow);
//  d.featuresOnDestroyed.push_back(feature_stoneFloor);
//  addToListAndReset(d);
  /*---------------------------------------------*/
  d.id = feature_tree;
  d.name_a = "a tree";
  d.name_the = "the tree";
  d.glyph = '|';
  d.color = clrBrownDark;
  d.tile = tile_tree;
  d.isMovePassable[moveType_walk] = false;
  d.isMovePassable[moveType_ethereal] = true;
  d.isMovePassable[moveType_fly] = true;
  d.isMovePassable[moveType_ooze] = false;
  d.isShootPassable = false;
  d.isVisionPassable = false;
  d.canHaveBlood = false;
  d.canHaveGore = false;
  d.canHaveCorpse = false;
  d.canHaveStaticFeature = false;
  d.canHaveItem = false;
  d.shockWhenAdjacent = 1;
  d.messageOnPlayerBlocked = "There is a tree in the way.";
  addToListAndReset(d);
  /*---------------------------------------------*/
  d.id = feature_grass;
  d.name_a = "grass";
  d.name_the = "the grass";
  d.glyph = '.';
  d.tile = tile_floor;
  d.color = clrGreen;
  d.isShootPassable = true;
  d.isVisionPassable = true;
  d.canHaveBlood = true;
  d.canHaveGore = true;
  d.canHaveCorpse = true;
  d.canHaveStaticFeature = true;
  d.canHaveItem = true;
  d.materialType = materialType_soft;
  addToListAndReset(d);
  /*---------------------------------------------*/
  d.id = feature_grassWithered;
  d.name_a = "withered grass";
  d.name_the = "the withered grass";
  d.glyph = '.';
  d.tile = tile_floor;
  d.color = clrBrownDark;
  d.isShootPassable = true;
  d.isVisionPassable = true;
  d.canHaveBlood = true;
  d.canHaveGore = true;
  d.canHaveCorpse = true;
  d.canHaveStaticFeature = true;
  d.canHaveItem = true;
  d.materialType = materialType_soft;
  addToListAndReset(d);
  /*---------------------------------------------*/
  d.id = feature_bush;
  d.name_a = "a shrub";
  d.name_the = "the shrub";
  d.glyph = '"';
  d.color = clrGreen;
  d.tile = tile_bush;
  d.isShootPassable = true;
  d.isVisionPassable = true;
  d.canHaveBlood = true;
  d.canHaveGore = true;
  d.canHaveCorpse = true;
  d.canHaveStaticFeature = true;
  d.canHaveItem = true;
  d.materialType = materialType_soft;
  addToListAndReset(d);
  /*---------------------------------------------*/
  d.id = feature_bushWithered;
  d.name_a = "a withered shrub";
  d.name_the = "the withered shrub";
  d.glyph = '"';
  d.color = clrBrownDark;
  d.tile = tile_bush;
  d.isShootPassable = true;
  d.isVisionPassable = true;
  d.canHaveBlood = true;
  d.canHaveGore = true;
  d.canHaveCorpse = true;
  d.canHaveStaticFeature = true;
  d.canHaveItem = true;
  d.materialType = materialType_soft;
  addToListAndReset(d);
  /*---------------------------------------------*/
  d.id = feature_forestPath;
  d.name_a = "a stony path";
  d.name_the = "the stony path";
  d.glyph = '.';
  d.tile = tile_floor;
  d.color = clrGray;
  d.isShootPassable = true;
  d.isVisionPassable = true;
  d.canHaveBlood = true;
  d.canHaveGore = true;
  d.canHaveCorpse = true;
  d.canHaveStaticFeature = false;
  d.canHaveItem = true;
  addToListAndReset(d);
  /*---------------------------------------------*/
  d.id = feature_stairsDown;
  d.name_a = "a downward staircase";
  d.name_the = "the downward staircase";
  d.glyph = '>';
  d.color = clrWhiteHigh;
  d.tile = tile_stairsDown;
  d.isShootPassable = true;
  d.isVisionPassable = true;
  d.canHaveBlood = false;
  d.canHaveGore = false;
  d.canHaveCorpse = false;
  d.canHaveStaticFeature = false;
  d.canHaveItem = false;
  addToListAndReset(d);
  /*---------------------------------------------*/
  d.id = feature_lever;
  d.spawnType = featureSpawnType_other;
  d.name_a = "a lever";
  d.name_the = "the lever";
  d.glyph = '%';
  d.color = clrWhite;
  d.tile = tile_leverLeft;
  d.isShootPassable = true;
  d.isVisionPassable = true;
  d.canHaveBlood = false;
  d.canHaveGore = false;
  d.canHaveCorpse = false;
  d.canHaveStaticFeature = false;
  d.canHaveItem = false;
  addToListAndReset(d);
  /*---------------------------------------------*/
  d.id = feature_brazierGolden;
  d.name_a = "a golden brazier";
  d.name_the = "the golden brazier";
  d.glyph = '0';
  d.color = clrYellow;
  d.tile = tile_brazier;
  d.isShootPassable = true;
  d.isVisionPassable = true;
  d.canHaveBlood = false;
  d.canHaveGore = false;
  d.canHaveCorpse = false;
  d.canHaveStaticFeature = false;
  d.canHaveItem = false;
  d.themedFeatureSpawnRules.set(3, placementRule_nextToWallsOrAwayFromWalls, roomTheme_ritual);
  addToListAndReset(d);
  /*---------------------------------------------*/
  d.id = feature_shallowWater;
  d.spawnType = featureSpawnType_other;
  d.name_a = "shallow water";
  d.name_the = "the shallow water";
  d.glyph = '~';
  d.color = clrBlueLight;
  d.tile = tile_water1;
  d.isShootPassable = true;
  d.isVisionPassable = true;
  d.canHaveBlood = false;
  d.canHaveGore = false;
  d.canHaveCorpse = true;
  d.canHaveStaticFeature = false;
  d.canHaveItem = true;
  d.dodgeModifier = -10;
  d.materialType = materialType_fluid;
  addToListAndReset(d);
  /*---------------------------------------------*/
  d.id = feature_deepWater;
  d.spawnType = featureSpawnType_other;
  d.name_a = "deep water";
  d.name_the = "the deep water";
  d.glyph = '~';
  d.color = clrBlue;
  d.tile = tile_water1;
  d.isMovePassable[moveType_walk] = false;
  d.isMovePassable[moveType_fly] = true;
  d.isMovePassable[moveType_ooze] = true;
  d.isMovePassable[moveType_ethereal] = true;
  d.isShootPassable = true;
  d.isVisionPassable = true;
  d.canHaveBlood = false;
  d.canHaveGore = false;
  d.canHaveCorpse = true;
  d.canHaveStaticFeature = false;
  d.canHaveItem = true;
  d.dodgeModifier = -10;
  d.shockWhenAdjacent = 8;
  d.materialType = materialType_fluid;
  addToListAndReset(d);
  /*---------------------------------------------*/
  d.id = feature_shallowMud;
  d.spawnType = featureSpawnType_other;
  d.name_a = "shallow mud";
  d.name_the = "the shallow mud";
  d.glyph = '~';
  d.color = clrBrownDark;
  d.tile = tile_water1;
  d.isShootPassable = true;
  d.isVisionPassable = true;
  d.canHaveBlood = false;
  d.canHaveGore = false;
  d.canHaveCorpse = true;
  d.canHaveStaticFeature = false;
  d.canHaveItem = true;
  d.dodgeModifier = -20;
  d.materialType = materialType_fluid;
  addToListAndReset(d);
  /*---------------------------------------------*/
  d.id = feature_poolBlood;
  d.spawnType = featureSpawnType_other;
  d.name_a = "a pool of blood";
  d.name_the = "the pool of blood";
  d.glyph = '~';
  d.color = clrRed;
  d.tile = tile_water1;
  d.isShootPassable = true;
  d.isVisionPassable = true;
  d.canHaveBlood = false;
  d.canHaveGore = false;
  d.canHaveCorpse = true;
  d.canHaveStaticFeature = false;
  d.canHaveItem = true;
  d.dodgeModifier = -10;
  d.shockWhenAdjacent = 3;
  d.materialType = materialType_fluid;
  addToListAndReset(d);
  /*---------------------------------------------*/
  d.id = feature_chasm;
  d.name_a = "a chasm";
  d.name_the = "the chasm";
  d.glyph = ' ';
  d.color = clrBlack;
  d.isMovePassable[moveType_walk] = false;
  d.isMovePassable[moveType_ethereal] = true;
  d.isMovePassable[moveType_fly] = true;
  d.isMovePassable[moveType_ooze] = false;
  d.isShootPassable = true;
  d.isVisionPassable = true;
  d.canHaveBlood = false;
  d.canHaveGore = false;
  d.canHaveCorpse = false;
  d.canHaveStaticFeature = false;
  d.canHaveItem = true;
  d.isBottomless = true;
  d.messageOnPlayerBlocked = "A chasm lies in my way.";
  d.messageOnPlayerBlockedBlind = "I realize I am standing on the edge of a chasm.";
  d.shockWhenAdjacent = 3;
  d.materialType = materialType_empty;
  addToListAndReset(d);
  /*---------------------------------------------*/
//  d.id = feature_caveWall;
//  d.name_a = "a cavern wall";
//  d.name_the = "the cavern wall";
//  d.glyph = eng->config->WALL_SYMBOL_FULL_SQUARE == false ? '#' : 10;
//  d.color = clrGray;
//  d.tile = tile_squareCheckered;
//  d.isMovePassable[moveType_walk] = false;
//  d.isMovePassable[moveType_ethereal] = true;
//  d.isMovePassable[moveType_fly] = false;
//  d.isMovePassable[moveType_ooze] = false;
//  d.isShootPassable = false;
//  d.isVisionPassable = false;
//  d.isSmokePassable = false;
//  d.canHaveBlood = true;
//  d.canHaveGore = false;
//  d.canHaveCorpse = false;
//  d.canHaveItem = false;
//  d.canHaveStaticFeature = false;
//  d.featuresOnDestroyed.push_back(feature_rubbleHigh);
//  d.featuresOnDestroyed.push_back(feature_rubbleLow);
//  d.featuresOnDestroyed.push_back(feature_stoneFloor);
//  addToListAndReset(d);
  /*---------------------------------------------*/
  d.id = feature_caveFloor;
  d.name_a = "cavern floor";
  d.name_the = "the cavern floor";
  d.glyph = '.';
  d.color = clrGray;
  d.tile = tile_floor;
  d.isShootPassable = true;
  d.isVisionPassable = true;
  d.canHaveBlood = true;
  d.canHaveGore = true;
  d.canHaveCorpse = true;
  d.canHaveItem = true;
  d.canHaveStaticFeature = true;
  addToListAndReset(d);
  /*---------------------------------------------*/
  d.id = feature_gravestone;
  d.name_a = "a gravestone";
  d.name_the = "the gravestone";
  d.spawnType = featureSpawnType_other;
  d.glyph = '&';
  d.color = clrWhite;
  d.tile = tile_graveStone;
  d.isMovePassable[moveType_walk] = false;
  d.isMovePassable[moveType_ethereal] = true;
  d.isMovePassable[moveType_fly] = true;
  d.isMovePassable[moveType_ooze] = false;
  d.isShootPassable = true;
  d.isVisionPassable = true;
  d.canHaveBlood = false;
  d.canHaveGore = false;
  d.canHaveCorpse = false;
  d.canHaveStaticFeature = false;
  d.canHaveItem = false;
  d.shockWhenAdjacent = 2;
  addToListAndReset(d);
  /*---------------------------------------------*/
  d.id = feature_churchBench;
  d.name_a = "a church bench";
  d.name_the = "the church bench";
  d.glyph = '[';
  d.color = clrBrown;
  d.tile = tile_churchBench;
  d.isMovePassable[moveType_walk] = false;
  d.isMovePassable[moveType_ethereal] = true;
  d.isMovePassable[moveType_fly] = true;
  d.isMovePassable[moveType_ooze] = true;
  d.isShootPassable = false;
  d.isVisionPassable = false;
  d.canHaveBlood = false;
  d.canHaveGore = false;
  d.canHaveCorpse = false;
  d.canHaveStaticFeature = false;
  d.canHaveItem = false;
  addToListAndReset(d);
  /*---------------------------------------------*/
  d.id = feature_churchCarpet;
  d.name_a = "a red carpet";
  d.name_the = "the red carpet";
  d.glyph = '.';
  d.color = clrRed;
  d.tile = tile_floor;
  d.isShootPassable = true;
  d.isVisionPassable = true;
  d.canHaveBlood = true;
  d.canHaveGore = true;
  d.canHaveCorpse = true;
  d.canHaveStaticFeature = false;
  d.canHaveItem = true;
  d.materialType = materialType_soft;
  addToListAndReset(d);
  /*---------------------------------------------*/
  d.id = feature_rubbleHigh;
  d.name_a = "a big pile of debris";
  d.name_the = "the big pile of debris";
  d.glyph = 8;
  d.color = clrGray;
  d.tile = tile_rubbleHigh;
  d.isMovePassable[moveType_walk] = false;
  d.isMovePassable[moveType_ethereal] = true;
  d.isMovePassable[moveType_fly] = false;
  d.isMovePassable[moveType_ooze] = true;
  d.isShootPassable = false;
  d.isVisionPassable = false;
  d.isSmokePassable = false;
  d.canHaveBlood = false;
  d.canHaveGore = false;
  d.canHaveCorpse = false;
  d.canHaveStaticFeature = false;
  d.canHaveItem = false;
  d.featuresOnDestroyed.push_back(feature_rubbleLow);
  addToListAndReset(d);
  /*---------------------------------------------*/
  d.id = feature_rubbleLow;
  d.name_a = "rubble";
  d.name_the = "the rubble";
  d.glyph = ',';
  d.color = clrGray;
  d.tile = tile_rubbleLow;
  d.isShootPassable = true;
  d.isVisionPassable = true;
  d.canHaveBlood = false;
  d.canHaveGore = true;
  d.canHaveCorpse = true;
  d.canHaveStaticFeature = false;
  d.canHaveItem = true;
  addToListAndReset(d);
  /*---------------------------------------------*/
  d.id = feature_statue;
  d.name_a = "a statue";
  d.name_the = "the statue";
  d.glyph = 5;
  d.color = clrWhite;
  d.tile = tile_witchOrWarlock;
  d.isMovePassable[moveType_walk] = false;
  d.isMovePassable[moveType_ooze] = false;
  d.isShootPassable = false;
  d.isVisionPassable = false;
  d.canHaveBlood = false;
  d.canHaveGore = false;
  d.canHaveCorpse = false;
  d.canHaveStaticFeature = false;
  d.canHaveItem = false;
  d.themedFeatureSpawnRules.set(4, placementRule_nextToWallsOrAwayFromWalls, roomTheme_human, roomTheme_crypt);
  addToListAndReset(d);
  /*---------------------------------------------*/
  d.id = feature_ghoulStatue;
  d.name_a = "a statue of a ghoulish creature";
  d.name_the = "the statue of a ghoulish creature";
  d.glyph = 'M';
  d.color = clrWhite;
  d.tile = tile_ghoul;
  d.isMovePassable[moveType_walk] = false;
  d.isMovePassable[moveType_ooze] = false;
  d.isShootPassable = false;
  d.isVisionPassable = false;
  d.canHaveBlood = false;
  d.canHaveGore = false;
  d.canHaveCorpse = false;
  d.canHaveStaticFeature = false;
  d.canHaveItem = false;
  d.shockWhenAdjacent = 6;
  d.themedFeatureSpawnRules.set(4, placementRule_nextToWallsOrAwayFromWalls, roomTheme_monster);
  addToListAndReset(d);
  /*---------------------------------------------*/
  d.id = feature_cocoon;
  d.name_a = "a cocoon";
  d.name_the = "the cocoon";
  d.spawnType = featureSpawnType_other;
  d.glyph = '8';
  d.color = clrWhite;
  d.tile = tile_cocoon;
  d.isShootPassable = true;
  d.isVisionPassable = true;
  d.canHaveBlood = false;
  d.canHaveGore = false;
  d.canHaveCorpse = false;
  d.canHaveStaticFeature = false;
  d.canHaveItem = false;
  d.shockWhenAdjacent = 3;
  d.themedFeatureSpawnRules.set(2, placementRule_nextToWallsOrAwayFromWalls, roomTheme_spider);
  addToListAndReset(d);
  /*---------------------------------------------*/
  d.id = feature_chest;
  d.name_a = "a chest";
  d.name_the = "the chest";
  d.spawnType = featureSpawnType_other;
  d.glyph = '+';
  d.color = clrBrownDark;
  d.tile = tile_chestClosed;
  d.isMovePassable[moveType_walk] = true;
  d.isShootPassable = true;
  d.isVisionPassable = true;
  d.canHaveBlood = false;
  d.canHaveGore = false;
  d.canHaveCorpse = false;
  d.canHaveStaticFeature = false;
  d.canHaveItem = false;
  d.themedFeatureSpawnRules.set(1, placementRule_nextToWalls, roomTheme_human, roomTheme_dungeon);
  addToListAndReset(d);
  /*---------------------------------------------*/
  d.id = feature_barrel;
  d.name_a = "a barrel";
  d.name_the = "the barrel";
  d.spawnType = featureSpawnType_other;
  d.glyph = '0';
  d.color = clrBrown;
  d.tile = tile_barrel;
  d.isMovePassable[moveType_walk] = false;
  d.isShootPassable = true;
  d.isVisionPassable = true;
  d.canHaveBlood = false;
  d.canHaveGore = false;
  d.canHaveCorpse = false;
  d.canHaveStaticFeature = false;
  d.canHaveItem = false;
//  d.themedFeatureSpawnRules.set(2, placementRule_nextToWallsOrAwayFromWalls, roomTheme_human, roomTheme_dungeon);
  addToListAndReset(d);
  /*---------------------------------------------*/
  d.id = feature_cabinet;
  d.name_a = "a cabinet";
  d.name_the = "the cabinet";
  d.spawnType = featureSpawnType_other;
  d.glyph = '7';
  d.color = clrBrownDark;
  d.tile = tile_cabinetClosd;
  d.isMovePassable[moveType_walk] = false;
  d.isMovePassable[moveType_ooze] = false;
  d.isShootPassable = false;
  d.isVisionPassable = false;
  d.canHaveBlood = false;
  d.canHaveGore = false;
  d.canHaveCorpse = false;
  d.canHaveStaticFeature = false;
  d.canHaveItem = false;
  d.themedFeatureSpawnRules.set(1, placementRule_nextToWalls, roomTheme_human);
  addToListAndReset(d);
  /*---------------------------------------------*/
  d.id = feature_pillarCarved;
  d.name_a = "a carved pillar";
  d.name_the = "the carved pillar";
  d.spawnType = featureSpawnType_other;
  d.glyph = '1';
  d.color = clrGray;
  d.tile = tile_pillarCarved;
  d.isMovePassable[moveType_walk] = false;
  d.isMovePassable[moveType_ooze] = false;
  d.isShootPassable = false;
  d.isVisionPassable = false;
  d.canHaveBlood = false;
  d.canHaveGore = false;
  d.canHaveCorpse = false;
  d.canHaveStaticFeature = false;
  d.canHaveItem = false;
//  d.themedFeatureSpawnRules.set(1, placementRule_awayFromWalls, roomTheme_crypt, roomTheme_ritual, roomTheme_monster);
  addToListAndReset(d);
  /*---------------------------------------------*/
  d.id = feature_pillar;
  d.name_a = "a pillar";
  d.name_the = "the pillar";
  d.glyph = '|';
  d.color = clrGray;
  d.tile = tile_pillar;
  d.isMovePassable[moveType_walk] = false;
  d.isMovePassable[moveType_ooze] = false;
  d.isShootPassable = false;
  d.isVisionPassable = false;
  d.canHaveBlood = false;
  d.canHaveGore = false;
  d.canHaveCorpse = false;
  d.canHaveStaticFeature = false;
  d.canHaveItem = false;
  d.themedFeatureSpawnRules.set(4, placementRule_awayFromWalls, roomTheme_crypt, roomTheme_ritual, roomTheme_monster);
  addToListAndReset(d);
  /*---------------------------------------------*/
  d.id = feature_pillarBroken;
  d.name_a = "a broken pillar";
  d.name_the = "the broken pillar";
  d.glyph = '|';
  d.color = clrGray;
  d.tile = tile_pillarBroken;
  d.isMovePassable[moveType_walk] = false;
  d.isMovePassable[moveType_ooze] = false;
  d.isShootPassable = false;
  d.isVisionPassable = false;
  d.canHaveBlood = false;
  d.canHaveGore = false;
  d.canHaveCorpse = false;
  d.canHaveStaticFeature = false;
  d.canHaveItem = false;
  d.themedFeatureSpawnRules.set(4, placementRule_awayFromWalls, roomTheme_crypt, roomTheme_ritual, roomTheme_monster);
  addToListAndReset(d);
  /*---------------------------------------------*/
  d.id = feature_altar;
  d.name_a = "an altar";
  d.name_the = "the altar";
  d.spawnType = featureSpawnType_other;
  d.glyph = '_';
  d.color = clrWhiteHigh;
  d.tile = tile_altar;
  d.isShootPassable = true;
  d.isVisionPassable = true;
  d.canHaveBlood = false;
  d.canHaveGore = false;
  d.canHaveCorpse = false;
  d.canHaveStaticFeature = false;
  d.canHaveItem = false;
  d.shockWhenAdjacent = 10;
  d.themedFeatureSpawnRules.set(1, placementRule_nextToWallsOrAwayFromWalls, roomTheme_ritual);
  addToListAndReset(d);
  /*---------------------------------------------*/
  d.id = feature_tomb;
  d.name_a = "a tomb";
  d.name_the = "the tomb";
  d.spawnType = featureSpawnType_other;
  d.glyph = '&';
  d.color = clrGray;
  d.tile = tile_tomb;
  d.isMovePassable[moveType_walk] = false;
  d.isMovePassable[moveType_ethereal] = true;
  d.isMovePassable[moveType_fly] = true;
  d.isMovePassable[moveType_ooze] = false;
  d.isShootPassable = true;
  d.isVisionPassable = true;
  d.canHaveBlood = false;
  d.canHaveGore = false;
  d.canHaveCorpse = false;
  d.canHaveStaticFeature = false;
  d.canHaveItem = false;
  d.shockWhenAdjacent = 3;
  d.themedFeatureSpawnRules.set(3, placementRule_nextToWalls, roomTheme_crypt);
  addToListAndReset(d);
  /*---------------------------------------------*/
  d.id = feature_pit;
  d.name_a = "a pit";
  d.name_the = "the pit";
  d.glyph = '^';
  d.color = clrGray;
  d.tile = tile_pit;
  d.isMovePassable[moveType_walk] = false;
  d.isMovePassable[moveType_ethereal] = true;
  d.isMovePassable[moveType_fly] = true;
  d.isMovePassable[moveType_ooze] = false;
  d.isShootPassable = true;
  d.isVisionPassable = true;
  d.canHaveBlood = false;
  d.canHaveGore = false;
  d.canHaveCorpse = false;
  d.canHaveStaticFeature = false;
  d.isBottomless = true;
  d.canHaveItem = true;
  d.messageOnPlayerBlocked = "A pit lies in my way.";
  d.messageOnPlayerBlockedBlind = "I realize I am standing on the edge of a pit.";
  d.shockWhenAdjacent = 5;
  addToListAndReset(d);
  /*---------------------------------------------*/
  d.id = feature_door;
  d.spawnType = featureSpawnType_other;
  d.canHaveBlood = false;
  d.canHaveGore = false;
  d.canHaveCorpse = false;
  d.canHaveStaticFeature = false;
  d.canHaveItem = false;
  d.featuresOnDestroyed.push_back(feature_rubbleLow);
  addToListAndReset(d);
  /*---------------------------------------------*/
  d.id = feature_trashedSpiderWeb;
  d.name_a = "a trashed spider web";
  d.name_the = "the trashed spider web";
  d.glyph = '.';
  d.color = clrWhite;
  d.tile = tile_floor;
  d.isShootPassable = true;
  d.isVisionPassable = true;
  d.canHaveBlood = true;
  d.canHaveGore = true;
  d.canHaveCorpse = true;
  d.canHaveStaticFeature = true;
  d.canHaveItem = true;
  addToListAndReset(d);
  /*---------------------------------------------*/
  d.id = feature_trap;
  d.spawnType = featureSpawnType_other;
  d.isShootPassable = true;
  d.isVisionPassable = true;
  d.canHaveStaticFeature = false;
  addToListAndReset(d);
  /*---------------------------------------------*/
  d.id = feature_litDynamite;
  d.spawnType = featureSpawnType_other;
  d.name_a = "a lit stick of dynamite";
  d.name_the = "the lit stick of dynamite";
  d.glyph = '/';
  d.color = clrRedLight;
  d.tile = tile_dynamiteLit;
  d.canHaveBlood = false;
  d.canHaveGore = false;
  d.canHaveCorpse = false;
  d.canHaveItem = false;
  addToListAndReset(d);
  /*---------------------------------------------*/
  d.id = feature_litFlare;
  d.spawnType = featureSpawnType_other;
  d.name_a = "a lit flare";
  d.name_the = "the lit flare";
  d.glyph = '/';
  d.color = clrYellow;
  d.tile = tile_flareLit;
  addToListAndReset(d);
  /*---------------------------------------------*/
  d.id = feature_smoke;
  d.spawnType = featureSpawnType_other;
  d.name_a = "smoke";
  d.name_the = "the smoke";
  d.glyph = '*';
  d.color = clrGray;
  d.tile = tile_smoke;
  d.isVisionPassable = false;
  addToListAndReset(d);
  /*---------------------------------------------*/
  d.id = feature_proxEventWallCrumble;
  d.spawnType = featureSpawnType_other;
  addToListAndReset(d);
  /*---------------------------------------------*/
}

