#include <iostream>

#include "monster.h"


Monster::Monster(const std::string& name, const std::string& loc, monsterType Mtype,
                 const std::string& Npower, const std::vector<ItemType>& itemType, int power)
    : chara(name, loc, Typechara::MONSTER), is_defeated(false), Npower(Npower),
      power(power), Mtype(Mtype), itemType(itemType) {}

monsterType Monster::getMtype() const{
    return Mtype; 
}
std::string Monster::getNpower() const{
    return Npower; 
}
bool Monster::getIs_defeated() const{
    return is_defeated; 
}
int Monster::getPower() const{
    return power; 
}
const std::vector<std::string>& Monster::getTargetLoc() const{
    return targetLoc; 
}
const std::vector<ItemType>& Monster::getItemType() const{
    return itemType; 
}

void Monster::setPower(int npower){
    power = npower; 
}
void Monster::addTargetLoc(std::string LOCC) {
    targetLoc.push_back(LOCC); 
}
void Monster::defeat() {
    is_defeated = true; 
}

bool Monster::checkItem_defeat(const std::vector<Item>& items) const {
    int totalPower = 0;
    bool allTypeFound=true;
    std::vector<bool> typeFound(itemType.size(), false);

    for (const auto& item : items) {
        for (size_t i = 0; i < itemType.size(); ++i) {
            if (item.getType() == itemType[i] && !typeFound[i]) {
                typeFound[i] = true;
                totalPower += item.getEffect();
                break;
            }
        }
    }

    for(bool found : typeFound){
        allTypeFound=(allTypeFound && found );
    }

    if(allTypeFound && totalPower >= power ){
        return true;
    }else{
        return false;
    }
  
}

bool Monster::can_defend() const{
    return false; 
}

Dracula::Dracula(const std::string& loc)
    : Monster("Dracula", loc, monsterType::DRACULA, "Count Dracula",
            {ItemType::Red, ItemType::Yellow}, 6), coffins_marker(0) {
    addTargetLoc("Cave");
    addTargetLoc("Dungeon");
    addTargetLoc("Crypt");
    addTargetLoc("Graveyard");
}

int Dracula::getCoffins_marker() const{
    return coffins_marker; 
}
void Dracula::Coffin() {
    if (coffins_marker < 4){
        coffins_marker++;
    }
}
bool Dracula::allTakeCoffin() const{
    if(coffins_marker >= 4){
        return true;
    }else{
        return false;
    }
}
void Dracula::useSpecialP() {
    setPower(getPower() + 2);
    std::cout << "Dracula's power increased!" <<std::endl;
}
bool Dracula::can_defeated() const{
    return allTakeCoffin();
}

Invisible_man::Invisible_man(const std::string& loc)
    : Monster("Invisible Man", loc, monsterType::INVISIBLE_MAN, "Death Angel",
              {ItemType::Red, ItemType::Yellow}, 9), itemPlaced(0) {
    addTargetLoc("Inn");
    addTargetLoc("Barn");
    addTargetLoc("Institute");
    addTargetLoc("Laboratory");
    addTargetLoc("Mansion");
}

int Invisible_man::getItemPlaced() const{
    return itemPlaced; 
}
void Invisible_man::ItemPlaced() {
    if (itemPlaced < 5) {
        itemPlaced++;
    }
}
bool Invisible_man::allTakeItemPlaced() const {
      if( itemPlaced >= 5){
        return true;
    }else{
        return false;
    }
}
void Invisible_man::useSpecialP() {
    setPower(getPower() + 5);
    std::cout << "Invisible Man's power increased!" <<std::endl;
}
bool Invisible_man::can_defeated() const {
    return allTakeItemPlaced();
}
