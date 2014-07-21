#include "FeatureStatic.h"

#include <string>

#include "Init.h"
#include "Log.h"
#include "Renderer.h"
#include "Map.h"
#include "Utils.h"
#include "Popup.h"
#include "DungeonClimb.h"
#include "SaveHandling.h"
#include "ItemFactory.h"
#include "MapParsing.h"
#include "FeatureMob.h"

using namespace std;

//------------------------------------------------------------------- STATIC FEATURE
FeatureStatic::FeatureStatic(Pos pos) :
  Feature(pos),
  goreTile_(TileId::empty),
  goreGlyph_(0),
  burnState_(BurnState::notBurned) {
  for(int dmgType = 0; dmgType < int(DmgType::END); ++dmgType) {
    for(int dmgMethod = 0; dmgMethod < int(DmgMethod::END); ++dmgMethod) {
      onHit[dmgType][dmgMethod] = [](Actor * const actor) {(void)actor;};
    }
  }
}

void FeatureStatic::onNewTurn() {
  if(burnState_ == BurnState::burning) {

    //Hit actor standing on feature
    auto* actor = Utils::getActorAtPos(pos_);
    if(actor) {
      //Occasionally try to set actor on fire, otherwise just do small fire damage
      if(Rnd::oneIn(4)) {
        auto& propHandler = actor->getPropHandler();
        propHandler.tryApplyProp(new PropBurning(PropTurns::standard));
      } else {
        actor->hit(1, DmgType::fire, true);
      }
    }

    //Finished burning?
    int finishBurningOneInN = 1;
    int hitAdjacentOneInN   = 1;

    switch(getMatl()) {
      case Matl::fluid:
      case Matl::empty: {
        finishBurningOneInN = 1;
        hitAdjacentOneInN   = 1;
      } break;

      case Matl::stone: {
        finishBurningOneInN = 12;
        hitAdjacentOneInN   = 12;
      } break;

      case Matl::metal: {
        finishBurningOneInN = 12;
        hitAdjacentOneInN   = 6;
      } break;

      case Matl::plant: {
        finishBurningOneInN = 20;
        hitAdjacentOneInN   = 6;
      } break;

      case Matl::wood: {
        finishBurningOneInN = 60;
        hitAdjacentOneInN   = 4;
      } break;

      case Matl::cloth: {
        finishBurningOneInN = 20;
        hitAdjacentOneInN   = 6;
      } break;
    }

    if(Rnd::oneIn(finishBurningOneInN)) {
      burnState_ = BurnState::hasBurned;
    }

    //Hit adjacent features and actors?
    if(Rnd::oneIn(hitAdjacentOneInN)) {
      const Pos p(DirUtils::getRndAdjPos(pos_, false));
      if(Utils::isPosInsideMap(p)) {
        Map::cells[p.x][p.y].featureStatic->hit(DmgType::fire, DmgMethod::elemental);

        actor = Utils::getActorAtPos(p);
        if(actor) {
          actor->hit(1, DmgType::fire, true);
        }
      }
    }

    //Create smoke?
    if(Rnd::oneIn(20)) {
      const Pos p(DirUtils::getRndAdjPos(pos_, true));
      if(Utils::isPosInsideMap(p)) {
        if(!CellPred::BlocksMoveCmn(false).check(Map::cells[p.x][p.y])) {
          GameTime::addMob(new Smoke(p, 10));
        }
      }
    }
  }
}

void FeatureStatic::tryStartBurning(const bool IS_MSG_ALLOWED) {
  if(burnState_ == BurnState::notBurned) {
    if(Map::canPlayerSeePos(pos_) && IS_MSG_ALLOWED) {
      string str = getName(true) + " catches fire.";
      str[0] = toupper(str[0]);
      Log::addMsg(str);
    }
    burnState_ = BurnState::burning;
  }
}

void FeatureStatic::setHitEffect(const DmgType dmgType, const DmgMethod dmgMethod,
                                 const function<void (Actor* const actor)>& effect) {
  onHit[int(dmgType)][int(dmgMethod)] = effect;
}

