//***************************************************************************
//
//  Justin Derus
//  z1854210
//  CSCI 463 Section 1
//
//  I certify that this is my own work and where appropriate an extension 
//  of the starter code provided for the assignment.
//
//***************************************************************************
#ifndef HEX_H
#define HEX_H
#include <iostream> //give libraries needed to run the program
#include <string>
#include <stdint.h>
#include <unistd.h>
#include <sstream>
#include <iomanip>
using namespace std;
class hex
{
    public:
        static std::string to_hex8(uint8_t i);
        static std::string to_hex32(uint32_t i);
        static std::string to_hex0x32(uint32_t i);
        static std::string to_hex0x20(uint32_t i);
        static std::string to_hex0x12(uint32_t i);
};
#endif