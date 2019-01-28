/**
 * File : Item.cpp
 * OOP 345 MS1
 * Vincent Terpstra :: 140665175
 * Oct 24 2018
 * Purpose
 * 	implementation file for Item object   
 */
#include <iostream>
#include <iomanip>

#include "Item.h"
#include "Utilities.h"

unsigned int Item::field_width = 1;

//Takes a record string and parses the data
//
Item::Item(std::string& record){
	Utilities util;
	size_t start = 0;
	bool more = true;
	name = util.extractToken(record, start, more);
	//set the fieldwidth by name
	if(name.length() > field_width) 
		field_width = name.length();
	try {
		serialNumber = std::stoi(util.extractToken(record, start, more));
	} catch (...){
		serialNumber = 0;
	}
	try {
		Quantity 	 = std::stoi(util.extractToken(record, start, more));
	} catch (...){
		Quantity = 0;
	}
	description = util.extractToken(record, start, more);
}

//Query Function for name
//
const std::string& Item::getName() const{
	return name;
};

//Query Function for serial Number
//
const unsigned int Item::getSerialNumber() const{
	return serialNumber;
};

//Query Function for Quantity
//
const unsigned int Item::getQuantity() const{
	return Quantity;
};

//Reduces Quantity by 1 minimium 0
//
void Item::updateQuantity(){
	if(Quantity > 0) Quantity--;
};

//Display function for Item
//
void Item::display(std::ostream& os, bool full) const{
	os << std::left << std::setw(field_width) << name << " ["
		<< std::setfill('0') << std::setw(6) //expected width of serial number
		<< serialNumber << "]" << std::setfill(' ');
	if(full){
		os << " Quantity "
		<< std::setw(3) << std::right << Quantity
		<< " Description " << std::left << description;
	}
	os << std::endl;
};