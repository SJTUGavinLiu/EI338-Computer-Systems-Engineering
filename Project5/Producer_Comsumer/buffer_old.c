#include "buffer.h"


buffer_item buffer[BUFFER_SIZE];
int tail = 0;


/* 
    insert item into buffer 
    return 0 if successful, 
    otherwise return -1 indicating an error condition 
*/

int insert_item(buffer_item item)
{
    if(tail < BUFFER_SIZE){
        buffer[tail++] = item;
        return 0;
    }
    else{
        return -1;
    }
}




/* 
    remove item from buffer 
    return 0 if successful, 
    otherwise return -1 indicating an error condition 
*/


int remove_item(buffer_item* item)
{
    if(tail > 0){
        * item = buffer[--tail];
        return 0;
    }
    else{
        return -1;
    }
}



