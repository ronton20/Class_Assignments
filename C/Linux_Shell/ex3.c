//Author:           Ron Amar
//ID:               318479458

//How to compile:   gcc ex3.c -o ex3
//How to run:       ./ex3

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <wait.h>
#include <sys/stat.h> 
#include <fcntl.h>

#define true 1
#define false 0
#define MAXLEN 512
#define MAX_PIPES 2     //determines the number of pipes alowed

char* fileName = "file.txt";    //name of history file
char* nohupFileName = "nohup.txt";
int pipeCount;
int runInBackground;
int hasEcho;
int firstQuote;
int secondQuote;

void handler(int sig) {
    waitpid(-1, NULL, WNOHANG);
}

//=========== P R E D E C L A R I N G === M E T H O D S ===========

void run();
void getWordsNChars(char*, int*, int*); //recives the string of [command] and 2 ints for updating the values of [words] and [chars] (only adding to the current value. not replacing)
char** toDoubleArr(char*, int*);        //recives the string of [command] and the number of [words] in it
void writeHistory(char*);               //recives sentence to append
void printHistory();
void freeArgv(char**);                  //recives the double array to free
int runCommand(char**);                 //recives the double array to run
void getCleanWord(char*, char*);        //recives a string of [word] and an [output] string to be updated with the 'clean' word
int checkDone(char*, int, int);         //recives [line] of the last command, [num] of commands entered so far and the [totalWords] entered so far
int checkHistory(char*);                //recives the [line] of the last command entered
int checkLineCommand(char*);            //recives the [line] of the last command entered
int checkPipe(char**, int);             //recives argv and its length
int runPipe(char**, int, int);          //recives double array to run, index of the pipe, index of first command and the length of the array.
int countPipes(char**);                 //recives double array to check
void checkBackground(char*);            //recives [line] to check for '&' nad words count to decrease if necessary
void checkNohup(char**);                //recives double array to check
int checkEcho(char*);                   //recives [line] to check
int translate(char*);                   //recives command line to translate

//============================ M A I N ============================
int main()
{
    signal(SIGCHLD, handler);
    run();

    return 0;
}

//========================= M E T H O D S =========================

//the main method of the program
//runs in a loop until "done" is entered
void run() {
    int finished = false;

    int totalPipes = 0;
    int commands = 0;

    char line[MAXLEN];
    char dir[MAXLEN];
    do {
        getcwd(dir, MAXLEN);
        printf("%s>", dir);
        fgets(line, MAXLEN, stdin);
        line[strcspn(line, "\n")] = 0;      //removes the '\n' at the end of the line
        
        if(line[0] == '\0' || line[0] == '\n') continue;
        if(line[0] == ' ' || line[strlen(line) - 1] == ' ') {
            printf("invalid command\n");
            continue;
        }
        
        int words = 0;          //counts the words in the sentence
        int totalChars = 0;     //counts the total characters in the sentence
        pipeCount = 0;
        runInBackground = false;

        checkBackground(line);  //checks for '&'
        int firstQuote, secondQuote;
        hasEcho = checkEcho(line);
        if(translate(line) == -1) { continue; }     //translate command in case '![num]' is included
        
        getWordsNChars(line, &words, &totalChars);
        
        //skips if only spaces were entered
        if(words < 1) continue;
        
        //checks if the commands "done", "history" or "![line_number]" were entered
        int cmdDone = 1;
        int cmdLine = 1;
        int cmdHist = 1;

        if(words == 1) {
            //checks if 'done' was entered and deal with it if it was
            cmdDone = checkDone(line, commands, totalPipes);
            if(cmdDone == 0) { break; }

            //checks if 'history' was entered and deal with it if it was
            cmdHist = checkHistory(line);
            if(cmdHist == 0) {
                commands++;
                continue;
            }
        }
        int argvLen = words + 1;
        char** argv = toDoubleArr(line, &argvLen);

        pipeCount = countPipes(argv);
        if(pipeCount > MAX_PIPES) {
            fprintf(stderr, "Too many pipes!\n");
            freeArgv(argv);
            continue;
        }

        // for(int i = 0; argv[i] != NULL; i++) printf("%s\n", argv[i]);
        
        int tempPipes = pipeCount;
        pipeCount = 0;
        int status = checkPipe(argv, argvLen);
        pipeCount = tempPipes;

        if(pipeCount == 0)
            status = runCommand(argv);

        if(status == 0)
            writeHistory(line);

        if(status == 0 || status == -1) {
            totalPipes += pipeCount;
            commands++;
        }

        if(argv != NULL)
            freeArgv(argv);
        
    } while(finished == false);

}   // run -> end


