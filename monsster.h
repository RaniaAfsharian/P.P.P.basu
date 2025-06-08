#ifndef MONSTER_H
#define MONSTER_H

#include <bits/stdc++.h>

#include "charactor.h"
#include "Item.h"


enum class monsterType{ INVISIDIE_MAN , DRACULA };

class Monster : public chara{
    private:
    monsterType Mtype;
    std::string Npower;
    bool is_defeated;
    std::vector<std::string> targetLoc;
    std::vector<Item> items;
    int power;
    //MonsterCard monstercard;


    public:
    Monster(const std::string name , const std::string &loc , monsterType Mtype , const std::string Npower , const std::vector<Item> items , int power);

    monsterType getMtype()const;
    std::string getNpower()const;
    bool getIs_defeated()const;
    int getPower()const;

    const std::vector<std::string>& getTargetLoc()const;
    const std::vector<Item>& getItems()const;
   // MonsterCard getMonstercard()const;

    void setPower(int npower);
    void addTargetLoc(std::string LOCC);
    void defeat();

    virtual void useSpecialP()=0;
    virtual bool can_defeated()const=0;

    bool checkItem_defeat(const std::vector<std::shared_ptr<Item>> &items) const;

    bool can_defend()const override;
 
};


class Dracula : public Monster{
    private:
    int coffins_marker;

    public:
    Dracula(const std::string& loc);
    
    int getCoffins_marker()const;

    void Coffin();//
    bool allTakeCoffine()const; 

    void useSpecialP() override;
    bool can_defeated()const override;

};


class Invisidie_man : public Monster{
    private:
    int itemPlaced;

    public:
    Invisidie_man(const std::string& loc);

    int getItemPlaced()const;
    void ItemPlaced();///
    bool allTakeItemPlaced()const;

    void useSpecialP() override;
    bool can_defeated()const override;
};

#endif
