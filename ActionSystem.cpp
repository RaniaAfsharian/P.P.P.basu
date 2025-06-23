#include <iostream>
#include "ActionSystem.h"

using namespace std;

ActionSystem::ActionSystem(Board& b) : heroActionsLeft(0), board(b) {}

ActionSystem::ActionSystem(const vector<shared_ptr<chara>>& persons, Board& b)
    : heroActionsLeft(0), board(b) {
    for (const auto& p : persons) {
        this->persons[p->getName()] = p;
        locations[p->getLoc()].insert(p->getName());
    }
}

void ActionSystem::movePerson(shared_ptr<chara> p, const Location& newLoc) {
    locations[p->getLoc()].erase(p->getName());
    p->setLoc(newLoc);
    locations[newLoc].insert(p->getName());
}

bool ActionSystem::isNeighbor(const Location& from, const Location& to) {
    auto paths = board.getAvailablePaths(from);
    return paths.find(to) != paths.end();
}

void ActionSystem::startHeroTurn(const string& heroName, int actionsCount) {
    auto it = persons.find(heroName);
    if (it == persons.end() || it->second->getType() != Typechara::HERO) {
        cout << "Hero not found!" << endl;
        return;
    }
    currentHero = dynamic_pointer_cast<Hero>(it->second);
    heroActionsLeft = actionsCount;
    activePerks.clear();
    cout << "Turn started for hero: " << heroName << ", actions = " << heroActionsLeft << endl;
}

void ActionSystem::addPerkCard(const PerkCard& card) {
    activePerks.push_back(card);
}

void ActionSystem::playPerkCard(int index) {
    if (index < 0 || index >= static_cast<int>(activePerks.size())) {
        cout << "Invalid perk card index!" << endl;
        return;
    }
    activePerks[index].applyEffect(*currentHero);
    activePerks.erase(activePerks.begin() + index);
}

