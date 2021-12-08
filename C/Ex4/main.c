//Author: Ron Amar
//ID: 318479458

#include "StudentList.h"
#include "GradesOperations.h"
#include <stdio.h>
#include <stdlib.h>
#include <libgen.h>
#include <string.h>

int main()
{
    char filePath[] = "StudentsGrades.txt"; //can enter any path to a file (as long as it exists), should work as intended
    char resultFileName[] = "/Report.txt";  //keep "/" at the begginning, otherwise the file will not be created properly

    FILE* file = fopen(filePath, "r");
    if(file == NULL) {
        printf("Couldnt open grades file.\n");
        exit(0);
    }
    
    //getting the parent path of the file
    char* realPath = realpath(filePath, NULL);
    char* parentPath = dirname(realPath);
    
    //setting the path of the report file
    char * newPath ;
    if((newPath = malloc(strlen(parentPath) + strlen(resultFileName) + 1)) != NULL){
        newPath[0] = '\0';   // ensures the memory is an empty string
        strcat(newPath, parentPath);
        strcat(newPath, resultFileName);    //appending the report file name to the parent path
    } else {
        printf("malloc failed!\n");
        exit(0);
    }
    
    StudentList* list = createList();
    int id, g1, g2;
    
    //checking if the file is empty
    int c = fgetc(file);
    if (c == EOF) {
        printf("Grades file is empty.\n");
    } else {
        ungetc(c, file);

        //adding all the students in the grades file to a Studentlist
        while(!feof(file)) {
            fscanf(file, "%d%d%d", &id, &g1, &g2);
            Student *student = (Student*)malloc(sizeof(Student));
            if(student == NULL) exit(1);
            student->id = id;
            student->grade1 = g1;
            student->grade2 = g2;
            addToList(list, student);
        }
    }
    fclose(file);
    
    //initializing the default states of every parameter we are going to check
    int numOfStudents = 0, onlyOneTest = 0, noTests = 0, highestGradeID = 0, lowestGradeID = 0;
    double failPercentageA = 0, failPercentageB = 0, avarageGradeA = 0, avarageGradeB = 0;
    
    //utilizing all the methods in "GradeOperations.h" to get all the info to put into the report file
    numOfStudents = countStudents(list);
    onlyOneTest = countOneTesters(list);
    noTests = countDitchers(list);
    updateHighAndLow(list, &highestGradeID, &lowestGradeID);
    updateFailPercetages(list, &failPercentageA, &failPercentageB);
    updateAvarages(list, &avarageGradeA, &avarageGradeB);
    
    //creating a report file and writing all the information into it
    FILE* report = fopen(newPath, "w");
    if(report == NULL) exit(0);
    
    fprintf(report, "========================================================================\n");
    fprintf(report, "Number of Students included in the grades file:           \t%d\n", numOfStudents);
    fprintf(report, "Number of students that took part in only one test period:\t%d\n", onlyOneTest);
    fprintf(report, "Number of students that didnt take part in any exam:      \t%d\n", noTests);
    fprintf(report, "========================================================================\n");
    fprintf(report, "Percentage of failures in the first term of the exam:     \t%.2f%%\n", failPercentageA);
    fprintf(report, "Percentage of failures in the second term of the exam:    \t%.2f%%\n", failPercentageB);
    fprintf(report, "Avarage grade in the first term of the exam:              \t%d\n", (int)avarageGradeA);
    fprintf(report, "Avarage grade in the second term of the exam:             \t%d\n", (int)avarageGradeB);
    fprintf(report, "========================================================================\n");
    fprintf(report, "The student who got the highest grade in the entire exam: \t%d\n", highestGradeID);
    fprintf(report, "The student who got the lowest grade in the entire exam:  \t%d\n", lowestGradeID);
    fprintf(report, "========================================================================\n");
    
    fclose(report);

    printStudentList(list); //not necessary, just prints the StudentList to show whats inside of it for comparison
    deleteList(list);       //deletes the list and all the students
    free(newPath);          //freeing the path we allocated for the report file

    return 0;
}
