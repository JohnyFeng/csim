#include "UnitTypes.h"

// data from https://liquipedia.net/starcraft/List_of_Unit_and_Building_Sizes
// etc.

Unit makeMarine(int owner, const Vec2 &pos)
{
  Unit u(owner, MARINE, pos);

  u.radius = 9;
  u.maxSpeed = 4;
  u.visionRange = 7*32;
  u.attackRange = 4*32;
  u.maxHp = 40;
  u.damage = 6;
  u.cooldown = 15;
  u.onlyAttackWhenStopped = true;

  u.startValues();

  return u;
}

Unit makeTank(int owner, const Vec2 &pos)
{
  Unit u(owner, TANK, pos);

  u.radius = 16;
  u.maxSpeed = 4;
  u.visionRange = 10*32;
  u.attackRange = 7*32;
  u.maxHp = 150;
  u.damage = 30;
  u.cooldown = 37;
  u.onlyAttackWhenStopped = true;

  u.startValues();

  return u;
}
