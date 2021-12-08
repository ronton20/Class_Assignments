#include "GradesOperations.h"
#include <stdio.h>
#include <limits.h>

//method for counting the total number of students in a list
int countStudents(StudentList* const list) {
    if(list == NULL || list->head == NULL) return 0;
    
    int counter = 0;
    StudentNode* temp = list->head;
    while(temp != NULL) {
        counter++;
        temp = temp->next;
    }
    return counter;
}

//method for counting the number of students that attended only 1 test
int countOneTesters(StudentList* const list) {
    if(list == NULL || list->head == NULL) return 0;
    
    int counter = 0;
    StudentNode* temp = list->head;
    while(temp != NULL) {
        if((temp->st->grade1 == -1 && temp->st->grade2 != -1) || (temp->st->grade1 != -1 && temp->st->grade2 == -1))
            counter++;
        temp = temp->next;
    }
    return counter;
}

//method for counting the number of students that didnt attend any test
int countDitchers(StudentList* const list) {
    if(list == NULL || list->head == NULL) return 0;
    
    int counter = 0;
    StudentNode* temp = list->head;
    while(temp != NULL) {
        if((temp->st->grade1 == -1 && temp->st->grade2 == -1))
            counter++;
        temp = temp->next;
    }
    return counter;
}

//method for checking who are the students with the highest and lowest test scores and updating the corresponding pointers
void updateHighAndLow(StudentList* const list, int* highID, int* lowID) {
    if(list == NULL || list->head == NULL) return;
    
    int highest = INT_MIN;
    int lowest = INT_MAX;
    StudentNode* temp = list->head;
    while(temp != NULL) {
        if(temp->st->grade1 > highest) {
            *highID = temp->st->id;
            highest = temp->st->grade1;
        }
        if(temp->st->grade2 > highest) {
            *highID = temp->st->id;
            highest = temp->st->grade2;
        }
        if(temp->st->grade1 < lowest) {
            if(temp->st->grade1 >= 0) {
                *lowID = temp->st->id;
                lowest = temp->st->grade1;
            }
        }
        if(temp->st->grade2 < lowest) {
            if(temp->st->grade2 >= 0) {
                *lowID = temp->st->id;
                lowest = temp->st->grade2;
            }
        }
        temp = temp->next;
    }
}

//method for calculating the avarages on both periods of the exam
void updateFailPercetages(StudentList* const list, double* failPercentA, double* failPercentB) {
    if(list == NULL || list->head == NULL) return;
    
    int studentsA = 0;
    int failsA = 0;
    int studentsB = 0;
    int failsB = 0;
    
    StudentNode* temp = list->head;
    while(temp != NULL) {
        if(temp->st->grade1 >= 0) {
            studentsA++;
            if(temp->st->grade1 < 60) {
                failsA++;
            }
        }
        if(temp->st->grade2 >= 0) {
            studentsB++;
            if(temp->st->grade2 < 60) {
                failsB++;
            }
        }
        temp = temp->next;
    }
    *failPercentA = ((double)failsA / studentsA) * 100;
    *failPercentB = ((double)failsB / studentsB) * 100;
}

//method for calculating the avarage score for each of the test periods and updating the corresponding pointers
void updateAvarages(StudentList* const list, double* avarageA, double* avarageB) {
    if(list == NULL || list->head == NULL) return;
    
    int studentsA = 0;
    int sumA = 0;
    int studentsB = 0;
    int sumB = 0;
    
    StudentNode* temp = list->head;
    while(temp != NULL) {
        if(temp->st->grade1 >= 0) {
            studentsA++;
            sumA += temp->st->grade1;
        }
        if(temp->st->grade2 >= 0) {
            studentsB++;
            sumB += temp->st->grade2;
        }
        temp = temp->next;
    }
    *avarageA = (double)sumA / studentsA;
    *avarageB = (double)sumB / studentsB;
}