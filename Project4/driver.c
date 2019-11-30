#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define SIZE 100
#define DELIM ","
#include "task.h"
#include "scheduler.h"
#include "vector.h"



int main(int argc, char *argv[])
{
    head = malloc(sizeof(struct node));
    tail = head;
    head->next = NULL;

    FILE *in = fopen(argv[1],"r");
    char task[SIZE];
    char** args = malloc(5*sizeof(char*));
    char* token;
    char* tmp;
    int cnt = 0;
    while(fgets(task,SIZE,in))
    {
        cnt = 0;
        tmp = strdup(task);
        token = strsep(&tmp,DELIM);
        while(token)
        {
            args[cnt++] = token;
            token = strsep(&tmp,DELIM);
        }
        add(args[0],atoi(args[1]),atoi(args[2]));
        //printf("%s %d %d",args[0],atoi(args[1]),atoi(args[2]));
        free(tmp);
    }
    fclose(in);

    schedule();

}
