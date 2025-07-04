// File: hero.cpp
#include <bits/stdc++.h>
#include "hero.h"
#include "Board.h"

Hero::Hero(const std::string& name, const std::string& loc, HeroType hType)
    : chara(name, loc, Typechara::HERO), hType(hType), actions(3), Act_max(3), Act_remaining(3), in_hospital(false) {}

int Hero::getAct_max() const { return Act_max; }
int Hero::getAct_rem() const { return Act_remaining; }
bool Hero::getin_hos() const { return in_hospital; }

void Hero::setAct_max(int a) { Act_max = a; }
void Hero::setAct_rem(int a) { Act_remaining = a; }
void Hero::setin_hos(bool a) { in_hospital = a; }

void Hero::startTurn() {
    Act_remaining = Act_max;
    in_hospital = false;
}

void Hero::heal() {
    std::cout << name << " healed!" << std::endl;
}

void Hero::send_hos() {
    in_hospital = true;
    Act_remaining = 0;
    std::cout << name << " sent to hospital!" << std::endl;
}

void Hero::move(const std::string& newLoc) {
    setLoc(newLoc);
    std::cout << name << " moved to " << newLoc << std::endl;
}

void Hero::move_valliger(std::shared_ptr<chara> villager, const std::string& newLoc) {
    villager->setLoc(newLoc);
    std::cout << villager->getName() << " moved to " << newLoc << std::endl;
}

void Hero::use_Item(const std::string& itemname) {
    for (auto it = items.begin(); it != items.end(); ++it) {
        if (it->getName() == itemname) {
            items.erase(it);
            std::cout << name << " used item " << itemname << std::endl;
            break;
        }
    }
}

Mayor::Mayor() : Hero("Mayor", "Precinct", HeroType::Mayor) {}

Ancient::Ancient() : Hero("Ancient", "Museum", HeroType::Ancient) {}

void Ancient::takeSpcial_item(const std::string& itemLoc, Board& board) {
    auto items = board.getItems();
    auto it = items.find(itemLoc);
    if (it != items.end() && !it->second.empty()) {
        pickup_item(it->second[0]);
        board.removeItems(itemLoc, it->second[0]);
        std::cout << "Ancient picked up special item at " << itemLoc << std::endl;
    }
}