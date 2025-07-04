// File: monster.cpp
#include <bits/stdc++.h>
#include "monster.h"

Monster::Monster(const std::string& name, const std::string& loc, monsterType mType, const std::string& npower, const std::vector<ItemType>& itemType, int power)
    : chara(name, loc, Typechara::MONSTER), mType(mType), is_defeated(false), Npower(npower), power(power), itemType(itemType) {}

void Monster::setPower(int npower) { power = npower; }
void Monster::addTargetLoc(std::string LOCC) { TargetLoc.push_back(LOCC); }

bool Monster::checkItem_defeat(const std::vector<Item>& items) const {
    for (const auto& item : items) {
        if (std::find(itemType.begin(), itemType.end(), item.getType()) != itemType.end()) {
            return true;
        }
    }
    return false;
}

bool Monster::can_defend() const {
    return power > 0;
}

Dracula::Dracula(const std::string& loc)
    : Monster("Dracula", loc, monsterType::DRACULA, "Coffin Power", {ItemType::Red}, 10), coffins_marker(0) {}

Invisible_man::Invisible_man(const std::string& loc)
    : Monster("Invisible Man", loc, monsterType::INVISIBLE_MAN, "Invisibility", {ItemType::Yellow, ItemType::Red}, 8), itemPlaced(0) {}