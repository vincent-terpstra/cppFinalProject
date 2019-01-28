/**
 * File : CustomerOrder.cpp
 * OOP 345 MS2
 * Vincent Terpstra :: 140665175
 * Nov 10 2018
 * Purpose
 * 	implementation file for CustomerOrder object   
 */
#include <iostream>
#include <vector>
#include <iomanip>

#include "CustomerOrder.h"
#include "Item.h"
#include "Utilities.h"


//Default constructor set to safe empty state
//
unsigned int CustomerOrder::name_width = 0;

CustomerOrder::CustomerOrder(){
    ItemCount = 0;
    ItemList = nullptr;
}

//Constructor with single string record
//
CustomerOrder::CustomerOrder(std::string& entry){
    Utilities util;
    size_t idx = 0;
    bool more = true;
    Name    = util.extractToken(entry, idx, more);
    if(Name.length() > name_width)
        name_width = Name.length(); //for display purposes
        
    Product = util.extractToken(entry, idx, more);
    std::vector<std::string> names;
    while(more)
        names.push_back( util.extractToken(entry, idx, more));
    
    ItemCount = names.size();
    ItemList  = new ItemInfo*[names.size()];
    std::transform(names.begin(), names.end(), ItemList,
        [](std::string name){ return new ItemInfo(name); }
    );
}

//Destructor
//
CustomerOrder::~CustomerOrder(){
    //delete each ItemInfo in ItemList
    if(ItemCount > 0)
    std::for_each(ItemList, ItemList + ItemCount, 
        [](ItemInfo* item){
            delete item;
        }
    );
    delete[] ItemList;
}

//Object move functionality
//
CustomerOrder::CustomerOrder(CustomerOrder&& move) : CustomerOrder() {
    *this = std::move(move); //use move assignment operator
}

//Move Assignment Operator
//
CustomerOrder& CustomerOrder::operator=(CustomerOrder&& rhs){
    if(this != &rhs ){ //check for self assign
        Name    = rhs.Name;
        Product = rhs.Product;
        //switch counts
        int tmpCount  = ItemCount;
            ItemCount = rhs.ItemCount;
        rhs.ItemCount = tmpCount;

        //switch lists
        ItemInfo** tmpList = ItemList;
            ItemList = rhs.ItemList;
        rhs.ItemList = tmpList;
    }
    return *this;
}

//Returns true if order completely filled, false otherwise
//
bool CustomerOrder::getOrderFillState(){
    return std::all_of( ItemList, ItemList + ItemCount,
        [](ItemInfo * item){ return item->FillState; }
    );
}

//Returns true if the given item has been filled
//
bool CustomerOrder::getItemFillState(std::string state){
    return std::all_of(ItemList, ItemList + ItemCount,
        [&](ItemInfo * info){
            //item must either not be the checked item OR be filled
            return info->ItemName.compare(state) != 0 || info->FillState;
        }
    );
}

//Fills the customer order item if the inventory on Item is available
//
void CustomerOrder::fillItem(Item& item, std::ostream& os){
    std::for_each(ItemList, ItemList + ItemCount, 
        [&](ItemInfo * info){
            if((info->ItemName.compare(item.getName()) == 0) && //same name
               !info->FillState &&   //not filled
                item.getQuantity()){ //more then one Item in inventory
                    info->FillState = true;
                    info->SerialNumber = item.getSerialNumber();
                 // item.updateQuantity(); //NOT ENOUGH ITEMS IN INVENTORY TO PASS MILESTONE 3
                    os  << "Filled " << std::setw(name_width) << Name << " " 
                        << Product << "[" 
                        << item.getName() << "]" << std::endl;
            }    
        }
    );
}

//Member Function
//  outputs into ostream
void CustomerOrder::Display(std::ostream& os){
    //check that the Order exists
    if(ItemList){
        os  << Name << std::endl 
            << Product << std::endl;

        std::for_each(ItemList, ItemList + ItemCount, 
            [&](ItemInfo * item){
                os  << "[" 
                    << std::setw(6) << std::setfill('0')
                    << item->SerialNumber << "] " 
                    << std::setw(Item::nameWidth()) << std::setfill(' ')
                    << item->ItemName << " - " 
                    <<(item->FillState ? "FILLED" : "MISSING")
                    << std::endl;
            }
        );
    } else {
        os << "Empty Customer";
    }
}