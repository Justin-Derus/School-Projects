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

/**
 * The constructor of the memory class
 *
 * @param s eventually the size of the mem class vector "mem"
 *
 * @note the param s is rounded up before it actually sets mem.size
 ********************************************************************************/
memory::memory(uint32_t s){
    s = (s+15)&0xfffffff0; //round the length up, mod-16
    mem.resize(s,0xa5);    //set mem size and initialize every index to 0xa5 default value
}

/**
 * The destructor of the memory class
 *
 * @note this destructor frees the memory used in the mem vector
 ********************************************************************************/
memory::~memory(){
    mem.clear();           //clear the contents of the memory vector to free up memory
}

/**
 * Check to see if the address is valid or not
 *
 * @param addr Holds the address that we are checking for in mem vector
 *
 * @return returns a boolean true if the address is not valid, or false if it is
 *
 * @note calls to_hex0x32 to print the addr if it is invalid and then warns user
 ********************************************************************************/
bool memory::check_illegal(uint32_t addr) const{
    if(addr >= mem.size()){ //check to see if the address is greater than the addr OR if it is negative
        cout << "WARNING: Address out of range: " << hex::to_hex0x32(addr) << endl;
        return true;
    }
    else{
        return false;
    }
}

/**
 * Get the current size of the mem vector 
 *
 * @return returns the uint32_t size of the vector by calling mem.size()
 *
 * @note the size is set in the constructor by its passed value, otherwise default is 0x100
 ********************************************************************************/
uint32_t memory::get_size() const{
    return mem.size();  //return the size of the memory vector
}

/**
 * Checks the value at addr in memory
 *
 * @param addr Holds the address that we are checking for in mem vector
 *
 * @return If the address is valid, it then returns the 8 bit value at that address in mem
 *
 * @note function calls check_illegal() to see if the address is valid or not
 ********************************************************************************/
uint8_t memory::get8(uint32_t addr) const{
    if(!check_illegal(addr)){   //only code that will ever read values from the mem vector
        return mem[addr];
    }
    else{   //if the addr is not within range, then return 0 to the user
        return 0;
    }
}

/**
 * Calls get8() twice and then combines the two values in little endian order
 *
 * @param addr Holds the address that we are checking for in mem vector
 *
 * @return If the address is valid, it then returns the 16 bit value at that address in mem
 *
 * @note function calls get8() twice to get two 8 bit values and then combines them by 
 * shifting and using the OR bitwise operator
 ********************************************************************************/
uint16_t memory::get16(uint32_t addr) const{
    uint8_t first_half = memory::get8(addr);    //get the first byte of the addr
    uint8_t second_half = memory::get8(addr+1); //get the second byte of the addr
    uint16_t result = ((second_half << 8) | first_half);
    return result;
}

/**
 * Calls get16() twice to get two 16 bit values to combine
 *
 * @param addr Holds the address that we are checking for in mem vector
 *
 * @return If the address is valid, it then returns the 32 bit value at that address in mem
 *
 * @note function calls get16() twice to get two 16 bit values and then combines them by 
 * shifting and using the OR bitwise operator
 ********************************************************************************/
uint32_t memory::get32(uint32_t addr) const{
    uint16_t first_half = memory::get16(addr);
    uint16_t second_half = memory::get16(addr+2);//needs + 2 instead of +1 because it is 16 bits not 8
    uint32_t result = ((second_half << 16) | first_half);
    return result;
}

/**
 * Gets the signed version of the mem[addr] byte in memory
 *
 * @param addr Holds the address that we are checking for in mem vector
 *
 * @return If the address is valid, it then returns the sign extended
 * value of the byte as a 32-bit signed integer
 *
 * @note ffunction calls get8() and then shifts and checks to find the correct sign values
 ********************************************************************************/
int32_t memory::get8_sx(uint32_t addr) const{
    uint32_t result = get8(addr);   //call get 8 to get the unsigned version
    uint32_t signAnd = 0x80000000;  //for finding sign bit
    uint32_t temp = result << 24;   //get sign bit in front
    int32_t sign = (temp & signAnd);//sign bit now stored
    if(sign){//if negative
       result <<= 1;
       result >>=1;
       result = (result | 0xffffff00);
    }
    else{//positive
        result = (result | sign);
    }
    return result;
}

/**
 * Gets the signed version of the mem[addr] byte in memory
 *
 * @param addr Holds the address that we are checking for in mem vector
 *
 * @return If the address is valid, it then returns the sign extended
 * value of the 16 bit value as a 32-bit signed integer
 *
 * @note function calls get16() and then shifts and checks to find the correct sign values
 ********************************************************************************/
int32_t memory::get16_sx(uint32_t addr) const{
    uint32_t result = get16(addr);  //call get 8 to get the unsigned version
    uint32_t signAnd = 0x80000000;  //for finding sign bit
    uint32_t temp = result << 16;   //get sign bit in front
    int32_t sign = (temp & signAnd);//sign bit now stored
    if(sign){                       //if negative 
        result <<= 1;               //shift off the first bit
        result >>= 1;               //get back positions
        result = (result | 0xffff0000);//OR it with ffff0000 to fill f's and keep bytes
    }
    else{                           //sign is positive
        result = (result | sign);   //just OR result with the sign
    }                               
    return result;                  //return the result
}

