#include <bits/stdc++.h>

#include "game.h"



Game::Game() : gameTerrorLevelTracker(5), playerindex(0), gameOver(false) {
    setGame();
}

void Game::setGame() {
    gameHero.push_back(std::make_shared<Mayor>());
    gameHero.push_back(std::make_shared<Ancient>());

    gameMonster.push_back(std::make_shared<Invisible_man>("Crypt"));
    gameMonster.push_back(std::make_shared<Dracula>("Laboratory"));

    gamevillager.push_back(std::make_shared<villager>("Wilbur & Chick", "Inn", "Dungeon"));
    gamevillager.push_back(std::make_shared<villager>("Maria", "Abbey", "Camp"));
    gamevillager.push_back(std::make_shared<villager>("Maleva", "Barn", "Shop"));
    gamevillager.push_back(std::make_shared<villager>("Prof. Pearson", "Tower", "Museum"));
    gamevillager.push_back(std::make_shared<villager>("Dr. Reed", "Docks", "Camp"));
    gamevillager.push_back(std::make_shared<villager>("Dr. Cranley", "Mansion", "Precinct"));
    gamevillager.push_back(std::make_shared<villager>("Frits", "Cave", "Institute"));

    for (const auto& item : createItems()) {
        gameitem.push_back(item);
        gameBoard.placeItem(item.getName().substr(item.getName().find('_') + 1), item);
    }

    for(const auto& card : createPerkCards()){
        gamePerkCard.push_back(card);
    }
}

