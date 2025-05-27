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
#ifndef CPU_SINGLE_HART_H
#define CPU_SINGLE_HART_H
#include "rv32i_hart.h"
class cpu_single_hart: public rv32i_hart
{
public:
    /**
    * Constructor for cpu_single_hart class
    *
    * @param mem Vector where the memory will be held
    ********************************************************************************/
    cpu_single_hart(memory &mem) : rv32i_hart(mem) {}

    /**
    * Runs the simulation (the big final button that brins everything together)
    *
    * @param exec_limt Keeps track of if someone specified when to stop the process.
    * @note Called in main.cpp to run the decoding, and execution within regs and memory
    ********************************************************************************/
    void run(uint64_t exec_limit);
};
#endif