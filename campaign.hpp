#ifndef CAMPAIGN_HPP
#define CAMPAIGN_HPP

#include <string>
#include <vector>

struct Character {
    std::string name;
    std::string archetype;
    int health;
    int attack;
    int defense;
    int gold;
};

struct Enemy {
    std::string name;
    int health;
    int attack;
    int defense;
    bool isBoss;
};

void playCampaign();

#endif
