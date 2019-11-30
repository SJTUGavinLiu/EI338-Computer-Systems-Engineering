#ifndef BUFFER_H
#define BUFFER_H

#define BUFFER_SIZE 5
typedef int buffer_item; 
int insert_item(buffer_item item);
int remove_item(buffer_item* item);

#endif