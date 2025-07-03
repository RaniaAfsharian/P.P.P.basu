#include <bits/stdc++.h>
#include "ActionSystem.h"
#include "game.h"
#include "MonsterCard.h"

ActionSystem::ActionSystem(Board& board, Game& game) : heroActionsLeft(0), currentHero(nullptr), board(board), game(game) {}

ActionSystem::ActionSystem(const std::vector<std::shared_ptr<chara>>& persons, Board& board, Game& game)
    : heroActionsLeft(0), currentHero(nullptr), board(board), game(game) {
    for (const auto& p : persons) {
        this->persons[p->getName()] = p;
    }
    for (const auto& pair : board.getItems()) {
        locations[pair.first] = board.getNeighbors(pair.first);
    }
}

void ActionSystem::movePerson(std::shared_ptr<chara> p, const std::string& newLoc) {
    if (isNeighbor(p->getLoc(), newLoc)) {
        p->setLoc(newLoc);
        std::cout << p->getName() << " moved to " << newLoc << std::endl;
    } else {
        std::cout << "Invalid move: " << p->getLoc() << " to " << newLoc << " are not neighbors." << std::endl;
    }
}

bool ActionSystem::isNeighbor(const std::string& from, const std::string& to) {
    auto it = locations.find(from);
    if (it != locations.end()) {
        return it->second.find(to) != it->second.end();
    }
    return false;
}

void ActionSystem::startHeroTurn(const std::string& heroName, int actionsCount) {
    auto it = persons.find(heroName);
    if (it != persons.end() && it->second->getType() == Typechara::HERO) {
        currentHero = std::dynamic_pointer_cast<Hero>(it->second);
        heroActionsLeft = actionsCount;
        std::cout << heroName << "'s turn started with " << actionsCount << " actions." << std::endl;
    } else {
        std::cout << "Hero " << heroName << " not found or is not a hero." << std::endl;
    }
}

void ActionSystem::addPerkCard(const PerkCard& card) {
    activePerks.push_back(card);
}

void ActionSystem::playPerkCard(int index) {
    if (index >= 0 && index < activePerks.size() && activePerks[index].activate()) {
        activePerks[index].applyEffect(*currentHero, board, persons, game);
        activePerks.erase(activePerks.begin() + index);
    } else {
        std::cout << "Invalid perk card index or card cannot be activated." << std::endl;
    }
}

bool ActionSystem::performAction(ActionType action, const std::vector<std::shared_ptr<Monster>>& monsters) {
    if (heroActionsLeft <= 0 || !currentHero) {
        std::cout << "No actions left or no hero selected." << std::endl;
        return false;
    }

    bool actionPerformed = false;
    switch (action) {
        case ActionType::MOVE: {
            std::vector<std::string> locs;
            for (const auto& pair : locations) {
                locs.push_back(pair.first);
            }
            if (!locs.empty()) {
                int locIndex = rand() % locs.size();
                movePerson(currentHero, locs[locIndex]);
                actionPerformed = true;
            }
            break;
        }
        case ActionType::GUIDE: {
            for (auto& p : persons) {
                if (p.second->getType() == Typechara::VILLAGER && p.second->getLoc() == currentHero->getLoc()) {
                    auto villagerPtr = std::dynamic_pointer_cast<villager>(p.second);
                    if (villagerPtr && !villagerPtr->getIs_saved()) {
                        villagerPtr->rescue();
                        std::cout << "Villager " << villagerPtr->getName() << " rescued!" << std::endl;
                        actionPerformed = true;
                        break;
                    }
                }
            }
            break;
        }
        case ActionType::PICK_UP: {
            auto items = board.getItems();
            auto it = items.find(currentHero->getLoc());
            if (it != items.end() && !it->second.empty()) {
                currentHero->pickup_item(it->second[0]);
                board.removeItems(currentHero->getLoc(), it->second[0]);
                std::cout << currentHero->getName() << " picked up an item." << std::endl;
                actionPerformed = true;
            }
            break;
        }
        case ActionType::ADVANCE: {
            for (const auto& monster : monsters) {
                if (monster->getLoc() == currentHero->getLoc() && !monster->getIs_defeated()) {
                    if (monster->getMtype() == monsterType::DRACULA) {
                        auto dracula = std::dynamic_pointer_cast<Dracula>(monster);
                        dracula->Coffin();
                        std::cout << "Dracula's coffin marker increased." << std::endl;
                    } else if (monster->getMtype() == monsterType::INVISIBLE_MAN) {
                        auto invisible = std::dynamic_pointer_cast<Invisible_man>(monster);
                        invisible->ItemPlaced();
                        std::cout << "Invisible Man's item placed." << std::endl;
                    }
                    actionPerformed = true;
                    break;
                }
            }
            break;
        }
        case ActionType::DEFEAT: {
            for (const auto& monster : monsters) {
                if (monster->getLoc() == currentHero->getLoc() && !monster->getIs_defeated() && monster->can_defeated()) {
                    currentHero->defeatMonster(monster);
                    actionPerformed = true;
                    break;
                }
            }
            break;
        }
        case ActionType::SPECIAL_ACTION: {
            if (!activePerks.empty()) {
                playPerkCard(0);
                actionPerformed = true;
            }
            break;
        }
    }

    if (actionPerformed) {
        currentHero->useAct();
        heroActionsLeft--;
        std::cout << "Actions left: " << heroActionsLeft << std::endl;
    }
    return actionPerformed;
}

int ActionSystem::getActionsLeft() const {
    return heroActionsLeft;
}

void ActionSystem::showStatus() const {
    std::cout << "Current hero: " << (currentHero ? currentHero->getName() : "None") << ", Actions left: " << heroActionsLeft << std::endl;
    for (const auto& p : persons) {
        std::cout << p.second->getName() << " at " << p.second->getLoc() << std::endl;
    }
}

std::vector<PerkCard> ActionSystem::getActivePerks() const {
    return activePerks;
}