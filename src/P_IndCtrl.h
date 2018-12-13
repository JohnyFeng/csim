#pragma once

#include "Player.h"
#include <sstream>

class P_IndCtrl : public Player
{
public:

  enum Policy { ATTACK_NONE=0, ATTACK_CLOSEST, ATTACK_WEAKEST, ATTACK_MOST_DANGEROUS };

  P_IndCtrl(World *world_, int playerId_, const std::string &name_, Pix qtEps_,
            const std::string &par, int seed_)
  {
    setup(world_, playerId_, name_, qtEps_, par, seed_);

    // parameters:  <policy(string)> [<avoidOverkill(bool)>]
    
    std::istringstream is(par);
    std::string pol;
    
    is >> pol;
      
    if (!is) {
      ERR("P_IndCtrl: policy parameter missing");
    }

    polEnum = policyFromString(pol);

    avoidOverkill = false;
    
    is >> avoidOverkill;

    if (!is) {
      // default = false;
      // ERR("P_IndCtrl: avoid overkill parameter missing");
    }
  }
  
  // returns policy name
  static std::string policyToString(Policy pol);

  static Policy policyFromString(const std::string &ps);

  void onFrame(int /*frameCount*/) override;

  void onGameEnd() override;

private:

  Policy polEnum;
  bool avoidOverkill;
};
