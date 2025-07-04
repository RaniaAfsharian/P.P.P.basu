// File: character.h
#ifndef CHARACTER_H
#define CHARACTER_H

#include <bits/stdc++.h>

enum class Typechara { HERO, VILLAGER, MONSTER };

class Item;

class chara {
protected:
    std::string name;
    std::string loc;
    Typechara type;
    int health;
    std::vector<Item> items;

public:
    chara(const std::string& name, const std::string& loc, Typechara type);
    virtual ~chara() = default;
    std::string getName() const { return name; }
    std::string getLoc() const { return loc; }
    Typechara getType() const { return type; }
    void setLoc(const std::string& newLoc) { loc = newLoc; }
    int getHealth() const;
    const std::vector<Item>& getItems() const;
    void setName(std::string n);
    void setHealth(int hea);
    void takeHarm(int amount);
    bool can_defend() const;
};

class villager : public chara {
private:
    bool isSaved;
    std::string safeLoc;

public:
    villager(const std::string& name, const std::string& loc, const std::string& safeLoc);
    std::string getSafeLoc() const { return safeLoc; }
    bool getIs_saved() const { return isSaved; }
    void rescue() { isSaved = true; }
    void diee() { /* فرضی: مرگ روستایی */ }
};

#endif