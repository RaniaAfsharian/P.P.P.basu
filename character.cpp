// File: character.cpp
#include <bits/stdc++.h>
#include "character.h"
#include "Board.h"

chara::chara(const std::string &name, const std::string &loc, Typechara type)
    : name(name), loc(loc), type(type), health(100) {}

int chara::getHealth() const {
    return health;
}

const std::vector<Item> &chara::getItems() const {
    return items;
}

void chara::setName(std::string n) {
    name = n;
}

void chara::setHealth(int hea) {
    health = hea;
}

void chara::takeHarm(int amount) {
    health -= amount;
    if (health < 0) health = 0;
}

bool chara::can_defend() const {
    return health > 0;
}

villager::villager(const std::string &name, const std::string &loc, const std::string &safeLoc)
    : chara(name, loc, Typechara::VILLAGER), isSaved(false), safeLoc(safeLoc) {}