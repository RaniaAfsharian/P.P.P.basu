#ifndef BOARD_H
#define BOARD_H

#include <string>
#include <map>
#include <vector>
#include <set>
#include <queue>
#include "Item.h"

class Board {
private:
    std::set<std::string> locations;
    std::map<int, std::string> playerPositions;
    std::map<int, std::string> monsterPositions;
    std::map<std::string, std::vector<std::string>> paths;
    std::map<std::string, std::vector<Item>> items;

public:
    Board();

    Board(const Board&) = delete;
    Board& operator=(const Board&) = delete;

    void initializeBoard();
    std::set<std::string> getAvailablePaths(const std::string& location) const;
    std::string findShortestPath(const std::string& start, const std::string& target) const;
    bool movePlayer(int playerId, const std::string& destination);
    bool moveMonster(int monsterId, const std::string& destination);
    void placeItem(const std::string& location, const Item& item);
    void removeItems(const std::string& location);
    void displayBoard() const;
    void displayMap(const std::vector<std::string>& heroLocs,
                    const std::vector<std::string>& monsterLocs,
                    const std::vector<std::string>& villagerLocs) const;

    const std::set<std::string>& getLocations() const { return locations; }
    const std::map<int, std::string>& getPlayerPositions() const { return playerPositions; }
    const std::map<int, std::string>& getMonsterPositions() const { return monsterPositions; }
    const std::map<std::string, std::vector<std::string>>& getPaths() const { return paths; }
    const std::map<std::string, std::vector<Item>>& getItems() const { return items; }
    std::map<std::string, std::vector<Item>>& getItemsNonConst() { return items; }
};

#endif