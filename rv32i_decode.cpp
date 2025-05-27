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
#include "rv32i_decode.h"
#include <assert.h>
/**
 * Returns the opcode for the instruction passed in
 *
 * @param insn the 32 bit instruction passed into the function
 *
 * @return a 32 bit unsigned integer with the opcode section of the instruction
 ********************************************************************************/
uint32_t rv32i_decode::get_opcode(uint32_t insn){
    return (insn & 0x0000007f);//returns opcode (7 LSB) DONT NEED TO SHIFT
}
/**
 * Returns the rd section for the instruction passed in
 *
 * @param insn the 32 bit instruction passed into the function
 *
 * @return a 32 bit unsigned integer with the rd section of the instruction with 
 * the results shifted all the way to the right to get a number 0-31
 ********************************************************************************/
uint32_t rv32i_decode::get_rd(uint32_t insn){
    return (insn & 0x00000f80) >> 7; //shift right 7 to make this a valid number 0-31
}
/**
 * Returns the rs1 section for the instruction passed in
 *
 * @param insn the 32 bit instruction passed into the function
 *
 * @return a 32 bit unsigned integer with the rs1 section of the instruction with 
 * the results shifted all the way to the right
 ********************************************************************************/
uint32_t rv32i_decode::get_rs1(uint32_t insn){
    return (insn & 0x000f8000) >> 15; //shift right 15 to get rs1
}
/**
 * Returns the rs2 section for the instruction passed in
 *
 * @param insn the 32 bit instruction passed into the function
 *
 * @return a 32 bit unsigned integer with the rs2 section of the instruction with 
 * the results shifted all the way to the right
 ********************************************************************************/
uint32_t rv32i_decode::get_rs2(uint32_t insn){
    return (insn & 0x01f00000) >> 20; //shift right 20 to get rs2
}
/**
 * Returns the funct3 section for the instruction passed in
 *
 * @param insn the 32 bit instruction passed into the function
 *
 * @return a 32 bit unsigned integer with the funct3 section of the instruction with 
 * the results shifted all the way to the right
 ********************************************************************************/
uint32_t rv32i_decode::get_funct3(uint32_t insn){
    return (insn & 0x00007000) >> 12; //shift right 12 to get funct3
}
/**
 * Returns the funct7 section for the instruction passed in
 *
 * @param insn the 32 bit instruction passed into the function
 *
 * @return a 32 bit unsigned integer with the funct7 section of the instruction with 
 * the results shifted all the way to the right
 ********************************************************************************/
uint32_t rv32i_decode::get_funct7(uint32_t insn){
    return (insn & 0xfe000000) >> 25; //shift right 25 to get funct7
}
/**
 * Returns the imm_i section for the instruction passed in
 *
 * @param insn the 32 bit instruction passed into the function
 *
 * @return a 32 bit signed integer containing the imm_i section of an instruction
 * 
 ********************************************************************************/
int32_t rv32i_decode::get_imm_i(uint32_t insn){
    int32_t sign = (insn & 0x80000000); //find the sign bit
    int32_t imm_i = (insn >> (30-10));
    if(sign){
        imm_i = (imm_i | 0xfffff800);
    }
    return imm_i;
}
/**
 * Returns the imm_u section for the instruction passed in
 *
 * @param insn the 32 bit instruction passed into the function
 *
 * @return a 32 bit signed integer containing the imm_u section of an instruction
 * 
 ********************************************************************************/
int32_t rv32i_decode::get_imm_u(uint32_t insn){
    int32_t imm_u = 0;
    imm_u |= (insn & 0xfffff000) >> 12;
    return imm_u;
}
/**
 * Returns the imm_b section for the instruction passed in
 *
 * @param insn the 32 bit instruction passed into the function
 *
 * @return a 32 bit signed integer containing the imm_b section of an instruction
 * 
 ********************************************************************************/
