//Author: Ron Amar
//ID: 318479458

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define true 1
#define false 0

#define N 20            //defining the size of all the arrays in the program
int numOfCandidates;    //will contain the number of candidates currently in the system
int numOfWorkers;       //will contain the number of employees currently in the system

//structure for recommenders from outside the company
typedef struct Recommender {
    char firstName[N];
    char lastName[N];
    char eMail[N];
} Recommender;

//structure for every employee or candidate, as well as recommenders who are employees of the company
typedef struct Developer {
    char firstName[N];
    char lastName[N];
    char degree[N];
    struct Developer *dev1, *dev2;      //recommenders from inside the company
    struct Recommender *rec1, *rec2;    //recommenders from outside the company
} Developer;

//private methods declaration
void printCandidates(Developer *candidates[]);
void printWorkers(Developer *employees[]);
void insertCandidate(Developer *candidates[], Developer *employees[]);
void hireCandidate(Developer *employees[], Developer *candidates[]);
int hasRecommendersFromCompany(Developer *candidate);
int hasRecommendersFromOutside(Developer *candidate);
int isInside(Developer *workers[], char firstName[], char lastName[]);
void printDev(Developer *dev);
void printRec(Recommender *rec);
void printWorker(Developer *dev);
void removeRecommenders(Developer *dev);
void freeAllMemory(Developer *workers[], Developer *candidates[]);

int main() {

    numOfCandidates = 0;
    numOfWorkers = 0;

    Developer *employees[N], *candidates[N];
    for (int i = 0; i < N; i++) {
        employees[i] = NULL;
        candidates[i] = NULL;
    }

    int exit = false;

    do {
        printf("============== W E L C O M E ==============\n");
        printf("What do you want to do?\n");
        printf("1. Print candidates.\n");
        printf("2. Print workers.\n");
        printf("3. Insert new candidate.\n");
        printf("4. Hire candidate.\n");
        printf("5. Exit.\n");

        int choice;
        scanf("%d", &choice);
        getchar();

        switch (choice) {
            case 1:
                printCandidates(candidates);
                break;
            case 2:
                printWorkers(employees);
                break;
            case 3:
                insertCandidate(candidates, employees);
                break;
            case 4:
                hireCandidate(employees, candidates);
                break;
            case 5:
                freeAllMemory(employees, candidates);
                printf("Goodbye!\n");
                exit = true;
                break;
            default:
                printf("Please enter a valid number.\n");
        };
    } while(exit == false);

    return 0;
}

//method for printing all the candidates
void printCandidates(Developer *candidates[]) {
    if(candidates[0] == NULL) {
        printf("No candidates found.\n");
        printf("\n");
        return;
    }
    printf("====== C A N D I D A T E === L I S T ======\n");
    for(int i = 0; i < N; i++) {
        if(candidates[i] == NULL) return;
        printf("-------- Candidate %d --------\n", (i + 1));
        printDev(candidates[i]);
        if(hasRecommendersFromCompany(candidates[i]) == true) {
            printf("Recommenders from inside the company:\n");
            if(candidates[i]->dev1 != NULL) {
                printWorker(candidates[i]->dev1);
            }
            if(candidates[i]->dev2 != NULL) {
                printWorker(candidates[i]->dev2);
            }
        }
        if(hasRecommendersFromOutside(candidates[i]) == true) {
            printf("Recommenders from outside the company:\n");
            if(candidates[i]->rec1 != NULL) {
                printRec(candidates[i]->rec1);
            }
            if(candidates[i]->rec2 != NULL) {
                printRec(candidates[i]->rec2);
            }
        }
        printf("\n");
    }
}

//method for printing all the workers
void printWorkers(Developer *employees[]) {
    if(employees[0] == NULL) {
        printf("No workers found.\n");
        printf("\n");
        return;
    }
    printf("======= E M P L O Y E E === L I S T =======\n");
    for(int i = 0; i < N; i++) {
        if (employees[i] == NULL) {
            return;
        }
        printf("%d. ", (i + 1));
        printWorker(employees[i]);
        printf("\n");
    }
    printf("\n");
}

