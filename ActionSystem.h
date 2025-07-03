#ifndef ACTIONSYSTEM_H
#define ACTIONSYSTEM_H

#include <bits/stdc++.h>
#include "character.h"
#include "hero.h"
#include "monster.h"
#include "Board.h"
#include "PerkCard.h"

class Game;

enum class ActionType { MOVE, GUIDE, PICK_UP, ADVANCE, DEFEAT, SPECIAL_ACTION };

class ActionSystem {
private:
    int heroActionsLeft;
    std::shared_ptr<Hero> currentHero;
    std::map<std::string, std::shared_ptr<chara>> persons;
    std::map<std::string, std::set<std::string>> locations;
    std::vector<PerkCard> activePerks;
    Board& board;
    Game& game;

public:
    ActionSystem(Board& board, Game& game);
    ActionSystem(const std::vector<std::shared_ptr<chara>>& persons, Board& board, Game& game);
    void movePerson(std::shared_ptr<chara> p, const std::string& newLoc);
    bool isNeighbor(const std::string& from, const std::string& to);
    void startHeroTurn(const std::string& heroName, int actionsCount);
    void addPerkCard(const PerkCard& card);
    void playPerkCard(int index);
    bool performAction(ActionType action, const std::vector<std::shared_ptr<Monster>>& monsters);
    int getActionsLeft() const;
    void showStatus() const;
    std::vector<PerkCard> getActivePerks() const;
};

#endif