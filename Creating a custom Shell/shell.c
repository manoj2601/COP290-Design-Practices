
// C Program to design a shell in Linux
#include<stdio.h>
#include<string.h>
#include<stdlib.h>
#include<unistd.h>
#include<sys/types.h>
#include<sys/wait.h>
#include<readline/readline.h>
#include<readline/history.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h> // open function
#include <unistd.h> // close function

#define MAXCOM 1000 // max number of letters to be supported
#define MAXLIST 1000 // max number of commands to be supported

// Clearing the shell using escape sequences
#define clear() printf("\033[H\033[J")

int takeInput(char* inputString)
{
    char* str;
    str = readline("\nshell>");
    if(strlen(str) == 0)
    {
        return 0;
    }
    else
    {
        add_history(str);
        strcpy(inputString, str);
        return 1;
    }
}

void parseSpace(char* inputString, char** parsed)
{
    int i=0;
    while(1)
    {
        parsed[i] = strsep(&inputString, " ");
        if(parsed[i] == NULL) break;
        if(strlen(parsed[i]) == 0)
            i--;
        i++;
    }
}

int checkcmdHandler(char** parsed)
{
    //storing builtin commands
    char* ownCmds[5];
    ownCmds[0] = "cd";
    ownCmds[1] = "pwd";
    ownCmds[2] = "mkdir";
    ownCmds[3] = "rmdir";
    ownCmds[4] = "exit";

    int j=5;
    int i=0;
    for(i=0; i<5; i++)
    {
        // printf("parsed[0] is %s\n", parsed[0]);
        // printf("ownCmds[i] is %s\n", ownCmds[i]);
        if(strcmp(parsed[0],ownCmds[i]) == 0)
        {
            j=i;
            break;
        }
    }
    if(j==5)
        {
            // printf("Nothing\n");
            return 0;
        }
    else if(j==4)
    {
        exit(0);
    }
    else if(j==1)
    {
        char path[100];
        getcwd(path, sizeof(path));
        printf("%s", path);
    }
    else if(j==0)
    {
        char comma = '"';
        if(parsed[1][0] == comma && parsed[1][strlen(parsed[1])-1] == comma)
        {
            // printf("Yoooo\n");
            char* directory;
            // printf("strlength is : %lu", strlen(parsed[1]));
            int k=1;
            for(k=1; k<strlen(parsed[1]); k++)
            {
                directory[k-1] = parsed[1][k];
            }
            // printf("directory is : %s\n", directory);
            chdir(directory);
        }
        else
        {
            chdir(parsed[1]);
        }
    }
    else if(j==2)
    {
        int check;
        check = mkdir(parsed[1], 0777);
        if(check == 0)
            {// printf("directory created");
    }
        else printf("creation failed");
    }
    else if(j==3)
    {
        int check = rmdir(parsed[1]);
        if(check == 0)
            {// printf("folder deleted\n");
            }
        else printf("Error in deleting\n");
    }
    return 1;
}

int processString(char* inputString, char** parsed, char** parsedpipe)
{
    char* strpiped[2];
    int piped=0;
    // printf("inputString is : %s\n", inputString);
    char inputString1[MAXCOM];
    strcpy(inputString1, inputString);
    // printf("Manoj : %s\n", inputString);
    int i=0;
    for(i=0; i<2; i++)
    {
        strpiped[i] = strsep(&inputString, "|");
        if(strpiped[i] == NULL) break;
    }
    if(strpiped[1] == NULL) //no pipes
    {
        // printf("goint to store in parsed\n inputString is : %s\n", inputString1);
        piped = 0;
        parseSpace(inputString1, parsed); //function for parsing command words
    }
    else //pipes
    {
        piped = 1;
        parseSpace(strpiped[0], parsed); //function for parsing command words
        parseSpace(strpiped[1], parsedpipe); //function for parsing command words
    }
    //check whether parsed is a builtin command or not
    if(checkcmdHandler(parsed))
    {
        // printf("builtin command hai ye to %s\n", parsed[0]);
        return 0; //it was builtin command
    }
    else
    {
        if(strpiped[1] == NULL) return 1; //no pipes
        else return 2;
    }

}

void parseLESS(char* inputString, char* parse[MAXCOM])
{
        int i=0;
    char input[MAXCOM];

    strcpy(input, inputString);
    char* new = input;
    while(1)
    {
        parse[i] = strsep(&new, "<");
        if(parse[i] == NULL) break;
        if(strlen(parse[i]) == 0)
            i--;
        i++;
    }



    // int i=0;
    // char input[MAXCOM];
    //     // printf("LOOOOOOOOOOL\n");

    // strcpy(input, inputString);
    // char* new = input;
    // while(1)
    // {
    //     parse[i] = strsep(&new, "<");
    //     if(parse[i] == NULL) break;
    //     // if(strlen(parse[i]) == 0)
    //     //     i--;
    //     i++;
    // }
}

