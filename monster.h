// File: monster.h
#ifndef MONSTER_H
#define MONSTER_H

#include "character.h"
#include "Board.h"

enum class monsterType { DRACULA, INVISIBLE_MAN };

class Monster : public chara {
protected:
    monsterType mType;
    bool is_defeated;
    std::string Npower;
    int power;
    std::vector<std::string> TargetLoc;
    std::vector<ItemType> itemType;

public:
    Monster(const std::string& name, const std::string& loc, monsterType mType, const std::string& npower, const std::vector<ItemType>& itemType, int power);
    virtual ~Monster() = default;
    monsterType getMtype() const { return mType; }
    bool getIs_defeated() const { return is_defeated; }
    std::string getNpower() const { return Npower; }
    int getPower() const { return power; }
    const std::vector<std::string>& getTargetLoc() const { return TargetLoc; }
    const std::vector<ItemType>& getItemType() const { return itemType; }
    void setPower(int npower);
    void addTargetLoc(std::string LOCC);
    void defeat() { is_defeated = true; }
    bool checkItem_defeat(const std::vector<Item>& items) const;
    virtual void useSpecialP() = 0;
    virtual bool can_defeated() const = 0;
    virtual bool can_defend() const;
};

class Dracula : public Monster {
private:
    int coffins_marker;

public:
    Dracula(const std::string& loc);
    void Coffin() { coffins_marker++; }
    int getCoffins_marker() const { return coffins_marker; }
    void useSpecialP() override { /* فرضی */ }
    bool can_defeated() const override { return coffins_marker >= 4; }
};

class Invisible_man : public Monster {
private:
    int itemPlaced;

public:
    Invisible_man(const std::string& loc);
    void ItemPlaced() { itemPlaced++; }
    int getItemPlaced() const { return itemPlaced; }
    void useSpecialP() override { /* فرضی */ }
    bool can_defeated() const override { return itemPlaced >= 5; }
};

#endif