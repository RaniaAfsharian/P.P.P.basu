#ifndef PERKCARD_HPP
#define PERKCARD_HPP
#include <string>
#include <vector>
using namespace std;

class PerkCard {
private:
    string name;
    int quantity;

public:
    PerkCard(string name, int quantity);

    PerkCard(const PerkCard&) = delete;
    PerkCard& operator=(const PerkCard&) = delete;

    bool activate(); 
    std::string getDetails() const; 

    string getDetails() const;
};

#endif 
