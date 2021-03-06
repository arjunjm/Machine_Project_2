/*
    File: kernel.C

    Author: R. Bettati
            Department of Computer Science
            Texas A&M University
    Date  : 09/03/05


    This file has the main entry point to the operating system.

*/


/*--------------------------------------------------------------------------*/
/* DEFINES */
/*--------------------------------------------------------------------------*/

#define MB * (0x1 << 20)
#define KB * (0x1 << 10)
#define KERNEL_POOL_START_FRAME ((2 MB) / (4 KB))
#define KERNEL_POOL_SIZE ((2 MB) / (4 KB))
#define PROCESS_POOL_START_FRAME ((4 MB) / (4 KB))
#define PROCESS_POOL_SIZE ((28 MB) / (4 KB))
/* definition of the kernel and process memory pools */

#define MEM_HOLE_START_FRAME ((15 MB) / (4 KB))
#define MEM_HOLE_SIZE ((1 MB) / (4 KB))
/* we have a 1 MB hole in physical memory starting at address 15 MB */

#define FAULT_ADDR (4 MB)
/* used in the code later as address referenced to cause page faults. */
#define NACCESS ((16 MB) / 4)
/* NACCESS integer access (i.e. 4 bytes in each access) are made starting at address FAULT_ADDR */

/*--------------------------------------------------------------------------*/
/* INCLUDES */
/*--------------------------------------------------------------------------*/

#include "console.H"
#include "gdt.H"
#include "idt.H"
#include "irq.H"
#include "exceptions.H"
#include "interrupts.H"

#include "simple_timer.H"

#include "page_table.H"
#include "paging_low.H"


/*--------------------------------------------------------------------------*/
/* EXCEPTION HANDLERS */
/*--------------------------------------------------------------------------*/

/* -- EXAMPLE OF THE DIVISION-BY-ZERO HANDLER */

void dbz_handler(REGS * r) {
  Console::puts("DIVISION BY ZERO\n");
  for(;;);
}


/*--------------------------------------------------------------------------*/
/* MAIN ENTRY INTO THE OS */
/*--------------------------------------------------------------------------*/

int main() {

    GDT::init();
    Console::init();
    IDT::init();
    init_exception_dispatcher();
    IRQ::init();
    init_interrupt_dispatcher();


    /* -- EXAMPLE OF AN EXCEPTION HANDLER -- */

    register_exception_handler(0, dbz_handler);

    register_exception_handler(14, PageTable::handle_fault);


    /* -- INITIALIZE FRAME POOLS -- */
    FramePool kernel_mem_pool(KERNEL_POOL_START_FRAME,
                              KERNEL_POOL_SIZE,
                              0);
    unsigned long process_mem_pool_info_frame = kernel_mem_pool.get_frame();

    FramePool process_mem_pool(PROCESS_POOL_START_FRAME,
                               PROCESS_POOL_SIZE,
                               process_mem_pool_info_frame);

    process_mem_pool.mark_inaccessible(MEM_HOLE_START_FRAME, MEM_HOLE_SIZE);

    /* -- INITIALIZE MEMORY (PAGING) -- */
    PageTable::init_paging(&kernel_mem_pool,
                           &process_mem_pool,
                           4 MB);

    PageTable pt;

    pt.load();

    PageTable::enable_paging();

    /* -- INITIALIZE THE TIMER (we use a very simple timer).-- */
    SimpleTimer::init(100); /* timer ticks every 10ms. */
    register_interrupt_handler(0, SimpleTimer::handler);

    /* NOTE: The timer chip starts periodically firing as
             soon as we enable interrupts.
             It is important to install a timer handler, as we
             would get a lot of uncaptured interrupts otherwise. */

    /* -- ENABLE INTERRUPTS -- */

    __asm__ __volatile__ ("sti");

    /* -- MOST OF WHAT WE NEED IS SETUP. THE KERNEL CAN START. */

    Console::puts("Hello World!\n");

    /* -- GENERATE MEMORY REFERENCES */

    int *foo = (int *) FAULT_ADDR;
    int i;

    for (i=0; i<NACCESS; i++) {
       foo[i] = i;
    }

    for (i=0; i<NACCESS; i++) {
       if(foo[i] != i) {
          Console::puts("TEST FAILED for access number:");
          Console::putui(i);
          Console::puts("\n");
          break;
       }
    }
    if(i == NACCESS) {
       Console::puts("TEST PASSED\n");
    }
    for(;;);
}
