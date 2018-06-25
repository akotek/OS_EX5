#include "VirtualMemory.h"
#include "PhysicalMemory.h"
#include <iostream>

using namespace std;



void clearTable(uint64_t frameIndex) {
    for (uint64_t i = 0; i < PAGE_SIZE; ++i) {
        PMwrite(frameIndex * PAGE_SIZE + i, 0);
    }
}

// Helper Functions

void createAddressTable(uint64_t fullVirtualAddress, uint64_t
pagesAddresses[TABLES_DEPTH]);



// DFS recursive implementation
word_t findEmptyFrame()
{
    uint64_t root = 0;


}



void dfs(uint64_t physicalAddress, word_t value)
{
    PMread(physicalAddress, &value);
    if(value != 0)
    {
        for(int i = 0; i < PAGE_SIZE; i++)
        {



            dfs(physicalAddress + i, value);

        }
    }
}


void VMinitialize() {
    clearTable(0);
}


int VMread(uint64_t virtualAddress, word_t* value) {



    int max_frame_index = 0;


    return 1;
}


int VMwrite(uint64_t virtualAddress, word_t value)
{

    uint64_t offset = virtualAddress % (2 << (OFFSET_WIDTH - 1));
    uint64_t fullVirtualAddress = (virtualAddress >> OFFSET_WIDTH);

    cout << "input :" << virtualAddress << " offset : " << offset <<
         " address : " << fullVirtualAddress << endl;


    uint64_t rootFrame = 0;

    // Create address table
    uint64_t pagesAddresses[TABLES_DEPTH] = {0};

    for (int i = 0; i < TABLES_DEPTH ; i++)
    {
        pagesAddresses[(TABLES_DEPTH - 1) - i] =
                        (fullVirtualAddress >> OFFSET_WIDTH * i )
                                        % (2 << (OFFSET_WIDTH - 1));
    }


    word_t addr = 0;
    for (int j = 0; j < TABLES_DEPTH; j++)
    {
        word_t tmp_addr = addr;
        PMread((addr * PAGE_SIZE) + pagesAddresses[j], &addr);

        if (addr == 0)
        {
            // findEmptyFrame --> dfs + clearTable;
            word_t frameIndex = findEmptyFrame();
            PMwrite((tmp_addr * PAGE_SIZE) + pagesAddresses[j], frameIndex);
            tmp_addr = addr;
            addr = frameIndex;
        }

        cout << "addr : " << (int)addr << endl;
    }




    for (int k = 0; k < TABLES_DEPTH; ++k)
    {
        cout << k << " : " << pagesAddresses[k] << endl;
    }







}






// Example: input 70
//          70 >> 4 ( == 4) == 1000110 >> 4 == 100 ( == 4)

















