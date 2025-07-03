// File: Item.cpp
#include <bits/stdc++.h>
#include "Item.h"

Item::Item(const std::string& name, ItemType type) : name(name), type(type) {}

std::string Item::getName() const { return name; }

ItemType Item::getType() const { return type; }

std::vector<Item> createItems() {
    std::vector<Item> items;
    items.emplace_back("Red1", ItemType::Red);
    items.emplace_back("Red2", ItemType::Red);
    items.emplace_back("Red3", ItemType::Red);
    items.emplace_back("Yellow1", ItemType::Yellow);
    items.emplace_back("Yellow2", ItemType::Yellow);
    items.emplace_back("Yellow3", ItemType::Yellow);
    return items;
}