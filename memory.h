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
#ifndef MEMORY_H
#define MEMORY_H
#include "hex.h"                //more libraries and included header files for program
#include <vector>
#include <fstream>
class memory : public hex       //child class of hex
{
    public:
        memory(uint32_t s);                     //constructor
        ~memory();                              //destructor

        bool check_illegal(uint32_t addr) const;
        uint32_t get_size() const;
        uint8_t get8(uint32_t addr) const;     //gives the bytes
        uint16_t get16(uint32_t addr) const;    
        uint32_t get32(uint32_t addr) const;

        int32_t get8_sx(uint32_t addr) const;  //gets the signed version
        int32_t get16_sx(uint32_t addr) const;
        int32_t get32_sx(uint32_t addr) const;
        
        void set8(uint32_t addr, uint8_t val);  //sets the bytes
        void set16(uint32_t addr, uint16_t val);
        void set32(uint32_t addr, uint32_t val);

        void dump() const;//dumps contents of vector

        bool load_file(const std::string &fname);//loads in data

    private:
        std::vector<uint8_t> mem;//holds the actual memory
};
#endif