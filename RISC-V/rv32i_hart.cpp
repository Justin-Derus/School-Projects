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
#include "rv32i_hart.h"
void rv32i_hart::reset(){
    regs.set(pc, 0);
    //rv32i_hart::pc = 0;
    regs.reset();
    rv32i_hart::insn_counter = 0;
    rv32i_hart::halt = false;
    rv32i_hart::halt_reason = "none";
}
void rv32i_hart::dump(const std::string &hdr) const{
    regs.dump(hdr);//dump register contents with an optional header
    cout << endl << std::right << std::setw(4) << "pc " << hex::to_hex32(pc) << endl;//formatting
}
void rv32i_hart::tick(const std::string &hdr){
    if(halt){//returns if the halt status is true
        return;
    } 
    else{//otherwise halt is false
        if(show_registers){
            rv32i_hart::dump(hdr);
        }
        if(pc % 4 != 0){
            halt = true;
            halt_reason = "PC alignment error";
            return;
        }
        insn_counter++;
        uint32_t insn = mem.get32(pc);
        if(show_instructions){
            cout << hdr << hex::to_hex32(pc) << ": " << hex::to_hex32(insn) << " ";
            exec(insn, &std::cout);//execute instruction and render insn and simulation details
            cout << endl;
        }
        else{
            exec(insn, nullptr); //execute instruction without rendering anything
        }
    }  
}
void rv32i_hart::exec(uint32_t insn, std::ostream* pos){//overarching exec calls specific execs
    switch(get_opcode(insn))//get the opcode
    {   
        default:                    return exec_illegal_insn(insn, pos);
        case opcode_lui:            return exec_lui(insn, pos);
        case opcode_auipc:          return exec_auipc(insn, pos);
        case opcode_jal:            return exec_jal(insn, pos);
        case opcode_jalr:           return exec_jalr(insn, pos);
        case opcode_btype://render all of the btype instructions
            switch(get_funct3(insn)){
                default:            return exec_illegal_insn(insn, pos);
                case funct3_beq:    return exec_beq(insn, pos);
                case funct3_bne:    return exec_bne(insn, pos);
                case funct3_blt:    return exec_blt(insn, pos);
                case funct3_bge:    return exec_bge(insn, pos);
                case funct3_bgeu:   return exec_bgeu(insn, pos);
                case funct3_bltu:   return exec_bltu(insn, pos);
            }
            assert(0 && "unrecognized funct3");//impossible
        case opcode_stype://render all of the s type instructions
            switch(get_funct3(insn)){//rendered based off of different funct3 values
                default:            return exec_illegal_insn(insn, pos);
                case funct3_sb:     return exec_sb(insn, pos);
                case funct3_sh:     return exec_sh(insn, pos);
                case funct3_sw:     return exec_sw(insn, pos);
            }
            assert(0 && "unrecognized funct3");//impossible
        case opcode_rtype://render all of the r type instructions
            switch(get_funct3(insn)){//first check the funct3 values 
                default:                    return exec_illegal_insn(insn, pos);
                case funct3_add:
                    switch(get_funct7(insn)){//add and sub are the same funct3 so find funct7 instead
                        default:            return exec_illegal_insn(insn, pos);
                        case funct7_add:    return exec_add(insn, pos);
                        case funct7_sub:    return exec_sub(insn, pos);
                    }
                    assert(0 && "unrecognized funct7");//impossible
                case funct3_sll:            return exec_sll(insn, pos);
                case funct3_slt:            return exec_slt(insn, pos);
                case funct3_sltu:           return exec_sltu(insn, pos);
                case funct3_xor:            return exec_xor(insn, pos);
                case funct3_srx:
                    switch(get_funct7(insn)){//cant be differentiated from funct3 so use funct 7
                        default:            return exec_illegal_insn(insn, pos);
                        case funct7_srl:    return exec_srl(insn, pos);
                        case funct7_sra:    return exec_sra(insn, pos);
                    }
                    assert(0 && "unrecognized funct7");//impossible
                case funct3_or:             return exec_or(insn, pos);
                case funct3_and:            return exec_and(insn, pos);
            }
            assert(0 && "unrecognized funct3");//impossible
        case opcode_load_imm://decode the load_imm instructions
            switch(get_funct3(insn)){//can be differentiated using solely funct3
                default:            return exec_illegal_insn(insn, pos);
                case funct3_lb:     return exec_lb(insn, pos);
                case funct3_lh:     return exec_lh(insn, pos);
                case funct3_lw:     return exec_lw(insn, pos);
                case funct3_lbu:    return exec_lbu(insn, pos);
                case funct3_lhu:    return exec_lhu(insn, pos);
            }
            assert(0 && "unrecognized funct3");//impossible
        case opcode_alu_imm://decode the alu immediates
            switch(get_funct3(insn)){//differentiate based on funct3 values
                default:            return exec_illegal_insn(insn, pos);
                case funct3_add:    return exec_addi(insn, pos);
                case funct3_slt:    return exec_slti(insn, pos);
                case funct3_sltu:   return exec_sltiu(insn, pos);
                case funct3_xor:    return exec_xori(insn, pos);
                case funct3_or:     return exec_ori(insn, pos);
                case funct3_and:    return exec_andi(insn, pos);
                case funct3_sll:    return exec_slli(insn, pos);
                case funct3_srx:
                    switch(get_funct7(insn)){//cannot be differentiated from funct3 anymore so uses funct7
                        default:         return exec_illegal_insn(insn, pos);
                        case funct7_srl: return exec_srli(insn, pos);
                        case funct7_sra: return exec_srai(insn, pos);
                    }
                    assert(0 && "unrecognized funct7");//impossible
            }
            assert(0 && "unrecognized funct3");//impossible
        case opcode_system://decode all fo the system instructions
            switch(get_funct3(insn)){//first differentiated based on funct 3 values
                case funct3_csrrw:  return exec_illegal_insn(insn, pos);
                case funct3_csrrs:  return exec_csrrs(insn, pos);
                case funct3_csrrc:  return exec_illegal_insn(insn, pos);
                case funct3_csrrwi: return exec_illegal_insn(insn, pos);
                case funct3_csrrsi: return exec_illegal_insn(insn, pos);
                case funct3_csrrci: return exec_illegal_insn(insn, pos);
            }
            switch(insn){//funct 3 cannot differentiate so just use the instruction as a whole
                default:            return exec_illegal_insn(insn, pos);
                case insn_ebreak:   return exec_ebreak(insn, pos);
                case insn_ecall:    return exec_ecall(insn, pos);
            }
            assert(0 && "unrecognized funct3");//impossible
    }
    assert(0 && "unrecognized opcode");//impossible
    //similar format to decode from rv32i_decode, only exec instead of render
}
void rv32i_hart::exec_illegal_insn(uint32_t insn, std::ostream* pos){
    (void)insn;
    if(pos){//outputs to ostream
        *pos << render_illegal_insn();
    }
    halt = true;//set halt and its reason
    halt_reason = "Illegal instruction";
}
void rv32i_hart::exec_slt(uint32_t insn, std::ostream* pos){
    uint32_t rd = get_rd(insn);
    uint32_t rs1 = get_rs1(insn);
    uint32_t rs2 = get_rs2(insn);
    int32_t val = (regs.get(rs1) < regs.get(rs2)) ? 1 : 0;
    if(pos){
        *pos << " ";
        std::string s = render_rtype(insn, "slt");
        *pos << std::setw(instruction_width) << std::setfill(' ') << std::left << s;
        *pos << "// " << render_reg(rd) << " = (" << hex::to_hex0x32(regs.get(rs1)) << " < " 
             << hex::to_hex0x32(regs.get(rs2)) << ") ? 1 : 0 = " << hex::to_hex0x32(val);
    }
    regs.set(rd, val);//set registers and move to next instruction
    pc += 4;
}
void rv32i_hart::exec_lui(uint32_t insn, std::ostream* pos){
    uint32_t rd = get_rd(insn);
    uint32_t imm_u = get_imm_u(insn);
    imm_u = imm_u << 12;
    if(pos){
        *pos << " ";
        std::string s = render_lui(insn);
        *pos << std::setw(instruction_width) << std::setfill(' ') << std::left << s;
        *pos << "// " << render_reg(rd) << " = " << hex::to_hex0x32(imm_u);
    }
    regs.set(rd, imm_u);//set rd = to imm_u value
    pc += 4;
}
void rv32i_hart::exec_auipc(uint32_t insn, std::ostream* pos){
    uint32_t rd = get_rd(insn);
    uint32_t imm_u = get_imm_u(insn);
    imm_u = imm_u << 12;
    int32_t value = pc + imm_u;//add the address of the instruction to the imm_u value and store result in rd
    if(pos){
        *pos << " ";
        std::string s = render_auipc(insn);
        *pos << std::setw(instruction_width) << std::setfill(' ') << std::left << s;
        *pos << "// " << render_reg(rd) << " = " << hex::to_hex0x32(pc) << " + " << hex::to_hex0x32(imm_u) << " = " 
            << hex::to_hex0x32(value);
    }
    regs.set(rd, value); //set rd value = to addr + imm_u value
    pc += 4;
}
void rv32i_hart::exec_jal(uint32_t insn, std::ostream* pos){
    uint32_t rd = get_rd(insn);
    uint32_t imm_j = get_imm_j(insn);
    int32_t next = pc + 4;
    int32_t jumpTo = pc + imm_j;
    if(pos){
        *pos << " ";
        std::string s = render_jal(pc, insn);
        *pos << std::setw(instruction_width) << std::setfill(' ') << std::left << s;
        *pos << "// " << render_reg(rd) << " = " << hex::to_hex0x32(next) << ",  pc = " << hex::to_hex0x32(pc) 
            << " + " << hex::to_hex0x32(imm_j) << " = " << hex::to_hex0x32(jumpTo);
    }
    regs.set(rd, next);
    pc = jumpTo;
}
void rv32i_hart::exec_jalr(uint32_t insn, std::ostream* pos){
    //pc register can never be an odd address, always set LSB to 0
    uint32_t rd = get_rd(insn);
    uint32_t rs1 = get_rs1(insn);
    uint32_t imm_i = get_imm_i(insn);
    int32_t next = pc + 4;
    int32_t jumpTo = regs.get(rs1) + imm_i;//possible problem here
    jumpTo = (jumpTo & 0xfffffffe);
    if(pos){
        *pos << " ";
        std::string s = render_jalr(insn);
        *pos << std::setw(instruction_width) << std::setfill(' ') << std::left << s;
        *pos << "// " << render_reg(rd) << " = " << to_hex0x32(next) << ",  pc = (" 
            << hex::to_hex0x32(imm_i) << " + " << hex::to_hex0x32(regs.get(rs1)) << ") & 0xfffffffe = " 
            << hex::to_hex0x32(jumpTo);
    }
    regs.set(rd, next);
    pc = jumpTo;
}
void rv32i_hart::exec_sltiu(uint32_t insn, std::ostream* pos){
    uint32_t rd = get_rd(insn);
    uint32_t rs1 = get_rs1(insn);
    int32_t imm_i = get_imm_i(insn);
    uint32_t val = 0;
    if(static_cast<uint32_t>(regs.get(rs1)) < static_cast<uint32_t>(imm_i)){
        val = 1;
    }
    if(pos){
        *pos << " ";
        std::string s = render_itype_alu(insn, "sltiu", imm_i);
        *pos << std::setw(instruction_width) << std::setfill(' ') << std::left << s;
        *pos << "// " << render_reg(rd) << " = (" << hex::to_hex0x32(regs.get(rs1)) << " <U "
            << std::dec << imm_i << ") ? 1 : 0 = " << hex::to_hex0x32(val);
    }
    regs.set(rd, val);
    pc += 4;
}
void rv32i_hart::exec_xori(uint32_t insn, std::ostream* pos){
    uint32_t rd = get_rd(insn);
    uint32_t rs1 = get_rs1(insn);
    int32_t imm_i = get_imm_i(insn);
    int32_t val = (regs.get(rs1) ^ imm_i);
    if(pos){
        *pos << " ";
        std::string s = render_itype_alu(insn, "xori", imm_i);
        *pos << std::setw(instruction_width) << std::setfill(' ') << std::left << s;
        *pos << "// " << render_reg(rd) << " = " << hex::to_hex0x32(regs.get(rs1)) << " ^ "
            << to_hex0x32(imm_i) << " = " << hex::to_hex0x32(val);
    }
    regs.set(rd, val);
    pc += 4;
}
void rv32i_hart::exec_beq(uint32_t insn, std::ostream* pos){
    uint32_t rs1 = get_rs1(insn);
    uint32_t rs2 = get_rs2(insn);
    uint32_t imm_b = get_imm_b(insn);
    int32_t val = pc;
    if(regs.get(rs1) == regs.get(rs2)){
        val += imm_b;
    }
    else{
        val += 4;
    }
    if(pos){
        *pos << " ";
        std::string s = render_btype(pc, insn, "beq");
        *pos << std::setw(instruction_width) << std::setfill(' ') << std::left << s;
        *pos << "// pc += (" << hex::to_hex0x32(regs.get(rs1)) << " == " << hex::to_hex0x32(regs.get(rs2))
            << " ? " << hex::to_hex0x32(imm_b) << " : 4) = " << hex::to_hex0x32(val);
    }
    pc = val;
}
void rv32i_hart::exec_bne(uint32_t insn, std::ostream* pos){
    uint32_t rs1 = get_rs1(insn);
    uint32_t rs2 = get_rs2(insn);
    uint32_t imm_b = get_imm_b(insn);
    int32_t val = pc;
    if(regs.get(rs1) != regs.get(rs2)){//if true
        val += imm_b;
    }
    else{//if false
        val += 4;
    }
    if(pos){
        *pos << " ";
        std::string s = render_btype(pc, insn, "bne");
        *pos << std::setw(instruction_width) << std::setfill(' ') << std::left << s;
        *pos << "// pc += (" << hex::to_hex0x32(regs.get(rs1)) << " != " << hex::to_hex0x32(regs.get(rs2))
            << " ? " << hex::to_hex0x32(imm_b) << " : 4) = " << hex::to_hex0x32(val);
    }
    pc = val;//set the program counter to the branch addr
}
void rv32i_hart::exec_blt(uint32_t insn, std::ostream* pos){
    uint32_t rs1 = get_rs1(insn);
    uint32_t rs2 = get_rs2(insn);
    uint32_t imm_b = get_imm_b(insn);
    int32_t val = pc;
    if(regs.get(rs1) < regs.get(rs2)){
        val += imm_b;
    }
    else{
        val += 4;
    }
    if(pos){
        *pos << " ";
        std::string s = render_btype(pc, insn, "blt");
        *pos << std::setw(instruction_width) << std::setfill(' ') << std::left << s;
        *pos << "// pc += (" << hex::to_hex0x32(regs.get(rs1)) << " < " << hex::to_hex0x32(regs.get(rs2))
            << " ? " << hex::to_hex0x32(imm_b) << " : 4) = " << hex::to_hex0x32(val);
    }
    pc = val;
}
void rv32i_hart::exec_bge(uint32_t insn, std::ostream* pos){
    uint32_t rs1 = get_rs1(insn);
    uint32_t rs2 = get_rs2(insn);
    uint32_t imm_b = get_imm_b(insn);
    int32_t val = pc;
    if(regs.get(rs1) >= regs.get(rs2)){
        val += imm_b;
    }
    else{
        val += 4;
    }
    if(pos){
        *pos << " ";
        std::string s = render_btype(pc, insn, "bge");
        *pos << std::setw(instruction_width) << std::setfill(' ') << std::left << s;
        *pos << "// pc += (" << hex::to_hex0x32(regs.get(rs1)) << " >= " << hex::to_hex0x32(regs.get(rs2))
            << " ? " << hex::to_hex0x32(imm_b) << " : 4) = " << hex::to_hex0x32(val);
    }
    pc = val;//branch to next instruction
}
void rv32i_hart::exec_bgeu(uint32_t insn, std::ostream* pos){
    uint32_t rs1 = get_rs1(insn);
    uint32_t rs2 = get_rs2(insn);
    uint32_t imm_b = get_imm_b(insn);
    uint32_t val = pc;
    if(static_cast<uint32_t>(regs.get(rs1)) >= static_cast<uint32_t>(regs.get(rs2))){
        val += imm_b;
    }
    else{
        val += 4;
    }
    if(pos){
        *pos << " ";
        std::string s = render_btype(pc, insn, "bgeu");
        *pos << std::setw(instruction_width) << std::setfill(' ') << std::left << s;
        *pos << "// pc += (" << hex::to_hex0x32(regs.get(rs1)) << " >=U " << hex::to_hex0x32(regs.get(rs2))
            << " ? " << hex::to_hex0x32(imm_b) << " : 4) = " << hex::to_hex0x32(val);
    }
    pc = val;//branch to next instruction
}
void rv32i_hart::exec_bltu(uint32_t insn, std::ostream* pos){
    uint32_t rs1 = get_rs1(insn);
    uint32_t rs2 = get_rs2(insn);
    uint32_t imm_b = get_imm_b(insn);
    uint32_t val = pc;
    if(static_cast<uint32_t>(regs.get(rs1)) < static_cast<uint32_t>(regs.get(rs2))){
        val += imm_b;
    }
    else{
        val += 4;
    }
    if(pos){
        *pos << " ";
        std::string s = render_btype(pc, insn, "bltu");
        *pos << std::setw(instruction_width) << std::setfill(' ') << std::left << s;
        *pos << "// pc += (" << hex::to_hex0x32(regs.get(rs1)) << " <U " << hex::to_hex0x32(regs.get(rs2))
            << " ? " << hex::to_hex0x32(imm_b) << " : 4) = " << hex::to_hex0x32(val);
    }
    pc = val;//branch to next instruction
}
void rv32i_hart::exec_sh(uint32_t insn, std::ostream* pos){
    uint32_t rs1 = get_rs1(insn);
    uint32_t rs2 = get_rs2(insn);
    int32_t imm_s = get_imm_s(insn);
    int32_t address = regs.get(rs1) + imm_s;
    int32_t val = regs.get(rs2) & 0x0000ffff;//16 LSB's
    if(pos){
        *pos << " ";
        std::string s = render_stype(insn, "sh");
        *pos << std::setw(instruction_width) << std::setfill(' ') << std::left << s;
        *pos << "// m16(" << hex::to_hex0x32(regs.get(rs1)) << " + " << hex::to_hex0x32(imm_s) << ") = "
            << hex::to_hex0x32(val);
    }
    mem.set16(address,val);//set memory and increment program counter
    pc += 4;
}
void rv32i_hart::exec_sb(uint32_t insn, std::ostream* pos){
    uint32_t rs1 = get_rs1(insn);
    uint32_t rs2 = get_rs2(insn);
    int32_t imm_s = get_imm_s(insn);
    int32_t address = regs.get(rs1) + imm_s;
    int32_t val = regs.get(rs2) & 0x000000ff;//8 LSB's
    if(pos){
        *pos << " ";
        std::string s = render_stype(insn, "sb");
        *pos << std::setw(instruction_width) << std::setfill(' ') << std::left << s;
        *pos << "// m8(" << hex::to_hex0x32(regs.get(rs1)) << " + " << hex::to_hex0x32(imm_s) << ") = "
            << hex::to_hex0x32(val);
    }
    mem.set8(address,val);//set memory and increment
    pc += 4;
}
void rv32i_hart::exec_sw(uint32_t insn, std::ostream* pos){
    uint32_t rs1 = get_rs1(insn);
    uint32_t rs2 = get_rs2(insn);
    int32_t imm_s = get_imm_s(insn);
    int32_t address = regs.get(rs1) + imm_s;
    if(pos){
        *pos << " ";
        std::string s = render_stype(insn, "sw");
        *pos << std::setw(instruction_width) << std::setfill(' ') << std::left << s;
        *pos << "// m32(" << hex::to_hex0x32(regs.get(rs1)) << " + " << hex::to_hex0x32(imm_s) << ") = "
            << hex::to_hex0x32(regs.get(rs2));
    }
    mem.set32(address, regs.get(rs2));//set memory and increment
    pc += 4;
}
void rv32i_hart::exec_sll(uint32_t insn, std::ostream* pos){
    //shift rs1 left by lsb 5 bits rs2 store in rd
    uint32_t rd = get_rd(insn);
    uint32_t rs1 = get_rs1(insn);
    int32_t rs2 = get_rs2(insn);
    rs2 = (regs.get(rs2) & 0x0000001f);//gets last 5 bits
    uint32_t val = static_cast<uint32_t>(regs.get(rs1)) << static_cast<uint32_t>(rs2);
    if(pos){
        *pos << " ";
        std::string s = render_rtype(insn, "sll");
        *pos << std::setw(instruction_width) << std::setfill(' ') << std::left << s;
        *pos << "// " << render_reg(rd) << " = " << hex::to_hex0x32(regs.get(rs1)) << " << "
            << std::dec << rs2 % (uint32_t)32 << " = " << hex::to_hex0x32(val);
    }
    regs.set(rd, val);//set registers and increment
    pc += 4;
}
void rv32i_hart::exec_sltu(uint32_t insn, std::ostream* pos){
    uint32_t rd = get_rd(insn);
    uint32_t rs1 = get_rs1(insn);
    uint32_t rs2 = get_rs2(insn);
    uint32_t val = 0;//val keeps track of what register will be set to
    if(static_cast<uint32_t>(regs.get(rs1)) < static_cast<uint32_t>(regs.get(rs2))){
        val = 1;
    }
    if(pos){
        *pos << " ";
        std::string s = render_rtype(insn, "sltu");
        *pos << std::setw(instruction_width) << std::setfill(' ') << std::left << s;
        *pos << "// " << render_reg(rd) << " = (" << hex::to_hex0x32(regs.get(rs1)) << " <U " << hex::to_hex0x32(regs.get(rs2))
             << ") ? 1 : 0 = " << hex::to_hex0x32(val);
    }
    regs.set(rd, val);
    pc += 4;
}
void rv32i_hart::exec_slti(uint32_t insn, std::ostream* pos){
    uint32_t rd = get_rd(insn);
    uint32_t rs1 = get_rs1(insn);
    int32_t imm_i = get_imm_i(insn);
    int32_t val = 0; //keeps track of what register will be set to
    if(static_cast<int32_t>(regs.get(rs1)) < imm_i){
        val = 1;
    }
    if(pos){
        *pos << " ";
        std::string s = render_itype_alu(insn, "slti", imm_i);
        *pos << std::setw(instruction_width) << std::setfill(' ') << std::left << s;
        *pos << "// " << render_reg(rd) << " = (" << hex::to_hex0x32(regs.get(rs1)) << " < " << std::dec << imm_i 
             << ") ? 1 : 0 = " << hex::to_hex0x32(val);
    }
    regs.set(rd, val);//set regs and increment
    pc += 4;
}
void rv32i_hart::exec_sra(uint32_t insn, std::ostream* pos){
   uint32_t rd = get_rd(insn);
   uint32_t rs1 = get_rs1(insn);
   uint32_t rs2 = get_rs2(insn);
   rs2 = (regs.get(rs2) & 0x0000001f);//last 5 bits
   int32_t val = regs.get(rs1) >> rs2;//shift right byt shift amount
   if(pos){
       *pos << " ";
        std::string s = render_rtype(insn, "sra");
        *pos << std::setw(instruction_width) << std::setfill(' ') << std::left << s;
        *pos << "// " << render_reg(rd) << " = " << hex::to_hex0x32(regs.get(rs1)) << " >> " << std::dec 
             << rs2 % (uint32_t)32 << " = " << hex::to_hex0x32(val);
   }
   regs.set(rd, val);
   pc += 4;
}
void rv32i_hart::exec_sub(uint32_t insn, std::ostream* pos){
   uint32_t rd = get_rd(insn);
   uint32_t rs1 = get_rs1(insn);
   uint32_t rs2 = get_rs2(insn);
   int32_t val = regs.get(rs1) - regs.get(rs2);
   if(pos){
       *pos << " ";
       std::string s = render_rtype(insn, "sub");
       *pos << std::setw(instruction_width) << std::setfill(' ') << std::left << s;
       *pos << "// " << render_reg(rd) << " = " << hex::to_hex0x32(regs.get(rs1)) << " - " << hex::to_hex0x32(regs.get(rs2))
            << " = " << hex::to_hex0x32(val);
   }
   regs.set(rd, val);//set regs and increment
   pc += 4;
}
void rv32i_hart::exec_srl(uint32_t insn, std::ostream* pos){
   uint32_t rd = get_rd(insn);
   uint32_t rs1 = get_rs1(insn);
   int32_t rs2 = get_rs2(insn);
   rs2 = (regs.get(rs2) & 0x0000001f);
   int32_t val = static_cast<uint32_t>(regs.get(rs1)) >> static_cast<uint32_t>(rs2);
   if(pos){
       *pos << " ";
       std::string s = render_rtype(insn, "srl");
       *pos << std::setw(instruction_width) << std::setfill(' ') << std::left << s;
       *pos << "// " << render_reg(rd) << " = " << hex::to_hex0x32(regs.get(rs1)) << " >> " << std::dec 
            << rs2 % (uint32_t)32<< " = " << hex::to_hex0x32(val);//&32 to get right value
   }
   regs.set(rd, val);//set regs and increment
   pc += 4;
}
void rv32i_hart::exec_addi(uint32_t insn, std::ostream* pos){
    uint32_t rd = get_rd(insn);
    uint32_t rs1 = get_rs1(insn);
    uint32_t imm_i = get_imm_i(insn);
    int32_t val = regs.get(rs1) + imm_i;//val tracks what to put in register rd
    if(pos){
        *pos << " ";
        std::string s = render_itype_alu(insn, "addi", imm_i);
       *pos << std::setw(instruction_width) << std::setfill(' ') << std::left << s;
       *pos << "// " << render_reg(rd) << " = " << hex::to_hex0x32(regs.get(rs1)) << " + " << hex::to_hex0x32(imm_i)
            << " = " << hex::to_hex0x32(val);
    }
    regs.set(rd, val);//set regs and increment
    pc += 4;
}
void rv32i_hart::exec_slli(uint32_t insn, std::ostream* pos){
    uint32_t rd = get_rd(insn);
    uint32_t rs1 = get_rs1(insn);
    int32_t imm_i = get_rs2(insn);
    int32_t val = regs.get(rs1) << imm_i; //get rs1 and shift left
    if(pos){
        *pos << " ";
        std::string s = render_itype_alu(insn, "slli", imm_i);
        *pos << std::setw(instruction_width) << std::setfill(' ') << std::left << s;
        *pos << "// " << render_reg(rd) << " = " << hex::to_hex0x32(regs.get(rs1)) << " << " << std::dec << imm_i
             << " = " << hex::to_hex0x32(val);
    }
    regs.set(rd, val);//set regs and increment
    pc += 4;
}
void rv32i_hart::exec_srli(uint32_t insn, std::ostream* pos){
    uint32_t rd = get_rd(insn);
    uint32_t rs1 = get_rs1(insn);
    int32_t imm_i = get_rs2(insn);
    int32_t val = (static_cast<uint32_t>(regs.get(rs1)) >> imm_i);//must static cast to uint32_t
    if(pos){
        *pos << " ";
        std::string s = render_itype_alu(insn, "srli", imm_i);
        *pos << std::setw(instruction_width) << std::setfill(' ') << std::left << s;
        *pos << "// " << render_reg(rd) << " = " << hex::to_hex0x32(regs.get(rs1)) << " >> " << std::dec << imm_i
             << " = " << hex::to_hex0x32(val);
    }
    regs.set(rd, val);//set regs and icnrement
    pc += 4;
}
void rv32i_hart::exec_srai(uint32_t insn, std::ostream* pos){
    uint32_t rd = get_rd(insn);
    uint32_t rs1 = get_rs1(insn);
    int32_t imm_i = get_rs2(insn);
    int32_t val = regs.get(rs1) >> imm_i; //shift right imm_i
    if(pos){
        *pos << " ";
        std::string s = render_itype_alu(insn, "srai", imm_i);
        *pos << std::setw(instruction_width) << std::setfill(' ') << std::left << s;
        *pos << "// " << render_reg(rd) << " = " << hex::to_hex0x32(regs.get(rs1)) << " >> " << std::dec << imm_i
             << " = " << hex::to_hex0x32(val);
    }
    regs.set(rd, val);//set regs and increment
    pc += 4;
}
void rv32i_hart::exec_ori(uint32_t insn, std::ostream* pos){
    uint32_t rd = get_rd(insn);
    uint32_t rs1 = get_rs1(insn);
    uint32_t imm_i = get_imm_i(insn);
    int32_t val = (regs.get(rs1) | imm_i); //or rs1 value with imm_i
    if(pos){
        *pos << " ";
        std::string s = render_itype_alu(insn, "ori", imm_i);
        *pos << std::setw(instruction_width) << std::setfill(' ') << std::left << s;
        *pos << "// " << render_reg(rd) << " = " << hex::to_hex0x32(regs.get(rs1)) << " | " << hex::to_hex0x32(imm_i)
             << " = " << hex::to_hex0x32(val);
    }
    regs.set(rd, val);//set and increment
    pc += 4;
}
void rv32i_hart::exec_xor(uint32_t insn, std::ostream* pos){
   uint32_t rd = get_rd(insn);
   uint32_t rs1 = get_rs1(insn);
   uint32_t rs2 = get_rs2(insn);
   int32_t val = (regs.get(rs1) ^ regs.get(rs2)); // XOR rs1 with rs2 which is what rd is set to
   if(pos){
       *pos << " ";
       std::string s = render_rtype(insn, "xor");
       *pos << std::setw(instruction_width) << std::setfill(' ') << std::left << s;
       *pos << "// " << render_reg(rd) << " = " << hex::to_hex0x32(regs.get(rs1)) << " ^ " << hex::to_hex0x32(regs.get(rs2))
            << " = " << hex::to_hex0x32(val);
   }
   regs.set(rd, val);//set and increment
   pc += 4;
}
void rv32i_hart::exec_lb(uint32_t insn, std::ostream* pos){
    uint32_t rd = get_rd(insn);
    uint32_t imm_i = get_imm_i(insn);
    uint32_t rs1 = get_rs1(insn);
    int32_t address = regs.get(rs1) + imm_i;
    int32_t val = mem.get8(address);//get the mem from addr
    if(val & 0x00000080){//check to see if this bit is on
        val = val | 0xffffff00;
    }
    if(pos){
        *pos << " ";
        std::string s = render_itype_load(insn, "lb");
        *pos << std::setw(instruction_width) << std::setfill(' ') << std::left << s;
        *pos << "// " << render_reg(rd) << " = sx(m8(" << hex::to_hex0x32(regs.get(rs1)) << " + " << hex::to_hex0x32(imm_i)
             << ")) = " << hex::to_hex0x32(val);  
    }
    regs.set(rd, val);//set and increment
    pc += 4;
}
void rv32i_hart::exec_lh(uint32_t insn, std::ostream* pos){
    uint32_t rd = get_rd(insn);
    uint32_t imm_i = get_imm_i(insn);
    uint32_t rs1 = get_rs1(insn);
    int32_t address = regs.get(rs1) + imm_i;
    int32_t val = mem.get16(address);
    if(val & 0x00008000){//check to see if bit is on
        val = val | 0xffff0000;
    }
    if(pos){
        *pos << " ";
        std::string s = render_itype_load(insn, "lh");
        *pos << std::setw(instruction_width) << std::setfill(' ') << std::left << s;
        *pos << "// " << render_reg(rd) << " = sx(m16(" << hex::to_hex0x32(regs.get(rs1)) << " + " << hex::to_hex0x32(imm_i)
             << ")) = " << hex::to_hex0x32(val);  
    }
    regs.set(rd, val);//set and increment
    pc += 4;
}
void rv32i_hart::exec_lw(uint32_t insn, std::ostream* pos){
    uint32_t rd = get_rd(insn);
    uint32_t imm_i = get_imm_i(insn);
    uint32_t rs1 = get_rs1(insn);
    int32_t address = regs.get(rs1) + imm_i;
    int32_t val = mem.get32(address);//get mem
    if(pos){
        *pos << " ";
        std::string s = render_itype_load(insn, "lw");
        *pos << std::setw(instruction_width) << std::setfill(' ') << std::left << s;
        *pos << "// " << render_reg(rd) << " = sx(m32(" << hex::to_hex0x32(regs.get(rs1)) << " + " << hex::to_hex0x32(imm_i)
             << ")) = " << hex::to_hex0x32(val);  
    }
    regs.set(rd, val);//set and increment
    pc += 4;
}
void rv32i_hart::exec_lbu(uint32_t insn, std::ostream* pos){
    uint32_t rd = get_rd(insn);
    uint32_t imm_i = get_imm_i(insn);
    uint32_t rs1 = get_rs1(insn);
    int32_t address = regs.get(rs1) + imm_i;//get addr
    uint32_t val = mem.get8(address);//get mem
    if(pos){
        *pos << " ";
        std::string s = render_itype_load(insn, "lbu");
        *pos << std::setw(instruction_width) << std::setfill(' ') << std::left << s;
        *pos << "// " << render_reg(rd) << " = zx(m8(" << hex::to_hex0x32(regs.get(rs1)) << " + " << hex::to_hex0x32(imm_i)
             << ")) = " << hex::to_hex0x32(val);  
    }
    regs.set(rd, val);//set and increment
    pc += 4;
}
void rv32i_hart::exec_lhu(uint32_t insn, std::ostream* pos){
    uint32_t rd = get_rd(insn);
    uint32_t imm_i = get_imm_i(insn);
    uint32_t rs1 = get_rs1(insn);
    int32_t address = regs.get(rs1) + imm_i;//get addr
    uint32_t val = mem.get16(address);//get mem
    if(pos){
        *pos << " ";
        std::string s = render_itype_load(insn, "lhu");
        *pos << std::setw(instruction_width) << std::setfill(' ') << std::left << s;
        *pos << "// " << render_reg(rd) << " = zx(m16(" << hex::to_hex0x32(regs.get(rs1)) << " + " << hex::to_hex0x32(imm_i)
             << ")) = " << hex::to_hex0x32(val);  
    }
    regs.set(rd, val);//set and increment
    pc += 4;
}
void rv32i_hart::exec_add(uint32_t insn, std::ostream* pos){
   uint32_t rd = get_rd(insn);
   uint32_t rs1 = get_rs1(insn);
   uint32_t rs2 = get_rs2(insn);
   int32_t val = regs.get(rs1) + regs.get(rs2);//get value which is future rd value
   if(pos){
       *pos << " ";
       std::string s = render_rtype(insn, "add");
       *pos << std::setw(instruction_width) << std::setfill(' ') << std::left << s;
       *pos << "// " << render_reg(rd) << " = " << hex::to_hex0x32(regs.get(rs1)) << " + " << hex::to_hex0x32(regs.get(rs2))
            << " = " << hex::to_hex0x32(val);
   }
   regs.set(rd, val);//set val and increment
   pc += 4;
}
void rv32i_hart::exec_or(uint32_t insn, std::ostream* pos){
   uint32_t rd = get_rd(insn);
   uint32_t rs1 = get_rs1(insn);
   uint32_t rs2 = get_rs2(insn);
   int32_t val = regs.get(rs1) | regs.get(rs2);//get value by or ing and then set regs off that
   if(pos){
       *pos << " ";
       std::string s = render_rtype(insn, "or");
       *pos << std::setw(instruction_width) << std::setfill(' ') << std::left << s;
       *pos << "// " << render_reg(rd) << " = " << hex::to_hex0x32(regs.get(rs1)) << " | " << hex::to_hex0x32(regs.get(rs2))
            << " = " << hex::to_hex0x32(val);
   }
   regs.set(rd, val);
   pc += 4;
}
void rv32i_hart::exec_and(uint32_t insn, std::ostream* pos){
   uint32_t rd = get_rd(insn);
   uint32_t rs1 = get_rs1(insn);
   uint32_t rs2 = get_rs2(insn);
   int32_t val = regs.get(rs1) & regs.get(rs2);//get value by and ing and then set rd reg
   if(pos){
       *pos << " ";
       std::string s = render_rtype(insn, "and");
       *pos << std::setw(instruction_width) << std::setfill(' ') << std::left << s;
       *pos << "// " << render_reg(rd) << " = " << hex::to_hex0x32(regs.get(rs1)) << " & " << hex::to_hex0x32(regs.get(rs2))
            << " = " << hex::to_hex0x32(val);
   }
   regs.set(rd, val);
   pc += 4;//increment to next instruction
}
void rv32i_hart::exec_andi(uint32_t insn, std::ostream* pos){
   uint32_t rd = get_rd(insn);
   uint32_t rs1 = get_rs1(insn);
   uint32_t imm_i = get_imm_i(insn);
   int32_t val = (regs.get(rs1) & imm_i);//get rs1 value and and it with immediate i
   if(pos){
       *pos << " ";
       std::string s = render_itype_alu(insn, "andi", imm_i);
       *pos << std::setw(instruction_width) << std::setfill(' ') << std::left << s;
       *pos << "// " << render_reg(rd) << " = " << hex::to_hex0x32(regs.get(rs1)) << " & " << hex::to_hex0x32(imm_i)
            << " = " << hex::to_hex0x32(val);
   }
   regs.set(rd, val);
   pc += 4;
}
void rv32i_hart::exec_ecall(uint32_t insn, std::ostream* pos){
    if(pos){
        *pos << " ";
        std::string s = render_ecall(insn);//for ecalls
        *pos << std::setw(instruction_width) << std::setfill(' ') << std:: left << s;
        *pos << "// HALT";//prints where other instructions would be
    }
    halt = true;//set halt and reason
    halt_reason = "ECALL instruction";
}
void rv32i_hart::exec_ebreak(uint32_t insn, std::ostream* pos){
    if(pos){
        *pos << " ";
        std::string s = render_ebreak(insn);
        *pos << std::setw(instruction_width) << std::setfill(' ') << std:: left << s;
        *pos << "// HALT";
    }
    halt = true;//set halt and reason
    halt_reason = "EBREAK instruction";
}
void rv32i_hart::exec_csrrs(uint32_t insn, std::ostream* pos){
    uint32_t rd = get_rd(insn);
    int32_t csr = (insn & 0xfff00000) >> (31-11);
    if(csr == 0xf14){//ONLY WORKS IF CSR is f14 otherwise halt and give reason
        if(pos){
            *pos << " ";
            std::string s = render_csrrx(insn, "csrrs");
            *pos << std::setw(instruction_width) << std::setfill(' ') << std::left << s;
            *pos << "// " << render_reg(rd) << " = " << std::dec << mhartid;
        }
        regs.set(rd, mhartid);//set rd to id and increment
        pc += 4;
    }
    else{
        halt = true;
        halt_reason = "Illegal CSR in CSRSS instruction";
    }
}