//translates a command line to the appropriate corresponding commands in case it contains '![num]' commands in it
int translate(char* line) {
    int pos = strcspn(line, "!");
    
    while(pos < strlen(line)) {
        if(!(pos > firstQuote && pos < secondQuote)) {
            char* templine = (char*)malloc(20 * sizeof(char));
            int i = 0;
            for(; line[i + pos] != ' ' && line[i + pos] != '|' && line[i + pos] != '\0'; i++) {
                templine[i] = line[i + pos];
                templine[i + 1] = 0;
            }
            if(checkLineCommand(templine) == -1) {
                free(templine);
                return -1;
            }
    
            char nextline[MAXLEN];
            strcpy(nextline, line + i);               //copies the rest of the command to a temporary array
            strcpy(line, templine);
            line[strcspn(line, "\n")] = 0;
            strcpy(line + strlen(line), nextline);      //completes the rest of the string
            free(templine);
        }
        pos = strcspn(line + pos + 1, "!") + pos + 1;
    }
    return 0;
}

//counts the number of pipes
int countPipes(char** argv) {
    int pipes = 0;
    for(int i = 0; argv[i] != NULL; i++) {
        if(strcmp(argv[i], "|") == 0) pipes++;
    }
    return pipes;
}

//puts a clean version (no spaces and puts '\0' at the end) of [line] in [newLine]
void getCleanWord(char* line, char* newLine) {
    int i = 0;
    int j = 0;
    char c = line[i];
            
    //copies only the word without the spaces
    while(c != '\n' && c != '\0') {
        if(c != ' ') newLine[j++] = c;
        c = line[++i];
    }
    newLine[j] = c;
    newLine[strcspn(newLine, "\n")] = 0;      //removes the '\n' at the end of the sentence (if there is any)
}

//check for 'done' command
int checkDone(char* line, int commands, int totalPipes) {
    char newLine[MAXLEN];
    getCleanWord(line, newLine);
        
    //checks for 'done'
    if(strcmp(newLine, "done") == 0) {
        // commands++;                                                      //disable comment if want to include 'done'
        printf("Num of commands: %d\n", commands);                          //only counts the number of commands entered (including 'cd' and 'done' commands)
        printf("Total number of pipes in all commands: %d\n", totalPipes);  //prints the total number of pipes in the commands entered
        return 0;
    }
    return -1;
}

//checking for pipe
int checkPipe(char** argv, int length) {
    for(int i = 0; argv[i] != NULL; i++) {
        if(strcmp(argv[i], "|") == 0) {
            return runPipe(argv, i, length);
        }
    }
    return -1;
}

//check for 'history' command
int checkHistory(char* line) {
    char newLine[MAXLEN];
    getCleanWord(line, newLine);
    
    if(strcmp(newLine, "history") == 0) {
        writeHistory(line);
        printHistory(fileName);
        return 0;
    }
    return -1;
}

