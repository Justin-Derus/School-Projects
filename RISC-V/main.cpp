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
#include "memory.h"
#include "rv32i_decode.h"
#include "rv32i_hart.h"
#include "registerfile.h"
#include "cpu_single_hart.h"
/**
 * Tells the user what the command line structure should look like and its default
 *
 * @return The "return" is the cerr message printed to the console
 *
 * @note Function is static
 ********************************************************************************/
static void usage()
{
	cerr << "Usage: (executable_name_here) [-d] [-i] [-r] [-z] [-l exec-limit] [-m hex-mem-size] infile > (outfile_optional)" << endl;
    cerr << "    -d show disassembly before program execution" << endl;
    cerr << "    -i show instruction printing during execution" << endl;
    cerr << "    -l maximum number of instructions to exec" << endl;
	cerr << "    -m specify memory size (default = 0x100)" << endl;
    cerr << "    -r show register printing during execution" << endl;
    cerr << "    -z show a dump of the regs & memory after simulation" << endl;
	exit(1);
}
/**
 * Decodes the bytes in memory to corresponding instructions
 *
 * @param mem This is the memory class object being passed in that holds the mem vector
 *
 * @note Function calls rv32i::decode to return the rendered instructions
 ********************************************************************************/
static void disassemble(const memory &mem)
{   
    for(uint32_t i = 0; i < mem.get_size(); i+=4){
        uint32_t insn = mem.get32(i);
        std::string rendered_insn = rv32i_decode::decode(i,insn);
        cout << hex::to_hex32(i) << ": " << hex::to_hex32(insn) << "  " << rendered_insn << endl;
    }
}
/**
 * Simulates memory based off of input files and shows their instructions
 *
 * @param argc the amount of arguments passed into the file via command lines
 * @param argv the string values that the user has passed in via command line
 *
 * @return Program returns 0 upon success
 *
 * @note Main calls disassemble() and usage()
 ********************************************************************************/
int main(int argc, char **argv)
{
    uint32_t memory_limit = 0x100;
    uint64_t execution_limit = 0;
    int opt;
    bool dFlag = false;
    bool iFlag = false;
    bool rFlag = false;
    bool zFlag = false;
    while ((opt = getopt(argc, argv, "dil:m:rz")) != -1)
    {
        switch (opt)//check all of the optional command line args and set flags for them
        {
        case 'd':
            {
                dFlag = true;
            }
            break;
        case 'i':
            {
                iFlag = true;
            }
            break;
        case 'l':
            {
                std::istringstream iss(optarg);
                iss >> std::hex >> execution_limit;
            }
            break;
        case 'm':
            {
                std::istringstream iss(optarg);
                iss >> std::hex >> memory_limit;
            }
            break;
        case 'r':
            {
                rFlag = true;
            }
            break;
        case 'z':
            {
                zFlag = true;
            }
            break;
        default: /* '?' */
            usage();
        }
    }

    if (optind >= argc)
        usage();    //missing filename
    //declare memory and registers
    memory mem(memory_limit);
    cpu_single_hart cpu(mem);
    if (!mem.load_file(argv[optind]))
        usage();
    cpu.reset();
    if(dFlag){
        disassemble(mem);
    }
    cpu.set_show_instructions(iFlag);
    cpu.set_show_registers(rFlag);
    cpu.run(execution_limit);//run the simulation with a freshly reset cpu
    if(zFlag){
        cpu.dump();
        mem.dump();
    }

    return 0;
}