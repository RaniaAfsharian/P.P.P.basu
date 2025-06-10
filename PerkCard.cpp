#include "PerkCard.hpp"
#include <sstream>
#include <vector>

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
    if (quantity <= 0) {
        return false; 
    }
    --quantity; 
    return true; 
}

void PerkCard::applyEffect(Person& hero) {
    std::cout << "Special card application: " << name << " for " << hero.name << "\n";
}

string PerkCard::getDetails() const {
    return "PerkCard Name: " + name + ", Quantity: " + to_string(quantity);
}
