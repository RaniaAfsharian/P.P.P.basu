#include <bits/stdc++.h>

#include "Item.h"

using namespace std;

vector<Item> createItems() {
    vector<Item> items;
    items.emplace_back("Flower_Docks", ItemType::Yellow, 2, 2);
    items.emplace_back("TarotDeck_Camp", ItemType::Yellow, 3, 2);
    items.emplace_back("Garlic_Inn", ItemType::Yellow, 2, 2);
    items.emplace_back("MirroredBox_Mansion", ItemType::Yellow, 3, 2);
    items.emplace_back("Stake_Abbey", ItemType::Yellow, 3, 2);
    items.emplace_back("ScrollOfThoth_Museum", ItemType::Yellow, 4, 2);
    items.emplace_back("Violin_Camp", ItemType::Yellow, 3, 2);
    items.emplace_back("Tablet_Museum", ItemType::Yellow, 3, 2);
    items.emplace_back("Wolfsbane_Camp", ItemType::Yellow, 4, 2);
    items.emplace_back("Charm_Camp", ItemType::Yellow, 4, 2);
    items.emplace_back("Dart_Inn", ItemType::Red, 2, 2);
    items.emplace_back("FirePoker_Mansion", ItemType::Red, 3, 2);
    items.emplace_back("Rapier_Theatre", ItemType::Red, 5, 2);
    items.emplace_back("Shovel_Graveyard", ItemType::Red, 2, 2);
    items.emplace_back("Torch_Barn", ItemType::Red, 5, 2);
    items.emplace_back("Pitchfork_Barn", ItemType::Red, 4, 2);
    items.emplace_back("Rifle_Barn", ItemType::Red, 6, 2);
    items.emplace_back("SilverCane_Shop", ItemType::Red, 6, 2);
    items.emplace_back("Knife_Docks", ItemType::Red, 3, 2);
    items.emplace_back("Pistol_Precinct", ItemType::Red, 6, 2);
    items.emplace_back("BearTrap_Shop", ItemType::Red, 4, 2);
    items.emplace_back("Speargun_Institute", ItemType::Red, 4, 2);
    items.emplace_back("AnatomyText_Institute", ItemType::Blue, 1, 2);
    items.emplace_back("Centrifuge_Laboratory", ItemType::Blue, 1, 2);
    items.emplace_back("Kite_Tower", ItemType::Blue, 1, 2);
    items.emplace_back("Research_Tower", ItemType::Blue, 2, 2);
    items.emplace_back("Telescope_Mansion", ItemType::Blue, 2, 2);
    return items;
}

Item::Item(const std::string& name, ItemType itemType, int effect, int quantity)
    : name(name), type(itemType), effect(effect), quantity(quantity) {}

bool Item::useItem() {
    if (quantity <= 0) {
        cout << "No quantity left for item: " << name << endl;
        return false;
    }
    quantity--;
    cout << "Item used: " << name << endl;
    return true;
}

std::string Item::getDetails() const {
    stringstream ss;
    ss << "Item: " << name << ", Type: ";
    switch (type) {
        case ItemType::Red: ss << "Red"; break;
        case ItemType::Blue: ss << "Blue"; break;
        case ItemType::Yellow: ss << "Yellow"; break;
    }
    ss << ", Effect: " << effect << ", Quantity: " << quantity;
    return ss.str();
}