/**
 * Gets the signed version of the mem[addr] byte in memory
 *
 * @param addr Holds the address that we are checking for in mem vector
 *
 * @return If the address is valid, it returns the value as a 32 bit signed integer
 *
 * @note function calls get32()
 ********************************************************************************/
int32_t memory::get32_sx(uint32_t addr) const{  
    return get32(addr);             //return the 32 bit addr value
}

/**
 * Does the actual manipulating of the simulated memory
 *
 * @param addr Holds the address that we are checking for in mem vector
 * @param val Will become the value that mem[addr] holds
 *
 * @return Returns if the addr is not valid
 *
 * @note function calls check_illegal() to see if the addr is a legal addr
 ********************************************************************************/        
void memory::set8(uint32_t addr, uint8_t val){ //only code that will ever write into the mem vector
    if(!check_illegal(addr)){                  //if address is valid
        mem[addr] = val;                       //set the address in mem to the value parameter
    }
    else{                                      //if the address is invalid, return and do nothing with the value passed
        return;
    }
}

/**
 * Does the actual manipulating of the simulated memory in little endian order
 *
 * @param addr Holds the address that we are checking for in mem vector
 * @param val Will become the value that mem[addr] holds
 *
 * @note function calls set8() at end which then sets the values (pseudo recursive)
 ********************************************************************************/   
void memory::set16(uint32_t addr, uint16_t val){
    uint16_t temp = (val & 0x00ff);//& value with 00ff to get the left 8 bits of the 16 bit value off
    uint8_t first_half = temp;     //now use the right 8 bits of temp and set first_half (for hello.in it will be 34)
    uint8_t second_half = val >> 8;//get the second half of val by just shifting right 8 bits - no need for temp
    set8(addr, first_half);        //call set 8
    set8(addr+1, second_half);     //call set 8 to addr + 1
}

/**
 * Does the actual manipulating of the simulated memory
 *
 * @param addr Holds the address that we are checking for in mem vector
 * @param val Will become the value that mem[addr] holds
 *
 * @note function calls set16() which calls set8() to manipulate the vector
 ********************************************************************************/   
void memory::set32(uint32_t addr, uint32_t val){
    uint32_t temp = (val & 0x0000ffff);//& value with 0000ffff to get left 16 bits off
    uint16_t first_half = temp;        //now use right 16 bits of temp and set first_half
    uint16_t second_half = val >> 16;  //get second half by shifting val 16 - dont need temp
    set16(addr, first_half);           //call set16 twice on addr and addr + 2 (+2 because it will be 2 bytes past addr (16 bits))
    set16(addr+2, second_half);
}

/**
 * Prints the actual contents of the mem vector in groups of 8 bytes, the ASCII characters
 * as well as the address of the bytes to the left
 *
 * @note function calls to_hex32() to print the address lines
 * and to_hex8() to print the actual values of mem in hex, must call to_hex8() so
 * that it actually prints the correct values as a string
 ********************************************************************************/   
void memory::dump() const{          
    cout << to_hex32(0) << ": ";                        //print the addr line
    for(long unsigned int i = 0; i < mem.size(); i++){  //loop thru and print the memory
        if(i != 0 && i % 8 == 0 && i % 16 != 0){        //adds spaces every 8 bytes
            cout << " ";
        }
        if(i % 16 == 0 && i != 0){                      //before new line print the ASCII characters
            cout << "*";
            for(auto j = i - 16; j < i; j++){
                uint8_t ch = get8(j);
                ch = isprint(ch) ? ch : '.';
                cout << ch;
            }
            cout << "*";
            cout << endl;
            cout << to_hex32(i) << ": ";    //print the addr line
        }
        cout << to_hex8(mem[i]) << " ";     //print mem
        if(i + 1 == mem.size()){            //check to see if it is the last line of memory
            cout << "*";
            for(auto j = i - 15; j <= i; j++){//print ASCII
                uint8_t ch = get8(j);
                ch = isprint(ch) ? ch : '.';
                cout << ch;
            }
            cout << "*";
            cout << endl;
        }
    }
}

/**
 * Opens the file that holds the data to be passed into memory if applicable
 *
 * @param fname contains the name of the file that will be opened which the data
 * will be streamed in from
 *
 * @return Returns a boolean true if the values are all loaded in successfully,
 * false if the file cannot open, or if the file is too big for the simulated memory.
 *
 * @note function opens the files in binary and does not skip whitespaces
 * function calls check_illegal() to see if the address (for loop counter) is in memory
 ********************************************************************************/   
bool memory::load_file(const std::string &fname){
    std::ifstream infile(fname, std::ios::in|std::ios::binary); //open the file in binary
    if(infile.fail()){           //check to see if the file 
        cerr << "Can't open file '" << fname << "' for reading." << endl;
        return false;
    }
    uint8_t i;
    infile >> std::noskipws;     //do not skip reading whitespaces
    for(uint32_t addr = 0; infile >> i; ++addr){
        if(!check_illegal(addr)){//if valid address read in the data
            mem[addr] = i;
        }
        else{//if address is invalid close and return 
            cerr << "Program too big." << endl;
            infile.close();
            return false;
        }
    }
    infile.close();//close the file when done successfully
    return true;
}
