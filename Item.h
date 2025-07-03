#ifndef ITEM_H
#define ITEM_H

#include <bits/stdc++.h>

enum class ItemType { Red, Yellow };

class Item {
private:
    std::string name;
    ItemType type;

public:
    Item(const std::string& name, ItemType type);
    std::string getName() const;
    ItemType getType() const;
};

std::vector<Item> createItems();

#endif
