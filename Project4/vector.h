/*
    Simple vector to implement the link list.
*/

#ifndef VECTOR_H
#define VECTOR_H


#include "task.h"
struct node* head;
struct node* tail;

#define bool int
#define true 1
#define false 0


struct node {
    Task *task;
    struct node *next;
};
void addTohead(struct task* newTask);
void insert(struct node* p, struct task* newTask);
void addTotail(struct task* newTask);
bool isEmpty();
void deleteHead();
void moveTotail();
void delete(struct node*p);

#endif