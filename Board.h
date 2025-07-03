// File: Board.h
#ifndef BOARD_H
#define BOARD_H

#include <bits/stdc++.h>
#include "Item.h"

class Board {
private:
    std::map<std::string, std::vector<Item>> items;
    std::map<std::string, std::set<std::string>> neighbors;
    std::map<std::string, std::vector<std::string>> availablePaths;

public:
    void initializeBoard();
    void addItem(const std::string& loc, const Item& item);
    void removeItems(const std::string& loc, const Item& item);
    std::map<std::string, std::vector<Item>> getItems() const { return items; }
    std::set<std::string> getNeighbors(const std::string& loc) const;
    std::vector<std::string> findShortestPath(const std::string& from, const std::string& to) const;
};

#endif