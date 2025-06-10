#include <bits/stdc++.h>

#include "ActionSystem.h"
#include "Board.h"
#include "charactor.h"
#include "Item.h"
#include "hero.h"
#include "monster.h"
#include "MonsterCard.h"
#include "PerkCard.h"
#include "TerrorLevelTracker.h"
#include "game.h"

Game::Game() : gameTerrorLevelTracker(5) , playerindex(0) , gameOver(false)
{
    setGame();
}


void Game::setGame(){
    gameHero.push_back(std::make_shared<Mayor>());
    gameHero.push_back(std::make_shared<Ancient>());

    gameMonster.push_back(std::make_shared<Invisible_man>("Crypt"));
    gameMonster.push_back(std::make_shared<Dracula>("Laboratory"));

    gamevillager.push_back(std::make_shared<villager>("wilbur & Chick","Inn","Dungeon"));
    gamevillager.push_back(std::make_shared<villager>("Maria","Abbey","Camp"));
    gamevillager.push_back(std::make_shared<villager>("Maleva","Barn","Shop"));
    gamevillager.push_back(std::make_shared<villager>("Prof.Pearson","Tower","Museum"));
    gamevillager.push_back(std::make_shared<villager>("Dr.Reed","Docks","Camp"));
    gamevillager.push_back(std::make_shared<villager>("Dr.Cranley","Mansion","Precinct"));
    gamevillager.push_back(std::make_shared<villager>("Frits","Cave","Institute"));
}   

void Game::loop_Game(){
    while(!gameOver){
        show_Gamestate();

        for(auto& hero : gameHero){
            heroGame(*hero);
            chech_win();
            if(gameOver){
                break;
            }
        }

        if(!gameOver){
            monsterGame();
            chech_win();
        }
    }

    if(gameTerrorLevelTracker.isGameOver()){
        std::cout<<"Game over! the city is overrun by monster"<<std::endl;

    }else{
        std::cout<<"Congratulations! All monsters have been defeated..."<<std::endl;
    }
}

void Game::heroGame(Hero& hero){
    std::cout<<"\n ==="<<hero.getName()<<" 's TUrn===\n";
    hero.startTurn();
    gameActionSystem.startHeroTurn(hero.getName() , hero.getAct_max());

    while(hero.getAct_rem()>0 && !gameOver){
        show_Gamestate();
        int choice;
        bool actSuccess=false;
        std::string goal;
        std::cout<<"Action remaining : "<<hero.getAct_rem()<<std::endl;
        std::cout<<"Choose action : "<<std::endl;
        std::cout<<"1. Move"<<std::endl;
        std::cout<<"2. Guide Villager"<<std::endl;
        std::cout<<"3. Pick Up"<<std::endl;
        std::cout<<"4. Defeat Monster"<<std::endl;
        std::cout<<"5. Use Item"<<std::endl;
        std::cin>>choice;
        switch(choice){
            case 1:{
            std::cout<<"Available location : "<<std::endl;
            auto availablePaths=gameBoard.getAvailablePaths(hero.getLoc());
            for(const auto& loc : availablePaths){
                std::cout<<loc<<" "<<std::endl;
                }
                std::cout<<"Enter destination : "<<std::endl;
                std::cin>>goal;
                if(gameBoard.movePlayer(playerindex+1 , goal)){
                    hero.move(goal);
                    actSuccess=true;
                }
                break;
            }
            case 2:{
            std::cout<<"Villager in current location : "<<std::endl;
            for(const auto& villager: gamevillager){
                if(villager->getLoc() == hero.getLoc() && !villager->getIs_saved()){
                    std::cout<<villager->getName() << "safe loction : "<<villager->getSafeLoc()<<std::endl;
                }
            }
            std::cout<<"Enter villager name to Guid : "<<std::endl;
            std::string villagername;
            std::cin>>villagername;

            std::shared_ptr<villager> foundvill=nullptr;
            for(const auto& villager : gamevillager){
                if(villager->getName() == villagername){
                    foundvill=villager;
                    break;
                }
            }
            if(foundvill != nullptr){
                hero.move_valliger(foundvill , goal);
                actSuccess=true;
                    if(goal == foundvill->getSafeLoc()){
                        foundvill->rescue();
                    }
            }
            break;
            }
            case 3:{

            }
            case 4:{
                std::string monsterName;
                std::cout<<"Monster in current location : "<<std::endl;
                for(const auto& monster : gameMonster){
                    if(monster->getLoc() == hero.getLoc() && !monster->getIs_defeated()){
                        std::cout<<monster->getName()<< "power : "<<monster->getPower()<<std::endl;
                    }
                }
                std::cout<<"Enter monster name to attack : "<<std::endl;
                std::cin>>monsterName;

                std::shared_ptr<Monster> targetMon=nullptr;
                for (const auto& monster : gameMonster){
                    if(monster->getName() == monsterName){
                        targetMon=monster;
                        break;
                    }
                }
                if(targetMon != nullptr){
                    hero.defeatMonster(targetMon);
                    if(targetMon->getIs_defeated()){
                        std::cout<<"Monster defeated ... "<<std::endl;
                    }else{
                        std::cout<<"Failed to defeat the monster !!"<<std::endl;
                        std::cout<<"You need more power or special items to defeat ."<<targetMon->getName()<<std::endl;
                    }
                }else{
                    std::cout<<"Monster not found in this location! "<<std::endl;
                }
            }
            case 5:{
                std::cout<<"You items : "<<std::endl;
                const auto& items=hero.getItems();
                for(size_t i=0 ; i<items.size() ; i++ ){
                    std::cout<<i+1<<"  "<<items[i]->getDetails()<<std::endl;
                }
                std::cout<<"Enter item number to use : "<<std::endl;
                int itemNum;
                std::cin>>itemNum;
                if(itemNum>0 && itemNum<= static_cast<int>(items.size()) ){
                    hero.use_Item(items[itemNum-1]->getName());
                    actSuccess=true;
                }
                break;
            }
            default:
            std::cout<<"invalid choice!!!!!!!!!"<<std::endl;
        }

        if(actSuccess){
            hero.useAct();
        }
    }
}

