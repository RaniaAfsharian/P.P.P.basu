#ifndef PERKCARD_H
#define PERKCARD_H

#include <string>
#include <vector>
#include "hero.h"

class PerkCard {
private:
    std::string name;
    int quantity;

public:
    PerkCard(const std::string& name, int quantity);

    PerkCard(const PerkCard&) = delete;
    PerkCard& operator=(const PerkCard&) = delete;

    bool activate();
    void applyEffect(Hero& hero);
    std::string getDetails() const;

    std::string getName() const { return name; }
    int getQuantity() const { return quantity; }
};

std::vector<PerkCard> createPerkCards();

#endif
