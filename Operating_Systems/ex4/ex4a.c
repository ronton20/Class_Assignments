//Author:           Ron Amar
//ID:               318479458

//How to compile:   gcc ex4a.c -o ex4a -lpthread
//How to run:       ./ex4a

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <pthread.h>

#define true 1
#define false 0
#define MAXLEN 512
#define INT_LEN 20
#define NUM_OF_VARS 4

//=============== G L O B A L === V A R I A B L E S ===============

int coeffs[NUM_OF_VARS];
int pows[NUM_OF_VARS];
int numOfCoeffs;
int numOfPows;
int val;

//=========== P R E D E C L A R I N G === M E T H O D S ===========

void run();
int checkInput(char*);
int checkPolynom(char*);
int checkVal(char*);
void generateArrays(char*);
int getVal(char*);
int power(int, int);
void* calc(void*);

void printSum(int);
void printArrays();

//============================ M A I N ============================

int main() {
    run();
    return(EXIT_SUCCESS);
}

//========================= M E T H O D S =========================

void run() {
    int done = false;
    do {
        numOfCoeffs = 0;
        numOfPows = 0;
        char input[MAXLEN];
        printf("Enter polynom (POLYNOM, VAL):\n");
        fgets(input, MAXLEN, stdin);
        input[strcspn(input, "\n")] = 0;    //removing '\n'

        //checks if 'done' was entered
        if(strcmp(input, "done") == 0) {
            done = true;
            printf("Goodbye!\n");
            continue;
        }

        if(checkInput(input) == EXIT_FAILURE) { //checks for invalid input
            printf("Invalid input.\n");
            continue;
        }

        generateArrays(input);
        val = getVal(input);

        pthread_t threadsArray[numOfPows];
        int indexes[numOfPows];

        //creating the threads to claculate the polynom
        for(int i = 0; i < numOfPows; i++) {
            indexes[i] = i;
            if(pows[i] != 0) {
                if(pthread_create(&threadsArray[i], NULL, calc, &indexes[i]) != 0) {
                    fprintf(stderr, "pthread_create() failed.\n");
                    exit(EXIT_FAILURE);
                }
            }
        }

        int sum = 0;
        //joining the threads and summing up the result
        for(int i = 0; i < numOfPows;) {
            if(pows[i] == 0) {
                sum += coeffs[i];
                i++;
                continue;
            } else {
                void* retValue;
                if(pthread_join(threadsArray[i], &retValue) == 0) {
                    // printf("Thread #%d joined\n", i);
                    int val = *((int*)retValue);
                    // printf("sum %d: %d\n", i + 1, val);  //remove comment to see each thread's result
                    sum += val;
                    free(retValue);
                    retValue = NULL;
                    i++;
                }
            }
        }

        printSum(sum);

    } while(done == false);

}

//checks if the input is valid
int checkInput(char* input) {
    if(input == NULL) return EXIT_FAILURE;
    if(strlen(input) == 0) return EXIT_FAILURE;
    if(strcspn(input, " ") < strcspn(input, ",")) return EXIT_FAILURE;
    if(checkPolynom(input) == EXIT_FAILURE) return EXIT_FAILURE;
    if(checkVal(input) == EXIT_FAILURE) return EXIT_FAILURE;

    return EXIT_SUCCESS;
}

//checks if the polygon entered is valid
int checkPolynom(char* input) {
    int end = strcspn(input, ",");
    for(int i = 0; i < end; i++) {
        //checks if there is an invalid char in the polynom
        if(!((input[i] >= '0' && input[i] <= '9') || input[i] == 'x' || input[i] == '+' || input[i] == '^' || input[i] == '*')) return EXIT_FAILURE;
    }
    return EXIT_SUCCESS;
}

//checks if Val formatting is valid
int checkVal(char* input) {
    int start = strcspn(input, ",") + 1;
    if(input[start++] != ' ') return EXIT_FAILURE;
    for(int i = start; input[i] != 0; i++) {
        if(input[i] < '0' || input[i] > '9') return EXIT_FAILURE;
    }
    return EXIT_SUCCESS;
}

//creates the global arrays containing the coeffs and powers
void generateArrays(char* input) {
    char coeff[INT_LEN];
    char pow[INT_LEN];

    int coeffIndex = 0;
    int powIndex = 0;

    for(int i = 0; i <= strcspn(input, ","); i++) {
        char c = input[i];
        if(numOfCoeffs == numOfPows) {      //reading coeff
            if(c >= '0' && c <= '9') {
                coeff[coeffIndex++] = input[i];
                coeff[coeffIndex] = 0;
            } else if(c == '*') {
                coeffs[numOfCoeffs++] = atoi(coeff);
                coeffIndex = 0;
            } else if(c == 'x') {
                coeffs[numOfCoeffs++] = 1;
            } else if(c == ',') {
                coeffs[numOfCoeffs++] = atoi(coeff);
                coeffIndex = 0;
                pows[numOfPows++] = 0;
            }
        } else {                            //reding pow
            if(c == '^' || c == 'x') continue;
            if(c >= '0' && c <= '9') {
                pow[powIndex++] = input[i];
                pow[powIndex] = 0;
            } else if(c == '+' || c == ',') {
                pows[numOfPows++] = atoi(pow);
                powIndex = 0;
            }
        }
    }
}

//returns the [val] to calculate
int getVal(char* input) {
    int start = strcspn(input, ", ") + 2;
    char val[INT_LEN];
    int valIndex = 0;
    for(int i = start; input[i] != 0; i++) {
        char c = input[i];
        if(c >= '0' && c <= '9') {
            val[valIndex++] = input[i];
            val[valIndex] = 0;
        }
    }
    int res = atoi(val);
    return res;
}

//returns x^y
int power(int x, int y) {
    int res = 1;
    for(int i = 0; i < y; i++)
        res *= x;
    return res;
}

//this method calculates a single cell in a polynom
void* calc(void* _index) {
    if(_index == NULL) return NULL;
    int index = *((int*)_index);
    // printf("Thread #%d created\n", index);
    int* res = (int*)malloc(sizeof(int));
    *res = power(val, pows[index]) * coeffs[index];
    pthread_exit((void*)res);
}

//method for printing the polynom and its sum
void printSum(int sum) {
    for(int i = 0; i < numOfCoeffs; i++) {
        printf("%d", coeffs[i]);
        if(pows[i] != 0) printf("*%d^%d",val, pows[i]);
        if(i < numOfCoeffs - 1) printf(" + ");
        else printf(" = %d\n", sum);
    }
}

//optional method for printing the arrays to see the coeffs and powers
void printArrays() {
    printf("Coeffs:\n");
    for(int i = 0; i < numOfCoeffs; i++) {
        if(i==0) printf("[");
        printf("%d", coeffs[i]);
        if(i < numOfCoeffs - 1) printf(", ");
        else printf("]\n");
    }

    printf("Pows:\n");
    for(int i = 0; i < numOfPows; i++) {
        if(i==0) printf("[");
        printf("%d", pows[i]);
        if(i < numOfPows - 1) printf(", ");
        else printf("]\n");
    }
}
