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
#ifndef RV32IHART_H
#define RV32IHART_H
#include "registerfile.h"
#include "rv32i_decode.h"
#include "memory.h"
#include <assert.h>
class rv32i_hart: public rv32i_decode
{
public:
    /**
    * Constructor for the rv32i_hart class
    *
    * @param m the amount of memory the constructor creates space for
    ********************************************************************************/
    rv32i_hart(memory &m) : mem(m) {} 

    /**
    * Boolean flag that shows instructions during execution
    *
    * @param b The boolean flag passed in
    ********************************************************************************/
    void set_show_instructions(bool b) { show_instructions = b; } 

    /**
    * Boolean flag that shows registers during execution
    *
    * @param b the boolean flag passed in
    ********************************************************************************/
    void set_show_registers(bool b) { show_registers = b; }

    /**
    * Function for checking if the process is halted or not
    *
    * @return Returns true or false if the process is stopped
    ********************************************************************************/
    bool is_halted() const { return halt; } 

    /**
    * Function that keeps track of why the process halted
    *
    * @return Returns a string containing the reason of halting
    ********************************************************************************/
    const std::string &get_halt_reason() const { return halt_reason; } 

    /**
    * Function that keep track of the instructions executed
    *
    * @return Returns a number in int64_t form that represents the instructions counted
    ********************************************************************************/
    uint64_t get_insn_counter() const { return insn_counter; } 

    /**
    * Function to set the mhartid for csrrs execution
    *
    * @param i The value that the mhartid will be set to
    ********************************************************************************/
    void set_mhartid(int i) { mhartid = i; }   

    /**
    * Function that will execute an instruction at a time
    *
    * @param hdr Optional header that will be placed before dumps
    ********************************************************************************/
    void tick(const std::string &hdr="");   

    /**
    * Function that will dump the contents of the execution
    *
    * @param hdr Optiona header that will be placed before dumps
    ********************************************************************************/
    void dump(const std::string &hdr="") const;  

    /**
    * Function that resets the execution process
    * @note Resets the pc, registers, halt and halt reason, and instruction counter
    ********************************************************************************/
    void reset();                                           

private:
    static constexpr int instruction_width          = 35; 

    /**
    * Overarching exec function that will give specific exec function calls
    *
    * @param insn The instruction that will be decoded and executed
    * @param pos The stream which formatted output will be placed into
    ********************************************************************************/
    void exec(uint32_t insn, std::ostream* pos); 

    /**
    * Executes illegal isntructions
    *
    * @param insn The instruction that will be decoded and executed
    * @param pos The stream which formatted output will be placed into
    ********************************************************************************/
    void exec_illegal_insn(uint32_t insn, std::ostream* pos); 

    /**
    * Executes the lui instruction
    *
    * @param insn The instruction that will be decoded and executed
    * @param pos The stream which formatted output will be placed into
    ********************************************************************************/
    void exec_lui(uint32_t insn, std::ostream* pos);  

    /**
    * Executes the auipc instruction
    *
    * @param insn The instruction that will be decoded and executed
    * @param pos The stream which formatted output will be placed into
    ********************************************************************************/
    void exec_auipc(uint32_t insn, std::ostream* pos); 

    /**
    * Executes the jal instruction
    *
    * @param insn The instruction that will be decoded and executed
    * @param pos The stream which formatted output will be placed into
    ********************************************************************************/
    void exec_jal(uint32_t insn, std::ostream* pos);   

    /**
    * Executes the jalr instruction
    *
    * @param insn The instruction that will be decoded and executed
    * @param pos The stream which formatted output will be placed into
    ********************************************************************************/
    void exec_jalr(uint32_t insn, std::ostream* pos);  

    /**
    * Executes the sltiu instruction
    *
    * @param insn The instruction that will be decoded and executed
    * @param pos The stream which formatted output will be placed into
    ********************************************************************************/
    void exec_sltiu(uint32_t insn, std::ostream* pos); 

    /**
    * Executes the xori instruction
    *
    * @param insn The instruction that will be decoded and executed
    * @param pos The stream which formatted output will be placed into
    ********************************************************************************/
    void exec_xori(uint32_t insn, std::ostream* pos); 

    /**
    * Executes the beq instruction
    *
    * @param insn The instruction that will be decoded and executed
    * @param pos The stream which formatted output will be placed into
    ********************************************************************************/
    void exec_beq(uint32_t insn, std::ostream* pos); 

