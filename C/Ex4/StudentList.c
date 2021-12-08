
#include "StudentList.h"
#include <stdio.h>
#include <stdlib.h>

//method to create a list of student nodes
StudentList* createList() {
    StudentList *list = (StudentList*)malloc(sizeof(StudentList));
    if(list == NULL) exit(1);
    list->head = NULL;

    return list;
}

//deletes all the students from the list and deleting the list itself
void deleteList(StudentList *list) {
    if(list == NULL) return;

    StudentNode *temp = list->head;
    while(temp != NULL) {
        list->head = list->head->next;
        free(temp->st);
        free(temp);
        temp = list->head;
    }
    free(list);
    list = NULL;
}

//adds a student node to the bottom of the student list
void addToList(StudentList *list, Student *student) {
    if(student == NULL) {
        printf("ERROR");
        return;
    }

    StudentNode *node = (StudentNode*) malloc(sizeof(StudentNode));
    if(node == NULL) exit(1);
    node->st = student;
    node->next = NULL;

    if(list->head == NULL) {
        list->head = node;
        return;
    }

    StudentNode* temp = list->head;
    while(temp->next != NULL) {
        temp = temp->next;
    }
    temp->next = node;
}

//prints a specific node
void printStudentNode(StudentNode* const node) {
    if(node->st == NULL) return;
    printf("ID: ");
    if(node->st->id < 1000000)
        printf("0");
    printf("%d\t Grade 1: %d\t Grade 2: %d\n", node->st->id, node->st->grade1, node->st->grade2);
}

//prints the whole list of nodes
void printStudentList(StudentList* const list) {
    if(list == NULL || list->head == NULL) {
        printf("List is empty.\n");
        return;
    }
    StudentNode *temp = list->head;
    while(temp != NULL) {
        printStudentNode(temp);
        temp = temp->next;
    }
}

