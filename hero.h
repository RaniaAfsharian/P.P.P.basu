
#ifndef HERO_H
#define HERO_H

#include <bits/stdc++.h>

#include "character.h"
#include "monster.h"
#include "Item.h"

enum class HeroType { Mayor, Ancient };

class Hero : public chara {
private:
    HeroType htype;
    int Act_max;
    int Act_remaining;
    bool in_hospital;

public:
    Hero(const std::string& name, const std::string& loc, HeroType htype);

    int getAct_max() const;
    int getAct_rem() const;
    bool getin_hos() const;
    HeroType getHType() const;
    const std::vector<Item>& getItems() const;

    void setAct_max(int a);
    void setAct_rem(int a);
    bool setin_hos(bool a);


    void startTurn();
    void heal();
    void useAct();
    void send_hos();
    void move(const std::string& newLoc);
    void move_valliger(std::shared_ptr<chara> villager, const std::string& newLoc);
    void pickup_item(const Item& item);
    void use_Item(const std::string& itemname);
    void defeatMonster(std::shared_ptr<Monster> monster);
};

class Mayor : public Hero {
public:
    Mayor();
};

class Ancient : public Hero {
public:
    Ancient();
    void takeSpcial_item(const std::string& itemLoc, std::vector<std::string>& items);
};

#endif
