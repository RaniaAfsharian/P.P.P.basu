#include <vector>
#include "Item.hpp"
#include <sstream>

using namespace std;

// تابع برای ایجاد لیست آیتم‌ها
vector<Item> createItems() {
    vector<Item> items;
    items.emplace_back("Flower_Docks", ItemType::Yellow, 2, 2);       
    items.emplace_back("TarotDeck_Camp", ItemType::Yellow, 3, 2);    
    items.emplace_back("Garlic_Inn", ItemType::Yellow, 2, 2);      
    items.emplace_back("Inn", ItemType::Yellow, 2, 2);
    items.emplace_back("MirroredBox_Mansion", ItemType::Yellow, 3, 2);
    items.emplace_back("Stake_Abbey", ItemType::Yellow, 3, 2);       
    items.emplace_back("ScrollOfThoth_Museum", ItemType::Yellow, 4, 2); 
    items.emplace_back("Violin_Camp", ItemType::Yellow, 3, 2);        // اصلاح املا (Violon به Violin)
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
    items.emplace_back("Searchlight_Precinct", ItemType::Blue, 2, 2); 
    items.emplace_back("Experiment_Laboratory", ItemType::Blue, 2, 2); 
    items.emplace_back("Analysis_Institute", ItemType::Blue, 2, 2);   
    items.emplace_back("Rotenone_Institute", ItemType::Blue, 3, 2);   
    items.emplace_back("CosmicRayDiffuser_Tower", ItemType::Blue, 3, 2); 
    items.emplace_back("Nebularium_Tower", ItemType::Blue, 3, 2);    
    items.emplace_back("MonoCaneMixture_Inn", ItemType::Blue, 3, 2); 
    items.emplace_back("Fossil_Camp", ItemType::Blue, 3, 2);         
    return items;
}

Item::Item(string name, ItemType type, int effect, int quantity)
    : name(name), type(type), effect(effect), quantity(quantity) {}

// استفاده از آیتم و بررسی تعداد
bool Item::useItem() {
    if (quantity <= 0) {
        return false; // آیتم موجود نیست
    }
    --quantity;
    //  اثر آیتم 
    return true;
}

// دریافت جزئیات آیتم به صورت رشته
string Item::getDetails() const {
    stringstream ss;
    ss << "Name: " << name << ", Type: ";
    switch (type) {
        case ItemType::Red: ss << "Red"; break;
        case ItemType::Blue: ss << "Blue"; break;
        case ItemType::Yellow: ss << "Yellow"; break;
    }
    ss << ", Effect: " << effect << ", Quantity: " << quantity;
    return ss.str();
}