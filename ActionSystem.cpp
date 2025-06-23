#include "ActionSystem.h"

#include <iostream>


ActionSystem::ActionSystem() : board(board){}

ActionSystem::ActionSystem(Board& b) : heroActionsLeft(0), board(b) {}

ActionSystem::ActionSystem(const std::vector<std::shared_ptr<chara>>& persons, Board& b)
    : heroActionsLeft(0), board(b) {
    for (const auto& p : persons) {
        this->persons[p->getName()] = p;
        locations[p->getLoc()].insert(p->getName());
    }
}

void ActionSystem::movePerson(std::shared_ptr<chara> p, const Location& newLoc) {
    locations[p->getLoc()].erase(p->getName());
    p->setLoc(newLoc);
    locations[newLoc].insert(p->getName());
}

bool ActionSystem::isNeighbor(const Location& from, const Location& to) {
    auto paths = board.getAvailablePaths(from);
    return paths.find(to) != paths.end();
}

void ActionSystem::startHeroTurn(const std::string& heroName, int actionsCount) {
    auto it = persons.find(heroName);
    if (it == persons.end() || it->second->getType() != Typechara::HERO) {
        std::cout << "Hero not found!" <<std::endl;
        return;
    }
    currentHero = std::dynamic_pointer_cast<Hero>(it->second);
    heroActionsLeft = actionsCount;
    activePerks.clear();
    std::cout << "Turn started for hero: " << heroName << ", actions = " << heroActionsLeft <<std::endl;
}

void ActionSystem::addPerkCard(const PerkCard& card) {
    activePerks.push_back(card);
}

void ActionSystem::playPerkCard(int index) {
    if (index < 0 || index >= static_cast<int>(activePerks.size())) {
        std::cout << "Invalid perk card index!" << std::endl;
        return;
    }
    activePerks[index].applyEffect(*currentHero);
    activePerks.erase(activePerks.begin() + index);
}