void LESSexec(char **parsed)
{
    // printf("in LESSexec %s %s\n", parsed[0], parsed[1]);
    char **nullarray = malloc(2*MAXCOM);
    printf("parsed [0] : %s\n", parsed[0]);
    nullarray[0] = parsed[0];
    nullarray[1] = NULL;
    // printf("nullarray[0] : %s\n", nullarray[0]);
    pid_t child_pid;
    int stat_loc;
    child_pid = fork();
    if(child_pid<0)
    {
        printf("fork failed");
        exit(1);
    }
    else if(child_pid==0)
    {
        close(0);
        int fd = open(parsed[1], O_RDONLY, 0);
        printf("fd is %d\n", fd);
        if(execvp(parsed[0], nullarray) < 0)
        {
            printf("execvp failed\n");
            exit(1);
        }
    }
    else
    {
        waitpid(child_pid, &stat_loc, WUNTRACED);
    }
}
char* remove_spaces(const char *input)
{
    
    int count1=0;
    int count2=0;
    int i=0;
    int size = strlen(input);
    // printf("size of input is : %d\n", size);
    for(i=0; i<size; i++)
    {
        if(isspace((unsigned char) input[i]))
        {
            count1++;
        }
        else break;
    }


    for(i=strlen(input)-1; i>=0; i--)
    {
        if(isspace((unsigned char) input[i]))
        {
            count2++;
        }
        else break;
    }
    int j=0;
    char* result = malloc(size - count1 -count2);
    for(i=count1; i<size-count2; i++)
    {
        // printf("entering : %c\n", input[i]);
        result[j]=input[i];
        j++;
    }
    result[j] = '\0';
  return result;
}

void GRATexec(char **parsed)
{
    // printf("in GRATexec\n");
    char **nullarray = malloc(2*MAXCOM);
    nullarray[0] = parsed[0];
    nullarray[1] = NULL;
    pid_t child_pid;
    int stat_loc;

    child_pid = fork();
    if(child_pid < 0)
    {
        printf("fork failed");
    }
    else if(child_pid == 0)
    {
        close(1);
        int fd = open(parsed[1], O_WRONLY, 1);
        if(execvp(nullarray[0], nullarray) < 0)
        {
            printf("execvp failed\n");
            // exit(0);
        }
    }
    else
    {
        waitpid(child_pid, &stat_loc, WUNTRACED);
    }
}

void parseGRAT(char*inputString, char* parse[MAXCOM])
{
    int i=0;
    char input[MAXCOM];

    strcpy(input, inputString);
    char* new = input;
    while(1)
    {
        parse[i] = strsep(&new, ">");
        if(parse[i] == NULL) break;
        if(strlen(parse[i]) == 0)
            i--;
        i++;
    }
}

