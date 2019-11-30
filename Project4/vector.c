/**
 * Vector operations
 */
 
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include "vector.h"
#include "task.h"



// delete the node after p
void delete(struct node* p)
{
    struct node* q = p->next;
    p->next = q->next;
    if(!q->next)    tail = p;   // when delete the tail
    free(q);
}


// add a new node to the head
void addTohead(struct task* newTask)
{
    struct node* newNode = malloc(sizeof(struct node));
    newNode->task = newTask;
    newNode->next = head->next;
    head->next = newNode;
    if(isEmpty()){
        tail = newNode;
        tail->next = NULL;
    }   
}


// add a new node to the tail
void addTotail(struct task* newTask)
{
    struct node* newNode = malloc(sizeof(struct node));
    newNode->task = newTask;
    newNode->next = NULL;

    tail->next = newNode;
    tail = newNode;

}

// add a new node after p
void insert(struct node* p, struct task* newTask)
{
    struct node* newNode = malloc(sizeof(struct node));
    newNode->task = newTask;
    struct node* tmp = p->next;
    p->next = newNode;
    newNode->next = tmp;
    if(!tmp)
        tail = newNode; //reset tail
}


// denote whether vector is empty
bool isEmpty()
{
    return head == tail;
}


// delete head node
void deleteHead()
{
    if(!isEmpty())
    {
        struct node* tmp = head->next;
        head->next = tmp->next;
        if(tail == tmp) tail = head;    // the vector is empty.
        free(tmp);
    }
}




// move the headnode to the tail
void moveTotail()
{
    if(!isEmpty() && head->next->next)  //have at least two elements
    {
        struct node* tmp = head->next;
        head->next = head->next->next;
        tail->next = tmp;
        tmp->next = NULL;
        tail = tmp;
    }
}



/*
// add a new node to the head
struct node* addTohead(struct node* head, struct task* newTask)
{
    struct node* newNode = malloc(sizeof(struct node));
    newNode->task = newTask;
    newNode->next = head;
    return newNode;
}

// add a new node after p
void insert(struct node* p, struct task* newTask)
{
    struct node* newNode = malloc(sizeof(struct node));
    newNode->task = newTask;
    struct node* tmp = p->next;
    p->next = newNode;
    newNode->next = tmp;
}

// delete head node
struct node* deleteHead(struct node* head)
{
    struct node* tmp = head->next;
    free(head);
    return tmp;
}
*/