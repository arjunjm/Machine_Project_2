#include "page_table.H"

PageTable::PageTable()
{
    /* Any 4k-aligned address can be used for the page_directory */
    page_directory = (unsigned long*) 0X9C000;

    /* The page table follows the page_directory at the next 4k-aligned address */
    page_table     = (unsigned long*) 0X9D000;

    unsigned long address = 0;

    /* The following is to map the first 4 MB of memory */

    for (int i = 0; i < ENTRIES_PER_PAGE; i++)
    {
        /* The last 3 bits stand for superviser/user, read/write and present/non-present status respectively.
         * We set the these attribute bits to superviser, read/write and present */
        page_table[i] = address | 3;
        address       = address + 4096;
    }

    /* The page table initialized above is stored in the page directory */
    page_directory[0] = page_table;

    /* We then set the attribute bits to superviser, read/write and present */
    page_directory[0] |= 3;

    /* The rest of the entries in the page_directory have to be filled now.
     * Since the page tables in these entries have not been created yet, we just store zeros here. 
     * Also, we mark the attribute sets' present status to 0.
     * Hence we do a bitwise OR by 2
     */
    for (int i = 1; i < ENTRIES_PER_PAGE; i++)
    {
        page_directory[1] = 0 | 2;
    }

}

void PageTable::init_paging(FramePool * _kernel_mem_pool,
                            FramePool * _process_mem_pool,
                            const unsigned long _shared_size)
{
    kernel_mem_pool  = _kernel_mem_pool;
    process_mem_pool = _process_mem_pool;
    shared_size      = _shared_size;
}

void PageTable::load()
{
    /*
     * Page table loaded into processor context
     * by loading the page directory address into 
     * the CR3 register
     */
    write_cr3(page_directory);
}

void PageTable::enable_paging()
{
    /*
     * Write 1 to the 31st bit of CR0 register
     * to enable paging
     */
    paging_enabled = 1;
    write_cr0(read_cr0() | 0x80000000);
}

void PageTable::handle_fault(REGS * _r)
{
    /* Page fault handler */

}