bool ActionSystem::performAction(ActionType action, const std::vector<std::shared_ptr<Monster>>& monsters) {
    if (heroActionsLeft <= 0 && action != ActionType::SPECIAL_ACTION) {
        std::cout << "No actions remaining!" <<std::endl;
        return false;
    }

    switch (action) {
    case ActionType::MOVE: {
        std::cout << "Enter destination for movement: ";
        Location dest;
        std::cin >> dest;

        if (!isNeighbor(currentHero->getLoc(), dest)) {
            std::cout << "Destination is not adjacent!" <<std::endl;
            return false;
        }

        movePerson(currentHero, dest);
        currentHero->useAct();
        std::cout << currentHero->getName() << " moved to " << dest <<std::endl;
        heroActionsLeft--;
        return true;
    }
    case ActionType::GUIDE: {
        std::cout << "Enter villager name to guide: ";
        std::string villagerName;
        std::cin >> villagerName;

        auto it = persons.find(villagerName);
        if (it == persons.end() || it->second->getType() != Typechara::VILLAGER) {
            std::cout << "Invalid villager!" <<std::endl;
            return false;
        }

        auto Villager =std::dynamic_pointer_cast<villager>(it->second);
        bool canMove = false;
        if (Villager->getLoc() == currentHero->getLoc()) {
            std::cout << "Enter neighboring location for villager movement: ";
            Location newLoc;
            std::cin >> newLoc;
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
            std::cout << "Cannot guide villager in this way!" <<std::endl;
            return false;
        }
        std::cout << "Villager " << Villager->getName() << " guided successfully!" <<std::endl;
        heroActionsLeft--;
        return true;

    }
    case ActionType::PICK_UP: {
        std::cout << "Collecting all items at current location..." <<std::endl;
        auto items = board.getItems().find(currentHero->getLoc());
        if (items != board.getItems().end()) {
            for (const auto& item : items->second) {
                currentHero->pickup_item(item);
                std::cout << "Item " << item.getName() << " picked up!" <<std::endl;
            }
            board.removeItems(currentHero->getLoc());
        } else {
            std::cout << "No items found at " << currentHero->getLoc() << "!" <<std::endl;
        }
        currentHero->useAct();
        heroActionsLeft--;
        return true;
    }
    case ActionType::ADVANCE: {
        std::cout << "Select monster to advance mission (Dracula/Invisible Man): ";
        std::string monsterName;
        std::cin >> monsterName;
        std::shared_ptr<Monster> target = nullptr;
        for (const auto& monster : monsters) {
            if (monster->getName() == monsterName && !monster->getIs_defeated()) {
                target = monster;
                break;
            }
        }
        if (!target) {
            std::cout << "Invalid monster!" << std::endl;
            return false;
        }
        if (target->getMtype() == monsterType::DRACULA) {
            auto dracula = std::dynamic_pointer_cast<Dracula>(target);
            if (currentHero->getLoc() == "Crypt" || currentHero->getLoc() == "Graveyard" ||
                currentHero->getLoc() == "Cave" || currentHero->getLoc() == "Dungeon") {
                dracula->Coffin();
                std::cout << "Dracula's coffin destroyed! (" << dracula->getCoffins_marker() << "/4)" <<std::endl;
            } else {
                std::cout << "You must be in Crypt, Graveyard, Cave, or Dungeon!" <<std::endl;
                return false;
            }
        } else if (target->getMtype() == monsterType::INVISIBLE_MAN) {
            auto invisible = std::dynamic_pointer_cast<Invisible_man>(target);
            if (currentHero->getLoc() == "Inn" || currentHero->getLoc() == "Barn" ||
                currentHero->getLoc() == "Institute" || currentHero->getLoc() == "Laboratory" ||
                currentHero->getLoc() == "Mansion") {
                invisible->ItemPlaced();
                std::cout << "Evidence collected! (" << invisible->getItemPlaced() << "/5)" <<std::endl;
            } else {
                std::cout << "You must be in Inn, Barn, Institute, Laboratory, or Mansion!" <<std::endl;
                return false;
            }
        }
        currentHero->useAct();
        heroActionsLeft--;
        return true;
    }
    case ActionType::DEFEAT: {
        std::cout << "Attempting to defeat monster at current location..." <<std::endl;
        std::shared_ptr<Monster> targetMon = nullptr;
        for (const auto& p : persons) {
            if (p.second->getType() == Typechara::MONSTER &&
                p.second->getLoc() == currentHero->getLoc() &&
                !std::dynamic_pointer_cast<Monster>(p.second)->getIs_defeated()) {
                targetMon = std::dynamic_pointer_cast<Monster>(p.second);
                break;
            }
        }
        if (targetMon) {
            currentHero->defeatMonster(targetMon);
            if (targetMon->getIs_defeated()) {
                std::cout << "Monster defeated!" <<std::endl;
            } else {
                std::cout << "Cannot defeat the monster!" <<std::endl;
            }
            currentHero->useAct();
            heroActionsLeft--;
            return true;
        } else {
            std::cout << "No monster found at this location!" <<std::endl;
            return false;
        }
    }
    case ActionType::SPECIAL_ACTION: {
        if (currentHero->getHType() == HeroType::Ancient) {
            std::cout << "Using Ancient's special ability to pick up item from neighboring location..." <<std::endl;
            std::cout << "Select location: ";
            Location neighborLoc;
            std::cin >> neighborLoc;
            if (!isNeighbor(currentHero->getLoc(), neighborLoc)) {
                std::cout << "Selected location is not adjacent!" <<std::endl;
                return false;
            }
            auto items = board.getItems().find(neighborLoc);
            if (items != board.getItems().end()) {
                for (const auto& item : items->second) {
                    currentHero->pickup_item(item);
                    std::cout << "Item " << item.getName() << " picked up from " << neighborLoc << "!" <<std::endl;
                }
                board.removeItems(neighborLoc);
            } else {
                std::cout << "No items found at " << neighborLoc << "!" <<std::endl;
            }
            currentHero->useAct();
            return true;
        } else {
            std::cout << "Mayor has no special ability!" <<std::endl;
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
    std::cout << "Hero: " << currentHero->getName()
         << ", Location: " << currentHero->getLoc()
         << ", Actions remaining: " << heroActionsLeft
         << ", Items: ";
    for (const auto& item : currentHero->getItems()) {
        std::cout << item.getName() << ", ";
    }
    std::cout << std::endl;
}
