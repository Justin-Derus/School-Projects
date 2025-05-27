#include "htable.h"
#include <algorithm>

#define N1 10  // num of decimal digits
#define N2 26  // num of upper-case letters

// Hash function, you may NOT modify this function
int HT::hash(const string& s) {//KT5 returns 5 -- ZZ0 returns 23 -- takes in Entry key
    int n = (N2 * N1) * (s[0] - 'A') + N1 * (s[1] - 'A') + (s[2] - '0');

    return n % hsize;
}

HT::HT(const unsigned& hs) {
    hsize = hs;             //set hsize to hs
    hTable.resize(hsize);   //after the hsize is set, resize the htable to hsize
}

HT::~HT() {//working
    hTable.clear();//destroys all objects within the hashtable
    pTable.clear();//destroy all objects
    hTable.shrink_to_fit();//makes the memory as minimal as possible
    pTable.shrink_to_fit();//make memory minimal
}

void HT::insert(const Entry& e) {//working
    int hashvalue = HT::hash(e.key);  
    bool flagged = false;
    for (auto i = 0; i < hsize; ++i) {
        for (auto j = hTable[i].begin(); j != hTable[i].end(); ++j) {
            if (j->key == e.key) {
                cout << " not inserted - duplicate key!!!" << endl;
                flagged = true;
            }
            else
                continue;
        }    
    }
    if (flagged == false) {
        for (int i = 0; i < hsize; ++i) {
            if (hashvalue == i) {
                hTable[i].push_front(e);
                pTable.push_back(&hTable[i].front());
                cout << " entry =  " << i << endl;
            }
        }
    }
}

void HT::search(const string& s) {//working
    bool check = false;
    for (int i = 0; i < hsize; ++i) {//loop through the hTable
        for (auto j = hTable[i].begin(); j != hTable[i].end(); ++j) {//loop thru the list
            if (j->key == s) {//if the list nodes key is equal to what we want, output the data
                check = true;//if the key is found, set the flag to true
                cout << " ==> number:" << setw(5) << j->num << " - item:" << j->desc << endl;
            }
            else continue;//the key is not in the current spot
        }
    }
    if (check == false) {//this means that the flag was not set to true/ key is not in table
        cout << " not in table!!" << endl;
    }

}

void HT::hTable_print() {//update setw between number and description
    cout << endl;
    for (int i = 0; i < hsize; ++i) {//loop entire hashtable size
        if (hTable[i].size() == 1) {//if the list has a value in it, print the contents of the first/only element
            cout << setw(4) << i << ":" << "  " << hTable[i].front().key << "  -" << setw(8) << right
                << hTable[i].front().num << "  -  " << hTable[i].front().desc << endl;
        }
        else if (hTable[i].size() > 1) {
            for (auto j = hTable[i].begin(); j != hTable[i].end(); ++j) {
                cout << setw(4) << i << ":" << "  " << j->key << "  -" << setw(8) << right
                    << j->num << "  -  " << j->desc << endl;
            }
        }
    }
}

bool compareKeys(const Entry* one, const Entry* two) {//working compare function for sort()
    return one->key < two->key;
}
void HT::pTable_print() {//working
    sort(pTable.begin(), pTable.end(), compareKeys);//compares based off of the keys
    for (int i = 0; i < pTable.size();++i) {//print all data 
        cout << setw(6) << pTable[i]->key << "  - " << setw(5) << pTable[i]->num
            << "  -  " << pTable[i]->desc << endl;
    }
}