#include <iostream>
#include <random>
#include <algorithm>
#include <queue>
#include <map>
#include "game.h"

using namespace std;

Game::Game() : gameTerrorLevelTracker(5), gameActionSystem(gameBoard), playerindex(0), gameOver(false) {
    setGame();
}

void Game::assignHeroes() {
    string player1, player2;
    int time1, time2;
    cout << "Enter name of player 1: ";
    cin >> player1;
    cout << "Enter hours since " << player1 << " last ate garlic: ";
    cin >> time1;
    cout << "Enter name of player 2: ";
    cin >> player2;
    cout << "Enter hours since " << player2 << " last ate garlic: ";
    cin >> time2;

    bool player1Starts = time1 < time2;
    string starter = player1Starts ? player1 : player2;
    string other = player1Starts ? player2 : player1;

    cout << starter << ", choose your hero (1 for Mayor, 2 for Ancient): ";
    int choice;
    cin >> choice;

    if (player1Starts) {
        if (choice == 1) {
            gameHero.push_back(make_shared<Mayor>());
            gameHero.push_back(make_shared<Ancient>());
        } else {
            gameHero.push_back(make_shared<Ancient>());
            gameHero.push_back(make_shared<Mayor>());
        }
    } else {
        if (choice == 1) {
            gameHero.push_back(make_shared<Mayor>());
            gameHero.push_back(make_shared<Ancient>());
        } else {
            gameHero.push_back(make_shared<Ancient>());
            gameHero.push_back(make_shared<Mayor>());
        }
    }
    cout << starter << " is " << gameHero[0]->getName() << ", " << other << " is " << gameHero[1]->getName() << endl;
}

void Game::setGame() {
    assignHeroes();

    gameMonster.push_back(make_shared<Invisible_man>("Crypt"));
    gameMonster.push_back(make_shared<Dracula>("Laboratory"));

    gamevillager.push_back(make_shared<villager>("Wilbur & Chick", "", "Dungeon"));
    gamevillager.push_back(make_shared<villager>("Maria", "", "Camp"));
    gamevillager.push_back(make_shared<villager>("Maleva", "", "Shop"));
    gamevillager.push_back(make_shared<villager>("Prof. Pearson", "", "Museum"));
    gamevillager.push_back(make_shared<villager>("Dr. Reed", "", "Camp"));
    gamevillager.push_back(make_shared<villager>("Dr. Cranley", "", "Precinct"));
    gamevillager.push_back(make_shared<villager>("Frits", "", "Institute"));

    for (const auto& item : createItems()) {
        gameitem.push_back(item);
        string loc = item.getName().substr(item.getName().find('_') + 1);
        if (gameBoard.getLocations().find(loc) != gameBoard.getLocations().end()) {
            gameBoard.placeItem(loc, item);
        }
    }

    gamePerkCard = createPerkCards();
    random_device rd;
    mt19937 gen(rd());
    shuffle(gamePerkCard.begin(), gamePerkCard.end(), gen);
    for (auto& hero : gameHero) {
        if (!gamePerkCard.empty()) {
            gameActionSystem.addPerkCard(gamePerkCard.back());
            gamePerkCard.pop_back();
        }
    }

    monsterCards = createMonsterCards();
}

void Game::addLog(const string& log) {
    gameLogs.push_back(log);
}

void Game::loop_Game() {
    while (!gameOver) {
        show_Gamestate();

        for (auto& hero : gameHero) {
            heroGame(*hero);
            check_win();
            if (gameOver) break;
        }

        if (!gameOver) {
            monsterGame();
            check_win();
        }
    }

    if (gameTerrorLevelTracker.isGameOver()) {
        cout << "Game over! The city is overrun by monsters!" << endl;
    } else if (monsterCards.empty()) {
        cout << "Game over! Monster cards depleted!" << endl;
    } else {
        cout << "Congratulations! All monsters have been defeated!" << endl;
    }
}