void Game::loop_Game() {
    while (!gameOver) {
        show_Gamestate();
        show_menu();
        int choice;
        std::cin>>choice;


        switch(choice){
            case 1: {
                for (auto& hero : gameHero) {
                    heroGame(*hero);
                    check_win();
                    if (gameOver) break;
                }

                if (!gameOver) {
                    monsterGame();
                    check_win();
                }
                break;
            }
            case 2:{
                savegame_state();
                break;
            }
            case 3:{
                loadgame_state();
                break;
            }
            case 4:{
                gameOver=true;
                std::cout<<"Game exited by player . . . . "<<std::endl;
                break;
            }
            default:{
                std::cout<<"Invalid choice !!"<<std::endl;
            }
        }

            if (gameTerrorLevelTracker.isGameOver()) {
                std::cout << "Game over! The city is overrun by monsters!" <<std::endl;
            } else {
                std::cout << "Congratulations! All monsters have been defeated!" <<std::endl;
            }
    }
}
void Game::heroGame(Hero& hero) {
    std::cout << "\n===" << hero.getName() << "'s Turn ===\n";
    hero.startTurn();
    gameActionSystem.startHeroTurn(hero.getName(), hero.getAct_max());

    while (hero.getAct_rem() > 0 && !gameOver) {
        show_Gamestate();
        int choice;
        bool actSuccess = false;
        std::string goal;
        std::cout << "Actions remaining: " << hero.getAct_rem() <<std::endl;
        std::cout << "Choose action: " <<std::endl;
        std::cout << "1. Move" <<std::endl;
        std::cout << "2. Guide Villager" <<std::endl;
        std::cout << "3. Pick Up Item" <<std::endl;
        std::cout << "4. Defeat Monster" <<std::endl;
        std::cout << "5. Use Item" <<std::endl;
        std::cout << "6. Use Perk card "<<std::endl;
        std::cout << "7. perform special action"<<std::endl;
        std::cin >> choice;

        switch (choice) {
        case 1: {
            std::cout << "Available locations: \n";
            auto availablePaths = gameBoard.getAvailablePaths(hero.getLoc());
            for (const auto& loc : availablePaths) {
                std::cout << loc << " ";
            }
            std::cout << "\nEnter destination: ";
            std::cin >> goal;
            if (gameBoard.movePlayer(playerindex + 1, goal)) {
                hero.move(goal);
                actSuccess = true;
            } else {
                std::cout << "Invalid move!" <<std::endl;
            }
            break;
        }
        case 2: {
            std::cout << "Villagers in current location: " << std::endl;
            for (const auto& villager : gamevillager) {
                if (villager->getLoc() == hero.getLoc() && !villager->getIs_saved()) {
                    std::cout << villager->getName() << ", safe location: " << villager->getSafeLoc() <<std::endl;
                }
            }
            std::cout << "Enter villager name to guide: ";
            std::string villagername;
            std::cin >> villagername;

            std::shared_ptr<villager> foundvill = nullptr;
            for (const auto& villager : gamevillager) {
                if (villager->getName() == villagername) {
                    foundvill = villager;
                    break;
                }
            }
            if (foundvill != nullptr) {
                std::cout << "Enter destination: ";
                std::cin >> goal;
                hero.move_valliger(foundvill, goal);
                actSuccess = true;
                if (goal == foundvill->getSafeLoc()) {
                    foundvill->rescue();
                }
            } else {
                std::cout << "Villager not found!" <<std::endl;
            }
            break;
        }
        case 3: {
            std::cout << "Items available at " << hero.getLoc() << ":" <<std::endl;
            auto items = gameBoard.getItems().find(hero.getLoc());
            if (items != gameBoard.getItems().end()) {
                for (const auto& item : items->second) {
                    std::cout << item.getDetails() <<std::endl;
                    hero.pickup_item(item);
                    gameitem.push_back(item);
                }
                gameBoard.removeItems(hero.getLoc());
                actSuccess = true;
            } else {
                std::cout << "No items found!" <<std::endl;
            }
            break;
        }
        case 4: {
            std::cout << "Monsters in current location: " <<std::endl;
            for (const auto& monster : gameMonster) {
                if (monster->getLoc() == hero.getLoc() && !monster->getIs_defeated()) {
                    std::cout << monster->getName() << ", power: " << monster->getPower() <<std::endl;
                }
            }
            std::cout << "Enter monster name to attack: ";
            std::string monsterName;
            std::cin >> monsterName;

            std::shared_ptr<Monster> targetMon = nullptr;
            for (const auto& monster : gameMonster) {
                if (monster->getName() == monsterName) {
                    targetMon = monster;
                    break;
                }
            }
            if (targetMon != nullptr) {
                hero.defeatMonster(targetMon);
                if (targetMon->getIs_defeated()) {
                    std::cout << "Monster defeated!" <<std::endl;
                } else {
                    std::cout << "Failed to defeat the monster! More power or items needed." <<std::endl;
                }
                actSuccess = true;
            } else {
                std::cout << "Monster not found in this location!" <<std::endl;
            }
            break;
        }
        case 5: {
            std::cout << "Your items: " <<std::endl;
            const auto& items = hero.getItems();
            for (size_t i = 0; i < items.size(); i++) {
                std::cout << i + 1 << " " << items[i].getDetails() <<std::endl;
            }
            std::cout << "Enter item number to use: ";
            int itemNum;
            std::cin >> itemNum;
            if (itemNum > 0 && itemNum <= static_cast<int>(items.size())) {
                hero.use_Item(items[itemNum - 1].getName());
                actSuccess = true;
            } else {
                std::cout << "Invalid item number!" <<std::endl;
            }
            break;
        }
        case 6:{
            std::cout<<"Available perk cards : \n";
            for(size_t i=0 ; i < gamePerkCard.size() ; i++ ){
                std::cout<<i+1<<". "<<gamePerkCard[i].getDetails()<<std::endl;
            }
            std::cout<<"Enter perk card number to use: \n";
            int perknum;
            std::cin>>perknum;
            if(perknum>0 && perknum <= static_cast<int>(gamePerkCard.size())){
                if(gamePerkCard[perknum-1].activate()){
                    gamePerkCard[perknum-1].applyEffect(hero);

                    actSuccess=true;
                }else{
                    std::cout<<"No uses left for this perk card!  \n";
                }
            }else{
                std::cout<<"Invalid perk card number ! \n";
            }
            break;
        }
        case 7:{
            if(hero.getHType()== HeroType::Ancient){
                std::cout<<"Using Ancient's special ability to pick up item from neighboring location..\n";
                std::cout<<"Available neighboring location : \n";
                auto availablepaths = gameBoard.getAvailablePaths(hero.getLoc());
                for (const auto& loc : availablepaths){
                    std::cout<<loc<<"  ";
                }
                std::cout<<"\n Enter location to pick up item from : \n";
                std::cin>>goal;
                if(availablepaths.find(goal) != availablepaths.end()){
                    auto items=gameBoard.getItems().find(goal);
                    if(items != gameBoard.getItems().end()){
                        for(const auto& item : items->second){
                            hero.pickup_item(item);
                            gameitem.push_back(item);
                            std::cout<<"Pick up item: "<<item.getDetails()<<std::endl; 
                        }
                        gameBoard.removeItems(goal);
                        actSuccess=true;
                    }else{
                        std::cout<<"No items found at "<<goal<<std::endl;
                    }
                }else{
                    std::cout<<"Invalid location !! \n"; 
                }
            }else{
                std::cout<<"Only Ancient has a special action ! \n";
            }
            break;
        }
        default:
            std::cout << "Invalid choice!" <<std::endl;
        }

        if (actSuccess) {
            hero.useAct();
        }
    }
}