void FeatureStatic::examine() {
  Log::addMsg("I find nothing specific there to examine or use.");
}

void FeatureStatic::disarm() {
  Log::addMsg(msgDisarmNoTrap);
  Renderer::drawMapAndInterface();
}

void FeatureStatic::hit(const DmgType dmgType, const DmgMethod dmgMethod, Actor* actor) {
  if(actor == Map::player) {
    const bool IS_BLIND    = !Map::player->getPropHandler().allowSee();
    const bool IS_BLOCKING = !canMoveCmn() && getId() != FeatureId::stairs;

    if(dmgMethod == DmgMethod::kick) {
      assert(actor);

      if(IS_BLOCKING) {
        Log::addMsg("I kick " + (IS_BLIND ? "something" : getName(false)) + "!");

        if(Rnd::oneIn(4)) {
          Log::addMsg("I sprain myself.", clrMsgBad);
          actor->hit(Rnd::range(1, 5), DmgType::pure, false);
        }

        if(Rnd::oneIn(4)) {
          Log::addMsg("I am off-balance.");

          actor->getPropHandler().tryApplyProp(new PropParalyzed(PropTurns::specific, 2));
        }

      } else {
        Log::addMsg("I kick the air!");
        Audio::play(SfxId::missMedium);
      }
    }
  }

  onHit[int(dmgType)][int(dmgMethod)](actor);

  if(actor) {GameTime::actorDidAct();}
}

void FeatureStatic::tryPutGore() {
  if(getData().canHaveGore) {
    const int ROLL_GLYPH = Rnd::dice(1, 4);
    switch(ROLL_GLYPH) {
      case 1: {goreGlyph_ = ',';} break;
      case 2: {goreGlyph_ = '`';} break;
      case 3: {goreGlyph_ = 39;}  break;
      case 4: {goreGlyph_ = ';';} break;
    }

    const int ROLL_TILE = Rnd::dice(1, 8);
    switch(ROLL_TILE) {
      case 1: {goreTile_ = TileId::gore1;} break;
      case 2: {goreTile_ = TileId::gore2;} break;
      case 3: {goreTile_ = TileId::gore3;} break;
      case 4: {goreTile_ = TileId::gore4;} break;
      case 5: {goreTile_ = TileId::gore5;} break;
      case 6: {goreTile_ = TileId::gore6;} break;
      case 7: {goreTile_ = TileId::gore7;} break;
      case 8: {goreTile_ = TileId::gore8;} break;
    }
  }
}

Clr FeatureStatic::getClr() const {
  switch(burnState_) {
    case BurnState::notBurned:  return getData().clr; break;
    case BurnState::burning:    return clrOrange;     break;
    case BurnState::hasBurned:  return clrGray;       break;
  }
  assert(false && "Failed to set color");
  return clrYellow;
}

Clr FeatureStatic::getClrBg() const {
  switch(burnState_) {
    case BurnState::notBurned:  return clrBlack;  break;
    case BurnState::burning:    return Clr {Uint8(Rnd::range(32, 255)), 0, 0, 0}; break;
    case BurnState::hasBurned:  return clrBlack;  break;
  }
  assert(false && "Failed to set color");
  return clrYellow;
}

string FeatureStatic::getName(const bool DEFINITE_ARTICLE) const {
  return DEFINITE_ARTICLE ? getData().nameThe : getData().nameA;
}

void FeatureStatic::clearGore() {
  goreTile_   = TileId::empty;
  goreGlyph_  = ' ';
  hasBlood_   = false;
}

//------------------------------------------------------------------- FLOOR
Floor::Floor(Pos pos) : FeatureStatic(pos), type_(FloorType::cmn) {
  setHitEffect(DmgType::fire, DmgMethod::elemental, [&](Actor * const actor) {
    (void)actor;
    if(Rnd::oneIn(3)) {tryStartBurning(false);}
  });
}

