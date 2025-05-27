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
#include "cpu_single_hart.h"
void cpu_single_hart::run(uint64_t exec_limit){
    uint32_t value = mem.get_size();
    regs.set(2, value);//set register x2 to mem size
    //uint64_t counter = 0;
    if(exec_limit == 0){
        while(!is_halted()){
            tick();
        }
    }
    else{
        for(uint64_t i = 0; i != exec_limit; i++){//loop until exec_limit is reached
            if(is_halted()){//always check to see if this iteration was halted
                break;
            }
            else{
                tick();
            }
        }
    }
    if(is_halted()){//check halt
        cout << "Execution terminated. Reason: " << get_halt_reason() << endl;
    }
    cout << get_insn_counter() << " instructions executed" << endl;//give total instruction count
}