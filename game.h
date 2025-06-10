#ifndef GAME_H
#define GAME_H


#include <bits/stdc++.h>

#include "ActionSystem.h"
#include "board.h"
#include "charactor.h"
#include "Item.h"
#include "hero.h"
#include "monster.h"
#include "MonsterCard.h"
#include "PerkCard.h"
#include "TerrorLevelTracker.h"


class Game {
    private:
    Board gameBoard;
    TerrorLevelTracker gameTerrorLevelTracker;
    ActionSystem gameActionSystem;
    std::vector<std::shared_ptr<Hero>> gameHero;
    std::vector<std::shared_ptr<Monster>> gameMonster;
    std::vector<std::shared_ptr<villager>> gamevillager;
    std::vector<std::shared_ptr<Item>> gameitem;

    int playerindex;
    bool gameOver;

    public:
    Game();
    
    void setGame();
    void loop_Game();
    void heroGame(Hero& hero);
    void monsterGame();
    void chech_win();
    void show_Gamestate();
    



};

#endif
