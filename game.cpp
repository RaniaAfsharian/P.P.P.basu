// File: game.cpp
#include <bits/stdc++.h>
#include "game.h"
#include "MonsterCard.h"

Game::Game() : terrorLevel(0), actionSystem(board, *this) {
    srand(time(0));
}

void Game::addHero(std::shared_ptr<Hero> hero) {
    gameCharacters.push_back(hero);
}

void Game::addVillager(std::shared_ptr<villager> villager) {
    gameCharacters.push_back(villager);
}

void Game::addMonster(std::shared_ptr<Monster> monster) {
    gameCharacters.push_back(monster);
    gameMonster.push_back(monster);
}

void Game::setGame() {
    gameitem = createItems();
    perkCards = createPerkCards();
    monsterCards = createMonsterCards();
    board.initializeBoard();

    std::vector<std::string> loc;
    for (const auto& pair : board.getItems()) {
        loc.push_back(pair.first);
    }

    for (int i = 0; i < 12 && !gameitem.empty(); ++i) {
        int itemIndex = rand() % gameitem.size();
        int locIndex = rand() % loc.size();
        board.addItem(loc[locIndex], gameitem[itemIndex]);
        gameitem.erase(gameitem.begin() + itemIndex);
    }
}

void Game::monsterGame() {
    MonsterCard card = monsterCards[rand() % monsterCards.size()];
    card.applyEvent(*this);

    for (auto& monster : gameMonster) {
        if (!monster->getIs_defeated()) {
            monster->useSpecialP();
        }
    }

    bool attackApplied = false;
    for (auto& monster : gameMonster) {
        if (!monster->getIs_defeated()) {
            attackApplied = card.applyMonsterStrike(monster, gameCharacters, board, terrorLevel, *this);
            if (attackApplied) {
                break;
            }
        }
    }

    for (auto& monster : gameMonster) {
        if (!monster->getIs_defeated() && monster->getMtype() == monsterType::INVISIBLE_MAN) {
            auto invisible = std::dynamic_pointer_cast<Invisible_man>(monster);
            std::shared_ptr<villager> nearestVillager = nullptr;
            int minDistance = INT_MAX;
            for (const auto& p : gameCharacters) {
                if (p->getType() == Typechara::VILLAGER && !std::dynamic_pointer_cast<villager>(p)->getIs_saved()) {
                    int distance = board.findShortestPath(monster->getLoc(), p->getLoc()).size();
                    if (distance < minDistance) {
                        minDistance = distance;
                        nearestVillager = std::dynamic_pointer_cast<villager>(p);
                    }
                }
            }
            if (nearestVillager) {
                auto path = board.findShortestPath(monster->getLoc(), nearestVillager->getLoc());
                if (path.size() >= 2) {
                    monster->setLoc(path[1]);
                    std::cout << "Invisible Man moved to " << path[1] << std::endl;
                    if (path.size() >= 3) {
                        monster->setLoc(path[2]);
                        std::cout << "Invisible Man moved to " << path[2] << std::endl;
                    }
                }
            }
        }
    }
}

void Game::check_win() {
    bool allMonstersDefeated = true;
    for (const auto& monster : gameMonster) {
        if (!monster->getIs_defeated() && monster->can_defeated()) {
            monster->defeat();
            std::cout << monster->getName() << " defeated!" << std::endl;
            if (monster->getMtype() == monsterType::INVISIBLE_MAN) {
                auto items = createItems();
                for (const auto& item : items) {
                    if (item.getType() == ItemType::Red || item.getType() == ItemType::Yellow) {
                        gameitem.push_back(item);
                    }
                }
                std::cout << "Items used for Invisible Man returned to bag!" << std::endl;
            }
        }
        if (!monster->getIs_defeated()) {
            allMonstersDefeated = false;
        }
    }
    if (allMonstersDefeated) {
        std::cout << "All monsters defeated! Heroes win!" << std::endl;
    }
}

std::vector<PerkCard> Game::getActivePerks() const {
    return actionSystem.getActivePerks();
}

std::vector<Item>& Game::getGameItem() {
    return gameitem;
}