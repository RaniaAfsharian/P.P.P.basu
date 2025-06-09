#include "Board.hpp"
#include <iostream>

using namespace std;

// تابع کمکی خصوصی برای اضافه کردن مسیر دوطرفه
static void addPath(std::map<std::string, std::vector<std::string>>& paths, 
                    std::set<std::string>& locations, 
                    const std::string& from, 
                    const std::string& to) {
    if (locations.find(from) == locations.end() || locations.find(to) == locations.end()) {
        cout << "Uncertain route" << endl;
        return;
    }
    paths[from].push_back(to);
    paths[to].push_back(from);   // مسیر دوطرفه
}

Board::Board() {
    initializeBoard();
}

void Board::initializeBoard() {
    //مکان‌ها
    locations = {"Inn","Barn","Crypt","Precinct","Dungeon","Cave","Camp","Abbey","Tower","Mansion","Museum","Shop","Docks","Theatre","Church","Laboratory","Institute","Hospital","Graveyard"};

    // مسیرها
    addPath(paths, locations, "Cave", "Camp");
    addPath(paths, locations, "Camp", "Inn");
    addPath(paths, locations, "Camp", "Precinct");
    addPath(paths, locations, "Camp", "Mansion");
    addPath(paths, locations, "Camp", "Inn");
    addPath(paths, locations, "Inn", "Mansion");
    addPath(paths, locations, "Precinct", "Mansion");
    addPath(paths, locations, "Theater", "Mansion");
    addPath(paths, locations, "Camp", "Barn");
    addPath(paths, locations, "Camp", "Theater");
    addPath(paths, locations, "Crypt", "Abbey");
    addPath(paths, locations, "Abbey", "Mansion");
    addPath(paths, locations, "Hospital", "Church");
    addPath(paths, locations, "Graveyard", "Church");
    addPath(paths, locations, "Institute", "Laboratory");
    addPath(paths, locations, "Docks", "Tower");
    addPath(paths, locations, "Dungeon", "Tower");
    addPath(paths, locations, "Shop", "Laboratory");
    addPath(paths, locations, "Theatre", "Tower");
    addPath(paths, locations, "Theatre", "Shop");
    addPath(paths, locations, "Church", "Museum");
    addPath(paths, locations, "Church", "Mansion");
    addPath(paths, locations, "Museum", "Mansion");
    addPath(paths, locations, "Theatre", "Barn");
    addPath(paths, locations, "Theatre", "Inn");
    addPath(paths, locations, "Theatre", "Precinct");
    addPath(paths, locations, "Inn", "Precinct");
    addPath(paths, locations, "Barn", "Precinct");
    addPath(paths, locations, "Barn", "Inn");


    playerPositions[1] = "Docks"; // Ancient (بازیکن 1) در Docks
    playerPositions[2] = "Theatre"; // Mayor (بازیکن 2) در Theatre
    // تنظیم موقعیت اولیه هیولا
    monsterPositions[1] = "Castle"; // هیولا 1 در Castle
}

std::set<std::string> Board::getAvailablePaths(const std::string& location) const {
    std::set<std::string> result;
    auto it = paths.find(location);
    if (it != paths.end()) {
        for (const auto& p : it->second) {
            result.insert(p);
        }
    }
    return result;
}

bool Board::movePlayer(int playerId, const std::string& destination) {
    auto it = playerPositions.find(playerId);
    if (it == playerPositions.end()) {
        cout << "There are no players." << playerId << endl;
        return false;
    }
    if (locations.find(destination) == locations.end()) {
        cout << "The desired location does not exist." << destination << endl;
        return false;
    }
    std::string currentPos = it->second;
    auto available = getAvailablePaths(currentPos);
    if (available.find(destination) == available.end()) {
        cout << "Movement is not allowed." << currentPos << destination << endl;
        return false;
    }
    playerPositions[playerId] = destination;
    cout << "Successful move" << playerId << destination << endl;
    return true;
}


// قرار دادن آیتم در یک مکان
void Board::placeItem(const std::string& location, const Item& item) {
    if (locations.find(destination) == locations.end()) {
        cout << "The desired location does not exist." << location  << endl;
        return;
    }
    items[location].push_back(item);
    cout << "Item placed successfully." << item.getDetails() << location << endl;
}

void Board::displayBoard() const {
    cout << "Places: ";
    for (const auto& loc : locations) {
        cout << loc << " ";
    }
    cout << endl;

    cout << "Players position:" << endl;
    for (const auto& p : playerPositions) {
        cout << "Player" << p.first << " (" << (p.first == 1 ? "Archaeologist" : "Mayor") << ") in " << p.second << endl;
    }

    cout << "Monsters' location:" << endl;
    for (const auto& m : monsterPositions) {
        cout << "Monster" << m.first << " in " << m.second << endl;
    }

    cout << "Items on the board:" << endl;
    for (const auto& pair : items) {
        cout << "place " << pair.first << ":" << endl;
        for (const auto& item : pair.second) {
            cout << "  " << item.getDetails() << endl;
        }
    }
}
