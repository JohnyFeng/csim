#pragma once

// struct that represents a unit
// currently supported: vision, attacking, moving

#include <set>
#include "Global.h"
#include <iostream>

struct Unit
{
  // id increases with every constructor call
  // careful: only create new units via construction
  
  static int id;
  
  Unit(int owner_ = -1, int type_ = -1, const Vec2 &pos_ = { 0, 0 })
    : owner(owner_), type(type_), pos(pos_)
  {
    radius = 0;
    maxSpeed = 0;
    visionRange = 0;
    attackRange = 0;
    maxHp = 0;
    damage = 0;
    cooldown = 0;
    onlyAttackWhenStopped = true;

    unitId = ++id;

    delta = {0,0};
    targetPos = {0,0};
    moveCount = 0;
    
    hp = 0;
    cooldownCount = 0;

    startValues();
  }

  // fixed unit type properties
  Pix radius;       // pixels
  Pix maxSpeed;     // pixels/frame
  Pix visionRange;  // pixels
  Pix attackRange;  // pixels
  int  maxHp;
  int  damage;       // subtracted from hp of targeted unit
  int  cooldown;     // frames to wait before attacking next (0 = constant firing)
  bool onlyAttackWhenStopped;
  
  // static
  int owner;
  int type;
  int unitId;

  // dynamic

  // motion related

  Vec2 pos;
  Vec2 delta;     // position change per tick
  Vec2 targetPos;
  int  moveCount; // = 0 <=> stop

  // attack related
  
  int hp;
  int cooldownCount;  // <= 0 <=> can attack

  // helpers
  
  Pix getX() const { return pos.x; }
  Pix getY() const { return pos.y; }
  
  void startValues()
  {
    hp = maxHp;
    cooldownCount = 0;
    stopMotion();
  }
  
  void stopMotion()
  {
    moveCount = 0;
  }

  bool isMoving() const
  {
    return moveCount > 0;
  }

  // set unit in motion towards target position whereTo
  
  void startMotion(const Vec2 &whereTo)
  {
    DPRINT("world: start motion " << unitId);
    
    Pix speed = maxSpeed; // future: pass on as parameter?
    Pix d = static_cast<Pix>(sqrt(whereTo.dist2(pos)));
    Pix time = d / speed;

    if (time >= 100'000) {
      ERR("motion too slow");
    }

    moveCount = static_cast<int>(ceil(time));

    if (!moveCount) {
      return;
    }

    assert(time > 0);    
    
    targetPos = whereTo;
    delta = whereTo.sub(pos);
    delta.scale(static_cast<Pix>(1.0/time));
  }

  // @return true iff unit can attack now
  
  bool readyForAttack() const
  {
    return cooldownCount <= 0;
  }

  // @return true if uTo is dead
  // cooldownDelta: random delta
  
  bool executeAttack(Unit &uTo, int cooldownDelta)
  {
    assert(readyForAttack());
    uTo.hp -= damage;
    // randomize cooldown -1..+2
    cooldownCount = (cooldown+1)+cooldownDelta; // note: +1 because first tick happens at end of this frame
    cooldownCount = std::max(cooldownCount, 1);
    return uTo.hp <= 0;
  }
  
  void tick()
  {
    DPRINT("world: tick " << unitId << " " << moveCount << " " << cooldownCount << " " << isMoving());

    if (isMoving()) {
      DPRINT("move tick");
      --moveCount; 
   }

    if (!readyForAttack()) {
      --cooldownCount;
      DPRINT("cooldown tick foo");
    }

    DPRINT("world: tick2" << unitId << " " << moveCount << " " << cooldownCount << " " << isMoving())
  }
  
};

// print major Unit attributes

inline std::ostream &operator<<(std::ostream &os, const Unit &u)
{
  os << "id " << u.unitId << " x " << u.pos.x << " y " << u.pos.y << " r " << u.radius
     << " v " << u.visionRange << " hp " << u.hp;
  return os;
}

// how to compare units

inline bool operator<(const Unit &u, const Unit &v)
{
  return u.unitId < v.unitId;
}