//------------------------------------------------------------------- WALL
Wall::Wall(Pos pos) : FeatureStatic(pos), type_(WallType::cmn), isMossy_(false) {

  setHitEffect(DmgType::physical, DmgMethod::forced, [&](Actor * const actor) {
    (void)actor;
    destrAdjDoors();
    mkLowRubbleAndRocks();
  });

  setHitEffect(DmgType::physical, DmgMethod::explosion, [&](Actor * const actor) {
    (void)actor;

    if(Rnd::fraction(3, 4)) {

      destrAdjDoors();

      if(Rnd::coinToss()) {
        mkLowRubbleAndRocks();
      } else {
        Map::put(new RubbleHigh(pos_));
      }
    }
  });

  setHitEffect(DmgType::physical, DmgMethod::bluntHeavy, [&](Actor * const actor) {
    (void)actor;

    if(Rnd::fraction(1, 4)) {

      destrAdjDoors();

      if(Rnd::coinToss()) {
        mkLowRubbleAndRocks();
      } else {
        Map::put(new RubbleHigh(pos_));
      }
    }
  });
}

void Wall::destrAdjDoors() const {
  for(const Pos& d : DirUtils::cardinalList) {
    const Pos p(pos_ + d);
    if(Utils::isPosInsideMap(p)) {
      if(Map::cells[p.x][p.y].featureStatic->getId() == FeatureId::door) {
        Map::put(new RubbleLow(p));
      }
    }
  }
}

void Wall::mkLowRubbleAndRocks() {
  const Pos pos(pos_);
  Map::put(new RubbleLow(pos_)); //Note: "this" is now deleted!
  if(Rnd::coinToss()) {ItemFactory::mkItemOnMap(ItemId::rock, pos);}
}

bool Wall::isTileAnyWallFront(const TileId tile) {
  return
    tile == TileId::wallFront      ||
    tile == TileId::wallFrontAlt1  ||
    tile == TileId::wallFrontAlt2  ||
    tile == TileId::caveWallFront  ||
    tile == TileId::egyptWallFront;
}

bool Wall::isTileAnyWallTop(const TileId tile) {
  return
    tile == TileId::wallTop      ||
    tile == TileId::caveWallTop  ||
    tile == TileId::egyptWallTop ||
    tile == TileId::rubbleHigh;
}

string Wall::getName(const bool DEFINITE_ARTICLE) const {
  const string modStr   = isMossy_ ? "moss-grown " : "";
  const string article  = (DEFINITE_ARTICLE ? "the " : "a ");

  switch(type_) {
    case WallType::cmn:
    case WallType::cmnAlt: {return article + modStr + "stone wall";}
    case WallType::cave:   {return article + modStr + "cavern wall";}
    case WallType::egypt:  {return article + modStr + "stone wall";}
  }
  assert(false && "Failed to get door description");
  return "";
}

Clr Wall::getClr() const {
  if(isMossy_)                  {return clrGreenDrk;}
  if(type_ == WallType::cave)   {return clrBrownGray;}
  if(type_ == WallType::egypt)  {return clrBrownGray;}
  return getData().clr;
}

char Wall::getGlyph() const {
  return Config::isAsciiWallFullSquare() ? 10 : '#';
}

TileId Wall::getFrontWallTile() const {
  if(Config::isTilesWallFullSquare()) {
    switch(type_) {
      case WallType::cmn:     return TileId::wallTop;         break;
      case WallType::cmnAlt:  return TileId::wallTop;         break;
      case WallType::cave:    return TileId::caveWallTop;     break;
      case WallType::egypt:   return TileId::egyptWallTop;    break;
      default:                return TileId::wallTop;         break;
    }
  } else {
    switch(type_) {
      case WallType::cmn:     return TileId::wallFront;       break;
      case WallType::cmnAlt:  return TileId::wallFrontAlt1;   break;
      case WallType::cave:    return TileId::caveWallFront;   break;
      case WallType::egypt:   return TileId::egyptWallFront;  break;
      default:                return TileId::wallFront;       break;
    }
  }
}

