#ifndef PERKCARD_H
#define PERKCARD_H


#include <bits/stdc++.h>

#include "hero.h"

class PerkCard {
private:
    std::string name;
    int quantity;

public:
    PerkCard(const std::string& name, int quantity);

    PerkCard(const PerkCard&) = default;
    PerkCard& operator=(const PerkCard&) = default;
  

    bool activate();
    void applyEffect(Hero& hero);
    std::string getDetails() const;

    std::string getName() const {
        return name;
    }
    int getQuantity() const {
        return quantity;
    }
};

std::vector<PerkCard> createPerkCards();

#endif