//checks for '![number]' command and changes the command line accordingly
int checkLineCommand(char* line) {
    char newLine[MAXLEN];
    getCleanWord(line, newLine);

    if(newLine[0] == '!') {
        //getting the line number
        char lineNumStr[sizeof(int)];
        strncpy(lineNumStr, newLine + 1, sizeof(int));
    
        int ind = 0;
        //checks if the command contains any non numeric chars (except the '!' at the start of it)
        while(lineNumStr[ind] != '\0') {
            if(!(lineNumStr[ind] >= '0' && lineNumStr[ind] <= '9')) {
                printf("NOT IN HISTORY\n");
                return -1;
            }
            ind++;
        }

        int lineNum = atoi(lineNumStr);
        if(lineNum <= 0) {
            printf("NOT IN HISTORY\n");
            return -1;
        }

        FILE* file = fopen(fileName, "r");
        if(file == NULL) {
            fprintf(stderr, "Couldn't open file\n");
            exit(1);
        }
    
        //getting the selected command from the history file
        int numOfLine = 1;
        char tempLine[MAXLEN];
        while(numOfLine <= lineNum && fgets(tempLine, MAXLEN, file)) { numOfLine++; }
        fclose(file);

        //checking if the desired command reached
        if(numOfLine <= lineNum) {
            printf("NOT IN HISTORY\n");
            return -1;
        }

        strcpy(line, tempLine);
        return 0;
    }
    return 1;
}

//checks for '&' and removes it from [line]
void checkBackground(char* line) {
    if(line == NULL || strlen(line) == 0) return;
    int last = strlen(line) - 1;
    if(line[last] == '&') {
        runInBackground = true;
        line[last--] = 0;                         //removes the '&' at the end
        while(line[last] == ' ') {  //removes excess spaces between last word and '&'
            line[last--] = 0;
        }
    }
}

//checks for 'nohup' command and manages the process if found
void checkNohup(char** argv) {
    if(strcmp(argv[0], "nohup") == 0) {
        signal(SIGHUP, SIG_IGN);
        close(STDIN_FILENO);
        int fd = open(nohupFileName, O_WRONLY | O_CREAT | O_APPEND, S_IRUSR | S_IWUSR | S_IRGRP | S_IROTH);
        if(dup2(fd, STDOUT_FILENO) == -1) {
            fprintf(stderr, "dup2() failed\n");
            freeArgv(argv);
            exit(1);
        }
        free(argv[0]);
        for(int i = 0; argv[i] != NULL; i++)
            argv[i] = argv[i + 1];
    }
}

//checks if there is an echo command and updates to location of the following quotes
int checkEcho(char* line) {
    firstQuote = -1;
    secondQuote = -1;
    int echoLoc = strcspn(line, "echo");
    if(echoLoc >= strlen(line)) return -1;
    
    int echoEnd = echoLoc + 4;
    firstQuote = strcspn(line + echoEnd, "\"") + echoEnd;
    secondQuote = strcspn(line + firstQuote + 1, "\"") + firstQuote + 1;
    
    if(firstQuote >= strlen(line)) return -1;
    if(secondQuote >= strlen(line)) return -1;
    
    return 0;
}

//free dynamic memory
void freeArgv(char** argv) {
    if(argv == NULL) return;

    int i = 0;
    while(argv[i] != NULL) {
        free(argv[i]);
        argv[i++] = NULL;
    }
    free(argv);
    argv = NULL;
}

//runs a command in a child process and returns the childs exit status
int runCommand(char** argv) {
    //locks 'cd' function
    if(strcmp(argv[0], "cd") == 0) {
        printf("command not supported (yet)\n");
        freeArgv(argv);
        return -1;
    }

    //starts a child process to run the command
    pid_t pd = fork();
    if(pd == -1) {
        perror("fork() failed");
        freeArgv(argv);
        exit(1);
    }
    if(pd == 0) {
        checkNohup(argv);
        //executes the command entered by the user in the child process
        if(execvp(argv[0], argv) == -1) {
            perror("execvp() failed");
            freeArgv(argv);
            exit(1);
        }
        exit(0);
    }
    
    signal(SIGHUP, SIG_DFL);

    int status = 0;
    if(runInBackground == false) {
        wait(&status);
    }

    return status;
}