void Game::monsterGame() {
    std::cout << "\n=== Monster Turn ===\n";
    for (auto& monster : gameMonster) {
        if (monster->getIs_defeated()) continue;
        monster->useSpecialP();

        for (auto& villager : gamevillager) {
            if (!villager->getIs_saved() && villager->getLoc() == monster->getLoc()) {
                villager->diee();
                gameTerrorLevelTracker.increaseTerror(1);
            }
        }
        auto availablePaths = gameBoard.getAvailablePaths(monster->getLoc());
        if (!availablePaths.empty()) {
            for (const auto& target : monster->getTargetLoc()) {
                if (availablePaths.find(target) != availablePaths.end()) {
                    monster->setLoc(target);
                    std::cout << monster->getName() << " moved to " << target << "!" <<std::endl;
                    break;
                }
            }
        }
    }
}

void Game::check_win() {
    bool allDefeated = true;
    for (const auto& monster : gameMonster) {
        if (!monster->getIs_defeated()) {
            allDefeated = false;
            break;
        }
    }
    if (allDefeated || gameTerrorLevelTracker.isGameOver()) {
        gameOver = true;
    }
}

void Game::show_Gamestate() {
    std::cout << "\n==== Game State ====\n";
    gameTerrorLevelTracker.displayTerrorLevel();
    gameBoard.displayBoard();
    std::cout << "\nHeroes:\n";
    for (const auto& hero : gameHero) {
        std::cout << hero->getName() << " at " << hero->getLoc() << ", health: " << hero->getHealth()
             << ", actions: " << hero->getAct_rem() << "/" << hero->getAct_max() <<std::endl;
    }
    std::cout << "\nMonsters:\n";
    for (const auto& monster : gameMonster) {
        std::cout << monster->getName() << " at " << monster->getLoc() << ", power: " << monster->getPower()
             << ", status: " << (monster->getIs_defeated() ? "defeated" : "active") <<std::endl;
    }
    std::cout << "\nVillagers:\n";
    for (const auto& villager : gamevillager) {
        std::cout << villager->getName() << " at " << villager->getLoc() << ", status: "
             << (villager->getIs_saved() ? "saved" : "in danger") << ", safe location: " << villager->getSafeLoc() <<std::endl;
    }
}

void Game::show_menu(){
    std::cout<<"\n=== Main Menu ===\n";
    std::cout<<"1. Countinue Game \n";
    std::cout<<"2. Save Game \n";
    std::cout<<"3. Load game \n";
    std::cout<<"4. Exit Game \n";
    std::cout<<"Enter your choice...\n";
} 

void Game::savegame_state(){
    std::ofstream outfile ("game.txt" , std::ios::app);

    if(!outfile){
        std::cerr<<"ERORR1 for open file\n";
    }else{
        outfile<<"TERROR "<<gameTerrorLevelTracker.getTerrorLevel()<<std::endl;//سیو کردن سطح وحشت 

        for(const auto& hero : gameHero){
            outfile<<"HERO "<<hero->getName()<<"  "<<hero->getLoc()<<"  "<<hero->getHealth()<<"  "
            <<hero->getAct_rem()<<"  "<<hero->getAct_max()<<"  "<<(hero->getin_hos() ? 1 : 0) <<std::endl;

            for(const auto& item : hero->getItems()){
            outfile<<"HERO-ITEM "<<hero->getName()<<"  "<<item.getName()<<"  "<<static_cast<int>(item.getType())<<"  "
            <<item.getEffect()<<"  "<<item.getQuantity()<<std::endl;
            }
        }

        for(const auto& monster : gameMonster){
            outfile<<"MONSTER "<<monster->getName()<<"  "<<monster->getLoc()<<"  "<<monster->getPower()<<"  "
            <<(monster->getIs_defeated() ? 1 : 0) <<std::endl;
        }

        for(const auto& villager : gamevillager){
            outfile<<"VILLAGER "<<villager->getName()<<"  "<<villager->getLoc()<<"  "<<villager->getSafeLoc()<<"  "
            <<(villager->getIs_saved() ? 1 : 0 ) <<"  "<<villager->getHealth()<<std::endl;
        }

        for(const auto& locItems : gameBoard.getItems()){
            for(const auto& item : locItems.second){
                outfile<<"BOARD_ITEM "<<locItems.first<<"  "<<item.getName()<<"  "<<static_cast<int>(item.getType())<<"  "
                <<item.getEffect()<<"  "<<item.getQuantity()<<"  "<<std::endl;
            }
        }

        for(const auto& card : gamePerkCard){
            outfile<<"PERK_CARD "<<card.getName()<<"  "<<card.getQuantity()<<"  "<<std::endl;
        }

        outfile.close();
        std::cout<<"Game saved successfully . . . \n";
    }
}

