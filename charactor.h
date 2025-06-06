#ifndef CHARACTOR_H
#define CHARACTOR_H


#include <bits/stdc++.h>
//#include <item.h>

enum class Typechara{HERO , VILLAGER , MONSTER};

class chara{
    protected:
    std::string name;
    int health;//مقدار اولیه رو 10 میزارم
    std::string loc;//لوکیشن
    Typechara type;

    

    public:
    chara(const std::string &name , const std::string &loc , Typechara type);
    virtual ~chara() = default;

    std::string getName()const;
    std::string getLoc()const;
    int getHealth()const;
    Typechara getType()const;

    void setName(std::string n);
    void setLoc(std::string lo);
    void setHealth(int hea);

    virtual void takeHarm(int amount );
    virtual bool can_defend()const;
    /*
    ////مدیریت ایتم ها 

    void addITEM(std::shared_ptr<item> item);
    void removeITEM(int index);
    const std::vector<std::shared_ptr<item> & getITEM()const;
    */    
};


class villager : public chara{
    private:
    std::string safeLoc;
    bool is_saved;

    public:
    villager(const std::string& name , const std::string &loc , const std::string &safeLoc );

    std::string getSafeLoc()const;
    bool getIs_saved()const;

    void rescue();//زمان نجات
    void diee();///مرگ

};
#endif