int32_t rv32i_decode::get_imm_b(uint32_t insn){
    int32_t sign = (insn & 0x80000000); //get the sign
    int32_t a = (sign >> 19);       //get A's spot
    uint32_t y = (insn & 0x00000080) << 4;//get Y and put it in place
    int32_t bcdefg = (insn & 0x7e000000) >> 20;//get BCDEFG and put in place
    int32_t uvwx = (insn & 0x00000f00)  >> 7;//get UVWX and put in place
    int32_t imm_b = 0x00000000;//value we are now filling in
    if(sign){               
        imm_b = (imm_b | 0xffffe000); //fill F's if negative
    }
    imm_b = (imm_b | a);//put A in bit 13
    imm_b = (imm_b | y);  
    imm_b = (imm_b | bcdefg);
    imm_b = (imm_b | uvwx);
    imm_b = (imm_b & 0xfffffffe); //put last bit = 0
    return imm_b;
}
/**
 * Returns the imm_s section for the instruction passed in
 *
 * @param insn the 32 bit instruction passed into the function
 *
 * @return a 32 bit signed integer containing the imm_s section of an instruction
 * 
 ********************************************************************************/
int32_t rv32i_decode::get_imm_s(uint32_t insn){//from handout
    int32_t imm_s = (insn & 0xfe000000) >> (25-5);
    imm_s |= (insn & 0x00000f80) >> (7-0);
    if(insn & 0x80000000){
        imm_s |= 0xfffff000;    //sign-extend the left
    }
    return imm_s;
}
/**
 * Returns the imm_j section for the instruction passed in
 *
 * @param insn the 32 bit instruction passed into the function
 *
 * @return a 32 bit signed integer containing the imm_j section of an instruction
 * 
 ********************************************************************************/
int32_t rv32i_decode::get_imm_j(uint32_t insn){
    int32_t imm_j = 0x00000000;
    int32_t sign = (insn & 0x80000000);
    int32_t a = (sign >> (31-20));
    int32_t mnopqrst = (insn & 0x000ff000);
    int32_t l = (insn & 0x00100000) >> (20-11);
    int32_t bcdefghijk = (insn & 0x7fe00000) >> (30-10);
    if(sign){
        imm_j = (imm_j | 0xffe00000); //fill f's
    }
    imm_j = (imm_j | bcdefghijk);
    imm_j = (imm_j | l);
    imm_j = (imm_j | mnopqrst);
    imm_j = (imm_j | a);
    imm_j = (imm_j & 0xfffffffe); //last bit = 0
    return imm_j;
}
/**
 * Decode is used for decoding a 4 byte chunk of memory and decoding its values
 * into instructions for the user to read
 *
 * @param addr the 32 bit address in memory where the instruction is held which
 * will be used in decoding
 * @param insn the 32 bit instruction passed into the function
 *
 * @return a std string tht contains the rendered instruction
 * 
 ********************************************************************************/
