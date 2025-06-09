#include "TerrorLevelTracker.hpp"
#include <iostream>

TerrorLevelTracker::TerrorLevelTracker(int maxLevel)
    : currentLevel(0), maxLevel(maxLevel > 0 ? maxLevel : 5) {}

void TerrorLevelTracker::increaseTerror(int amount) {
    if (amount > 0) {
        currentLevel += amount;
        if (currentLevel > maxLevel) {
            currentLevel = maxLevel; // محدود کردن به حداکثر
        }
    }
}

int TerrorLevelTracker::getCurrentLevel() const {
    return currentLevel;
}


bool TerrorLevelTracker::isGameOver() const {
    return currentLevel >= maxLevel;
}

// نمایش سطح وحشت با نوار پیشرفت در ترمینال
void TerrorLevelTracker::displayTerrorLevel() const {
    std::cout << "Horror level: " << currentLevel << "/" << maxLevel << " [";
    for (int i = 0; i < maxLevel; ++i) {
        std::cout << (i < currentLevel ? "█" : " ");
    }
    std::cout << "]" << std::endl;
    if (isGameOver()) {
        std::cout << "End of the game: The city is overrun by monsters!" << std::endl;
    }
}