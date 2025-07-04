#ifndef HERO_H
#define HERO_H

#include "character.h"
#include "monster.h"

enum class HeroType { Ancient, Mayor };

class Hero : public chara {
protected:
    HeroType hType;
    std::vector<Item> items;
    int actions;
    int Act_max;
    int Act_remaining;
    bool in_hospital;

public:
    Hero(const std::string& name, const std::string& loc, HeroType hType);
    HeroType getHType() const { return hType; }
    std::vector<Item> getItems() const { return items; }
    void pickup_item(const Item& item) { items.push_back(item); }
    void useAct() { if (actions > 0) actions--; }
    void takeHarm(int damage) { }
    void defeatMonster(std::shared_ptr<Monster> monster) { }
    int getAct_max() const;
    int getAct_rem() const;
    bool getin_hos() const;
    void setAct_max(int a);
    void setAct_rem(int a);
    void setin_hos(bool a);
    void startTurn();
    void heal();
    void send_hos();
    void move(const std::string& newLoc);
    void move_valliger(std::shared_ptr<chara> villager, const std::string& newLoc);
    void use_Item(const std::string& itemname);
};

class Mayor : public Hero {
public:
    Mayor();
};

class Ancient : public Hero {
public:
    Ancient();
    void takeSpcial_item(const std::string& itemLoc, Board& board);
};

#endif