std::string rv32i_decode::decode(uint32_t addr, uint32_t insn){//structure from handout followed
    switch(get_opcode(insn))//get the opcode
    {   
        default:                    return render_illegal_insn();//get the unique instructions
        case opcode_lui:            return render_lui(insn);
        case opcode_auipc:          return render_auipc(insn);
        case opcode_jal:            return render_jal(addr, insn);
        case opcode_jalr:           return render_jalr(insn);
        case opcode_btype://render all of the btype instructions
            switch(get_funct3(insn)){
                default:            return render_illegal_insn();
                case funct3_beq:    return render_btype(addr,insn,"beq");
                case funct3_bne:    return render_btype(addr,insn,"bne");
                case funct3_blt:    return render_btype(addr,insn,"blt");
                case funct3_bge:    return render_btype(addr,insn,"bge");
                case funct3_bgeu:   return render_btype(addr,insn,"bgeu");
                case funct3_bltu:   return render_btype(addr,insn,"bltu");
            }
            assert(0 && "unrecognized funct3");//impossible
        case opcode_stype://render all of the s type instructions
            switch(get_funct3(insn)){//rendered based off of different funct3 values
                default:            return render_illegal_insn();
                case funct3_sb:     return render_stype(insn,"sb");
                case funct3_sh:     return render_stype(insn,"sh");
                case funct3_sw:     return render_stype(insn,"sw");
            }
            assert(0 && "unrecognized funct3");//impossible
        case opcode_rtype://render all of the r type instructions
            switch(get_funct3(insn)){//first check the funct3 values 
                default:                    return render_illegal_insn();
                case funct3_add:
                    switch(get_funct7(insn)){//add and sub are the same funct3 so find funct7 instead
                        default:            return render_illegal_insn();
                        case funct7_add:    return render_rtype(insn,"add");
                        case funct7_sra:    return render_rtype(insn,"sub");
                    }
                    assert(0 && "unrecognized funct7");//impossible
                case funct3_sll:            return render_rtype(insn,"sll");//now finish the funct3 cases
                case funct3_slt:            return render_rtype(insn,"slt");
                case funct3_sltu:           return render_rtype(insn,"sltu");
                case funct3_xor:            return render_rtype(insn,"xor");
                case funct3_srx:
                    switch(get_funct7(insn)){//cant be differentiated from funct3 so use funct 7
                        default:            return render_illegal_insn();
                        case funct7_srl:    return render_rtype(insn,"srl");
                        case funct7_sra:    return render_rtype(insn,"sra");
                    }
                    assert(0 && "unrecognized funct7");//impossible
                case funct3_or:             return render_rtype(insn,"or");//finish funct 3 cases
                case funct3_and:            return render_rtype(insn,"and");
            }
            assert(0 && "unrecognized funct3");//impossible
        case opcode_load_imm://decode the load_imm instructions
            switch(get_funct3(insn)){//can be differentiated using solely funct3
                default:            return render_illegal_insn();
                case funct3_lb:     return render_itype_load(insn,"lb");
                case funct3_lh:     return render_itype_load(insn,"lh");
                case funct3_lw:     return render_itype_load(insn,"lw");
                case funct3_lbu:    return render_itype_load(insn,"lbu");
                case funct3_lhu:    return render_itype_load(insn,"lhu");
            }
            assert(0 && "unrecognized funct3");//impossible
        case opcode_alu_imm://decode the alu immediates
            switch(get_funct3(insn)){//differentiate based on funct3 values
                default:            return render_illegal_insn();
                case funct3_add:    return render_itype_alu(insn,"addi",get_imm_i(insn));
                case funct3_slt:    return render_itype_alu(insn,"slti",get_imm_i(insn));
                case funct3_sltu:   return render_itype_alu(insn,"sltiu",get_imm_i(insn));
                case funct3_xor:    return render_itype_alu(insn,"xori",get_imm_i(insn));
                case funct3_or:     return render_itype_alu(insn,"ori",get_imm_i(insn));
                case funct3_and:    return render_itype_alu(insn,"andi",get_imm_i(insn));
                case funct3_sll:    return render_itype_alu(insn,"slli",get_imm_i(insn)%XLEN);
                case funct3_srx:
                    switch(get_funct7(insn)){//cannot be differentiated from funct3 anymore so uses funct7
                        default:         return render_illegal_insn();
                        case funct7_srl: return render_itype_alu(insn,"srli",get_imm_i(insn)%XLEN);//modulo XLEN per request of handout
                        case funct7_sra: return render_itype_alu(insn,"srai",get_imm_i(insn)%XLEN);
                    }
                    assert(0 && "unrecognized funct7");//impossible
            }
            assert(0 && "unrecognized funct3");//impossible
        case opcode_system://decode all fo the system instructions
            switch(get_funct3(insn)){//first differentiated based on funct 3 values
                case funct3_csrrw:  return render_csrrx(insn,"csrrw");
                case funct3_csrrs:  return render_csrrx(insn,"csrrs");
                case funct3_csrrc:  return render_csrrx(insn,"csrrc");
                case funct3_csrrwi: return render_csrrxi(insn,"csrrwi");
                case funct3_csrrsi: return render_csrrxi(insn,"csrrsi");
                case funct3_csrrci: return render_csrrxi(insn,"csrrci");
            }
            switch(insn){//funct 3 cannot differentiate so just use the instruction as a whole
                default:            return render_illegal_insn();//no implementations
                case insn_ebreak:   return render_ebreak(insn);
                case insn_ecall:    return render_ecall(insn);
            }
            assert(0 && "unrecognized funct3");//impossible
    }
    assert(0 && "unrecognized opcode");//impossible
}
/**
 * Renders the register into "xX" string format
 *
 * @param r the integer that will be converted into the X portion of the string
 *
 * @return an std string containing "xREGISTER#"
 * 
 ********************************************************************************/
