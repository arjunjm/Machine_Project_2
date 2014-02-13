#include "frame_pool.H"
#include "utils.H"
#include "console.H"

#define IS_SET(var, pos) ((var) & (1<<(pos)))
#define SET_BIT(var, pos) ((var) |= (1<<(pos)))
#define TOGGLE_BIT(var, pos) ((var) ^= (1<<(pos)))

unsigned long* FramePool::frame_map;

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
   if (_info_frame_no == 0)
   {
       /*
        * We initialize the bit map pointer here. Since it is static, 
        * it will be accessible to both the frame pools.
        */
        frame_map = reinterpret_cast <unsigned long*> (base_frame_number * FramePool::FRAME_SIZE);
        get_frame();
        memset(frame_map, 0, FramePool::FRAME_SIZE);
   }
}

unsigned long FramePool::get_frame()
{
   /*
    * This function will traverse through the array of structures and 
    * returns the first frame which is accessible and has not been 
    * allocated to any other process.
    */
    //Console::puts("Inside get_frame");
    //Console::putui(base_frame_number);
    //Console::puts(", start offset :");
    //Console::putui(START_OFFSET);
    int bit_map_index = (base_frame_number - START_OFFSET) / 32;
    bool frame_found = false;
    int i, j;
    unsigned long frame_number;

    for ( i = bit_map_index ; i <= (base_frame_number - START_OFFSET + number_of_frames_managed) / 32; i++)
    {
        if (frame_map[i] != 0xFFFF) 
        /* This check is made to check if there are any free frames. 
         * If the map is set to all ones, this would mean there are no free frames 
         * in this bitmap array
         */
        {
            for ( j = 0; j <= 31; j++ )
            {
                if (IS_SET(frame_map[i], j))
                    continue;
                else 
                {
                    frame_map[i] = TOGGLE_BIT(frame_map[i], j);
                    frame_found = true;
                    break;
                }
            }
            if (frame_found)
                break;
        }
    }
    if (frame_found)
    {
        frame_number = j + i*32 + START_OFFSET;
        return frame_number;
    }
    else
    {
        return 0;
    }
}

void FramePool::mark_inaccessible(unsigned long _base_frame_no,
                                  unsigned long _nframes)
{
    int index_of_frame = (_base_frame_no - START_OFFSET) / 32;
    for (int i = index_of_frame; i < (_base_frame_no - START_OFFSET + _nframes) / 32 ; i++)
    {
        //for (int j = 0; j <= 31; j++)
            //frame_map[i] = SET_BIT(frame_map[i], j);
            frame_map[i] = 0xFFFF;
    }
}

void FramePool::release_frame(unsigned long _frame_no)
{
    int bit_position = _frame_no % 32;
    int index_of_frame = (_frame_no - START_OFFSET) / 32;
    if (IS_SET (frame_map[index_of_frame], bit_position))
        frame_map[index_of_frame] = TOGGLE_BIT(frame_map[index_of_frame], bit_position);
}
