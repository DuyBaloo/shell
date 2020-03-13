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
    int i = 1;
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
        "Copyrights@ Duy Nguyen.",
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

void test()
{
    //test background process
    sleep(5);
}

void *run_background(char *cmd, char **args)
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
        test();
        if(execvp(*args, args) < 0)
        {     /* execute the command  */
            print_error();
        }
    }
    else
    {
        sleep(1);
        clr();
    }

}

int isRedirection(char *line)
{
    int res = 0;
    for(int i = 0; line[i] != '\0'; i++)
    {
        if(line[i] == '>' || line[i] == '<')
        {
            res = 1;
        }
    }
    return res;
}
int redirection(char **args, char *in, char *out)
{
    int pid = fork();
    if(pid >= 0)
    {
        if(pid == 0)
        {
            int outFile = open(out, O_WRONLY|O_CREAT|O_TRUNC, S_IRWXU|S_IRWXG|S_IRWXO);
            close(1);
            dup2(outFile, 1);
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
    return 1;
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
        printf("%s invoked.\n", cmd);

        quit();
    }
    else if(strcmp(cmd, "pause") == 0)
    {
        printf("%s invoked.\n", cmd);
        pause_shell();
    }
    else if(strcmp(cmd, "cd") == 0)
    {
        printf("%s invoked.\n", cmd);
        cd(args);
    }
    else if(strcmp(cmd, "help") == 0)
    {
        printf("%s invoked.\n", cmd);
        help();
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
    else if(strcmp(cmd, "\n") == 0)
    {
        print_error();
    }
    else
    {
        printf("pid before fork is: %d\n", getpid());
        pid = fork();
        if(pid == -1)
        {
            print_error();
        }
        else if(pid == 0)
        {
            printf("pid after fork is: %d\n", getpid());
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
    //read all command lines from file and execute
    FILE *fp;
    char *line = NULL, **args, *command, *left, *right;
    size_t len = 0;
    ssize_t read;

    fp = fopen(argv[1], "r");
    if (fp == NULL)
    {
        puts("Error opening file.");
        exit(1);
    }
    while ((read = getline(&line, &len, fp)) != -1) {
        // printf("%d\n", strlen("dir"));
        // printf("%d\n", strlen(line));
        line = remove_last2(line);
        printf("%s\n", line);
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
    FILE *fp;
    clr(); //clear the screen for the first time
    if(argc == 1){
        puts("Enter 'help' for user manual.");
        while(1)
        {
            //loop runs forever until exit is executed
            display_prompt(); //display the prompt for user
            command = read_command(); //store input into command
            // printf("%s", command);
            //put into tokens
           
            
            
            if(isBackground(command))
            {   
                command = remove_ampersand(command);
                args = parse_command(command);
                run_background(command, args);
                
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