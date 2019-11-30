#include "buffer.h"


buffer_item buffer[BUFFER_SIZE];
int head = 0;
int tail = 0;


/* 
    insert item into buffer 
    return 0 if successful, 
    otherwise return -1 indicating an error condition 
*/

int insert_item(buffer_item item)
{
    if(head != (tail + 1) % BUFFER_SIZE){
        buffer[tail++] = item;
        tail %= BUFFER_SIZE;
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
    if(head != tail){
        * item = buffer[head++];
        head %= BUFFER_SIZE;
        return 0;
    }
    else{
        return -1;
    }
}