std::string rv32i_decode::render_reg(int r){
    std::ostringstream os;
    os << "x" << r;
    return os.str();
}
/**
 * Renders the base displacement view 
 *
 * @param base the unsigned 32 bit integer that is the register AKA base
 *
 * @param disp the signed 32 bit integer that shows the displacement off of the base param
 *
 * @return an std string containing "disp(xBASE)"
 * 
 ********************************************************************************/
std::string rv32i_decode::render_base_disp(uint32_t base, int32_t disp){
    std::ostringstream os;
    os << disp << "(" << render_reg(base) << ")";
    return os.str();
}
/**
 * Renders the mnemonic value of the instructions 
 *
 * @param m Holds the string value of what the mnemonic is
 *
 * @return an std string containing a formatted mnemonic 
 * 
 ********************************************************************************/
std::string rv32i_decode::render_mnemonic(const std::string &m){
    std::ostringstream os;
    os << std::setfill(' ') << std::setw(mnemonic_width) << std::left << m;
    return os.str();
}
/**
 * Renders the lui instruction
 *
 * @param insn the 32 bit unsigned integer that holds the instruction from memory
 *
 * @return an std string containing "lui    xREGISTER,imm_u" from the instruction
 * 
 ********************************************************************************/
std::string rv32i_decode::render_lui(uint32_t insn){//from handout, same structure throughout all renders
    uint32_t rd = get_rd(insn);//get rd
    int32_t imm_u = get_imm_u(insn);//get imm_u
    std::ostringstream os;
    os << render_mnemonic("lui") << render_reg(rd) << ","
       << to_hex0x20((imm_u)&0x0fffff);
    return os.str();
}
/**
 * Renders an illegal instruction
 *
 * @return an std string containing an error message for an illegal instruction
 * 
 ********************************************************************************/
std::string rv32i_decode::render_illegal_insn(){
    return "ERROR: UNIMPLEMENTED INSTRUCTION";//from handout description
}
/**
 * Renders the auipc instruction
 *
 * @param insn the 32 bit unsigned integer that holds the instruction from memory
 *
 * @return an std string containing "auipc    xREGISTER,imm_u" from the instruction
 * 
 ********************************************************************************/
std::string rv32i_decode::render_auipc(uint32_t insn){
    uint32_t rd = get_rd(insn);
    int32_t imm_u = get_imm_u(insn);
    std::ostringstream os;
    os << render_mnemonic("auipc") << render_reg(rd) << ","
       << to_hex0x20(imm_u);
    return os.str();
}
/**
 * Renders the jal instruction
 *
 * @param addr the 32 bit unsigned integer that holds the address in memory of the 
 * instruction param
 * @param insn the 32 bit unsigned integer that holds the instruction from memory
 *
 * @return an std string containing "jal    xREGISTER,imm_j" from the instruction
 * 
 ********************************************************************************/
