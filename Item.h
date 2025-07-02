#ifndef ITEM_H
#define ITEM_H


#include <bits/stdc++.h>

enum class ItemType { Red, Blue, Yellow };

class Item {
private:
    std::string name;
    ItemType type;
    int effect;
    int quantity;

public:
    Item(const std::string& name, ItemType itemType, int effect, int quantity);

    Item(const Item&) = default;
    Item(Item&&) = default;
    Item& operator=(const Item&) = default;
    Item& operator=(Item&&) = default;

    bool useItem();
    std::string getDetails() const;
    std::string getName() const {
        return name;
    }
    ItemType getType() const {
        return type;
    }
    int getEffect() const {
        return effect;
    }
    int getQuantity() const {
        return quantity;
    }
};

std::vector<Item> createItems();

#endif