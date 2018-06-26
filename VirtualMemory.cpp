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



void dfs();


// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~



void createAddressTable(uint64_t fullVirtualAddress, uint64_t
pagesAddresses[TABLES_DEPTH])
{
    for (int i = 0; i < TABLES_DEPTH ; i++)
    {
        pagesAddresses[(TABLES_DEPTH - 1) - i] =
                (fullVirtualAddress >> OFFSET_WIDTH * i )
                % (2 << (OFFSET_WIDTH - 1));
    }
}



void dfs(uint64_t currentFrame)
{
    word_t value;

    for(int i = 0; i < PAGE_SIZE; i++)
    {
        PMread(currentFrame * PAGE_SIZE + i, &value);





    }



}

// DFS recursive implementation
uint64_t findEmptyFrame(uint64_t fullVirtualAddress)
{
    uint64_t root = 0;
    uint64_t maxFrameVisited = 0;
    uint64_t maxDistance = 0;
    uint64_t targetFrame = 0;
    uint64_t targetFrameParent = 0;
    uint64_t maxDistanceFrame = 0; // TODO: Understand why to use distance
    uint64_t distanceParent = 0; // TODO: As well
    uint64_t maxDistanceVirtualAddress = 0; // TODO: As well
    uint64_t targetVirtualAddress = 0;

//    dfs(); // TODO: Put all the fucking values

    if ((maxFrameVisited + 1) < NUM_FRAMES)
    {
        return maxFrameVisited + 1;
    }

    else if (targetFrame > 0 && (maxFrameVisited + 1) == NUM_FRAMES)
    {
        // TODO: func - helper - getOffset
        uint64_t offset = targetVirtualAddress % (2 << (OFFSET_WIDTH - 1));
        // TODO: split to func - write to parent
        PMwrite(targetFrameParent * PAGE_SIZE + offset, 0);
        return targetFrame;
    }
    else
    {
        PMevict(maxDistanceFrame, maxDistanceVirtualAddress);
        clearTable(maxDistanceFrame);
        uint64_t offset = maxDistanceVirtualAddress % (2 << (OFFSET_WIDTH - 1));
        PMwrite(distanceParent * PAGE_SIZE + offset, 0);
        return maxDistanceFrame;
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

    uint64_t pagesAddresses[TABLES_DEPTH] = {0};

    createAddressTable(fullVirtualAddress, pagesAddresses);



    word_t addr1 = rootFrame;
    word_t addr2;

    for (int j = 0; j < TABLES_DEPTH; j++)
    {
        PMread((addr1 * PAGE_SIZE) + pagesAddresses[j], &addr2);

        if (addr2 == 0)
        {
            // findEmptyFrame --> dfs + clearTable;
            word_t frameIndex = findEmptyFrame(fullVirtualAddress); // f1 - frameIndex1

            if(j+1 < TABLES_DEPTH)
            {
                clearTable(frameIndex);
            }
            else
            {
                PMrestore(frameIndex, fullVirtualAddress);
            }

            PMwrite((addr1 * PAGE_SIZE) + pagesAddresses[j], frameIndex);
            addr2 = frameIndex;
        }
        addr1 = addr2;
        cout << "addr1 : " << (int)addr1 << endl;
    }

    if(addr2 < 0)
    {
        return 0; // fail
    }
    PMwrite(addr2 * PAGE_SIZE + offset, value);
    return 1;

    // address held at addr2
}






// Example: input 70
//          70 >> 4 ( == 4) == 1000110 >> 4 == 100 ( == 4)

















