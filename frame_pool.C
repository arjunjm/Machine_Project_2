#include "frame_pool.H"
#include <stdlib.h>

FramePool::FramePool(unsigned long _base_frame_no,
                     unsigned long _nframes,
                     unsigned long _info_frame_no)
{
    number_of_free_frames    = 0;
    base_frame_number        = _base_frame_no;
    number_of_frames_managed = _nframes;
    managament_frame_number  = _info_frame_no;

    /* 
     * Add the physical addresses of free frames to the frame pool list. 
     * The get_frame routine will allocate frames from this list
     */
   unsigned long frame_address = _base_frame_no * (FramePool::FRAME_SIZE);

   for (int i = 0; i < number_of_frames_managed; i++)
   {
       free_frame_list[i].frame_address = frame_address;
       free_frame_list[i].is_accessible = true;
       free_frame_list[i].is_allocated  = false;
       frame_address                    = frame_address + FramePool::FRAME_SIZE;
       number_of_free_frames            += 1;
   }
}

unsigned long FramePool::get_frame()
{
    
}

void FramePool::mark_inaccessible(unsigned long _base_frame_no,
                                  unsigned long _nframes)
{

}

void FramePool::release_frame(unsigned long _frame_no)
{

}
