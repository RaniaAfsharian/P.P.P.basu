#include <bits/stdc++.h>

#include "charactor.h"
#include "hero.h"
#include "Item.h"


Hero::Hero(const std::string &name , const std::string &loc , HeroType htype):
chara(name ,  loc , Typechara::HERO) , Act_max((htype==HeroType::Mayor) ? 5 : 4) , in_hospital(false) , htype(htype)
{}


int Hero::getAct_max()const{
    return Act_max;
}
int Hero::getAct_rem()const{
    return Act_remaining;
}
bool Hero::getin_hos()const{
    return in_hospital;
}
HeroType Hero::getHType()const{
    return htype;
}
const std::vector<std::shared_ptr<Item>>& Hero::getItems() const{
    return items;
}

void Hero::startTurn(){//تابع در زمان شروع بازی فراخوانی میشود
    if(in_hospital){
        heal();
        in_hospital=false; 
    }
    Act_remaining=Act_max;
}
void Hero::heal(){
    setHealth(100);
}
void Hero::useAct(){
    if(Act_remaining>0){
        Act_remaining--;
    }else{
        Act_remaining=0;
    }
}
void Hero::send_hos(){////////////تغییر دادن بیمارستاننننننن
    in_hospital=true;
    heal();
    Act_remaining=0;
}
void Hero::move(const std::string& newLoc){///حرکت به محل جدید
    if(Act_remaining>0){
        useAct();
        setLoc(newLoc);
    }
}
void Hero::move_valliger(std::shared_ptr <chara> villager , const std::string& newLoc){ // جا به جایی محلیا
    if(Act_remaining > 0 && villager->getType()==Typechara::VILLAGER){
        villager->setLoc(newLoc);
        useAct();
    }
}
void Hero::pickup_item( std::shared_ptr<Item>  item){
    if(Act_remaining>0 ){
        items.push_back(item);
        std::cout<<"Item picked:"<<item->getDetails()<<std::endl;
        useAct();
    }
}

void Hero::use_Item(const std::string& itemname) {
    // اگر عملیتی باقی نمانده یا آیتم خالی است، برگرد
    if (Act_remaining <= 0 || itemname.empty()) {
        return;
    }

    // جستجوی آیتم در لیست
    for (size_t i = 0; i < items.size(); ++i) {
        if (items[i]->getName() == itemname) {
            // استفاده از آیتم
            if (items[i]->useItem()) {
                std::cout << "Used item: " << itemname << std::endl;
                
                // اگر تعداد آیتم به صفر رسید، از لیست حذف شود
                if (items[i]->getQuantity() <= 0) {
                    items.erase(items.begin() + i);
                }
                
                useAct();
            } else {
                std::cout << "Item cannot be used: " << itemname << std::endl;
            }
            return; // پس از پیدا کردن آیتم، حلقه را پایان بده
        }
    }

    std::cout << "Item not found: " << itemname << std::endl;
}

/*
void Hero::progress_inTask(std::shared_ptr <Monster> monster){
    if(Act_remaining>0){
        monstr.can_defeated();
        useAct();
    }
}
*/
void Hero::defeatMonster(std::shared_ptr <Monster> monster){
    if(Act_remaining>0){  
        if(monster->can_defeated() && getLoc()==monster->getLoc()){
            monster->can_defend();
            useAct();
        }
    }
}
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
Mayor::Mayor():Hero("MAYOR" , "Theater" , HeroType::Mayor)
{}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
Ancient::Ancient() : Hero("ANCIENT" , "Docks" , HeroType::Ancient)
{}

void Ancient::takeSpcial_item(const std::string& itemLoc , std::vector<std::string>& items){
    if(getAct_rem()>0){
        std::cout<<"Ancint has takeSpcial_item ..."<<std::endl;
    }
}
