#ifndef ITEM_HPP
#define ITEM_HPP

#include <string>
#include <vector>

enum class ItemType { Red, Blue, Yellow };

class Item {
private:
    std::string name;
    ItemType type;
    int effect;
    int quantity;

public:
    Item(std::string name, ItemType type, int effect, int quantity);

    Item(const Item&) = delete;
    Item& operator=(const Item&) = delete;

    bool useItem(); 
    std::string getDetails() const; 
    std::string getName() const { return name; }
    ItemType getType() const { return type; }
    int getEffect() const { return effect; }
    int getQuantity() const { return quantity; }
};

std::vector<Item> createItems();

#endif // ITEM_HPP
