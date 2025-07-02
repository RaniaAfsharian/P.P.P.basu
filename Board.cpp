#include <iostream>
#include <iomanip>
#include <queue>
#include <map>
#include "Board.h"

using namespace std;

static void addPath(std::map<std::string, std::vector<std::string>>& paths,
                    std::set<std::string>& locations,
                    const std::string& from,
                    const std::string& to) {
    if (locations.find(from) == locations.end() || locations.find(to) == locations.end()) {
        cout << "Invalid path!" << endl;
        return;
    }
    paths[from].push_back(to);
    paths[to].push_back(from);
}

Board::Board() {
    initializeBoard();
}

void Board::initializeBoard() {
    locations = {"Inn", "Barn", "Crypt", "Precinct", "Dungeon", "Cave", "Camp", "Abbey", "Tower",
                 "Mansion", "Museum", "Shop", "Docks", "Theatre", "Church", "Laboratory",
                 "Institute", "Hospital", "Graveyard"};

    addPath(paths, locations, "Cave", "Camp");
    addPath(paths, locations, "Camp", "Inn");
    addPath(paths, locations, "Camp", "Precinct");
    addPath(paths, locations, "Camp", "Mansion");
    addPath(paths, locations, "Inn", "Mansion");
    addPath(paths, locations, "Precinct", "Mansion");
    addPath(paths, locations, "Theatre", "Mansion");
    addPath(paths, locations, "Camp", "Barn");
    addPath(paths, locations, "Camp", "Theatre");
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

    playerPositions[1] = "Docks";
    playerPositions[2] = "Theatre";
    monsterPositions[1] = "Crypt";
    monsterPositions[2] = "Laboratory";
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

std::string Board::findShortestPath(const std::string& start, const std::string& target) const {
    if (start == target) return start;

    std::queue<std::string> q;
    std::map<std::string, std::string> parent;
    std::set<std::string> visited;

    q.push(start);
    visited.insert(start);
    parent[start] = "";

    while (!q.empty()) {
        std::string current = q.front();
        q.pop();

        auto neighbors = getAvailablePaths(current);
        for (const auto& neighbor : neighbors) {
            if (visited.find(neighbor) == visited.end()) {
                visited.insert(neighbor);
                parent[neighbor] = current;
                q.push(neighbor);

                if (neighbor == target) {
                    std::string step = neighbor;
                    while (parent[step] != start) {
                        step = parent[step];
                    }
                    return step;
                }
            }
        }
    }
    return "";
}

bool Board::movePlayer(int playerId, const std::string& destination) {
    auto it = playerPositions.find(playerId);
    if (it == playerPositions.end()) {
        cout << "Player #" << playerId << " does not exist!" << endl;
        return false;
    }
    if (locations.find(destination) == locations.end()) {
        cout << "Destination does not exist: " << destination << endl;
        return false;
    }
    std::string currentPos = it->second;
    auto available = getAvailablePaths(currentPos);
    if (available.find(destination) == available.end()) {
        cout << "Move from " << currentPos << " to " << destination << " is not allowed!" << endl;
        return false;
    }
    it->second = destination;
    cout << "Move successful: Player #" << playerId << " to " << destination << endl;
    return true;
}

bool Board::moveMonster(int monsterId, const std::string& destination) {
    auto it = monsterPositions.find(monsterId);
    if (it == monsterPositions.end()) {
        cout << "Monster #" << monsterId << " does not exist!" << endl;
        return false;
    }
    if (locations.find(destination) == locations.end()) {
        cout << "Destination does not exist: " << destination << endl;
        return false;
    }
    std::string currentPos = it->second;
    auto available = getAvailablePaths(currentPos);
    if (available.find(destination) == available.end()) {
        cout << "Move from " << currentPos << " to " << destination << " is not allowed!" << endl;
        return false;
    }
    it->second = destination;
    cout << "Move successful: Monster #" << monsterId << " to " << destination << endl;
    return true;
}

void Board::placeItem(const std::string& location, const Item& item) {
    if (locations.find(location) == locations.end()) {
        cout << "Destination does not exist: " << location << endl;
        return;
    }
    items[location].push_back(item);
    cout << "Item placed successfully: " << item.getDetails() << " at " << location << endl;
}

void Board::removeItems(const std::string& location) {
    items.erase(location);
}

void Board::displayBoard() const {
    cout << "Locations: ";
    for (const auto& loc : locations) {
        cout << loc << " ";
    }
    cout << endl;

    cout << "Player positions:\n";
    for (const auto& p : playerPositions) {
        cout << "Player #" << p.first << " (" << (p.first == 1 ? "Ancient" : "Mayor") << ") at " << p.second << endl;
    }

    cout << "Monster positions:\n";
    for (const auto& m : monsterPositions) {
        cout << "Monster #" << m.first << " at " << m.second << endl;
    }

    cout << "Items on board:\n";
    for (const auto& pair : items) {
        cout << "Location " << pair.first << ":\n";
        for (const auto& item : pair.second) {
            cout << "  " << item.getDetails() << endl;
        }
    }
}

void Board::displayMap(const vector<string>& heroLocs,
                       const vector<string>& monsterLocs,
                       const vector<string>& villagerLocs) const {
    cout << "Terror Level: [" << 3 << "]\n";
    string laboratory = (find(monsterLocs.begin(), monsterLocs.end(), "Laboratory") != monsterLocs.end()) ? "\033[31mLaboratory\033[0m" :
                        (find(heroLocs.begin(), heroLocs.end(), "Laboratory") != heroLocs.end()) ? "\033[32mLaboratory\033[0m" :
                        (find(villagerLocs.begin(), villagerLocs.end(), "Laboratory") != villagerLocs.end()) ? "\033[33mLaboratory\033[0m" : "Laboratory";
    string mansion = (find(monsterLocs.begin(), monsterLocs.end(), "Mansion") != monsterLocs.end()) ? "\033[31mMansion\033[0m" :
                     (find(heroLocs.begin(), heroLocs.end(), "Mansion") != heroLocs.end()) ? "\033[32mMansion\033[0m" :
                     (find(villagerLocs.begin(), villagerLocs.end(), "Mansion") != villagerLocs.end()) ? "\033[33mMansion\033[0m" : "Mansion";
    string crypt = (find(monsterLocs.begin(), monsterLocs.end(), "Crypt") != monsterLocs.end()) ? "\033[31mCrypt\033[0m" :
                   (find(heroLocs.begin(), heroLocs.end(), "Crypt") != heroLocs.end()) ? "\033[32mCrypt\033[0m" :
                   (find(villagerLocs.begin(), villagerLocs.end(), "Crypt") != villagerLocs.end()) ? "\033[33mCrypt\033[0m" : "Crypt";
    string hospital = (find(monsterLocs.begin(), monsterLocs.end(), "Hospital") != monsterLocs.end()) ? "\033[31mHospital\033[0m" :
                      (find(heroLocs.begin(), heroLocs.end(), "Hospital") != heroLocs.end()) ? "\033[32mHospital\033[0m" :
                      (find(villagerLocs.begin(), villagerLocs.end(), "Hospital") != villagerLocs.end()) ? "\033[33mHospital\033[0m" : "Hospital";
    string barn = (find(monsterLocs.begin(), monsterLocs.end(), "Barn") != monsterLocs.end()) ? "\033[31mBarn\033[0m" :
                  (find(heroLocs.begin(), heroLocs.end(), "Barn") != heroLocs.end()) ? "\033[32mBarn\033[0m" :
                  (find(villagerLocs.begin(), villagerLocs.end(), "Barn") != villagerLocs.end()) ? "\033[33mBarn\033[0m" : "Barn";
    string dungeon = (find(monsterLocs.begin(), monsterLocs.end(), "Dungeon") != monsterLocs.end()) ? "\033[31mDungeon\033[0m" :
                     (find(heroLocs.begin(), heroLocs.end(), "Dungeon") != heroLocs.end()) ? "\033[32mDungeon\033[0m" :
                     (find(villagerLocs.begin(), villagerLocs.end(), "Dungeon") != villagerLocs.end()) ? "\033[33mDungeon\033[0m" : "Dungeon";
    string camp = (find(monsterLocs.begin(), monsterLocs.end(), "Camp") != monsterLocs.end()) ? "\033[31mCamp\033[0m" :
                  (find(heroLocs.begin(), heroLocs.end(), "Camp") != heroLocs.end()) ? "\033[32mCamp\033[0m" :
                  (find(villagerLocs.begin(), villagerLocs.end(), "Camp") != villagerLocs.end()) ? "\033[33mCamp\033[0m" : "Camp";
    string abbey = (find(monsterLocs.begin(), monsterLocs.end(), "Abbey") != monsterLocs.end()) ? "\033[31mAbbey\033[0m" :
                   (find(heroLocs.begin(), heroLocs.end(), "Abbey") != heroLocs.end()) ? "\033[32mAbbey\033[0m" :
                   (find(villagerLocs.begin(), villagerLocs.end(), "Abbey") != villagerLocs.end()) ? "\033[33mAbbey\033[0m" : "Abbey";

    cout << "  " << laboratory << " ---- " << abbey << " ---- " << crypt << "\n";
    cout << "  |       |         |\n";
    cout << "  |       |         " << camp << "\n";
    cout << "  |       |         |\n";
    cout << hospital << "-" << barn << "       " << mansion << "\n";
    cout << "  |       |         |\n";
    cout << dungeon << "-------------|\n";
    cout << endl;
}