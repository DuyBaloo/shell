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

int environ()
{
    //list all environment strings
    printf("USER=%s\n", getenv("USER"));
    printf("LANG=%s\n", getenv("LANG"));
    printf("HOME=%s\n", getenv("HOME"));
    printf("PATH=%s\n", getenv("PATH"));
    printf("MAIL=%s\n", getenv("MAIL"));
    printf("SHELL=%s\n", getenv("SHELL"));
    printf("SSH_CLIENT=%s\n", getenv("SSH_CLIENT"));
    printf("SSH_TTY=%s\n", getenv("SSH_TTY"));
    printf("TERM=%s\n", getenv("TERM"));
    printf("XDG_SESSION_ID=%s\n", getenv("XDG_SESSION_ID"));
    printf("XDG_RUNTIME_DIR=%s\n", getenv("XDG_RUNTIME_DIR"));
    printf("HOSTTYPE=%s\n", getenv("HOSTTYPE"));
    printf("VENDOR=%s\n", getenv("VENDOR"));
    printf("OSTYPE=%s\n", getenv("OSTYPE"));
    printf("MATCHTYPE=%s\n", getenv("MATCHTYPE"));
    printf("SHLVL=%s\n", getenv("SHLVL"));
    printf("PWD=%s\n", getenv("PWD"));
    printf("GROUP=%s\n", getenv("GROUP"));
    printf("HOST=%s\n", getenv("HOST"));
    printf("REMOTEHOST=%s\n", getenv("REMOTEHOST"));
    printf("MANPATH=%s\n", getenv("MANPATH"));
    printf("WWW_HOME=%s\n", getenv("WWW_HOME"));
    return 1;
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

int *execute_args(char *cmd, char **args)
{
    if(strcmp(cmd, "cd") == 0)
        {
            printf("%s invoked.\n", cmd);
            cd(args);
        }
        else if(strcmp(cmd, "clr") == 0)
        {
            printf("%s invoked.\n", cmd);
            clr();
        }
        else if(strcmp(cmd, "echo") == 0)
        {
            printf("%s invoked.\n", cmd);
            echo(args);
        }
        else if(strcmp(cmd, "exit") == 0)
        {
            printf("%s invoked.\n", cmd);
            quit();
        }
        else if(strcmp(cmd, "dir") == 0)
        {
            printf("%s invoked.\n", cmd);
            dir();
        }
        else if(strcmp(cmd, "environ") == 0)
        {
            printf("%s invoked.\n", cmd);
            environ();
        }
        else
        {
            print_error();
        }
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
        execute_args(command, args);
            
        // }
        
        free(command);
        free(args);

    }
}