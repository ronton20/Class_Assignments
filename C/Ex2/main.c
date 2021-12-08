//Author: Ron Amar
//ID: 318479458

#include <stdio.h>
#include <string.h>
#define true 1
#define false 0

void addBigNumbers(char first[], char second[], char result[]);
int allNumbers(char arr[]);
void sortBigNumbers(char* bigNumbers[], int N);
void moveDown(char *bigNumbers[], int i);



//this method checks if the array contains only numbers
int allNumbers(char arr[]) {
    int arrLen = strlen(arr);
    for(int i = 0; i < arrLen; i++) {
        if(!(arr[i] >= '0' && arr[i] <= '9')) return false;
    }
    return true;
}


//this method adds 2 "Stringy" numbers and puts the result into the 3rd given array (result[])
void addBigNumbers(char first[], char second[], char result[]) {
    char temp[200];
    const int DIFF = 48;
    int firstLen = strlen(first);
    int secondLen = strlen(second);
    int carry = 0;
    int sum = 0;
    int i = 0;          //index for the longer array
    int j = 0;          //index for the shorter array
    int index = 0;      //index for the result array
    int biggest = 0;    //will determine which array is bigger (first - 1, second - 2)
    
    //makes sure all chars in the array are numbers
    if(allNumbers(first) == false || allNumbers(second) == false) {
        strcpy(result, "error");
        return;
    }
    
    //determines who the bigger array is
    if(firstLen >= secondLen) {
        i = firstLen - 1;
        j = secondLen - 1;
        biggest = 1;
    }
    else {
        i = secondLen - 1;
        j = firstLen - 1;
        biggest = 2;
    }
    
    //the main function that calculates the result
    while(i >= 0) {
        if(j >= 0) {
            switch(biggest) {
                case 1:
                    sum = ((int)first[i] - DIFF) + ((int)second[j] - DIFF);
                    break;
                case 2:
                    sum = ((int)second[i] - DIFF) + ((int)first[j] - DIFF);
            }
            if(carry > 0) {
                sum += carry;
                carry = 0;
            }
            if(sum >= 10) {
                carry++;
                sum -= 10;
            }
            temp[index] = (char)(sum + DIFF);
        }
        //if the smaller number ended
        else {
            if(carry > 0) 
            {
                switch(biggest) 
                {
                    case 1:
                        sum = ((int)first[i] - DIFF);
                        break;
                    case 2:
                        sum = ((int)second[i] - DIFF);
                }
                sum += carry;
                carry = 0;
                
                if(sum >= 10) {
                    carry = 1;
                    sum -= 10;
                }
                
                temp[index] = (char)(sum + DIFF);
            }
            else {
                switch(biggest) {
                    case 1:
                        temp[index] = first[i];
                        break;
                    case 2:
                        temp[index] = second[i];
                }
            }
        }
        i--;
        j--;
        index++;
    }
    temp[index] = '\0';
    
    //checking if there is a left over carry
    i = 0;
    if(carry > 0) {
        result[i] = (char)(carry + DIFF);
        i++;
        carry = 0;
    }
        
    //moving the result to result[]
    index--;
    while(index >= 0) {
        result[i] = temp[index];
        i++;
        index--;
    }
    result[i] = '\0';
}

//a method for sorting an array of pointers of big number arrays
void sortBigNumbers(char* bigNumbers[], int N) {
    if(N <= 1) return;
    for(int i = 1; i < N; i++) {
        int sizeA = strlen(bigNumbers[i]);
        int sizeB = strlen(bigNumbers[(i-1)]);
        if(sizeA < sizeB) {
            moveDown(bigNumbers, i);
        }
    }
}

//sorting the bigger size number to its appropriate place in the array
void moveDown(char *bigNumbers[], int i) {
    while((strlen(bigNumbers[i]) < strlen(bigNumbers[i-1])) && i > 0) {
        char *temp = bigNumbers[i];
        bigNumbers[i] = bigNumbers[i-1];
        bigNumbers[i-1] = temp;
        i--;
    }
}