int execArgs(char* inputString1)
{
    char* parsed1[MAXLIST];
    char* parsed2[MAXLIST];
    int i=0;
    char inputString[MAXCOM];
    char inputd[MAXCOM];
    strcpy(inputString, inputString1);
    strcpy(inputd, inputString1);
    parseLESS(inputString1, parsed1); 
    // printf("initially parsed1[0] isa  : %s\n", parsed1[0]);
    parseGRAT(inputd, parsed2);
      // printf("initially parsed1[0] isa  : %s\n", parsed1[0]);
    if(parsed1[1] != NULL && parsed2[1] == NULL)
    {
        parseLESS(inputString1, parsed1);
        // printf("parsed1[0] %s\n parsed1[1] %s\n", parsed1[0], parsed1[1]);
        parsed1[0] = remove_spaces(parsed1[0]);
        parsed1[1] = remove_spaces(parsed1[1]);
        // printf("parsed1[0] %s\n parsed1[1] %s\n", parsed1[0], parsed1[1]);
        LESSexec(parsed1);
    }
    else
    {
        // parseGRAT(inputString1, parsed2);
        if(parsed2[1] != NULL && parsed1[1] == NULL)
        {
            parsed2[0] = remove_spaces(parsed2[0]);
            parsed2[1] = remove_spaces(parsed2[1]);
            GRATexec(parsed2);    
        }
        else if(parsed1[1] == NULL && parsed2[1] == NULL)
        {
            char* argv[MAXLIST];
            parsed1[0] = remove_spaces(parsed1[0]);
            parseSpace(parsed1[0], argv);
            pid_t child_pid;
            int child_status;

            child_pid=fork();

            if(child_pid==0) //done by child
            {
                execvp(argv[0], argv);
                printf("Unknown Command1\n");
                exit(0);
            }
            else //done by parent
            {
                pid_t tpid = wait(&child_status);
                while(tpid != child_pid)
                {
                    tpid = wait(&child_status);
                }
                return child_status;
            }
        }
        else
        {
            // printf("Hello brother\n");
            if(parsed1[1] != NULL && parsed2[1] != NULL)
            {
                // printf("manoj : %s\n", inputString1);
                char* parsedcommon[MAXLIST];
                    int i=0;
                        char input[MAXCOM];
                            // printf("LOOOOOOOOOOL\n");

                        strcpy(input, inputString1);
                        char* new = input;
                        while(1)
                        {
                            parsedcommon[i] = strsep(&new, "<>\n");
                            if(parsedcommon[i] == NULL) break;
                            if(strlen(parsedcommon[i]) == 0)
                                i--;
                            i++;
                        }
                        parsedcommon[0] = remove_spaces(parsedcommon[0]);
                        parsedcommon[1] = remove_spaces(parsedcommon[1]);
                        parsedcommon[2] = remove_spaces(parsedcommon[2]);
                        int a1=0;
                        int b1=0;
                        i=0;
                        for(i=0; i<strlen(inputString1); i++)
                        {
                            if(inputString1[i] == '<')
                                {
                                    a1++; break;}
                            else if(inputString1[i] == '>')
                                {
                                    b1++; break;}
                        }
                        if(a1!= 0)
                        {
                            char **nullarray = malloc(2*MAXCOM);
                            nullarray[0] = parsedcommon[1];
                            nullarray[1]=NULL;
                            pid_t child_pid;
                                int stat_loc;
                                child_pid = fork();
                                if(child_pid<0)
                                {
                                    printf("fork failed");
                                    exit(1);
                                }
                                else if(child_pid==0)
                                {
                                    close(0);
                                    int fd = open(parsedcommon[1], O_RDONLY, 0);
                                    close (1);
                                    int fd2 = open(parsedcommon[2], O_WRONLY, 1);
                                    if(execvp(parsedcommon[0], nullarray) < 0)
                                    {
                                        printf("execvp failed\n");
                                        exit(1);
                                    }
                                }
                                else
                                {
                                    waitpid(child_pid, &stat_loc, WUNTRACED);
                                }

                        }
                        else if(b1 != 0)
                        {
                            char **nullarray = malloc(2*MAXCOM);
                            nullarray[0] = parsedcommon[1];
                            nullarray[1]=NULL;
                            pid_t child_pid;
                                int stat_loc;
                                child_pid = fork();
                                if(child_pid<0)
                                {
                                    printf("fork failed");
                                    exit(1);
                                }
                                else if(child_pid==0)
                                {
                                    close(0);
                                    int fd = open(parsedcommon[2], O_RDONLY, 0);
                                    close (1);
                                    int fd2 = open(parsedcommon[1], O_WRONLY, 1);
                                    if(execvp(parsedcommon[0], nullarray) < 0)
                                    {
                                        printf("execvp failed\n");
                                        exit(1);
                                    }
                                }
                                else
                                {
                                    waitpid(child_pid, &stat_loc, WUNTRACED);
                                }
                        }

                        // printf("parsedcommon is %s %s %s\n", parsedcommon[0], parsedcommon[1], parsedcommon[2]);
            }
        }
    }
}

void execPiping(char** parsed1, char** parsed2)
{
    char **nullarray = malloc(2*MAXCOM);
    nullarray[0] = parsed1[0];
    nullarray[1] = NULL;

    char **nullarray2 = malloc(2*MAXCOM);
    nullarray2[0] = parsed2[0];
    nullarray2[1] = NULL;

    pid_t child_pid;
    int stat_loc;
    int fd[2];
    pipe(fd);
    child_pid=fork();

    if(child_pid < 0)
    {
        printf("fork failed\n");
        exit(1);
    }

    if(child_pid ==0)
    {
        dup2(fd[1], STDOUT_FILENO);
        close(fd[0]);
        close(fd[1]);
        execlp(nullarray[0], nullarray[0], nullarray, (char*) NULL);
        fprintf(stderr, "Failed to execute %s\n", nullarray[0]);
        exit(1);
    }
    else
    {
        child_pid = fork();
        if(child_pid==0)
        {
            dup2(fd[0], STDIN_FILENO);
            close(fd[0]);
            close(fd[1]);
            execlp(nullarray2[0], nullarray2[0], nullarray2, (char*) NULL);
            exit(1);
        }
        else
        {
            close(fd[0]);
            close(fd[1]);
            waitpid(child_pid, &stat_loc, 0);
        }
    }
}



int main()
{
    char inputString[MAXCOM], *parsedArgs[MAXLIST];
    char* parsingpiped[MAXLIST];
    int totalpiping = 0;
    clear();
    char* username = getenv("USER");
    printf("Hello %s \n welcome to my shell.\n", username);
    //init_shell() end;

    while (1) {
        // print shell line  printDir() start;
        // char path[1024];
        // getcwd(path, 1024);
        // printf("\n$%s", path);
        //printDir ends
        //taking input
        if(takeInput(inputString) == 0)
            continue;
        //input taken
        // printf("hello : %s\n", inputString);
        char new[MAXCOM];
        strcpy(new, inputString);
        // printf("hello2 : %s\n", inputString);
        totalpiping = processString(inputString, parsedArgs, parsingpiped);
        // totalpiping returns zero if there is no command
        // or it is a builtin command,
        // 1 if it is a simple command
        // 2 if it is including a pipe.
        int a;
        if (totalpiping == 1)
            {
                a = execArgs(new);
            }
        if (totalpiping == 2)
            execPiping(parsedArgs, parsingpiped);
    }
    return 0;
}
