#ifndef GAME_H
#define GAME_H

#include <bits/stdc++.h>
#include "ActionSystem.h"
#include "Board.h"
#include "character.h"
#include "Item.h"
#include "hero.h"
#include "monster.h"
#include "PerkCard.h"
#include "MonsterCard.h"
#include "TerrorLevelTracker.h"

class Game {
private:
    Board gameBoard;
    TerrorLevelTracker gameTerrorLevelTracker;
    ActionSystem gameActionSystem;
    std::vector<PerkCard> gamePerkCard;
    std::vector<std::shared_ptr<Hero>> gameHero;
    std::vector<std::shared_ptr<Monster>> gameMonster;
    std::vector<std::shared_ptr<villager>> gamevillager;
    std::vector<Item> gameitem;
    std::vector<MonsterCard> monsterCards;
    std::vector<std::string> gameLogs;

    int playerindex;
    bool gameOver;

public:
    Game();
    void assignHeroes();
    void setGame();
    void loop_Game();
    void heroGame(Hero& hero);
    void monsterGame();
    void check_win();
    void show_Gamestate();
    void addLog(const std::string& log);
    const std::vector<PerkCard>& getActivePerks() const { return gamePerkCard; }
};

#endif