    /**
    * Executes the bne instruction
    *
    * @param insn The instruction that will be decoded and executed
    * @param pos The stream which formatted output will be placed into
    ********************************************************************************/
    void exec_bne(uint32_t insn, std::ostream* pos);  

    /**
    * Executes the blt instruction
    *
    * @param insn The instruction that will be decoded and executed
    * @param pos The stream which formatted output will be placed into
    ********************************************************************************/
    void exec_blt(uint32_t insn, std::ostream* pos);   

    /**
    * Executes the bge instruction
    *
    * @param insn The instruction that will be decoded and executed
    * @param pos The stream which formatted output will be placed into
    ********************************************************************************/
    void exec_bge(uint32_t insn, std::ostream* pos);   

    /**
    * Executes the bgeu instruction
    *
    * @param insn The instruction that will be decoded and executed
    * @param pos The stream which formatted output will be placed into
    ********************************************************************************/
    void exec_bgeu(uint32_t insn, std::ostream* pos);    

    /**
    * Executes the bltu instruction
    *
    * @param insn The instruction that will be decoded and executed
    * @param pos The stream which formatted output will be placed into
    ********************************************************************************/
    void exec_bltu(uint32_t insn, std::ostream* pos);  

    /**
    * Executes the sh instruction
    *
    * @param insn The instruction that will be decoded and executed
    * @param pos The stream which formatted output will be placed into
    ********************************************************************************/
    void exec_sh(uint32_t insn, std::ostream* pos); 

    /**
    * Executes the sw instruction
    *
    * @param insn The instruction that will be decoded and executed
    * @param pos The stream which formatted output will be placed into
    ********************************************************************************/
    void exec_sw(uint32_t insn, std::ostream* pos);  

    /**
    * Executes the sll instruction
    *
    * @param insn The instruction that will be decoded and executed
    * @param pos The stream which formatted output will be placed into
    ********************************************************************************/
    void exec_sll(uint32_t insn, std::ostream* pos);   

    /**
    * Executes the slt instruction
    *
    * @param insn The instruction that will be decoded and executed
    * @param pos The stream which formatted output will be placed into
    ********************************************************************************/
    void exec_slt(uint32_t insn, std::ostream* pos);  

    /**
    * Executes the sltu instruction
    *
    * @param insn The instruction that will be decoded and executed
    * @param pos The stream which formatted output will be placed into
    ********************************************************************************/
    void exec_sltu(uint32_t insn, std::ostream* pos);   

    /**
    * Executes the slti instruction
    *
    * @param insn The instruction that will be decoded and executed
    * @param pos The stream which formatted output will be placed into
    ********************************************************************************/
    void exec_slti(uint32_t insn, std::ostream* pos);  

    /**
    * Executes the srl instruction
    *
    * @param insn The instruction that will be decoded and executed
    * @param pos The stream which formatted output will be placed into
    ********************************************************************************/
    void exec_srl(uint32_t insn, std::ostream* pos);   

    /**
    * Executes the sub instruction
    *
    * @param insn The instruction that will be decoded and executed
    * @param pos The stream which formatted output will be placed into
    ********************************************************************************/
    void exec_sub(uint32_t insn, std::ostream* pos);   

    /**
    * Executes the sra instruction
    *
    * @param insn The instruction that will be decoded and executed
    * @param pos The stream which formatted output will be placed into
    ********************************************************************************/
    void exec_sra(uint32_t insn, std::ostream* pos);  

    /**
    * Executes the addi instruction
    *
    * @param insn The instruction that will be decoded and executed
    * @param pos The stream which formatted output will be placed into
    ********************************************************************************/
    void exec_addi(uint32_t insn, std::ostream* pos);  

    /**
    * Executes the srai instruction
    *
    * @param insn The instruction that will be decoded and executed
    * @param pos The stream which formatted output will be placed into
    ********************************************************************************/
    void exec_srai(uint32_t insn, std::ostream* pos);   

    /**
    * Executes the sb instruction
    *
    * @param insn The instruction that will be decoded and executed
    * @param pos The stream which formatted output will be placed into
    ********************************************************************************/
    void exec_sb(uint32_t insn, std::ostream* pos);    

