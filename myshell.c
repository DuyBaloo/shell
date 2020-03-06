#include <sys/wait.h>
#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

int cd(char **args)
{
    if(args[1] == NULL)
    {
        puts("Please input argument for cd.");
    }
    else
    {
        if(chdir(args[1]) != 0)
        {
            print_error();
        }
    }
    return 1;
    
}

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

char *read_command()
{
    //this function will read one line then tokenize the whole line
    //put the first word to put in cmd to execute the command
    //the rest will be put in para to serve as parameters for that command
    char *line;
    size_t bufsize = 0;
    
    getline(&line, &bufsize, stdin);
    
    return line;

}

char **parse_command(char *line)
{
    int buffer = 64, i = 0;
    char **tokens = malloc(buffer * sizeof(char*));
    char *token;

    token = strtok(line, " \n");
    while(token != NULL)
    {
        tokens[i] = token;
        i++;

        token = strtok(NULL, " \n");
    }
    tokens[i] = NULL;
    return tokens;
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

int main()
{
    char *command, **args;
    clr();
    while(1)
    {
        display_prompt();
        command = read_command();
        printf("%s", command);
        args = parse_command(command);
        
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