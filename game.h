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

    int playerindex;
    bool gameOver;

public:
    Game();

    void setGame();
    void loop_Game();
    void heroGame(Hero& hero);
    void monsterGame();
    void check_win();
    void show_Gamestate();
    void show_menu();
    void savegame_state();
    void loadgame_state();
};

#endif