void Game::heroGame(Hero& hero) {
    cout << "\n===" << hero.getName() << "'s Turn ===\n";
    hero.startTurn();
    gameActionSystem.startHeroTurn(hero.getName(), hero.getAct_max());

    while (hero.getAct_rem() > 0 && !gameOver) {
        show_Gamestate();
        int choice;
        bool actSuccess = false;
        string goal;
        cout << "Actions remaining: " << hero.getAct_rem() << endl;
        cout << "Choose action: " << endl;
        cout << "1. Move" << endl;
        cout << "2. Guide Villager" << endl;
        cout << "3. Pick Up Item" << endl;
        cout << "4. Advance Mission" << endl;
        cout << "5. Defeat Monster" << endl;
        cout << "6. Use Item" << endl;
        cout << "7. Play Perk Card" << endl;
        cin >> choice;

        switch (choice) {
        case 1: {
            cout << "Available locations: ";
            auto availablePaths = gameBoard.getAvailablePaths(hero.getLoc());
            for (const auto& loc : availablePaths) {
                cout << loc << " ";
            }
            cout << "\nEnter destination: ";
            cin >> goal;
            if (gameBoard.movePlayer(&hero == gameHero[0].get() ? 1 : 2, goal)) {
                hero.move(goal);
                actSuccess = gameActionSystem.performAction(ActionType::MOVE, gameMonster);
                addLog(hero.getName() + " moved to " + goal);
            } else {
                cout << "Invalid move!" << endl;
                actSuccess = false;
            }
            break;
        }
        case 2: {
            cout << "Villagers in current location: " << endl;
            for (const auto& villager : gamevillager) {
                if (villager->getLoc() == hero.getLoc() && !villager->getIs_saved()) {
                    cout << villager->getName() << ", safe location: " << villager->getSafeLoc() << endl;
                }
            }
            cout << "Enter villager name to guide: ";
            string villagername;
            cin >> villagername;

            shared_ptr<villager> foundvill = nullptr;
            for (const auto& villager : gamevillager) {
                if (villager->getName() == villagername) {
                    foundvill = villager;
                    break;
                }
            }
            if (foundvill != nullptr) {
                cout << "Enter destination: ";
                cin >> goal;
                if (gameBoard.getAvailablePaths(foundvill->getLoc()).count(goal)) {
                    hero.move_valliger(foundvill, goal);
                    actSuccess = gameActionSystem.performAction(ActionType::GUIDE, gameMonster);
                    addLog("Guided " + villagername + " to " + goal);
                    if (goal == foundvill->getSafeLoc()) {
                        foundvill->rescue();
                        addLog(villagername + " rescued at " + goal);
                    }
                } else {
                    cout << "Invalid destination for villager!" << endl;
                    actSuccess = false;
                }
            } else {
                cout << "Villager not found!" << endl;
                actSuccess = false;
            }
            break;
        }
        case 3: {
            actSuccess = gameActionSystem.performAction(ActionType::PICK_UP, gameMonster);
            addLog(hero.getName() + " picked up items at " + hero.getLoc());
            break;
        }
        case 4: {
            actSuccess = gameActionSystem.performAction(ActionType::ADVANCE, gameMonster);
            addLog(hero.getName() + " advanced mission at " + hero.getLoc());
            break;
        }
        case 5: {
            cout << "Monsters in current location: " << endl;
            for (const auto& monster : gameMonster) {
                if (monster->getLoc() == hero.getLoc() && !monster->getIs_defeated()) {
                    cout << monster->getName() << ", power: " << monster->getPower() << endl;
                }
            }
            actSuccess = gameActionSystem.performAction(ActionType::DEFEAT, gameMonster);
            addLog(hero.getName() + " attempted to defeat monster at " + hero.getLoc());
            break;
        }
        case 6: {
            cout << "Your items: " << endl;
            const auto& items = hero.getItems();
            for (size_t i = 0; i < items.size(); i++) {
                cout << i + 1 << " " << items[i].getDetails() << endl;
            }
            cout << "Enter item number to use: ";
            int itemNum;
            cin >> itemNum;
            if (itemNum > 0 && itemNum <= static_cast<int>(items.size())) {
                hero.use_Item(items[itemNum - 1].getName());
                actSuccess = true;
                addLog(hero.getName() + " used " + items[itemNum - 1].getName());
            } else {
                cout << "Invalid item number!" << endl;
            }
            break;
        }
        case 7: {
            cout << "Available perk cards: " << endl;
            for (size_t i = 0; i < gameActionSystem.getActivePerks().size(); ++i) {
                cout << i + 1 << ": " << gameActionSystem.getActivePerks()[i].getDetails() << endl;
            }
            cout << "Enter perk card number to play: ";
            int perkNum;
            cin >> perkNum;
            if (perkNum > 0 && perkNum <= static_cast<int>(gameActionSystem.getActivePerks().size())) {
                gameActionSystem.playPerkCard(perkNum - 1);
                actSuccess = true;
                addLog(hero.getName() + " played perk card");
            } else {
                cout << "Invalid perk card number!" << endl;
            }
            break;
        }
        default:
            cout << "Invalid choice!" << endl;
        }

        if (actSuccess) {
            hero.useAct();
        }
    }
}

