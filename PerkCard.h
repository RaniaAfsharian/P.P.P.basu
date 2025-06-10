#ifndef PERKCARD_HPP
#define PERKCARD_HPP
#include <string>
#include <vector>

class Person;

class PerkCard {
private:
    std::string name;
    int quantity;

public:
    PerkCard(const std::string& name, int quantity);

    PerkCard(const PerkCard&) = delete;
    PerkCard& operator=(const PerkCard&) = delete;

    bool activate(); 
    void applyEffect(Person& hero);
    std::string getDetails() const; 
    

    std::string getName() const { return name; }
    int getQuantity() const { return quantity; }
};

#endif 
