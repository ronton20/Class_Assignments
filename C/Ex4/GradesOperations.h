
#ifndef GRADES_OPERATIONS_H
#define GRADES_OPERATIONS_H

#include "StudentList.h"

int countStudents(StudentList* const list);
int countOneTesters(StudentList* const list);
int countDitchers(StudentList* const list);
void updateHighAndLow(StudentList* const list, int* highID, int* lowID);
void updateFailPercetages(StudentList* const list, double* failPercentA, double* failPercentB);
void updateAvarages(StudentList* const list, double* avarageA, double* avarageB);

#endif //GRADES_OPERATIONS_H