std::string rv32i_decode::render_jal(uint32_t addr, uint32_t insn){
    uint32_t rd = get_rd(insn);
    int32_t imm_j = get_imm_j(insn);//add this amount to addr to jump to next addr
    std::ostringstream os;
    os << render_mnemonic("jal") << render_reg(rd) << ","
       << to_hex0x32(addr+imm_j);//show what addr to jump too
    return os.str();
}
/**
 * Renders the jalr instruction
 *
 * @param insn the 32 bit unsigned integer that holds the instruction from memory
 *
 * @return an std string containing "jalr    xREGISTER,displacement(BASE)" from the instruction
 * 
 ********************************************************************************/
std::string rv32i_decode::render_jalr(uint32_t insn){
    uint32_t rd = get_rd(insn);
    uint32_t rs1 = get_rs1(insn);
    int32_t imm_i = get_imm_i(insn);
    std::ostringstream os;
    os << render_mnemonic("jalr") << render_reg(rd) << ","
       << render_base_disp(rs1,imm_i); //(base,displacement)
    return os.str();
}
/**
 * Renders the btype instructions
 *
 * @param addr the 32 bit unsigned integer containing the address in memory of
 * the instruction
 * 
 * @param mnemonic contains the string value of the mnemonic of the instruction
 *
 * @param insn the 32 bit unsigned integer that holds the instruction from memory
 *
 * @return an std string containing "b-inst    xREG,xREG,addr+imm_b" from the instruction
 * 
 ********************************************************************************/
std::string rv32i_decode::render_btype(uint32_t addr, uint32_t insn, const char *mnemonic){
    int32_t imm_b = get_imm_b(insn);
    uint32_t rs1 = get_rs1(insn);
    uint32_t rs2 = get_rs2(insn);
    std::ostringstream os;
    os << render_mnemonic(mnemonic) << render_reg(rs1) << "," << render_reg(rs2)
       << "," << to_hex0x32(addr + imm_b);
    return os.str();
}
/**
 * Renders the itype load instructions
 *
 * @param insn the 32 bit unsigned integer that holds the instruction from memory
 *
 * @param mnemonic contains the mnemonic of the instruction from the insn param
 *
 * @return an std string containing "load-inst  xREGISTER,displacement(base)" 
 * from the instruction
 * 
 ********************************************************************************/
std::string rv32i_decode::render_itype_load(uint32_t insn, const char *mnemonic){
    uint32_t rd = get_rd(insn);
    uint32_t rs1 = get_rs1(insn);
    int32_t imm_i = get_imm_i(insn);
    std::ostringstream os;
    os << render_mnemonic(mnemonic) << render_reg(rd) << "," << render_base_disp(rs1,imm_i);
    return os.str();
}
/**
 * Renders the stype instructions
 *
 * @param insn the 32 bit unsigned integer that holds the instruction from memory
 *
 * @param mnemonic contains the mnemonic of the instruction from the insn param
 *
 * @return an std string containing "s-inst    xREGISTER,displacement(base) " from
 * the instruction
 * 
 ********************************************************************************/
std::string rv32i_decode::render_stype(uint32_t insn, const char *mnemonic){
    int32_t imm_s = get_imm_s(insn);
    uint32_t rs1 = get_rs1(insn);
    uint32_t rs2 = get_rs2(insn);
    std::ostringstream os;
    os << render_mnemonic(mnemonic) << render_reg(rs2) << "," << render_base_disp(rs1,imm_s);
    return os.str();
}
/**
 * Renders the itype alu instructions
 *
 * @param insn the 32 bit unsigned integer that holds the instruction from memory
 *
 * @param mnemonic contains the mnemonic of the isntruction from the insn param
 *
 * @param imm_i contains the immediate i value from the insn param
 *
 * @return an std string containing "i-alu-inst    xREGISTER,xREGISTER,imm_i" from
 * the instruction
 * 
 ********************************************************************************/
