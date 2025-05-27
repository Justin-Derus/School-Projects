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
#include "hex.h"
/**
 * Returns a std::string with exactly 2 hex digits representing the 8 bits
 * of the i argument
 * @param i - the uint8_t value that is being converted into a string
 *
 * @return A std::string with exactly 2 hex digits representing the 8 bits
 *
 * @note Function is first "stepping stone" of the 3 versions of it
 * the static cast is necessary to prevent the insertion operator (<<)
 * from treating the 8-bit integer as a character and printing incorrectly
 ********************************************************************************/
std::string hex::to_hex8(uint8_t i){ //given in handout
    std::ostringstream os;
    os << std::hex << std::setfill('0') << std::setw(2) << static_cast<uint16_t>(i);
    return os.str();
}
/**
 * Returns a std::string with exactly 8 hex digits representing the 32 bits
 * of the i argument
 * @param i - the uint832_t value that is being converted into a string
 *
 * @return A std::string with exactly 8 hex digits representing the 32 bits
 *
 * @note Function is second "stepping stone" of the 3 versions of it
 ********************************************************************************/
std::string hex::to_hex32(uint32_t i){//given in handout
    std::ostringstream os;
    os << std::hex << std::setfill('0') << std::setw(8) << i;
    return os.str();
}
/**
 * Returns a std::string with exactly 8 hex digits representing the 32 bits
 * of the i argument - "0x" is added before the digits
 * @param i - the uint832_t value that is being converted into a string
 *
 * @return A std::string with exactly 8 hex digits representing the 32 bits prepended with 0x
 *
 * @note Function is last "stepping stone" of the 3 versions of it - used to show full hex 
 ********************************************************************************/
std::string hex::to_hex0x32(uint32_t i){//given in handout
    return std::string("0x")+to_hex32(i);
}
/**
 * returns a string with the 20 LSB from i
 * @param i - the uint32_t value that is being converted into a string
 *
 * @return A std::string with exactly 5 hex digits representing the 20 bits prepended with 0x 
 ********************************************************************************/
std::string hex::to_hex0x20(uint32_t i){
    std::ostringstream os;
    os << std::hex << std::setfill('0') << std::setw(5) << (i & 0x000fffff);
    return std::string("0x")+os.str();//mix of to_hex0x32 and to_hex8 w/o static casting
}
/**
 * Returns a string with the 12 LSB from i
 * @param i - the uint32_t value that is being converted into a string
 *
 * @return A std::string with exactly 3 hex digits representing the 12 bits prepended with 0x
 ********************************************************************************/
std::string hex::to_hex0x12(uint32_t i){
    std::ostringstream os;
    os << std::hex << std::setfill('0') << std::setw(3) << (i & 0x00000fff);
    return std::string("0x")+os.str();
}