//runs a command with pipe
int runPipe(char** argv, int pipeIndex, int length) {
    if(argv == NULL) return -1;

    //locks 'cd' function
    if(strcmp(argv[0], "cd") == 0) {
        printf("command not supported (yet)\n");
        freeArgv(argv);
        return -1;
    }
    
    pipeCount++;

    if(pipeCount > MAX_PIPES) { //determines how mant pipes we can run
        fprintf(stderr, "Too many pipes\n");
        return -1;
    }

    //creating the pipe
    int pipe_fd[2];
    if(pipe(pipe_fd) == -1) {
        perror("pipe() failed");
        exit(-1);
    }

    //starts a child process to start the pipe chain
    pid_t pd = fork();
    if(pd == -1) {
        perror("fork() failed");
        freeArgv(argv);
        exit(-1);
    }
    //==================== child 1 ====================
    if(pd == 0) {
        if(dup2(pipe_fd[1], STDOUT_FILENO) == -1) {
            fprintf(stderr, "dup2() failed");
            exit(-1);
        }
        close(pipe_fd[0]);
        close(pipe_fd[1]);

        //creating a new argv array to run
        char** argv1 = (char**)malloc((pipeIndex + 1) * sizeof(char*));
        if(argv1 == NULL) {
            fprintf(stderr, "malloc failed");
            freeArgv(argv1);
            exit(-1);
        }
        int i = 0;
        for(; i < pipeIndex; i++) {
            argv1[i] = (char*)malloc((strlen(argv[i]) * sizeof(char)));
            if(argv1[i] == NULL) {
                fprintf(stderr, "malloc failed");
                freeArgv(argv1);
                exit(-1);
            }
            strcpy(argv1[i], argv[i]);
            argv1[i+1] = NULL;
        }
        if(pipeIndex == 1) {
            if(checkHistory(argv1[0]) == 0) {
                freeArgv(argv1);
                exit(0);
            }
        }
        if(execvp(argv1[0], argv1) == -1) {     //runs the 1st command
            perror("execvp() failed");
            freeArgv(argv1);
            exit(-1);
        }
        freeArgv(argv1);
        exit(0);
    }

    
    pid_t child_pd = fork();
    if(child_pd == -1) {
        perror("fork() failed");
        freeArgv(argv);
        exit(-1);
    }
    //==================== child 2 ====================
    if(child_pd == 0) {
        if(dup2(pipe_fd[0], STDIN_FILENO) == -1) {
            fprintf(stderr, "dup2() failed\n");
            exit(-1);
        }
        close(pipe_fd[0]);
        close(pipe_fd[1]);
        
        int newSize = length - (pipeIndex + 1);
        char** newArgv = (char**)malloc(newSize * sizeof(char*));
        if(newArgv == NULL) {
            fprintf(stderr, "malloc failed");
            freeArgv(newArgv);
            exit(-1);
        }
        for(int i = 0; i < newSize - 1; i++) {
            newArgv[i] = (char*)malloc((strlen(argv[pipeIndex + 1 + i]) * sizeof(char)));   //creating new array for the commands after the pipe
            if(newArgv[i] == NULL) {
                fprintf(stderr, "malloc failed");
                freeArgv(newArgv);
                exit(-1);
            }
            strcpy(newArgv[i], argv[pipeIndex + 1 + i]);
            newArgv[i+1] = NULL;
        }
        
        checkNohup(newArgv);

        if(newSize == 1) {
            if(checkHistory(newArgv[0]) == 0) {
                freeArgv(newArgv);
                exit(0);
            }
        }

        //checks for any more pipes
        for(int i = 0; newArgv[i] != NULL; i++) {
            if(newArgv[i][0] == '|') {
                int status = runPipe(newArgv, i, newSize);
                freeArgv(newArgv);
                exit(status);
            }
        }
        //executes the command entered by the user in the child process
        if(execvp(newArgv[0], newArgv) == -1) { //runs the 2nd command
            perror("execvp() failed");
            freeArgv(newArgv);
            exit(-1);
        }
        exit(0);
    }
    close(pipe_fd[0]);
    close(pipe_fd[1]);
    
    signal(SIGHUP, SIG_DFL);

    int status1 = 0, status2 = 0;
    if(runInBackground == false) {
        waitpid(pd, &status1, 0);
        waitpid(child_pd, &status2, 0);
    }

    int returnStatus = 0;
    if(status1 < status2) returnStatus = status1;
    else returnStatus = status2;

    return returnStatus;

}

