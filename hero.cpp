#include <bits/stdc++.h>

#include "hero.h"


Hero::Hero(const std::string& name, const std::string& loc, HeroType htype)
    : chara(name, loc, Typechara::HERO), Act_max((htype == HeroType::Mayor) ? 5 : 4),
      Act_remaining(0), in_hospital(false), htype(htype) {}

int Hero::getAct_max() const{
    return Act_max; 
}

int Hero::getAct_rem() const{
    return Act_remaining; 
}

bool Hero::getin_hos() const{ 
    return in_hospital; 
}

HeroType Hero::getHType() const{
    return htype; 
}

const std::vector<Item>& Hero::getItems() const{
    return items; 
}

void Hero::setAct_max(int a){
    Act_max=a;
}

void Hero::setAct_rem(int a){
    Act_remaining=a;
}

bool Hero::setin_hos(bool a){
    in_hospital=a;
}

void Hero::startTurn() {
    if (in_hospital) {
        heal();
        in_hospital = false;
    }
    Act_remaining = Act_max;
}

void Hero::heal() {
    setHealth(100); 
}

void Hero::useAct() {
    if (Act_remaining > 0) {
        Act_remaining--;
    }
}

void Hero::send_hos() {
    in_hospital = true;
    heal();
    Act_remaining = 0;
}

void Hero::move(const std::string& newLoc) {
    if (Act_remaining > 0) {
        useAct();
        setLoc(newLoc);
    }
}

void Hero::move_valliger(std::shared_ptr<chara> villager, const std::string& newLoc) {
    if (Act_remaining > 0 && villager->getType() == Typechara::VILLAGER) {
        villager->setLoc(newLoc);
        useAct();
    }
}

void Hero::pickup_item(const Item& item) {
    if (Act_remaining > 0) {
        items.push_back(item);
        std::cout << "Item picked up: " << item.getDetails() <<std::endl;
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
        if (items[i].getName() == itemname) {
            // استفاده از آیتم
            if (items[i].useItem()) {
                std::cout << "Used item: " << itemname << std::endl;
                
                // اگر تعداد آیتم به صفر رسید، از لیست حذف شود
                if (items[i].getQuantity() <= 0) {
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

void Hero::defeatMonster(std::shared_ptr<Monster> monster) {
    if (Act_remaining > 0) {
        if (monster->can_defeated() && getLoc() == monster->getLoc()) {
            monster->defeat();
            useAct();
            std::cout << "Monster defeated!" <<std::endl;
        } else {
            std::cout << "Cannot defeat the monster!" <<std::endl;
        }
    }
}
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////

Mayor::Mayor() : Hero("Mayor", "Theatre", HeroType::Mayor)
{}

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////
Ancient::Ancient() : Hero("Ancient", "Docks", HeroType::Ancient)
{}

void Ancient::takeSpcial_item(const std::string& itemLoc, std::vector<std::string>& items) {
    if (getAct_rem() > 0) {
        std::cout << "Ancient used special ability to pick up item!" <<std::endl;
        useAct();
    }
}