bool ActionSystem::performAction(ActionType action, const vector<shared_ptr<Monster>>& monsters) {
    if (heroActionsLeft <= 0 && action != ActionType::SPECIAL_ACTION) {
        cout << "No actions remaining!" << endl;
        return false;
    }

    switch (action) {
    case ActionType::MOVE: {
        cout << "Enter destination for movement: ";
        Location dest;
        cin >> dest;

        if (!isNeighbor(currentHero->getLoc(), dest)) {
            cout << "Destination is not adjacent!" << endl;
            return false;
        }

        movePerson(currentHero, dest);
        currentHero->useAct();
        cout << currentHero->getName() << " moved to " << dest << endl;
        heroActionsLeft--;
        return true;
    }
    case ActionType::GUIDE: {
        cout << "Enter villager name to guide: ";
        string villagerName;
        cin >> villagerName;

        auto it = persons.find(villagerName);
        if (it == persons.end() || it->second->getType() != Typechara::VILLAGER) {
            cout << "Invalid villager!" << endl;
            return false;
        }

        auto Villager =dynamic_pointer_cast<villager>(it->second);
        bool canMove = false;
        if (Villager->getLoc() == currentHero->getLoc()) {
            cout << "Enter neighboring location for villager movement: ";
            Location newLoc;
            cin >> newLoc;
            if (isNeighbor(currentHero->getLoc(), newLoc)) {
                movePerson(Villager, newLoc);
                currentHero->useAct();
                canMove = true;
                if (newLoc == Villager->getSafeLoc()) {
                    Villager->rescue();
                }
            }
        } else if (isNeighbor(Villager->getLoc(), currentHero->getLoc())) {
            movePerson(Villager, currentHero->getLoc());
            currentHero->useAct();
            canMove = true;
        }
        if (!canMove) {
                        cout << "Cannot guide villager in this way!" << endl;
            return false;
        }
        cout << "Villager " << Villager->getName() << " guided successfully!" << endl;
        heroActionsLeft--;
        return true;

    }
    case ActionType::PICK_UP: {
        cout << "Collecting all items at current location..." << endl;
        auto items = board.getItems().find(currentHero->getLoc());
        if (items != board.getItems().end()) {
            for (const auto& item : items->second) {
                currentHero->pickup_item(item);
                cout << "Item " << item.getName() << " picked up!" << endl;
            }
            board.removeItems(currentHero->getLoc());
        } else {
            cout << "No items found at " << currentHero->getLoc() << "!" << endl;
        }
        currentHero->useAct();
        heroActionsLeft--;
        return true;
    }
    case ActionType::ADVANCE: {
        cout << "Select monster to advance mission (Dracula/Invisible Man): ";
        string monsterName;
        cin >> monsterName;
        shared_ptr<Monster> target = nullptr;
        for (const auto& monster : monsters) {
            if (monster->getName() == monsterName && !monster->getIs_defeated()) {
                target = monster;
                break;
            }
        }
        if (!target) {
            cout << "Invalid monster!" << endl;
            return false;
        }
        if (target->getMtype() == monsterType::DRACULA) {
            auto dracula = dynamic_pointer_cast<Dracula>(target);
            if (currentHero->getLoc() == "Crypt" || currentHero->getLoc() == "Graveyard" ||
                currentHero->getLoc() == "Cave" || currentHero->getLoc() == "Dungeon") {
                dracula->Coffin();
                cout << "Dracula's coffin destroyed! (" << dracula->getCoffins_marker() << "/4)" << endl;
            } else {
                cout << "You must be in Crypt, Graveyard, Cave, or Dungeon!" << endl;
                return false;
            }
        } else if (target->getMtype() == monsterType::INVISIBLE_MAN) {
            auto invisible = dynamic_pointer_cast<Invisible_man>(target);
            if (currentHero->getLoc() == "Inn" || currentHero->getLoc() == "Barn" ||
                currentHero->getLoc() == "Institute" || currentHero->getLoc() == "Laboratory" ||
                currentHero->getLoc() == "Mansion") {
                invisible->ItemPlaced();
                cout << "Evidence collected! (" << invisible->getItemPlaced() << "/5)" << endl;
            } else {
                cout << "You must be in Inn, Barn, Institute, Laboratory, or Mansion!" << endl;
                return false;
            }
        }
        currentHero->useAct();
        heroActionsLeft--;
        return true;
    }
    case ActionType::DEFEAT: {
        cout << "Attempting to defeat monster at current location..." << endl;
        shared_ptr<Monster> targetMon = nullptr;
        for (const auto& p : persons) {
            if (p.second->getType() == Typechara::MONSTER &&
                p.second->getLoc() == currentHero->getLoc() &&
                !dynamic_pointer_cast<Monster>(p.second)->getIs_defeated()) {
                targetMon = dynamic_pointer_cast<Monster>(p.second);
                break;
            }
        }
        if (targetMon) {
            currentHero->defeatMonster(targetMon);
            if (targetMon->getIs_defeated()) {
                cout << "Monster defeated!" << endl;
            } else {
                cout << "Cannot defeat the monster!" << endl;
            }
            currentHero->useAct();
            heroActionsLeft--;
            return true;
        } else {
            cout << "No monster found at this location!" << endl;
            return false;
        }
    }
    case ActionType::SPECIAL_ACTION: {
        if (currentHero->getHType() == HeroType::Ancient) {
            cout << "Using Ancient's special ability to pick up item from neighboring location..." << endl;
            cout << "Select location: ";
            Location neighborLoc;
            cin >> neighborLoc;
            if (!isNeighbor(currentHero->getLoc(), neighborLoc)) {
                cout << "Selected location is not adjacent!" << endl;
                return false;
            }
            auto items = board.getItems().find(neighborLoc);
            if (items != board.getItems().end()) {
                for (const auto& item : items->second) {
                    currentHero->pickup_item(item);
                    cout << "Item " << item.getName() << " picked up from " << neighborLoc << "!" << endl;
                }
                board.removeItems(neighborLoc);
            } else {
                cout << "No items found at " << neighborLoc << "!" << endl;
            }
            currentHero->useAct();
            return true;
        } else {
            cout << "Mayor has no special ability!" << endl;
            return false;
        }
    }
    }

    return false;
}

int ActionSystem::getActionsLeft() const {
    return heroActionsLeft;
}

void ActionSystem::showStatus() const {
    cout << "Hero: " << currentHero->getName()
         << ", Location: " << currentHero->getLoc()
         << ", Actions remaining: " << heroActionsLeft
         << ", Items: ";
    for (const auto& item : currentHero->getItems()) {
        cout << item.getName() << ", ";
    }
    cout << endl;
}
