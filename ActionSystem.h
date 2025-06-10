#ifndef ACTIONSYSTEM_HPP
#define ACTIONSYSTEM_HPP

#include <string>
#include <vector>
#include <map>
#include <set>
#include <iostream>
#include "hero.h"
#include "monster.h"
#include "Item.h"

enum class ActionType {MOVE, GUIDE, PICK_UP, ADVANCE, DEFEAT, SPECIAL_ACTION };

using Location = std::string;

/*struct Item {
    std::string name;
    ItemType type;
    Item(const std::string& name, ItemType type) : name(name), type(type) {}
    std::string getName() const { return name; }
};*/

struct Person {
    std::string name;
    Location location;
    bool isHero;
    bool isInjured;
    Typechara type; 
    std::vector<Item> items;

    Person(const std::string& name, const Location& loc, bool hero, Typechara type)
        : name(name), location(loc), isHero(hero), isInjured(false), type(type) {}
};

/*struct PerkCard {
    std::string description;
    void applyEffect(Person& hero);
};*/

class ActionSystem {
    std::map<std::string, Person> persons;
    std::map<Location, std::set<std::string>> locations;
    std::vector<PerkCard> activePerks;
    int heroActionsLeft;
    Person* currentHero;

    void movePerson(Person& p, const Location& newLoc);
    bool isNeighbor(const Location& from, const Location& to);

public:
    ActionSystem(const std::vector<Person>& heroes);

    void startHeroTurn(const std::string& heroName, int actionsCount);
    void addPerkCard(const PerkCard& card);
    void playPerkCard(int index);

    bool performAction(ActionType action);

    int getActionsLeft() const;
    void showStatus();
};

#endif 
