// File: game.h
#ifndef GAME_H
#define GAME_H

#include <bits/stdc++.h>
#include "character.h"
#include "hero.h"
#include "monster.h"
#include "Board.h"
#include "ActionSystem.h"
#include "PerkCard.h"
#include "MonsterCard.h"

class Game {
private:
    std::vector<std::shared_ptr<chara>> gameCharacters;
    std::vector<std::shared_ptr<Monster>> gameMonster;
    std::vector<Item> gameitem;
    std::vector<PerkCard> perkCards;
    std::vector<MonsterCard> monsterCards;
    ActionSystem actionSystem;
    Board board;
    int terrorLevel;

public:
    Game();
    void addHero(std::shared_ptr<Hero> hero);
    void addVillager(std::shared_ptr<villager> villager);
    void addMonster(std::shared_ptr<Monster> monster);
    void setGame();
    void monsterGame();
    void check_win();
    std::vector<PerkCard> getActivePerks() const;
    std::vector<Item>& getGameItem();
    std::vector<std::shared_ptr<chara>>& getGameCharacters() { return gameCharacters; }
    std::vector<std::shared_ptr<Monster>>& getGameMonster() { return gameMonster; }
    Board& getBoard() { return board; }
    int& getTerrorLevel() { return terrorLevel; }
};

#endif