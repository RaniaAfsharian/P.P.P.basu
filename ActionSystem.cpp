#include "ActionSystem.hpp"
#include <iostream>
#include <cmath>

void PerkCard::applyEffect(Person& hero) {
    std::cout << "Applying perk: " << description << " for " << hero.name << "\n";
}

ActionSystem::ActionSystem(const std::vector<Person>& heroes)
    : heroActionsLeft(0), currentHero(nullptr) {
    for (const auto& p : heroes) {
        persons[p.name] = p;
        locations[p.location].insert(p.name);
    }
}

void ActionSystem::movePerson(Person& p, const Location& newLoc) {
    locations[p.location].erase(p.name);
    p.location = newLoc;
    locations[newLoc].insert(p.name);
}

bool ActionSystem::isNeighbor(const Location& from, const Location& to) {
    if (from.size() != to.size()) return false;
    return std::abs(from.back() - to.back()) == 1 && from.substr(0, from.size() - 1) == to.substr(0, to.size() - 1);
}

void ActionSystem::startHeroTurn(const std::string& heroName, int actionsCount) {
    auto it = persons.find(heroName);
    if (it == persons.end() || !it->second.isHero) {
        std::cout << "Hero not found!\n";
        return;
    }
    currentHero = &it->second;
    heroActionsLeft = actionsCount;
    activePerks.clear();
    std::cout << "Turn started for hero: " << heroName << ", Actions = " << heroActionsLeft << "\n";
}

void ActionSystem::addPerkCard(const PerkCard& card) {
    activePerks.push_back(card);
}

void ActionSystem::playPerkCard(int index) {
    if (index < 0 || index >= static_cast<int>(activePerks.size())) {
        std::cout << "Invalid perk card index\n";
        return;
    }
    activePerks[index].applyEffect(*currentHero);
    activePerks.erase(activePerks.begin() + index);
}

bool ActionSystem::performAction(ActionType action) {
    if (heroActionsLeft <= 0 && action != ActionType::SPECIAL_ACTION) {
        std::cout << "No Actions left\n";
        return false;
    }

    switch (action) {
        case ActionType::MOVE: {
            std::cout << "Enter destination location to move: ";
            Location dest;
            std::cin >> dest;

            if (!isNeighbor(currentHero->location, dest)) {
                std::cout << "Destination not adjacent.\n";
                return false;
            }

            movePerson(*currentHero, dest);

            std::cout << currentHero->name << " moved to " << dest << "\n";
            heroActionsLeft--;
            return true;
        }
        case ActionType::GUIDE: {
            std::cout << "Enter villager name to guide: ";
            std::string villagerName;
            std::cin >> villagerName;

            auto it = persons.find(villagerName);
            if (it == persons.end() || it->second.isHero) {
                std::cout << "Invalid villager\n";
                return false;
            }

            Person& villager = it->second;
            bool canMove = false;
            if (villager.location == currentHero->location) {
                std::cout << "Enter neighbor location to move villager: ";
                Location newLoc;
                std::cin >> newLoc;
                if (isNeighbor(currentHero->location, newLoc)) {
                    movePerson(villager, newLoc);
                    canMove = true;
                }
            } else if (isNeighbor(villager.location, currentHero->location)) {
                movePerson(villager, currentHero->location);
                canMove = true;
            }
            if (!canMove) {
                std::cout << "Cannot guide villager as requested.\n";
                return false;
            }
            std::cout << "Villager " << villager.name << " guided successfully.\n";
            heroActionsLeft--;
            return true;
        }
        case ActionType::PICK_UP: {
            std::cout << "Picking up all items in current location.\n";

            Item exampleItem{"MedKit"};
            currentHero->items.push_back(exampleItem);

            heroActionsLeft--;
            return true;
        }
        case ActionType::ADVANCE: {
            std::cout << currentHero->name << " performed Advance Action on a monster mission\n";
            heroActionsLeft--;
            return true;
        }
        case ActionType::DEFEAT: {
            std::cout << "Attempting to defeat monster in current location...\n";
            bool canDefeat = true;
            if (canDefeat) {
                std::cout << "Monster defeated!\n";
                heroActionsLeft--;
                return true;
            } else {
                std::cout << "Cannot defeat, conditions not met.\n";
                return false;
            }
        }
        case ActionType::SPECIAL_ACTION: {
            if (currentHero->name == "Archaeologist") {
                std::cout << "Using Archaeologist's special ability to pick items from neighbor house.\n";

                Item specialItem{"AncientRelic"};
                currentHero->items.push_back(specialItem);
                return true;
            } else if (currentHero->name == "Mayor") {
                std::cout << "Mayor has no special action.\n";
                return false;
            }
            return false;
        }
    }
    return false;
}

int ActionSystem::getActionsLeft() const {
    return heroActionsLeft;
}

void ActionSystem::showStatus() {
    std::cout << "Hero: " << currentHero->name
              << ", Location: " << currentHero->location
              << ", Actions left: " << heroActionsLeft
              << ", Items: ";
    for (const auto& it : currentHero->items)
        std::cout << it.name << ", ";
    std::cout << "\n";
}