TileId Wall::getTopWallTile() const {
  switch(type_) {
    case WallType::cmn:       return TileId::wallTop;         break;
    case WallType::cmnAlt:    return TileId::wallTop;         break;
    case WallType::cave:      return TileId::caveWallTop;     break;
    case WallType::egypt:     return TileId::egyptWallTop;    break;
    default:                  return TileId::wallTop;         break;
  }
}

void Wall::setRandomNormalWall() {
  const int RND = Rnd::range(1, 6);
  switch(RND) {
    case 1:   type_ = WallType::cmnAlt; break;
    default:  type_ = WallType::cmn;    break;
  }
}

void Wall::setRandomIsMossGrown() {
  isMossy_ = Rnd::oneIn(40);
}

//------------------------------------------------------------------- HIGH RUBBLE
RubbleHigh::RubbleHigh(Pos pos) : FeatureStatic(pos) {
  setHitEffect(DmgType::physical, DmgMethod::forced, [&](Actor * const actor) {
    (void)actor;
    mkLowRubbleAndRocks();
  });

  setHitEffect(DmgType::physical, DmgMethod::explosion, [&](Actor * const actor) {
    (void)actor;
    mkLowRubbleAndRocks();
  });

  setHitEffect(DmgType::physical, DmgMethod::bluntHeavy, [&](Actor * const actor) {
    (void)actor;
    if(Rnd::fraction(2, 4)) {mkLowRubbleAndRocks();}
  });
}

void RubbleHigh::mkLowRubbleAndRocks() {
  const Pos pos(pos_);
  Map::put(new RubbleLow(pos_)); //Note: "this" is now deleted!
  if(Rnd::coinToss()) {ItemFactory::mkItemOnMap(ItemId::rock, pos);}
}

//------------------------------------------------------------------- GRAVE
string GraveStone::getName(const bool DEFINITE_ARTICLE) const {
  return (DEFINITE_ARTICLE ? getData().nameThe : getData().nameA) + "; " + inscr_;
}

void GraveStone::bump(Actor& actorBumping) {
  if(&actorBumping == Map::player) {Log::addMsg(inscr_);}
}

//------------------------------------------------------------------- STATUE
Statue::Statue(Pos pos) : FeatureStatic(pos) {

  setHitEffect(DmgType::physical, DmgMethod::kick, [&](Actor * const actor) {
    assert(actor);

    const AlertsMonsters alertsMonsters = actor == Map::player ?
                                          AlertsMonsters::yes :
                                          AlertsMonsters::no;
    if(Rnd::coinToss()) {
      if(Map::cells[pos_.x][pos_.y].isSeenByPlayer) {Log::addMsg("It topples over.");}

      Snd snd("I hear a crash.", SfxId::END, IgnoreMsgIfOriginSeen::yes,
              pos_, actor, SndVol::low, alertsMonsters);
      SndEmit::emitSnd(snd);

      const Pos dstPos = pos_ + (pos_ - actor->pos);

      Map::put(new RubbleLow(pos_)); //Note: "this" is now deleted!

      Map::player->updateFov();
      Renderer::drawMapAndInterface();
      Map::updateVisualMemory();

      if(!CellPred::BlocksMoveCmn(false).check(Map::cells[dstPos.x][dstPos.y])) {
        Actor* const actorBehind = Utils::getActorAtPos(dstPos);
        if(actorBehind) {
          if(actorBehind->deadState == ActorDeadState::alive) {
            vector<PropId> propList;
            actorBehind->getPropHandler().getAllActivePropIds(propList);
            if(find(begin(propList), end(propList), propEthereal) == end(propList)) {
              if(actorBehind == Map::player) {
                Log::addMsg("It falls on me!");
              } else if(Map::player->isSeeingActor(*actorBehind, nullptr)) {
                Log::addMsg("It falls on " + actorBehind->getNameThe() + ".");
              }
              actorBehind->hit(Rnd::dice(3, 5), DmgType::physical, true);
            }
          }
        }
        Map::put(new RubbleLow(dstPos));
      }
    }
  });
}