void Game::monsterGame() {
    cout << "\n=== Monster Turn ===\n";
    if (monsterCards.empty()) {
        gameOver = true;
        return;
    }

    auto card = monsterCards.back();
    monsterCards.pop_back();
    cout << "Monster card drawn: " << card.name << endl;

    int terrorLevel = gameTerrorLevelTracker.getTerrorLevel();
    card.applyEvent(gamevillager, terrorLevel, gameBoard.getLocations());
    card.applyItems(gameBoard.getItemsNonConst(), gameitem);
    gameTerrorLevelTracker.setTerrorLevel(terrorLevel);

    for (auto& monster : gameMonster) {
        if (monster->getIs_defeated()) continue;
        monster->useSpecialP();
        int terrorLevel = gameTerrorLevelTracker.getTerrorLevel();
        card.applyMonsterStrike(monster, gameHero, gamevillager, terrorLevel);
        gameTerrorLevelTracker.setTerrorLevel(terrorLevel);
        addLog(monster->getName() + " used special power");

        std::string nearestTarget;
        int minDistance = INT_MAX;
        for (const auto& hero : gameHero) {
            if (hero->getHealth() > 0) {
                std::string nextStep = gameBoard.findShortestPath(monster->getLoc(), hero->getLoc());
                if (!nextStep.empty()) {
                    int dist = 0;
                    std::string temp = hero->getLoc();
                    while (temp != monster->getLoc() && !temp.empty()) {
                        temp = gameBoard.findShortestPath(monster->getLoc(), temp);
                        dist++;
                    }
                    if (dist < minDistance) {
                        minDistance = dist;
                        nearestTarget = hero->getLoc();
                    }
                }
            }
        }
        for (const auto& villager : gamevillager) {
            if (!villager->getIs_saved() && !villager->getLoc().empty()) {
                std::string nextStep = gameBoard.findShortestPath(monster->getLoc(), villager->getLoc());
                if (!nextStep.empty()) {
                    int dist = 0;
                    std::string temp = villager->getLoc();
                    while (temp != monster->getLoc() && !temp.empty()) {
                        temp = gameBoard.findShortestPath(monster->getLoc(), temp);
                        dist++;
                    }
                    if (dist < minDistance) {
                        minDistance = dist;
                        nearestTarget = villager->getLoc();
                    }
                }
            }
        }

        if (!nearestTarget.empty()) {
            std::string nextStep = gameBoard.findShortestPath(monster->getLoc(), nearestTarget);
            if (!nextStep.empty() && gameBoard.moveMonster(monster == gameMonster[0] ? 1 : 2, nextStep)) {
                monster->setLoc(nextStep);
                addLog(monster->getName() + " moved to " + nextStep);
            } else {
                addLog(monster->getName() + " could not move to a valid location.");
            }
        } else {
            addLog(monster->getName() + " found no valid target to move towards.");
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
    if (allDefeated || gameTerrorLevelTracker.isGameOver() || monsterCards.empty()) {
        gameOver = true;
    }
}

void Game::show_Gamestate() {
    cout << "\n==== Game State ====\n";
    vector<string> heroLocs, monsterLocs, villagerLocs;
    for (const auto& hero : gameHero) heroLocs.push_back(hero->getLoc());
    for (const auto& monster : gameMonster) monsterLocs.push_back(monster->getLoc());
    for (const auto& villager : gamevillager) if (!villager->getLoc().empty()) villagerLocs.push_back(villager->getLoc());
    gameBoard.displayMap(heroLocs, monsterLocs, villagerLocs);
    cout << "Logs:\n";
    for (const auto& log : gameLogs) {
        cout << "- " << log << endl;
    }
    cout << "\nHeroes:\n";
    for (const auto& hero : gameHero) {
        cout << hero->getName() << " at " << hero->getLoc() << ", health: " << hero->getHealth()
             << ", actions: " << hero->getAct_rem() << "/" << hero->getAct_max() << endl;
    }
    cout << "\nMonsters:\n";
    for (const auto& monster : gameMonster) {
        cout << monster->getName() << " at " << monster->getLoc() << ", power: " << monster->getPower()
             << ", status: " << (monster->getIs_defeated() ? "defeated" : "active") << endl;
    }
    cout << "\nVillagers:\n";
    for (const auto& villager : gamevillager) {
        cout << villager->getName() << " at " << villager->getLoc() << ", status: "
             << (villager->getIs_saved() ? "saved" : "in danger") << ", safe location: " << villager->getSafeLoc() << endl;
    }
    gameLogs.clear();
}
