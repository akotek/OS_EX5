#include <bitset>
#include <iostream>
#include "VirtualMemory.h"
#include "PhysicalMemory.h"



void clearTable(uint64_t frameIndex)
{
    for (uint64_t i = 0; i < PAGE_SIZE; ++i)
    {
        PMwrite(frameIndex * PAGE_SIZE + i, 0);
    }
}

uint64_t distance(uint64_t a, uint64_t b)
{
    if(a>b){
        return a-b;
    }
    return b-a;
}

// frame - current traversed frame
// depth - current depth (?)
// max_frame - current max frame index
// zeros_frame - a frame with zeroes (empty) that we need to find
// zeros_father - the parent of the empty frame
// zeros_vpn - the virtual address of the empty frame's page
//

void find_empty_table(uint64_t frame, int depth, uint64_t &max_frame, uint64_t &zeros_frame, uint64_t &zeros_father,
                      uint64_t &zeros_vpn, uint64_t &max_dis_frame, uint64_t &dis_father,uint64_t &dis_vpn, uint64_t vpn,
                      uint64_t real_vpn,uint64_t &max_dis)
{

    int counter = 0;
    word_t value;
    // iterate through all rows of the current frame
    for (uint64_t j = 0; j < PAGE_SIZE; ++j)
    {

        PMread(frame * PAGE_SIZE + j, &value);

        if(depth == TABLES_DEPTH){
            uint64_t min=0;
            if (distance(real_vpn, vpn) > (NUM_PAGES) - distance(real_vpn, vpn))
            {
                min = (NUM_PAGES) - distance(real_vpn ,vpn);
            }
            else
            {
                min = distance(real_vpn, vpn);
            }
            if(min > max_dis){
                max_dis = min;
                max_dis_frame = frame;
                dis_vpn = vpn;
            }
            return;
        }

        if(value > max_frame){
            max_frame = value;
        }

        if(value == 0)
        {
            counter += 1;
        }
        else{

            uint64_t temp_vpn = (vpn << OFFSET_WIDTH) + j;
            find_empty_table(value , depth +1, max_frame, zeros_frame,zeros_father,zeros_vpn,max_dis_frame,dis_father,
                             dis_vpn, temp_vpn, real_vpn, max_dis);
            if(max_dis_frame == value){
                dis_father = frame;
            }
            if(zeros_frame == value){
                zeros_father = frame;
            }
        }
    }
    if(counter == PAGE_SIZE && (real_vpn >> (OFFSET_WIDTH*(TABLES_DEPTH-depth))) != vpn){
        zeros_frame = frame;
        zeros_vpn = vpn;
    }
}

uint64_t findUnusedFrame(uint64_t vpn)
{
    uint64_t max_frame = 0;
    uint64_t zeros_frame = 0;
    uint64_t zeros_father = 0;
    uint64_t max_dis_frame = 0;
    uint64_t dis_father = 0;
    uint64_t dis_vpn = 0;
    uint64_t zeros_vpn = 0;
    uint64_t max_dis = 0;

    find_empty_table(0,0,max_frame,zeros_frame,zeros_father,zeros_vpn ,max_dis_frame, dis_father,dis_vpn,0, vpn,max_dis);

    if(zeros_frame != 0 && max_frame +1 == NUM_FRAMES){
        uint64_t offset = zeros_vpn % (2 << (OFFSET_WIDTH-1));
        PMwrite(zeros_father*PAGE_SIZE + offset, 0);
        return zeros_frame;
    }
    if(max_frame +1 < NUM_FRAMES){
        return max_frame+1;
    }
    else{
        PMevict(max_dis_frame,dis_vpn);
        clearTable(max_dis_frame);
        uint64_t offset = dis_vpn % (2 << (OFFSET_WIDTH-1));
        PMwrite(dis_father*PAGE_SIZE + offset, 0);
        return max_dis_frame;
    }
}

int find_page(uint64_t root, uint64_t vpn, uint64_t &pAddr)
{
	uint64_t page_table[TABLES_DEPTH];
	uint64_t travelers = vpn;
	for (int i = 0; i < TABLES_DEPTH ; ++i)
	{
		page_table[TABLES_DEPTH - 1 - i] = (travelers % (2 << (OFFSET_WIDTH-1)));
		travelers = (travelers >> OFFSET_WIDTH);
	}

    word_t add1 = root ;
    word_t add2;

    for (int j = 0; j < TABLES_DEPTH; ++j)
    {

        uint64_t p = page_table[j];
        /** PMread(0 + 5, &addr1) // first translation */
        PMread((add1*PAGE_SIZE) + p, &add2);


        if (add2 == 0)
        {

            uint64_t f1 = findUnusedFrame(vpn);

            if(j+1 == TABLES_DEPTH){
                PMrestore(f1,vpn);
            }
            else
            {
                clearTable(f1);
            }
            PMwrite((add1*PAGE_SIZE) + p, f1);
            add2 = f1;
        }
        add1 = add2;
    }
    pAddr = add2;
	return add2;
}


void VMinitialize()
{
    //clear frame 0.
    clearTable(0);
}

/**
 *
 * @param virtualAddress
 * @param value-1
 * @return
 */
int VMread(uint64_t virtualAddress, word_t *value)
{
	//TODO  if virtualAddress not in 20 bit return 0
	
	uint64_t offset = virtualAddress % (2 << (OFFSET_WIDTH-1));
	uint64_t VPN = virtualAddress >> (OFFSET_WIDTH);
	uint64_t root_page = 0;
	uint64_t final_addr;
	int reval = find_page(root_page, VPN, final_addr);
	if (reval == -1)
	{
		return 0;
	}
    PMread(final_addr*PAGE_SIZE + offset, value);
    return 1;
}

int VMwrite(uint64_t virtualAddress, word_t value)
{
	uint64_t offset = virtualAddress % (2 << (OFFSET_WIDTH -1));
	uint64_t VPN = virtualAddress >> (OFFSET_WIDTH);
	uint64_t root_page = 0;
	uint64_t final_addr;
	int reval = find_page(root_page, VPN, final_addr);
	if (reval == -1)
	{
		return 0;
	}
    PMwrite(final_addr*PAGE_SIZE + offset, value);
	return 1;
}

