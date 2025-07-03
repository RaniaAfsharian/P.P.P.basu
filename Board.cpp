// File: Board.cpp
#include <bits/stdc++.h>
#include "Board.h"

void Board::initializeBoard() {
    availablePaths = {
        {"Precinct", {"Church", "Theater", "Hospital"}},
        {"Church", {"Precinct", "Square", "Museum"}},
        {"Theater", {"Precinct", "Square", "Docks"}},
        {"Square", {"Church", "Theater", "Hospital", "Museum", "Docks"}},
        {"Hospital", {"Precinct", "Square"}},
        {"Museum", {"Church", "Square"}},
        {"Docks", {"Theater", "Square"}}
    };
    
    neighbors.clear();
    for (const auto& pair : availablePaths) {
        neighbors[pair.first].insert(pair.second.begin(), pair.second.end());
    }
}

void Board::addItem(const std::string& loc, const Item& item) {
    items[loc].push_back(item);
}

void Board::removeItems(const std::string& loc, const Item& item) {
    auto it = items.find(loc);
    if (it != items.end()) {
        auto& itemList = it->second;
        for (auto itemIt = itemList.begin(); itemIt != itemList.end(); ++itemIt) {
            if (itemIt->getName() == item.getName()) {
                itemList.erase(itemIt);
                break;
            }
        }
        if (itemList.empty()) {
            items.erase(it);
        }
    }
}

std::set<std::string> Board::getNeighbors(const std::string& loc) const {
    auto it = neighbors.find(loc);
    if (it != neighbors.end()) {
        return it->second;
    }
    return std::set<std::string>();
}

std::vector<std::string> Board::findShortestPath(const std::string& from, const std::string& to) const {
    std::queue<std::string> q;
    std::map<std::string, std::string> parent;
    std::set<std::string> visited;

    q.push(from);
    visited.insert(from);
    parent[from] = "";

    while (!q.empty()) {
        std::string current = q.front();
        q.pop();

        if (current == to) {
            std::vector<std::string> path;
            std::string node = to;
            while (!node.empty()) {
                path.push_back(node);
                node = parent[node];
            }
            std::reverse(path.begin(), path.end());
            return path;
        }

        auto it = neighbors.find(current);
        if (it != neighbors.end()) {
            for (const auto& neighbor : it->second) {
                if (visited.find(neighbor) == visited.end()) {
                    q.push(neighbor);
                    visited.insert(neighbor);
                    parent[neighbor] = current;
                }
            }
        }
    }
    return std::vector<std::string>();
}