//------------------------------------------------------------------- STAIRS
void Stairs::bump(Actor& actorBumping) {
  if(&actorBumping == Map::player) {

    const vector<string> choices {"Descend", "Save and quit", "Cancel"};
    const int CHOICE =
      Popup::showMenuMsg("", true, choices, "A staircase leading downwards");

    if(CHOICE == 0) {
      Map::player->pos = pos_;
      TRACE << "Calling DungeonClimb::tryUseDownStairs()" << endl;
      DungeonClimb::tryUseDownStairs();
    } else if(CHOICE == 1) {
      Map::player->pos = pos_;
      SaveHandling::save();
      Init::quitToMainMenu = true;
    } else {
      Log::clearLog();
      Renderer::drawMapAndInterface();
    }
  }
}

//------------------------------------------------------------------- BRIDGE
TileId Bridge::getTile() const {
  return dir_ == hor ? TileId::hangbridgeHor : TileId::hangbridgeVer;
}

char Bridge::getGlyph() const {
  return dir_ == hor ? '|' : '=';
}

//------------------------------------------------------------------- SHALLOW LIQUID
void LiquidShallow::bump(Actor& actorBumping) {
  vector<PropId> props;
  actorBumping.getPropHandler().getAllActivePropIds(props);

  if(
    find(begin(props), end(props), propEthereal)  == end(props) &&
    find(begin(props), end(props), propFlying)    == end(props)) {

    actorBumping.getPropHandler().tryApplyProp(new PropWaiting(PropTurns::standard));

    if(&actorBumping == Map::player) Log::addMsg("*glop*");
  }
}

//------------------------------------------------------------------- DEEP LIQUID
void LiquidDeep::bump(Actor& actorBumping) {
  (void)actorBumping;
}

//------------------------------------------------------------------- LEVER
Clr Lever::getClr() const {
  return isPositionLeft_ ? clrGray : clrWhite;
}

TileId Lever::getTile() const {
  return isPositionLeft_ ? TileId::leverLeft : TileId::leverRight;
}

void Lever::examine() {
  pull();
}

void Lever::pull() {
  TRACE_FUNC_BEGIN;
  isPositionLeft_ = !isPositionLeft_;

  //TODO Implement something like openByLever in the Door class
  //Others should not poke around in the doors internal variables

//  if(!doorLinkedTo_->isBroken_) {
//    TRACE << "Door linked to is not broken" << endl;
//    if(!doorLinkedTo_->isOpen_) {doorLinkedTo_->reveal(true);}
//    doorLinkedTo_->isOpen_  = !doorLinkedTo_->isOpen_;
//    doorLinkedTo_->isStuck_ = false;
//  }
  Map::player->updateFov();
  Renderer::drawMapAndInterface();
  TRACE_FUNC_END;
}

//------------------------------------------------------------------- GRASS
Grass::Grass(Pos pos) : FeatureStatic(pos), type_(GrassType::cmn) {
  setHitEffect(DmgType::fire, DmgMethod::elemental, [&](Actor * const actor) {
    (void)actor;
    tryStartBurning(false);
  });
}

//------------------------------------------------------------------- BUSH
Bush::Bush(Pos pos) : FeatureStatic(pos), type_(BushType::cmn) {
  setHitEffect(DmgType::fire, DmgMethod::elemental, [&](Actor * const actor) {
    (void)actor;
    tryStartBurning(false);
  });
}

//------------------------------------------------------------------- TREE
Tree::Tree(Pos pos) : FeatureStatic(pos) {
  setHitEffect(DmgType::fire, DmgMethod::elemental, [&](Actor * const actor) {
    (void)actor;
    if(Rnd::oneIn(3)) {tryStartBurning(false);}
  });
}
