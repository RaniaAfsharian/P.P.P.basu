#ifndef TERROR_LEVEL_TRACKER_HPP
#define TERROR_LEVEL_TRACKER_HPP

class TerrorLevelTracker {
private:
    int currentLevel;    
    const int maxLevel;  

public:
    TerrorLevelTracker(int maxLevel = 5);

    void increaseTerror(int amount = 1);

    int getCurrentLevel() const;

    bool isGameOver() const;

    void displayTerrorLevel() const;
};

#endif 