std::string rv32i_decode::render_itype_alu(uint32_t insn, const char *mnemonic, int32_t imm_i){
    uint32_t rd = get_rd(insn);
    uint32_t rs1 = get_rs1(insn);
    std::ostringstream os;
    os << render_mnemonic(mnemonic) << render_reg(rd) << "," << render_reg(rs1) << "," << imm_i;
    return os.str();
}
/**
 * Renders the rtype instructions
 *
 * @param insn the 32 bit unsigned integer that holds the instruction from memory
 *
 * @param mnemonic contains the mnemonic of the instruction from the insn param
 *
 * @return an std string containing "r-inst    xREG,xREG,xREG" from the instruction
 * 
 ********************************************************************************/
std::string rv32i_decode::render_rtype(uint32_t insn, const char *mnemonic){
    uint32_t rd = get_rd(insn);
    uint32_t rs1 = get_rs1(insn);
    uint32_t rs2 = get_rs2(insn);
    std::ostringstream os;
    os << render_mnemonic(mnemonic) << render_reg(rd) << "," << render_reg(rs1) << "," << render_reg(rs2);
    return os.str();
}
/**
 * Renders the ecall instruction
 *
 * @param insn the 32 bit unsigned integer that holds the instruction from memory
 *
 * @return an std string containing "ecall" decoded from the instruction
 * 
 * @note the function requires insn param via the handout but it does not compile 
 * unless something is done with it via the handout g++ compilers
 ********************************************************************************/
std::string rv32i_decode::render_ecall(uint32_t insn){
    uint32_t opcode = get_opcode(insn);
    opcode -= 1;//warning compiled if not used - means errors
    std::ostringstream os;
    os << "ecall"; 
    return os.str();
}
/**
 * Renders the ebreak instruction
 *
 * @param insn the 32 bit unsigned integer that holds the instruction from memory
 *
 * @return an std string containing "ebreak" decoded from the instruction
 * 
 * @note the function requires insn param via the handout but it does not compile 
 * unless something is done with it via the handout g++ compilers
 ********************************************************************************/
std::string rv32i_decode::render_ebreak(uint32_t insn){
    uint32_t opcode = get_opcode(insn);
    opcode -= 1;//warning compiled if not used - means errors
    std::ostringstream os;
    os << "ebreak";
    return os.str();
}
/**
 * Renders the system instructions
 *
 * @param insn the 32 bit unsigned integer that holds the instruction from memory
 *
 * @param mnemonic contains the mnemonic of the instruction from insn param
 *
 * @return an std string containing "system-inst    xREG,imm_i,xREG" decoded from
 * the instruction
 ********************************************************************************/
std::string rv32i_decode::render_csrrx(uint32_t insn, const char *mnemonic){
    uint32_t rd = get_rd(insn);
    uint32_t rs1 = get_rs1(insn);
    int32_t imm_i = get_imm_i(insn);
    std::ostringstream os;
    os << render_mnemonic(mnemonic) << render_reg(rd) << "," << to_hex0x12(imm_i) << "," << render_reg(rs1);
    return os.str();
}
/**
 * Renders the system immediate instructions
 *
 * @param insn the 32 bit unsigned integer that holds the instruction from memory
 *
 * @param mnemonic contains the mnemonic of the instruction from insn param
 *
 * @return an std string containing "system-im-inst    xREG,imm_i,zimm" decoded from
 * the instruction
 ********************************************************************************/
std::string rv32i_decode::render_csrrxi(uint32_t insn, const char *mnemonic){
    uint32_t rd = get_rd(insn);
    int32_t imm_i = get_imm_i(insn);
    int32_t zimm = ((insn & 0x000f8000) >> (19-4));
    std::ostringstream os;
    os << render_mnemonic(mnemonic) << render_reg(rd) << "," << to_hex0x12(imm_i) << "," << zimm;
    return os.str();
}