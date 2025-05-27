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
#ifndef REGISTERFILE_H
#define REGISTERFILE_H
#include <stdint.h>
#include <vector>
#include "hex.h"
class registerfile: public hex
{
    public:
        /**
        * Constructor for a registerfile object
        * @note Uses reset to set the registers vector
        ********************************************************************************/
        registerfile();
        /**
        * Destructor for a registerfile object
        * @note Uses clear to free memory from registers vector
        ********************************************************************************/                    
        ~registerfile();                   
        /**
        * Sets the registers to their values, 0 for x0, f0f0f0f0 for else
        ********************************************************************************/
        void reset();
        /**
        * Sets the value of a register
        *
        * @param r  The register that we will be changing
        * @param val    The value that register (r) will be set to
        ********************************************************************************/                    
        void set(uint32_t r, int32_t val);
        /**
        * Gets the value of a register
        *
        * @param r  The register that we will be retrieving
        * @return Returns the register value in int32_t form
        ********************************************************************************/   
        int32_t get(uint32_t r) const;
        /**
        * Dumps the contents of the registers with an optional header
        *
        * @param hdr The optional header to the register dump
        ********************************************************************************/       
        void dump(const std::string &hdr) const;    
    private:
        vector<int32_t> registers;     // Vector of 32 registers               
};
#endif