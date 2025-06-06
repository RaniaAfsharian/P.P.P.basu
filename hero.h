#ifndef HERO_H
#define HERO_H


#include <bits/stdc++.h>

#include "charactor.h"
#include "monster.h"


enum class HeroType{Mayor , Ancint};

class Hero : public chara {
    private:
    HeroType htype;
    int Act_max;
    int Act_remaining;
    bool in_hospital;

    // std::vector<std::shared_ptr<item> >items;

    public:
    Hero(const std::string& name , const std::string &loc , HeroType htype );
   
    int getAct_max()const;
    int getAct_rem()const;
    bool getin_hos()const;
    HeroType getHType()const;


    void startTurn();
    void heal();
    void useAct();
    void send_hos();
    void move(const std::string& newLoc);
    void move_valliger(std::shared_ptr <chara> villager , const std::string& newLoc);


/*مانستر رو مینویسم و کاملش میکنمم
    void pickup_item(std::shared_ptr <item> items);
    void progress(std::shared_ptr <)
*/
};


#endif
