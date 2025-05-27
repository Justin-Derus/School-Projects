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
#include "registerfile.h"
registerfile::registerfile(){ //constructor calls reset
    registerfile::reset();
}
registerfile::~registerfile(){//destructor
    registerfile::registers.clear();
}
void registerfile::reset(){ //initialized register vector to 0 for x0, f0f0f0f0f0 for all else
    registerfile::registers.push_back(0);//first reg is 0
    for(int i = 0; i < 31; i++){//rest are f0f0f0f0
        registerfile::registers.push_back(0xf0f0f0f0);
    }
}
void registerfile::set(uint32_t r, int32_t val){
    if(r == 0){//register 0 always must have 0 in it
        return;
    }
    else{//otherwise we are not at register 0, give it a value
        registerfile::registers[r] = val;
        return;
    }
}
int32_t registerfile::get(uint32_t r) const{
    if(r == 0){//if the provided register is 0, return 0
        return 0;
    }
    else{//if we have anything other than 0, return the value of the register
        return registerfile::registers[r];
    }
}
void registerfile::dump(const std::string &hdr) const{//header
    cout << hdr << " x" << 0 << " ";//print out the first header if it exists
    for(uint32_t i = 0; i < 32; i++){//loop through the registers and dump their values
        cout << hex::to_hex32(registerfile::registers[i]);
        if(i % 8 != 7 && i % 4 != 3){//formatting
            if(i == 0)
                cout << " ";
            else
                cout << " ";
        }
        if(i % 4 == 3 && i % 8 != 7){//formatting
            cout << "  ";
        }
        if(i != 0 && i % 8 == 7 && i != 31){//formatting
            cout << endl;
            cout << hdr;
            if(i < 8) 
                cout << " x" << i + 1 << " ";
            else
                cout << "x" << i + 1 << " ";
        }
    }
}