#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/mman.h>
#include <sys/stat.h>
#include <errno.h>



#define bool int
#define true 1
#define false 0



#define OFFSET_BITS 8
#define PAGE_TABLE_ENTRIES 256
#define PAGE_SIZE 256
#define TLB_ENTRIES 16
#define FRAME_SIZE 256
#define FRAME_ENTRIES 256
#define MEM_SIZE (FRAME_ENTRIES * FRAME_SIZE)
#define ADDRESS_SIZE 80
#define STORE "BACKING_STORE.bin"



int page_table[PAGE_TABLE_ENTRIES];
int TLB[TLB_ENTRIES][2];
char memory[MEM_SIZE];

int mem_idx;
int tlb_pt;
bool tlb_full;


void initMemory()
{
    mem_idx = 0;
    printf("Memory inited.\n");
}

void initPageTable()
{
    for(int i = 0; i < PAGE_TABLE_ENTRIES; i++)
    {
        page_table[i] = -1;
    }
    printf("Page table inited.\n");
}


void initTLB()
{
    tlb_pt = 0;
    tlb_full = 0;
    for(int i = 0; i < TLB_ENTRIES; i++)
    {
        TLB[i][0] = -1;
        TLB[i][1] = -1;
    }
    printf("TLB inited.\n");
}






int getOffset(int vir)
{
    int mask = 255;
    return vir & mask;
}

int getPage(int vir)
{
    return vir >> OFFSET_BITS;
}

/* find whether the given page can be found in page table */
int requestPageTable(int page_num)
{
    return page_table[page_num];
}


int requestTLB(int page_num)
{
    if(tlb_full)
    {
        /* if TLB is full */
        for(int i = 0; i < TLB_ENTRIES; i++)
        {
            if(TLB[i][0] == page_num)
                return TLB[i][1];
        }
        return -1;
    }
    else
    {
        /* if TLB isn't full */
        for(int i = 0; i < tlb_pt; i++)
        {
            if(TLB[i][0] == page_num)
                return TLB[i][1];
        }
        return -1;
    }
}

void updateTLB(int page_num, int frame_num)
{
    TLB[tlb_pt][0] = page_num;
    TLB[tlb_pt][1] = frame_num;
    tlb_pt += 1;
    tlb_pt = tlb_pt % TLB_ENTRIES;
    if(tlb_pt == 0) 
        tlb_full = true;
}








int main(int argc, char* argv[])
{

    initMemory();

    initPageTable();

    initTLB();

    int tlb_hit = 0;
    int page_fault = 0;
    int address_cnt = 0;

    int page_num;
    int frame_num;
    int page_address; // denotes where the page is in backing storage.
    int offset;
    int physical;
    int value;
    char* out_file = "output.txt";
    char* in_file;
    FILE* in;
    FILE* out;
    if(argc != 2)
    {
        printf("ERROR INPUT!\n");
        exit(EXIT_FAILURE);
    }
    
    in_file = argv[1];
    in = fopen(in_file,"r");
    out = fopen(out_file,"a");
    /* STORE FILE */
    int store_f = open(STORE, O_RDONLY);
    char *store_data = mmap(0,MEM_SIZE, PROT_READ, MAP_SHARED, store_f, 0);


    
    char line[ADDRESS_SIZE];
    int address;
    while(fgets(line,80,in))
    {   
        
        address_cnt ++;
        address = atoi(line);
        page_num = getPage(address);
        offset = getOffset(address);
        frame_num = requestTLB(page_num);
        if(frame_num != -1)
        {
            tlb_hit++;
            physical = frame_num * FRAME_SIZE + offset;
            
            value = memory[physical];
        }
        else
        {
            frame_num = requestPageTable(page_num);
            if(frame_num != -1)
            {
                physical = frame_num * FRAME_SIZE + offset;
                value = memory[physical];
                updateTLB(page_num, frame_num);

            }
            else
            {
                page_address = page_num * PAGE_SIZE;
                page_fault++;
                if(mem_idx != -1)
                {
                    /* if there is still a free frame available */
                    memcpy(memory + mem_idx,store_data + page_address, PAGE_SIZE);
                    physical = mem_idx + offset;
                    value = memory[mem_idx + offset];
                    //printf("physical %d %d\n",physical,value);
                    page_table[page_num] = mem_idx >> OFFSET_BITS;
                    updateTLB(page_num,mem_idx >> OFFSET_BITS);
                    if(mem_idx < MEM_SIZE - FRAME_SIZE)
                    {
                        mem_idx += FRAME_SIZE;
                    }
                    else
                    {
                        /* the memory is used up */
                        mem_idx = -1;
                    }
                }

            }
        }
        
        fprintf(out,"Virtual address: %d ", address);
        fprintf(out,"Physical address: %d ", physical);
        fprintf(out,"Value: %d\n", value);

        

    }

        float fault_rate = (float) page_fault / (float) address_cnt;
        float hit_rate = (float) tlb_hit / (float) address_cnt;

        /* Print the statistics to the end of the output file. */
        fprintf(out, "Number of Translated Addresses = %d\n", address_cnt); 
        fprintf(out, "Page Faults = %d\n", page_fault);
        fprintf(out, "Page Fault Rate = %.3f\n", fault_rate);
        fprintf(out, "TLB Hits = %d\n", tlb_hit);
        fprintf(out, "TLB Hit Rate = %.3f\n", hit_rate);

        fclose(in);
        fclose(out);
        close(store_f);

}
