#include <bits/stdc++.h>

#include "charactor.h"
#include "game.h"


    chara::chara(const std::string &name , const std::string &loc , Typechara type) :name(name) , loc(loc), type(type) , health(100) {}
///virtual chara:: ~chara() = default;

    std::string chara::getName()const{
        return name;
    }
    std::string chara::getLoc()const{
        return loc;
    }
    int chara::getHealth()const{
        return health;
    }
    Typechara chara::getType()const{
        return type;
    }

    void chara::setName(std::string n){
        name =n;
    }
    void chara::setLoc(std::string lo){
        loc=lo;
    }
    void chara::setHealth(int hea){
        if(hea<0){
            hea=0;
        }

        health=hea;
    }
    void chara::takeHarm(int amount){
        health-=amount;
        if(health<0){
            health=0;
        }
    }
    bool chara::can_defend()const{///فقط قهرمان میتواند دفاع کند 
        if(type==Typechara::HERO){
            return true;
        }else{
            return false;
        }
    }
    /*
    ////مدیریت ایتم ها 

    void chara::addITEM(std::shared_ptr<item> item){
    items.push_back(item);
    }
    void chara::removeITEM(int index){
        if( index>0 && index<items.size() ){
            items.eraser(items.begin()+index)
        }
    }
    const std::vector<std::shared_ptr<item> & getITEM()const{
    return item;
    }
    */ 

    villager::villager(const std::string& name , const std::string &loc , const std::string &safeLoc ):chara(name , loc , Typechara::VILLAGER) , safeLoc(safeLoc) , is_saved(false)
    {
        std::cout<<"Villager "<<name<<" created at "<<loc<<" and safe location is: "<<safeLoc<<std::endl;
    }

    std::string villager::getSafeLoc()const{
        return safeLoc;
    }
    bool villager::getIs_saved()const{
        return is_saved;
    }
    void villager::rescue(){//زمان نجات
        if(loc==safeLoc && is_saved==false){
            is_saved=true;
            std::cout<<name<<" has been rescued at : "<<safeLoc<<"....."<<std::endl;


            //Game::instance().givePerk_player();///
            //Game::instance().removeVillager(*this);
        }
    }
    void villager::diee(){///مرگ
        if(!is_saved){
            std::cout<<name<<" has been killed by monster !!!!!!!!!"<<std::endl;
        }

        //Game::instance().increaseTERR();///
        //Game::instance().removeVillager(*this);
    }