//method for inserting a new candidate
void insertCandidate(Developer *candidates[], Developer *employees[]) {
    if(numOfCandidates >= N)
        printf("No space for any more candidates.\n");
    for(int i = 0; i < N; i++) {
        if(candidates[i] == NULL) {
            //allocating space for the new candidate
            candidates[i] = (struct Developer*)malloc(sizeof(Developer));
            candidates[i]->dev1 = NULL;
            candidates[i]->dev2 = NULL;
            candidates[i]->rec1 = NULL;
            candidates[i]->rec2 = NULL;

            printf("==== I N S E R T === C A N D I D A T E ====\n");
            printf("Insert candidate details:\n");
            printf("First name:\n");
            fgets(candidates[i]->firstName, N, stdin);
            candidates[i]->firstName[strcspn(candidates[i]->firstName, "\n")] = 0;

            printf("Last name:\n");
            fgets(candidates[i]->lastName, N, stdin);
            candidates[i]->lastName[strcspn(candidates[i]->lastName, "\n")] = 0;

            printf("Degree:\n");
            fgets(candidates[i]->degree, N, stdin);
            candidates[i]->degree[strcspn(candidates[i]->degree, "\n")] = 0;

            //checking for recommenders from inside the company
            int finish = false;
            int flag = false;
            do {
                printf("\nAny recommenders from inside the company?\n");
                int recs;
                printf("1. Yes.\n");
                printf("2. No.\n");
                scanf("%d", &recs);
                getchar();

                int counter;
                switch (recs) {
                    case 1:
                        counter = 0;
                        flag = false;
                        do {
                            if(candidates[i]->dev1 != NULL && candidates[i]->dev2 != NULL) {
                                printf("No more recommenders can be added.\n");
                                flag = true;
                                finish = true;
                                break;
                            }
                            if(employees[0] == NULL) {
                                printf("No employees found that could be recommenders.\n");
                                flag = true;
                                finish = true;
                                break;
                            }
                            char name1[N];
                            char name2[N];
                            printf("Enter recommender's first name:\n");
                            fgets(name1, N, stdin);
                            name1[strcspn(name1, "\n")] = 0;
                            printf("Enter recommender's last name:\n");
                            fgets(name2, N, stdin);
                            name2[strcspn(name2, "\n")] = 0;

                            //making sure the entered recommender is indeed from the company
                            for(int j = 0; j < N; j++) {
                                if(employees[j] == NULL) {
                                    flag = true;
                                    break;
                                }
                                if(isInside(employees, name1, name2) == true) {
                                    if(candidates[i]->dev1 == NULL) candidates[i]->dev1 = employees[j];
                                    else if(candidates[i]->dev2 == NULL) candidates[i]->dev2 = employees[j];
                                    printf("Recommender registered successfully.\n");
                                    flag = true;
                                    break;
                                }
                                else {
                                    if(counter < 2) {
                                        printf("Please enter an employee of the company\n");
                                        counter++;
                                    }
                                    else {
                                        printf("Failed to add recommender.\n");
                                        flag = true;
                                    }
                                }
                            }
                        } while(flag == false);
                        break;
                    case 2:
                        finish = true;
                        break;
                    default:
                        printf("Please enter a valid option.\n");
                };
            } while(finish == false);

            //checking for recommenders from outside the company
            finish = false;
            do {
                printf("\nAny recommenders from outside the company?\n");
                int recs;
                printf("1. Yes.\n");
                printf("2. No.\n");
                scanf("%d", &recs);
                getchar();

                switch (recs) {
                    case 1:
                        if(candidates[i]->rec1 != NULL && candidates[i]->rec2 != NULL) {
                            printf("No more place for recommenders from outside the company.\n");
                            finish = true;
                            break;
                        }
                        else if(candidates[i]->rec1 == NULL) {
                            candidates[i]->rec1 = (struct Recommender*)malloc(sizeof(Recommender));
                            printf("enter recommender's first name:\n");
                            fgets(candidates[i]->rec1->firstName, N, stdin);
                            candidates[i]->rec1->firstName[strcspn(candidates[i]->rec1->firstName, "\n")] = 0;
                            printf("enter recommender's last name:\n");
                            fgets(candidates[i]->rec1->lastName, N, stdin);
                            candidates[i]->rec1->lastName[strcspn(candidates[i]->rec1->lastName, "\n")] = 0;
                            printf("enter recommender's eMail address:\n");
                            fgets(candidates[i]->rec1->eMail, N, stdin);
                            candidates[i]->rec1->eMail[strcspn(candidates[i]->rec1->eMail, "\n")] = 0;
                            printf("Recommender registered successfully.\n");
                        }
                        else {
                            candidates[i]->rec2 = (struct Recommender*)malloc(sizeof(Recommender));
                            printf("enter recommender's first name:\n");
                            fgets(candidates[i]->rec2->firstName, N, stdin);
                            candidates[i]->rec2->firstName[strcspn(candidates[i]->rec2->firstName, "\n")] = 0;
                            printf("enter recommender's last name:\n");
                            fgets(candidates[i]->rec2->lastName, N, stdin);
                            candidates[i]->rec2->lastName[strcspn(candidates[i]->rec2->lastName, "\n")] = 0;
                            printf("enter recommender's eMail address:\n");
                            fgets(candidates[i]->rec2->eMail, N, stdin);
                            candidates[i]->rec2->eMail[strcspn(candidates[i]->rec2->eMail, "\n")] = 0;
                            printf("Recommender registered successfully.\n");
                        }
                        break;
                    case 2:
                        finish = true;
                        break;
                    default:
                        printf("Please enter '1' or '2'.\n");
                }
            } while(finish == false);

            numOfCandidates++;
            return;
        }
    }
}

