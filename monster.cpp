#include <bits/stdc++.h>

#include "charactor.h"
#include "monster.h"
//#include "item.h"

Monster::Monster(const std::string& name , const std::string &loc , MonsterType Mtype , const std::string Npower , /*const std::vector<item> items ,*/ int power ) :
chara(name , loc , Typechara::MONSTER) , is_defeated(false) ,Npower(Npower) , power(power) //, item
{
    MonsterType(Mtype);
}

MonsterType Monster::getMtype()const{
    return Mtype;
}
std::string Monster::getNpower()const{
    return Npower;
}
bool Monster::getIs_defeated()const{
    return is_defeated;
}
int Monster::getPower()const{
    return power;
}

const std::vector<std::string>& Monster::getTargetLoc()const{
    return targetLoc;
}
//const std::vector<item>& Monster::getItems()const{
// return item;
//}

void Monster::setPower(int npower){
    power=npower;
}
void Monster::addTargetLoc(std::string LOCC){
    targetLoc.push_back(LOCC);
}
void Monster::defeat(){
    is_defeated=true;
}
/*
bool Monster::checkItem_defeat(const std::vector<std::shared_ptr<Item>>& items) const {
    int totalPower = 0;
    std::vector<bool> typeFound(requiredItemTypes.size(), false);

    for (const auto& item : items) {
        for (size_t i = 0; i < requiredItemTypes.size(); ++i) {
            if (item->getType() == requiredItemTypes[i]) {
                typeFound[i] = true;
                totalPower += item->getPower();
                break;
            }
        }
    }
    return std::all_of(typeFound.begin(), typeFound.end(), [](bool found) { return found; }) &&
           totalPower >= requiredpower;
}
*/
bool Monster::can_defend()const{
    return false;
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
Dracula::Dracula(const std::string& loc) : 
Monster("Dracula " , loc , MonsterType::DRACULA , "count dracula" ,/* {} ,*/ 6),
coffins_marker(0){
    addTargetLoc("Cave");
    addTargetLoc("Dungeon"); 
    addTargetLoc("Crypt");
    addTargetLoc("Graveyard");
}


int Dracula::getCoffins_marker()const{
    return coffins_marker;
}
void Dracula::Coffin(){
    if(coffins_marker<4){
        coffins_marker++;
    }
}
bool Dracula::allTakeCoffine()const{
    if(coffins_marker<4){
        return true;
    }else{
        return false;
    }
} 
void Dracula::useSpecialP(){

}
bool Dracula::can_defeated()const {
    return allTakeCoffine();
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
Invisidie_man::Invisidie_man(const std::string& loc):
Monster("Invisidie_man" , loc , MonsterType::INVISIDIE_MAN , "death anghel" , /*{} ,*/ 9) ,itemPlaced(0)
{
    addTargetLoc("Inn");
    addTargetLoc("Barn"); 
    addTargetLoc("Institute");
    addTargetLoc("Laboratory");  
    addTargetLoc("Mansion");
}

int Invisidie_man::getItemPlaced()const{
    return itemPlaced;
}
void Invisidie_man::ItemPlaced(){/////////
    itemPlaced++;
}
bool Invisidie_man::allTakeItemPlaced()const{
    if(itemPlaced>5){
        return true;
    }else{
        return false;
    }
}

void Invisidie_man::useSpecialP(){

}
bool Invisidie_man::can_defeated()const{
    return allTakeItemPlaced();
}