//counts the words and chars in a given line
void getWordsNChars(char* line, int* words, int* totalChars) {
    int chars = 0;          //counts the characters in one word
    int i = 0;
    char c = line[i];
    while(c != '\n' && c != '\0') {
        if(c == ' ') {
            if(chars > 0) {
                *totalChars += chars;
                *words += 1;
                chars = 0;
            }
            c = line[++i];
            continue;
        }
        chars++;
        c = line[++i];
    }
    if(chars > 0) {
        *totalChars += chars;
        *words += 1;
        chars = 0;
    }
}

//takes a line of text and turns it to an array of words
char** toDoubleArr(char* line, int* length) {
    int i = 0;
    char c = line[i];

    while(c != '\n' && c != '\0') {
        if(c == '|') {
            if(line[i-1] != ' ') {
                strncpy(line + i + 1, line + i, strlen(line) - i);
                line[i++] = ' ';
                *length += 1;
            }
            if(line[i+1] != ' ') {
                strncpy(line + i + 1, line + i, strlen(line) - i);
                line[i+1] = ' ';
                *length += 1;
            }
        }
        c = line[++i];
    }

    char** arr = (char**)malloc(*length * sizeof(char*));
    if(arr == NULL) {
        fprintf(stderr, "malloc failed");
        exit(1);
    }

    int words = 0;
    int chars = 0;
    char word[MAXLEN];  //temp word

    i = 0;
    c = line[i];
    printf("secondQuote: %d\n", secondQuote);

    while(c != '\n' && c != '\0') {
        if(c == ' ') {
            if(chars > 0) {
                word[chars] = 0;
                arr[words] = (char*)malloc(((strlen(word) + 1) * sizeof(char)));
                if(arr[words] == NULL) {
                    fprintf(stderr, "malloc failed");
                    freeArgv(arr);
                    exit(1);
                }
                strcpy(arr[words], word);
                words++;
                chars = 0;
            }
            c = line[++i];
            continue;
        }
        if(i != firstQuote && i != secondQuote)
            word[chars++] = c;
        c = line[++i];
    }
    if(chars > 0) {
        word[chars] = 0;
        arr[words] = (char*)malloc((strlen(word) * sizeof(char)));
        strcpy(arr[words], word);
        words++;
        chars = 0;
    }
    arr[words] = NULL;
    
    return arr;
}

//writes the given sentence into a file with the given name
void writeHistory(char* line) {
    FILE* file = fopen(fileName, "a");
    if(file == NULL) {
        printf("Couldn't open file!\n");
        exit(1);
    }
    fprintf(file,"%s", line);
    if(line[strlen(line) - 1] != '\n')
        fprintf(file,"\n");
    fclose(file);
}

//prints all of the content in a file given a file name
void printHistory(char* fileName) {
    FILE* file = fopen(fileName, "r");
    if(file == NULL) {
        printf("Couldn't open file!\n");
        exit(1);
    }
    
    int numOfLine = 1;
    char line[MAXLEN];
    while(fgets(line, MAXLEN, file)) {
        printf("%d: %s",numOfLine++, line);
    }
    fclose(file);
}