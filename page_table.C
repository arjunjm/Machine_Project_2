#include "page_table.H"

PageTable::PageTable()
{
    unsigned long *page_directory = (unsigned long*) 0X9C000;

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
    write_cr0(read_cr0() | 0x80000000);
}

void PageTable::handle_fault(REGS * _r)
{
    /* Page fault handler */

}
