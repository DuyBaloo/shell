#include <sys/wait.h>
#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

int i;
// void cd(char directory[10])
// {
//     char s[100]; 

//     if(isEmpty(directory))
//     {
//         // printing current working directory 
//         printf("%s\n", getcwd(s, 100));
//     }
//     else
//     {
//         chdir(directory);
//         printf("%s\n", getcwd(s, 100));
//     }
// }

void clr()
{
    for(i = 0; i < 40; i++)
    {
        puts("");
    }
}

void dir (char directory)
{
    //this function takes in a directory and prints out what's in It
}

void environ()
{
    //list all environment strings
}

void echo(char *comment)
{
    printf("%s", comment);
}

void help()
{
    //display a help page with more filter
}

void quit()
{
    puts("Quitting Shell...");
    exit(0);
}

void display_prompt()
{
    //this function will clear out the whole screen and print out 
    //prompt for user to start putting in inputs
    printf("%s", "myshell>");
}

void read_command(char command[], char *para[])
{
    //this function will read one line then tokenize the whole line
    //put the first word to put in cmd to execute the command
    //the rest will be put in para to serve as parameters for that command
    char *line;
    size_t bufsize = 0;
    char *tokens;
    
    getline(&line, &bufsize, stdin);
    tokens = strtok(line, " \n");

    while(tokens != NULL)
    {
        printf("%s\n", tokens);
        tokens = strtok(NULL, " \n");
    }
}

void print_error()
{
    //this function will print out err msg as given in instruction
    char error_message[30] = "An error has occurred\n";
    write(STDERR_FILENO, error_message, strlen(error_message));
}

// void createProcess()
// {
//     pid_t pid = fork();
//     if(pid < 0)
//     {
//         perror("Fork failed.");
//     }
//     if(pid == 0)
//     {
//         printf("");
//     }
// }
// int isEmpty(char *head)
// {
//     if((*head) == NULL)
//     {
//         return 1;
//     }
// }

int main()
{
    char cmd[100], command[100], *para[10];
    clr();
    while(1)
    {
        display_prompt();
        read_command(command, para);
        printf("%s %s", command, para);
        // if(fork() != 0)
        //     wait(NULL);
        // else
        // {
        //     strcpy(cmd, "/bin/");
        //     strcat(cmd, command);
        //     execvp(cmd, para);

            
        // }
        if(command == "exit")
        {
            printf("%s invoked.", command);
            quit();
        }
        

    }
}