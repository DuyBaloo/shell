#include <sys/wait.h>
#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <dirent.h>

void display_prompt()
{
    //this function will clear out the whole screen and print out 
    //prompt for user to start putting in inputs
    printf("%s", "myshell>");
}

void print_error()
{
    //this function will print out err msg as given in instruction
    char error_message[30] = "An error has occurred\n";
    write(STDERR_FILENO, error_message, strlen(error_message));
}

int cd(char **args)
{
    char cwd[100];
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
        if(getcwd(cwd, sizeof(cwd)) != NULL)
        {
            printf("Current dir: %s\n", cwd);
        }
        else{
            print_error();
            return 1;
        }
    }
    return 1;
    
}

void clr()
{
    printf("%s", "\033[H\033[2J");
}

int dir()
{
    //this function takes in a directory and prints out what's in It
    DIR *dir = opendir(".");
    struct dirent *s;
    while((s = readdir(dir)) != NULL)
    {
        printf("%s\n", s->d_name);
    }
    puts("******************************************************************************");
    return 1;
}

void environ()
{
    //list all environment strings
}

int echo(char **args)
{
    int i = 1;
    for(i = 1; args[i] != NULL; i++)
    {
        printf("%s ", args[i]);
    }
    puts("");

    return 1;
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
        // printf("%s", command);
        args = parse_command(command);
        
        // if(fork() != 0)
        //     wait(NULL);
        // else
        // {
        //     strcpy(cmd, "/bin/");
        //     strcat(cmd, command);
        //     execvp(cmd, para);

            
        // }
        if(strcmp(command, "cd") == 0)
        {
            printf("%s invoked.\n", command);
            cd(args);
        }
        if(strcmp(command, "clr") == 0)
        {
            printf("%s invoked.\n", command);
            clr();
        }
        if(strcmp(command, "echo") == 0)
        {
            printf("%s invoked.\n", command);
            echo(args);
        }
        if(strcmp(command, "exit") == 0)
        {
            printf("%s invoked.\n", command);
            quit();
        }
        if(strcmp(command, "dir") == 0)
        {
            printf("%s invoked.\n", command);
            dir();
        }
        // else
        // {
        //     print_error();
        // }
        free(command);
        free(args);

    }
}