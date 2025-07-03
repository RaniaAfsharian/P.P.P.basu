#include <bits/stdc++.h>
#include "PerkCard.h"
#include "game.h" 

std::vector<PerkCard> createPerkCards() {
    std::vector<PerkCard> cards;
    cards.emplace_back("Visit from the Detective", 3);
    cards.emplace_back("Break of Dawn", 3);
    cards.emplace_back("Overstock", 4);
    cards.emplace_back("Late into the Night", 4);
    cards.emplace_back("Repel", 3);
    cards.emplace_back("Hurry", 3);
    return cards;
}

PerkCard::PerkCard(const std::string& name, int quantity)
    : name(name), quantity(quantity) {}

bool PerkCard::activate() {
    if (quantity <= 0) return false;
    quantity--;
    return true;
}

void PerkCard::applyEffect(Hero& hero, Board& board, std::map<std::string, std::shared_ptr<chara>>& persons, Game& game) {
    if (!activate()) {
        std::cout << "No quantity left for perk card: " << name << std::endl;
        return;
    }
    std::cout << "Applying perk card: " << name << " for " << hero.getName() << std::endl;
    if (name == "Overstock") {
        std::vector<std::string> loc;
        for (const auto& pair : board.getItems()) {
            loc.push_back(pair.first);
        }
        if (!loc.empty() && !game.getGameItem().empty()) {
            int itemIndex = rand() % game.getGameItem().size();
            int locIndex = rand() % loc.size();
            board.addItem(loc[locIndex], game.getGameItem()[itemIndex]);
            std::cout << "Item " << game.getGameItem()[itemIndex].getName() << " placed at " << loc[locIndex] << std::endl;
            game.getGameItem().erase(game.getGameItem().begin() + itemIndex);
        }
    } else if (name == "Repel" || name == "Hurry") {
        std::cout << "Select destination for " << name << " (must be 2 spaces away): ";
        std::string dest;
        std::cin >> dest;
        auto path = board.findShortestPath(hero.getLoc(), dest);
        if (path.size() == 3) { // 2 spaces away
            hero.setLoc(dest);
            std::cout << hero.getName() << " moved to " << dest << " using " << name << std::endl;
        } else {
            std::cout << "Invalid destination for " << name << "!" << std::endl;
        }
    }
}

std::string PerkCard::getDetails() const {
    std::stringstream ss;
    ss << "Perk card: " << name << ", quantity: " << quantity;
    return ss.str();
}