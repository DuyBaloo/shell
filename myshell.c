//Duy Nguyen - Lab 2 - CIS 3207 - Spring 2019
//I used some help from brennan.io for reading command and parsing command
#include <sys/wait.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <fcntl.h>
#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <dirent.h>
#include <time.h>

void display_prompt()
{
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
    //this will change the directory and print out current dir
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
    //using argument from slide
    printf("%s", "\033[H\033[2J");
}

int dir()
{
    //this function takes in a directory and prints out what's in it
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
    //list all environment strings using getenv
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
    //this will ignore the 1st index of args array and print out the rest
    int i;
    for(i = 1; args[i] != NULL; i++)
    {
        printf("%s ", args[i]);
    }
    puts("");

    return 1;
}

int help()
{
    //display a help page with more filter
    clr();    
    char *str[] = {
        "Welcome to my shell program, press Enter to navigate through help menu.",
        "Copyrights @ Duy Nguyen.",
        "Description:\n\tThis acts as a regular UNIX shell.",
        "Built-in commands you can try:",
        "\t1. cd - Change directory to your input.",
        "\t2. clr - Clear the screen.",
        "\t3. environ - List all the envorinment strings.",
        "\t4. echo - Display your own input strings.",
        "\t5. help - Display user manual."
        "\t6. pause - Pause the shell until you input 'Enter'.",
        "\t7. dir - List the current files in the directory.",
        "\t8. exit - Quit the shell.",
        "In addition, you can try regular UNIX commands like: ls, rm, cat, etc.",
        "End of help."
    };

    int i = 0;
    printf("%s\n", str[i]);
    for(i = 1; i < 13; i++)
    {
        char c = getchar();
        if(c == '\n')
        {
            printf("%s\n", str[i]);
        }
    }
       
    return 1;
}

void quit()
{
    //quit the shell
    printf("%s", "Quitting Shell\n");
    sleep(1);
    exit(0);
}

int pause_shell()
{
    //pause the shell until enter is hit
    clr();
    puts("Shell is paused. Press Enter to continue.");
    char c;
    c = getchar();
    while(&c != NULL)
    {
        if(c == '\n')
        {
            clr();
            return 1;
        }
        c = getchar();
    }
}

int isBackground(char *cmd)
{
    //check if user wants to execute command in background
    int i = strlen(cmd) - 2;
    if(cmd[i] == '&')
    {
        return 1;
    }
    else
    {
        return 0;
    }
    
}

int isPipe(char *line)
{
    int res = 0;
    for(int i = 0; line[i] != '\0'; i++)
    {
        if(line[i] == '|')
        {
            res = 1;
        }
    }
    return res;
}


char *remove_ampersand(char *cmd)
{
    //remove the ampersand to execute
    int i = strlen(cmd) - 2;
    if(cmd[i] == '&')
    {
        cmd[i] = '\0';
    }
    return cmd;
}

char *remove_last2(char *cmd)
{
    //remove last 2 chars of a line
    int i = strlen(cmd);
    if(cmd[i - 1] == ' ' || cmd[i - 1] == '\0' || cmd[i - 1] == '\n')
    {
        cmd[i - 1] = '\0';
        cmd[i - 2] = '\0';
    }
    
    return cmd;
}

char **put_left_array(char **args)
{
    for(int i = 0; args[i] != '\0'; i++)
    {
        if(*args[i] == '|')
        {
            args[i] = '\0';
        }
    }
    printf("%s left\n", args[0]);
    printf("%s left\n", args[1]);
    return args;
}

char **put_right_array(char **args)
{
    char **res = calloc(20, 1);
    int j = 0, k = 0, i = 0;
    for(i = 0; args[i] != '\0'; i++)
    {
        if(*args[i] == '|')
        {
            k = i;
            break;
        }
    }
    k += 1;
    for(j = 0; args[k] != '\0'; k++, j++)
    {
       res[j] = args[k];
    }
    printf("%s right\n", res[0]);
    printf("%s right\n", res[1]);
    res[j] = '\0';

    return res;
}

void *piping(char **left, char **right)
{
    pid_t pid, pid1;
    int fd[2];
    pipe(fd);
    if((pid = fork()) == -1)
    {
        print_error();
    }
    else if(pid == 0)
    {
        if((pid1 = fork()) == 0)
        {
            close(fd[0]);
            dup2(fd[1], 1);
            
            if(execvp(*left, left) < 0)
            {
                print_error();
            }
            close(fd[1]);
        }
        else
        {
            close(fd[1]);
            dup2(fd[0], 0);
            if(execvp(*right, right) < 0)
            {
                print_error();
            }
                        close(fd[0]);

        } 
    }
    else
    {

    }

}

void *run_background(char **args)
{
    pid_t pid;
    printf("pid before fork is: %d\n", getpid());

    if((pid = fork()) == -1)
    {
        print_error();
    }
    else if(pid == 0)
    {
        printf("pid after fork is: %d\n", getpid());

        //check to see which command to execute by comparing the strings, otherwise print the error
        if(execvp(*args, args) < 0)
        {
            print_error();
        }
        
    }
    else
    {
    }
}

char **remove_special(char **args)
{
    for(int i = 0; args[i] != '\0'; i++)
    {
        if(*args[i] == '>' || *args[i] == '<' || *args[i] == '|')
        {
            args[i] = '\0';
        }
    }
    
    return args;
}

int isRedirection(char *line)
{
    int res = 0;
    for(int i = 0; line[i] != '\0'; i++)
    {
        if(line[i] == '>')
        {
            res = 1;
        }
        else if(line[i] == '<')
        {
            res = 2;
        }
    }
    return res;
}

