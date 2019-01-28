/**
 * File Utilities.cpp
 * OOP 345 MS1
 * Vincent Terpstra :: 140665175
 * Oct 24 2018
 * Purpose
 * 	Implements the Utilities class   
 */

#include <fstream>
#include <iostream>

#include "Utilities.h"

//size_t field_width;           // maximum field width needed 
//static char delimiter;        // field delimiter character
//Default constructor
//  sets field width to 1
Utilities::Utilities(): field_width(1){
}

//Modifier Function for fieldWidth
//
void Utilities::setFieldWidth(size_t size){
    if(size > field_width)
        field_width = size;
}

//Query Function for fieldwidth
//
size_t Utilities::getFieldWidth() const {
    return field_width;
}

using std::string;
//Extracts a string from the input
//
const std::string Utilities::extractToken(const string& input, size_t& size, bool& more){
    size_t del = input.find(delimiter, size);
    if(del == string::npos){
        more = false;
        del = input.length();
        if(input.find('\r', 0) != string::npos) del --;
    }

    string sub = input.substr(size, del - size);
    size = del + 1;
    if(sub.length() == 0){
        throw("No token between the two delimiters");
    }
    setFieldWidth(sub.length());
    return sub;
}

char Utilities::delimiter = ' ';
//Modifier Function for Delimiter
//
void Utilities::setDelimiter(const char del){
    delimiter = del;
}

//Query Function for Delimiter
//
const char Utilities::getDelimiter() const {
    return delimiter;
}
