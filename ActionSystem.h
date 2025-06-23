#ifndef ACTIONSYSTEM_H
#define ACTIONSYSTEM_H

#include <string>
#include <vector>
#include <map>
#include <set>
#include <memory>
#include "hero.h"
#include "character.h"
#include "monster.h"
#include "Item.h"
#include "PerkCard.h"
#include "Board.h"

enum class ActionType {
    MOVE,
    GUIDE,
    PICK_UP,
    ADVANCE,
    DEFEAT,
    SPECIAL_ACTION
};

using Location = std::string;

class ActionSystem {
private:
    std::map<std::string, std::shared_ptr<chara>> persons;
    std::map<Location, std::set<std::string>> locations;
    std::vector<PerkCard> activePerks;
    int heroActionsLeft;
    std::shared_ptr<Hero> currentHero;
    Board& board;
  

    void movePerson(std::shared_ptr<chara> p, const Location& newLoc);
    bool isNeighbor(const Location& from, const Location& to);

public:
    ActionSystem(const std::vector<std::shared_ptr<chara>>& persons,Board& board);
    ActionSystem(Board& board);
    ActionSystem();
    
    void startHeroTurn(const std::string& heroName, int actionsCount);
    void addPerkCard(const PerkCard& card);
    void playPerkCard(int index);

    bool performAction(ActionType action , const std::vector<std::shared_ptr<Monster>>& monsters);

    int getActionsLeft() const;
    void showStatus() const;
};

#endif
