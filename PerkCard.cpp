#include "PerkCard.hpp"

PerkCard::PerkCard(string name, int quantity)
    : name(name), quantity(quantity) {}

void PerkCard::activate() {
    if (quantity > 0) {
        --quantity;
        // اثر کارت
    }
}

string PerkCard::getDetails() const {
    return "PerkCard Name: " + name + ", Quantity: " + to_string(quantity);
}