#ifndef PERKCARD_HPP
#define PERKCARD_HPP

#include <string>
using namespace std;

class PerkCard {
private:
    string name;
    int quantity;

public:
    PerkCard(string name, int quantity);

    void activate();

    string getDetails() const;
};

#endif 