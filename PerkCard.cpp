#include <iostream>
#include <sstream>
#include <vector>
#include "PerkCard.h"

using namespace std;

vector<PerkCard> createPerkCards() {
    vector<PerkCard> cards;
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

void PerkCard::applyEffect(Hero& hero) {
    cout << "Applying perk card: " << name << " for " << hero.getName() << endl;
}

string PerkCard::getDetails() const {
    stringstream ss;
    ss << "Perk card: " << name << ", quantity: " << quantity;
    return ss.str();
}