    /**
    * Executes the slli instruction
    *
    * @param insn The instruction that will be decoded and executed
    * @param pos The stream which formatted output will be placed into
    ********************************************************************************/
    void exec_slli(uint32_t insn, std::ostream* pos);    

    /**
    * Executes the srli instruction
    *
    * @param insn The instruction that will be decoded and executed
    * @param pos The stream which formatted output will be placed into
    ********************************************************************************/
    void exec_srli(uint32_t insn, std::ostream* pos);   

    /**
    * Executes the ori instruction
    *
    * @param insn The instruction that will be decoded and executed
    * @param pos The stream which formatted output will be placed into
    ********************************************************************************/
    void exec_ori(uint32_t insn, std::ostream* pos);  

    /**
    * Executes the xor instruction
    *
    * @param insn The instruction that will be decoded and executed
    * @param pos The stream which formatted output will be placed into
    ********************************************************************************/
    void exec_xor(uint32_t insn, std::ostream* pos);  

    /**
    * Executes the lb instruction
    *
    * @param insn The instruction that will be decoded and executed
    * @param pos The stream which formatted output will be placed into
    ********************************************************************************/
    void exec_lb(uint32_t insn, std::ostream* pos);   

    /**
    * Executes the lh instruction
    *
    * @param insn The instruction that will be decoded and executed
    * @param pos The stream which formatted output will be placed into
    ********************************************************************************/
    void exec_lh(uint32_t insn, std::ostream* pos);   

    /**
    * Executes the lw instruction
    *
    * @param insn The instruction that will be decoded and executed
    * @param pos The stream which formatted output will be placed into
    ********************************************************************************/
    void exec_lw(uint32_t insn, std::ostream* pos);   

    /**
    * Executes the lbu instruction
    *
    * @param insn The instruction that will be decoded and executed
    * @param pos The stream which formatted output will be placed into
    ********************************************************************************/
    void exec_lbu(uint32_t insn, std::ostream* pos);   

    /**
    * Executes the lhu instruction
    *
    * @param insn The instruction that will be decoded and executed
    * @param pos The stream which formatted output will be placed into
    ********************************************************************************/
    void exec_lhu(uint32_t insn, std::ostream* pos);  

    /**
    * Executes the add instruction
    *
    * @param insn The instruction that will be decoded and executed
    * @param pos The stream which formatted output will be placed into
    ********************************************************************************/
    void exec_add(uint32_t insn, std::ostream* pos);   

    /**
    * Executes the or instruction
    *
    * @param insn The instruction that will be decoded and executed
    * @param pos The stream which formatted output will be placed into
    ********************************************************************************/
    void exec_or(uint32_t insn, std::ostream* pos);    

    /**
    * Executes the and instruction
    *
    * @param insn The instruction that will be decoded and executed
    * @param pos The stream which formatted output will be placed into
    ********************************************************************************/
    void exec_and(uint32_t insn, std::ostream* pos);    

    /**
    * Executes the andi instruction
    *
    * @param insn The instruction that will be decoded and executed
    * @param pos The stream which formatted output will be placed into
    ********************************************************************************/
    void exec_andi(uint32_t insn, std::ostream* pos);  

    /**
    * Executes the ecall instruction
    *
    * @param insn The instruction that will be decoded and executed
    * @param pos The stream which formatted output will be placed into
    ********************************************************************************/
    void exec_ecall(uint32_t insn, std::ostream* pos);  

    /**
    * Executes the ebreak instruction
    *
    * @param insn The instruction that will be decoded and executed
    * @param pos The stream which formatted output will be placed into
    ********************************************************************************/
    void exec_ebreak(uint32_t insn, std::ostream* pos);  

    /**
    * Executes the csrrs instruction
    *
    * @param insn The instruction that will be decoded and executed
    * @param pos The stream which formatted output will be placed into
    ********************************************************************************/
    void exec_csrrs(uint32_t insn, std::ostream* pos);    
          


    //list of all private member variables used to keep track of process
    bool halt = { false };
    std::string halt_reason = { "none" };
    uint64_t insn_counter = { 0 };
    uint32_t pc = { 0 };
    uint32_t mhartid = { 0 };
    bool show_instructions = false;
    bool show_registers = false;

protected:
    //protected variables from other classes that holds memory and the registers
    registerfile regs;
    memory &mem;
};
#endif