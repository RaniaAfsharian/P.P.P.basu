#include <bits/stdc++.h>

#include "charactor.h"
#include "hero.h"

Hero::Hero(const std::string &name , const std::string &loc , HeroType htype):
chara(name ,  loc , Typechara::HERO) , Act_max((htype==HeroType::Mayor) ? 5 : 4) , in_hospital(false)
{
HeroType(htype);
}


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
void Hero::send_hos(){//این قسمت رو شک دارم
    in_hospital=true;
    setHealth(50);
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
