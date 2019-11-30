#include <sys/types.h>
#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <stdlib.h>
#include <fcntl.h>
#include <sys/stat.h>

#define false 0
#define true 1
#define MAX_LINE 80     /* the max length of the command */
#define MAX_ARG 40
#define ARG_DELIM " \t\r\n\a"
#define MAX_HISTORY 5

/* global variable to check parent and child process concurrency */
int bg = false;
char histories[MAX_HISTORY][MAX_LINE];
int command_count = 0;

/* split the commad into arguments */
char **command_split(char * cmd)
{
    char **args = malloc(MAX_ARG * sizeof(char*));
    if(!args){
        fprintf(stderr, "allocation error\n");
        exit(-2);
    }

    char *arg = strtok(cmd, ARG_DELIM);
    int cnt = 0;
    while(arg)
    {
        args[cnt++] = arg;
        arg = strtok(NULL, ARG_DELIM);
    }
    
    if(strcmp(args[cnt-1], "&") == 0)
    {
        bg = true;
        args[cnt - 1] = NULL;
    }
    args[cnt] = NULL;
    return args;
}



int redirect_output(char ** args)
{
    pid_t pid;

    pid = fork();
    if(pid == 0){ /* child process */
        char ** run_arg = malloc(MAX_LINE * sizeof(char*));
        int i=0;
        while (true)
        {
            if (strcmp(args[i], ">")==0) break;
            run_arg[i] = args[i];
            i++;
        }
        char * output = args[i+1];

        int out = open(output, O_WRONLY | O_TRUNC | O_CREAT, S_IRUSR | S_IRGRP | S_IWGRP | S_IWUSR);
        dup2(out, 1);
        close(out);
        if(execvp(run_arg[0], run_arg) == -1) perror("lsh");
        free(run_arg);
        exit(EXIT_FAILURE);

    }
    else if(pid > 0){ /* parent process */
        if (bg == 0) /* handle parent,wait for child */
			while (pid != wait(NULL)) ;
    }else{ /* error forking */
        perror("error");
    }
   
    bg = 0;
    return 1;
}
int redirect_input(char **args)
{
    pid_t pid;

    pid = fork();
    if(pid == 0){ /* child process */
        char ** run_arg = malloc(MAX_LINE * sizeof(char*));
        int i=0;
        while (true)
        {
            if (strcmp(args[i], "<")==0) break;
            run_arg[i] = args[i];
            i++;
        }
        char * input = args[i+1];

        int in = open(input, O_RDONLY);
        dup2(in, 0);
        close(in);
        if(execvp(run_arg[0], run_arg) == -1) perror("error");
        free(run_arg);
        exit(EXIT_FAILURE);

    }
    else if(pid > 0){ /* parent process */
        if (bg == 0) /* handle parent,wait for child */
			while (pid != wait(NULL)) ;
    }else{ /* error forking */
        perror("error");
    }
   
    bg = 0;
    return 1;
}

int pipe_(char **args)
{
    
    pid_t pid;

    pid = fork();
    if(pid == 0){ /* child process */
        char ** pipe_source = malloc(MAX_LINE * sizeof(char*));
        char ** pipe_des = malloc(MAX_LINE * sizeof(char*));
        int i=0;
        
        while (true)
        {
            if (strcmp(args[i], "|")==0) break;
            pipe_source[i] = args[i];
            i++;
        }
        pipe_source[i] = NULL;
        int tmp = ++i;
        while (true)
        {
            if (args[i] == NULL) break;
            pipe_des[i-tmp] = args[i];
            i++;
        }
        pipe_des[i-tmp+1] = NULL;
        


        int pipefd[2];
        pid_t child;
        pipe(pipefd);
        child = fork();
        if(child == 0)
        {
            dup2(pipefd[0], 0);
            close(pipefd[1]);
            execvp(pipe_des[0], pipe_des);
        }
        else
        {
            dup2(pipefd[1], 1);

            close(pipefd[0]);

            execvp(pipe_source[0], pipe_source);
        }
    }
    else if(pid > 0){ /* parent process */
        if (bg == 0) /* handle parent,wait for child */
			while (pid != wait(NULL)) ;
    }else{ /* error forking */
        perror("error");
    }
   
    bg = 0;
    return 1;
}


int launch(char **args)
{
    pid_t pid;

    pid = fork();
    if(pid == 0){ /* child process */
        if(execvp(args[0], args) == -1) perror("error");
        exit(-2);
    }
    else if(pid > 0){ /* parent process */
        if (bg == 0) /* handle parent,wait for child */
			while (pid != wait(NULL)) ;
    }else{ /* error forking */
        perror("error");
    }

    bg = false;
    return 1;
}


int history(char **args)
{
    if(command_count == 0){
        fprintf(stderr, "No commands in history\n");
        exit(-1);
    }
    if(strcmp(args[0], "history") == 0){
        
        if(command_count <= 5){
            int cnt = 1;
            for(int i = command_count - 1; i >= 0; i--)
            {
                printf("%d %s\n", cnt++, histories[i]);
            }
        }
        else{
            int cnt = command_count - 1;
            for(int i = 0; i < 5; i++)
            {
                int idx = cnt % MAX_HISTORY;
                printf("%d %s\n", i+1, histories[idx]);
                cnt--;
            }
        }
        return 1;
        
    }
    else
    {
        char *cmd;
        if(strcmp(args[0],"!!") == 0) {
            //cmd = malloc(sizeof(histories[(command_count-1) % MAX_HISTORY]));
            //strcat(cmd, histories[(command_count-1) % MAX_HISTORY]);
            return execute(histories[(command_count-1) % MAX_HISTORY]);
        }
        else if(args[0][0] == '!'){
            if(args[0][1] == '\0'){
                fprintf(stderr, "Expected arguments for \"!\"\n");
                exit(-1);
            }
            else{
                int pos = args[0][1] - '0';
                return execute(histories[(command_count-pos) % MAX_HISTORY]);
            }
        }
        
    }
    
    return 1;

}




char* read_command()
{
    char *buffer = malloc(sizeof(char) * MAX_LINE);
    
    do {
        printf("osh>");
        fflush(stdout);
        read(STDIN_FILENO, buffer,MAX_LINE); 
    }while (buffer[0] == '\n');
    
    return buffer;
}






int execute(char *cmd)
{
    char* cmd_tmp = malloc(MAX_LINE * sizeof(char));
    strcpy(cmd_tmp, cmd);

    char** args = command_split(cmd);
    //char **args = lsh_split_line(cmd);
    //printf(args[0]);
    //printf(args[1]);
    if(strncmp(cmd, "history", 7) == 0 || strncmp(cmd, "!!", 2) == 0 || strncmp(cmd, "!", 1) == 0)
        return history(args);
    else if(strncmp(cmd, "exit", 4) == 0)
        return 0;

    strcpy(histories[command_count % MAX_HISTORY],cmd_tmp);
    free(cmd_tmp);
    command_count++;

    int i = 0;


    
    while (true)
    {
        if (strcmp(args[i], ">") == 0)
            return redirect_output(args);
        else if (strcmp(args[i], "<") == 0)
            return redirect_input(args);
        else if (strcmp(args[i], "|") == 0)
            return pipe_(args);
        i++;
        if (args[i] == NULL) break;
    }


    return launch(args);

}





int main()
{
    char *command;
    int status;

    do {
        command = read_command();
        status = execute(command);
        free(command);

    }while(status);
    return 0;
}