void Game::loadgame_state(){
    std::ifstream infile("game.txt");

    if(!infile){
        std::cerr<<"ERROR to opening file \n";
    }else{
        gameHero.clear();
        gameMonster.clear();
        gamevillager.clear();
        gameitem.clear();
        gamePerkCard.clear();


        std::string line;
        while(getline(infile , line)){
            std::istringstream iss(line);
            std::string type;
            iss>>type;

            if(type=="TERROR"){
                int level;
                iss>>level;
                gameTerrorLevelTracker=TerrorLevelTracker(5);//
                while(gameTerrorLevelTracker.getTerrorLevel()<level){
                    gameTerrorLevelTracker.increaseTerror(1);
                }
            }else if (type=="HERO"){
                std::string name , loc;
                int health , act_rem , act_max , in_hos;
                iss>>name>>loc>>health>>act_rem>>act_max>>in_hos;
                std::shared_ptr<Hero> hero;
                if(name=="Mayor"){
                    hero=std::make_shared<Mayor>();
                }else{
                    hero=std::make_shared<Ancient>();
                }
                hero->setLoc(loc);
                hero->setHealth(health);
                hero->setAct_rem(act_rem);
                hero->setAct_max(act_max);
                hero->setin_hos(in_hos==1);

                gameHero.push_back(hero);
            }else if(type=="HERO-ITEM"){
                std::string heroname , itemname ;
                int typeInt , effect , quantity ; 

                iss>>heroname>>itemname>>typeInt>>effect>>quantity;

                ItemType itemType = static_cast<ItemType> (typeInt);
                Item item(itemname , itemType , effect , quantity);

                for(auto& hero : gameHero){
                    if(hero->getName()==heroname){
                        hero->pickup_item(item);
                        break;
                    }
                }
            }else if(type=="MONSTER"){
                std::string name , loc;
                int power , is_defeated;

                iss>>name>>loc>>power>>is_defeated;

                std::shared_ptr<Monster> monster;

                if(name=="Dracula"){
                    monster = std::make_shared<Dracula>(loc);
                }else{
                    monster = std::make_shared<Invisible_man>(loc);
                }

                monster->setPower(power);
                if(is_defeated==1){
                    monster->defeat();
                }
                gameMonster.push_back(monster);

            }else if(type=="VILLAGER"){
                std::string name ,  loc , safeloc ;
                int is_saved , health ;

                iss>>name>>loc>>safeloc>>is_saved>>health;

                auto vill=std::make_shared<villager>(name , loc , safeloc);
                vill->setHealth(health);

                if(is_saved==1){
                    vill->rescue();
                }

                gamevillager.push_back(vill);
            }else if(type=="BOARD_ITEM"){
                std::string name , loc ;
                int typeInt , effect , quantity ;

                iss>>loc>>name>>typeInt>>effect>>quantity;

                ItemType itemType=static_cast<ItemType>(typeInt);
                Item item (name , itemType , effect , quantity );
                gameBoard.placeItem(loc , item);
            }else if(type=="PERK_CARD"){
                std::string name;
                int quantity;
                
                iss>>name>>quantity;

                gamePerkCard.emplace_back(name , quantity);
            }
        }

        infile.close();
        std::cout<<"Game state loaded successfully . . .  \n";
    }
}
