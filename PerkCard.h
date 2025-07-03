#ifndef PERKCARD_H
#define PERKCARD_H

#include <bits/stdc++.h>
#include "character.h"
#include "hero.h"
#include "Board.h"

class Game;

class PerkCard {
private:
    std::string name;
    int quantity;

public:
    PerkCard(const std::string& name, int quantity);
    PerkCard(const PerkCard&) = default;
    PerkCard& operator=(const PerkCard&) = default;
    bool activate();
    void applyEffect(Hero& hero, Board& board, std::map<std::string, std::shared_ptr<chara>>& persons, Game& game);
    std::string getDetails() const;
    std::string getName() const { return name; }
    int getQuantity() const { return quantity; }
};

std::vector<PerkCard> createPerkCards();

#endif