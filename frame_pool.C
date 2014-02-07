#include "frame_pool.H"

FramePool::FramePool(unsigned long _base_frame_no,
                    unsigned long _nframes,
                    unsigned long _info_frame_no)
{
    base_frame_number        = _base_frame_no;
    number_of_frames_managed = _nframes;
    managament_frame_number  = _info_frame_no;

    /* 
     * Add the physical addresses of free frames to the frame pool list. 
     * The get_frame routine will allocate frames from this list
     */
   unsigned long frame_address = _base_frame_no * (FramePool::FRAME_SIZE);
   do
   {
       free_frame_list.push_back(frame_address);
       frame_address = frame_address + FramePool::FRAME_SIZE;
   }while (frame_address < (_base_frame_no + _nframes * (FramePool::FRAME_SIZE)));
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