//method for hiring a candidate into the company
void hireCandidate(Developer *employees[], Developer *candidates[]) {
    if(numOfWorkers >= N) {
        printf("No more room for workers.\n");
        return;
    }
    printf("====== H I R E === C A N D I D A T E ======\n");
    printf("Who would you like to hire?\n");
    char name1[N];
    char name2[N];
    printf("Enter candidate's first name:\n");
    fgets(name1, N, stdin);
    name1[strcspn(name1, "\n")] = 0;
    printf("Enter candidate's last name:\n");
    fgets(name2, N, stdin);
    name2[strcspn(name2, "\n")] = 0;

    if(isInside(candidates, name1, name2) == true){
        for(int i = 0; i < N; i++) {
            if((strcmp(candidates[i]->firstName, name1) == 0) && (strcmp(candidates[i]->lastName, name2) == 0)) {
                employees[numOfWorkers] = candidates[i];
                candidates[i] = candidates[numOfCandidates-1];
                candidates[numOfCandidates-1] = NULL;
                removeRecommenders(employees[numOfWorkers]);
                numOfCandidates--;
                numOfWorkers++;
                printf("Candidate hired successfully!\n");
                printf("\n");
                return;
            }
        }
    }
    else
        printf("This person is not a candidate.\n");
}

//method for checking if a candidate has recommendations from a worker in the company
int hasRecommendersFromCompany(Developer *candidate) {
    if(candidate->dev1 != NULL || candidate->dev2 != NULL) return true;
    return false;
}

//method for checking if a candidate has recommendations from outside the company
int hasRecommendersFromOutside(Developer *candidate) {
    if(candidate->rec1 != NULL || candidate->rec2 != NULL) return true;
    return false;
}

//method for printing the name and degree of a developer
void printDev(Developer *dev) {
    printf("Name: %s %s\n", dev->firstName, dev->lastName);
    printf("Degree: %s\n", dev->degree);
}

//method for printing the name and email of a recommender
void printRec(Recommender *rec) {
printf("Name: %s %s\n", rec->firstName, rec->lastName);
printf("eMail: %s\n", rec->eMail);
}

//method for printing the name and degree of a developer
void printWorker(Developer *dev) {
    printf("%s %s", dev->firstName, dev->lastName);
    printf("\n");
}

//method for checking if a person is inside the array
int isInside(Developer *workers[], char firstName[], char lastName[]) {
    for(int i = 0; i < N; i++) {
        if(workers[i] == NULL) return false;
        if ((strcmp(firstName, workers[i]->firstName) == 0) && (strcmp(lastName, workers[i]->lastName) == 0))
            return true;
    }
    return false;
}

//method for clearing all recommenders from a developer
void removeRecommenders(Developer *dev) {
    if(dev == NULL) return;
    if(dev->dev1 != NULL) dev->dev1 = NULL;
    if(dev->dev2 != NULL) dev->dev2 = NULL;
    if(dev->rec1 != NULL) {
        free(dev->rec1);
        dev->rec1 = NULL;
    }
    if(dev->rec2 != NULL) {
        free(dev->rec2);
        dev->rec2 = NULL;
    }
}

//method for wiping the memory clean
void freeAllMemory(Developer *workers[], Developer *candidates[]) {
    for(int i = 0; i < N; i++) {
        if(candidates[i] != NULL) {
            removeRecommenders(candidates[i]);
            free(candidates[i]);
        }
        if(workers[i] != NULL)
            free(workers[i]);
    }
    printf("Memory wiped successfully.\n");
}