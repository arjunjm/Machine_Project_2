#include "frame_pool.H"


frame_element_t FramePool::free_frame_list[TOTAL_POOL_SIZE];
unsigned int FramePool::number_of_free_frames;

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
   unsigned long frame_number = _base_frame_no;

   for (int i = base_frame_number - START_OFFSET; i < (base_frame_number - START_OFFSET + number_of_frames_managed); i++)
   {
       free_frame_list[i].frame_number = frame_number;
       free_frame_list[i].can_be_allocated  = true;
       frame_number                     += 1;
       number_of_free_frames            += 1;
   }
}

unsigned long FramePool::get_frame()
{
   /*
    * This function will traverse through the array of structures and 
    * returns the first frame which is accessible and has not been 
    * allocated to any other process.
    */
    unsigned long frame_number = 0;
    bool found_frame = false;
    for (int i = base_frame_number - START_OFFSET ; i < (base_frame_number - START_OFFSET + number_of_frames_managed); i++)
    {
        if (free_frame_list[i].can_be_allocated)
        {
            frame_number = free_frame_list[i].frame_number;
            free_frame_list[i].can_be_allocated = false;
            found_frame = true;
            number_of_free_frames--;
            break;
        }
    }
    if (found_frame)
        return frame_number;
    else
        return 0;
}

void FramePool::mark_inaccessible(unsigned long _base_frame_no,
                                  unsigned long _nframes)
{
    int index_of_frame = _base_frame_no - START_OFFSET;
    for (int i = index_of_frame; i < index_of_frame + _nframes; i++)
    {
        free_frame_list[i].can_be_allocated = false;
    }
}

void FramePool::release_frame(unsigned long _frame_no)
{
    int index_of_frame = _frame_no - START_OFFSET;
    free_frame_list[index_of_frame].can_be_allocated = true;
    number_of_free_frames++;
}