char *get_file_name(char **args)
{
    int i = 0;
    char *file = calloc(20, 1);
    // char **file = args;
    for(i = 0; args[i] != '\0'; i++)
    {
    }

    strcpy(file, args[i - 1]);
    return file;
}

void *redirection_out(char **args, char *file)
{
    int pid = fork();

    if(pid >= 0)
    {
        if(pid == 0)
        {
            int outFile = open(file, O_WRONLY|O_CREAT|O_APPEND, S_IRWXU|S_IRWXG|S_IRWXO);
            close(1);
            dup2(outFile, STDOUT_FILENO);
            close(outFile);

            if(execvp(args[0], args) < 0)
            {
                puts("ERROR EXECUTING COMMAND!");
                exit(0);
            }
        }
        else
        {
            int status = 0;
            wait(&status);
        }
    }
    else
    {
        puts("Forking error!");
    }
   
}

void *redirection_in(char **args, char *file)
{
    int pid = fork();

    if(pid >= 0)
    {
        if(pid == 0)
        {
            int outFile = open(file, O_RDONLY, S_IRWXU|S_IRWXG|S_IRWXO);
            close(0);
            dup2(outFile, 0);
            close(outFile);

            if(execvp(args[0], args) < 0)
            {
                puts("ERROR EXECUTING COMMAND!");
                exit(0);
            }
        }
        else
        {
            int status = 0;
            wait(&status);
        }
    }
    else
    {
        puts("Forking error!");
    }
   
}

char *read_command()
{
    //this program takes in input from user and will return the string to pass to parse_command
    char *line;
    size_t bufsize = 0;
    
    getline(&line, &bufsize, stdin);
    
    return line;
}

char **parse_command(char *line)
{
    //takes in line input by user and start tokenizing then return the array of arguments
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

void *execute_args(char *cmd, char **args)
{
    int status = 0;
    pid_t pid;
    
    if(strcmp(cmd, "exit") == 0)
    {
        // printf("%s invoked.\n", cmd);
        quit();
    }
    else if(strcmp(cmd, "pause") == 0)
    {
        // printf("%s invoked.\n", cmd);
        pause_shell();
    }
    else if(strcmp(cmd, "cd") == 0)
    {
        // printf("%s invoked.\n", cmd);
        cd(args);
    }
    else if(strcmp(cmd, "help") == 0)
    {
        // printf("%s invoked.\n", cmd);
        help();
    }
    else if(strcmp(cmd, "clr") == 0)
    {
        // printf("%s invoked.\n", cmd);
        clr();
    }
    else if(strcmp(cmd, "echo") == 0)
    {
        // printf("%s invoked.\n", cmd);
        echo(args);
    }
    else if(strcmp(cmd, "dir") == 0)
    {
        // printf("%s invoked.\n", cmd);
        dir();
    }
    else if(strcmp(cmd, "environ") == 0)
    {
        // printf("%s invoked.\n", cmd);
        environ();
    }
    else if(strcmp(cmd, "\n") == 0)
    {
        print_error();
    }
    else
    {
        // printf("pid before fork is: %d\n", getpid());
        pid = fork();
        if(pid == -1)
        {
            print_error();
        }
        else if(pid == 0)
        {
            // printf("pid after fork is: %d\n", getpid());
            //check to see which command to execute by comparing the strings, otherwise print the error
            
            if(execvp(*args, args) < 0)
            {     /* execute the command  */
                print_error();
            }
            
        }
        else
        {
            wait(&status);
        }
    }
}


void *read_from_batch(char *argv[])
{
    //read all command lines from file and execute, since text file has 2 extra character
    //I have created a function to delete last 2
    FILE *fp;
    char *line = NULL, **args;
    size_t len = 0;
    ssize_t read;

    fp = fopen(argv[1], "r");
    if (fp == NULL)
    {
        puts("Error opening file.");
        exit(1);
    }
    while ((read = getline(&line, &len, fp)) != -1) {

        line = remove_last2(line);
        args = parse_command(line);
        execute_args(line, args);
    }

    free(line);
    free(args);
    fclose(fp);
    exit(0);
}

int main(int argc, char *argv[])
{
    char *command, **args; //initialize the pointers to use
    int a;
    clr(); //clear the screen for the first time
    if(argc == 1){
        puts("Enter 'help' for user manual.");
        while(1)
        {
            //loop runs forever until exit is executed
            display_prompt(); //display the prompt for user
            command = read_command(); //store input into command

            if(isBackground(command))
            {   
                command = remove_ampersand(command);
                args = parse_command(command);
                run_background(args);
                
            }
            else if((a = isRedirection(command)) != 0)
            {
                args = parse_command(command);

                char *file = calloc(20, 1);
                strcpy(file, get_file_name(args));

                args = remove_special(args);//remove ">" or "<"
                if(a == 1)
                {
                    redirection_out(args, file);
                }
                else if(a == 2)
                {
                    redirection_in(args, file);
                }

            }
            else if(isPipe(command))
            {
                char *command1 = calloc(100, 1);
                strcpy(command1, command);
                printf("%s command\n", command);
                printf("%s command1\n", command1);

                args = parse_command(command);
                printf("%ld args size\n", sizeof(args));

                char **args1 = parse_command(command1);
                printf("%ld args1 size\n", sizeof(args1));

                char **left = put_left_array(args);
                printf("%ld args size\n", sizeof(args));

                char **right = put_right_array(args1);
                printf("%ld args1 size\n", sizeof(args1));


                piping(left, right);
            }
            else
            {
                args = parse_command(command);
                execute_args(command, args); //call execute_args function
            }
        }
    }
    else if(strcmp(argv[1], argv[1]) == 0)
    {
        read_from_batch(argv);
    }

    free(command);
    free(args);
    
}