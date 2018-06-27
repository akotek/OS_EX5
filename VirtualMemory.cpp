#include "VirtualMemory.h"
#include "PhysicalMemory.h"
#include <iostream>
#include <cmath>

using namespace std;



// Helper Functions
void clearTable(uint64_t frameIndex) {
    for (uint64_t i = 0; i < PAGE_SIZE; ++i) {
        PMwrite(frameIndex * PAGE_SIZE + i, 0);
    }
}

void createAddressTable(uint64_t fullVirtualAddress, uint64_t
pagesAddresses[TABLES_DEPTH]);




// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~


uint64_t getOffset(uint64_t address)
{
    return address % (2 << (OFFSET_WIDTH - 1));
}


void createAddressTable(uint64_t fullVirtualAddress, uint64_t
pagesAddresses[TABLES_DEPTH])
{
    for (int i = 0; i < TABLES_DEPTH ; i++)
    {
        pagesAddresses[(TABLES_DEPTH - 1) - i] =
                getOffset(fullVirtualAddress >> OFFSET_WIDTH * i);
    }
}




// ++++++++++++++++++++++++ TO CHANGE!!! +++++++++++++++++++++++++++

void dfs(
        uint64_t fullVirtualAddress, uint64_t virtualAddress,
        uint64_t currentFrame, uint64_t currentDepth,
         uint64_t &maxFrameVisited, uint64_t &targetFrame,
         uint64_t &targetFrameParent,uint64_t &targetVirtualAddress,
         uint64_t &maxDistanceFrame, uint64_t &distanceParent,
         uint64_t &maxDistanceVirtualAddress,
         uint64_t &maxDistance)
{

    int emptySlotCounter = 0;
    word_t rowValue;

    // iterate through all rows of the current currentFrame
    for (uint64_t i = 0; i < PAGE_SIZE; i++)
    {
        // read current row's rowValue
        PMread(currentFrame * PAGE_SIZE + i, &rowValue);

        // if current row has no reference value, increase counter
        if(rowValue == 0) emptySlotCounter++;
        // if we reach a new maximal frame index, assign it

        if(rowValue > maxFrameVisited)
        {
            maxFrameVisited = rowValue;
        }



        // if we reached the end of the pm
        if(currentDepth == TABLES_DEPTH)
        {
            // calculate cyclical distance as shown in the pdf
            uint64_t cyclicalDistance = (uint64_t)min(NUM_PAGES -
                                  abs(virtualAddress - fullVirtualAddress),
                                  abs(virtualAddress - fullVirtualAddress));

            // if cyclicalDistance is maximal, swap the frame before use
            if(cyclicalDistance > maxDistance)
            {
                maxDistance = cyclicalDistance;
                maxDistanceFrame = currentFrame;
                maxDistanceVirtualAddress = fullVirtualAddress;
            }
            return;
        }

            // if row not empty, enter it's reference recursively
        else
        {
            dfs(((fullVirtualAddress << OFFSET_WIDTH) + i) , virtualAddress,
                rowValue , currentDepth +1,
                maxFrameVisited, targetFrame,
                targetFrameParent, targetVirtualAddress,
                maxDistanceFrame, distanceParent,
                maxDistanceVirtualAddress,
                maxDistance);

            if(maxDistanceFrame == rowValue)
            {
                distanceParent = currentFrame;
            }

            if(targetFrame == rowValue)
            {
                targetFrameParent = currentFrame;
            }
        }
    }
    // if we reached the end of the current frame and it's empty
    if(emptySlotCounter == PAGE_SIZE &&
            (virtualAddress >> (OFFSET_WIDTH * (TABLES_DEPTH - currentDepth)))
            != fullVirtualAddress){
        targetVirtualAddress = fullVirtualAddress;
        targetFrame = currentFrame;
    }
}
// ++++++++++++++++++++++++ END TO CHANGE +++++++++++++++++++++++++++



// DFS recursive implementation
uint64_t findEmptyFrame(uint64_t fullVirtualAddress)
{

// ++++++++++++++++++++++++ TO CHANGE!!! +++++++++++++++++++++++++++
    uint64_t targetFrame = 0, targetFrameParent = 0, maxDistance = 0,
            maxFrameVisited = 0, maxDistanceFrame = 0, distanceParent = 0,
            targetVirtualAddress = 0, maxDistanceVirtualAddress = 0;

    // traverse through the pages' tree according to the algorithm in the pdf
    dfs(0, fullVirtualAddress,
        0, 0,
        maxFrameVisited, targetFrame,
        targetFrameParent, targetVirtualAddress ,
        maxDistanceFrame, distanceParent,
        maxDistanceVirtualAddress,
        maxDistance);

    // Flow according to the shown algorithm, ordered by priority:
    if ((maxFrameVisited + 1) < NUM_FRAMES)
    {
        return maxFrameVisited + 1;
    }
    else if (targetFrame > 0 && (maxFrameVisited + 1) == NUM_FRAMES)
    {
        uint64_t offset = getOffset(targetVirtualAddress);
        PMwrite(targetFrameParent * PAGE_SIZE + offset, 0);
        return targetFrame;
    }
    else
    {
        PMevict(maxDistanceFrame, maxDistanceVirtualAddress);
        clearTable(maxDistanceFrame);
        uint64_t offset = getOffset(maxDistanceVirtualAddress);
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
    // separate the offset and actual pages' virtual addresses sequence
    uint64_t offset = getOffset(virtualAddress);
    uint64_t fullVirtualAddress = (virtualAddress >> OFFSET_WIDTH);

    std::cout << "input :" << virtualAddress << " offset : " << offset <<
         " address : " << fullVirtualAddress << std::endl;

    // initialize and set pages' addresses table
    uint64_t pagesAddresses[TABLES_DEPTH] = {0};
    createAddressTable(fullVirtualAddress, pagesAddresses);

    // set traverse values
    uint64_t rootFrame = 0;
    word_t addr1 = rootFrame;
    word_t addr2;

    // iterate through the addresses as the algorithm showed in the docs
    for (int i = 0; i < TABLES_DEPTH; i++)
    {
        PMread((addr1 * PAGE_SIZE) + pagesAddresses[i], &addr2);

        if (addr2 == 0)
        {
            // findEmptyFrame --> dfs + clearTable;
            uint64_t frameIndex = findEmptyFrame(fullVirtualAddress); // f1 -
            // frameIndex1

            if(i+1 < TABLES_DEPTH)
            {
                clearTable(frameIndex);
            }
            else
            {
                PMrestore(frameIndex, fullVirtualAddress);
            }

            PMwrite((addr1 * PAGE_SIZE) + pagesAddresses[i], frameIndex);
            addr2 = frameIndex;
        }
        addr1 = addr2;
        std::cout << "addr1 : " << (int)addr1 << std::endl;
    }

    if(addr2 < 0) return 0;
    PMwrite(addr2 * PAGE_SIZE + offset, value);
    return 1;
}






// Example: input 70
//          70 >> 4 ( == 4) == 1000110 >> 4 == 100 ( == 4)

