void Game::monsterGame(){
    std::cout<<"\n=== Monster Turn ===\n";
    for(auto& monster : gameMonster){
        if(monster->getIs_defeated()) continue;
        monster->useSpecialP();

        for(auto& villager : gamevillager){
            if(!villager->getIs_saved() && villager->getLoc() == monster->getLoc()){
                villager->diee();
                gameTerrorLevelTracker.increaseTerror(1);
            }
        }
        auto availablePaths=gameBoard.getAvailablePaths(monster->getLoc());
        if(!availablePaths.empty()){
            for(const auto& target : monster->getTargetLoc()){
                if(availablePaths.find(target) != availablePaths.end()){
                    monster->setLoc(target);
                    break;
                }
            }
        }
    }
}

void Game::chech_win(){
    bool allDefeated=true;

    for(const auto& monster : gameMonster){
        if(!monster->getIs_defeated()){
            allDefeated=false;

            break;
        }
    }

    if(allDefeated){
        gameOver=true;
    }

    if (gameTerrorLevelTracker.isGameOver()){
        gameOver=true;
    }
}

void Game::show_Gamestate(){
    std::cout<<"\n ==== GAME state ===\n";
    gameTerrorLevelTracker.displayTerrorLevel();
    gameBoard.displayBoard();
    std::cout<<"\n Heroes : \n";
    for(const auto& hero : gameHero){
        std::cout<<hero->getName()<<" at "<<hero->getLoc()<<" health : "<<hero->getHealth()<<" Action : "<<hero->getAct_rem()<<"/"<<hero->getAct_max()<<std::endl;
    }

    std::cout<<"\n Monster : \n";
    for(const auto& monster : gameMonster){
        std::cout<<monster->getName()<<" at "<<monster->getLoc()<<" power : "<<monster->getPower()<<" status : "<<(monster->getIs_defeated() ? "defated " : "active")<<std::endl;
    }

    std::cout<<"\n Villager : \n";
    for(const auto& villager : gamevillager){
        std::cout<<villager->getName()<<" at "<<villager->getLoc()<<" status : "<<(villager->getIs_saved() ? "saved " : "in danger ")<<"safe location :"<<villager->getSafeLoc()<<std::endl;
    }   
}
