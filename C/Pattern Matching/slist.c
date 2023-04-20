#include <stdio.h>
#include <stdlib.h>
#include "slist.h"

//initializing dbllist
void dbllist_init(dbllist_t *list) {
    if(list == NULL) return;
    dbllist_head(list) = NULL;
    dbllist_tail(list) = NULL;
    dbllist_size(list) = 0;
}

//destroying a dbllist - NOT freeing the pointer (in case of static allocation) - free by user
void dbllist_destroy(dbllist_t *list, dbllist_destroy_t dealloc) {
    if(list == NULL) return;

    //freeing all the nodes in the list
    while(dbllist_head(list) != NULL) {
        dbllist_node_t* curr = dbllist_head(list);
        dbllist_head(list) = dbllist_next(curr);

        //freeing the data in the current node (if needed)
        if(dealloc == DBLLIST_FREE_DATA) free(dbllist_data(curr));
        free(curr);
    }
}

//adds a new node to the end of the list
int dbllist_append(dbllist_t *list, void *data) {
    if(list == NULL) return -1;
    dbllist_node_t* node = (dbllist_node_t*)malloc(sizeof(dbllist_node_t));
    if(node == NULL) {
        fprintf(stderr, "malloc() failed\n");
        return -1;
    }
    dbllist_data(node) = data;
    dbllist_next(node) = NULL;
    dbllist_prev(node) = dbllist_tail(list);

    //if list is empty
    if(dbllist_prev(node) == NULL) {
        //its the only node so its also the head of the list
        dbllist_head(list) = node;
    } else {
        //making the previous tail point to the new node
        dbllist_next(dbllist_tail(list)) = node;
    }

    dbllist_tail(list) = node;
    dbllist_size(list)++;

    return 0;
}

//adds a new node to the start of the list
int dbllist_prepend(dbllist_t *list ,void *data) {
    if(list == NULL) return -1;
    dbllist_node_t* node = (dbllist_node_t*)malloc(sizeof(dbllist_node_t));
    if(node == NULL) {
        fprintf(stderr, "malloc() failed\n");
        return -1;
    }

    dbllist_data(node) = data;
    dbllist_next(node) = dbllist_head(list);
    dbllist_prev(node) = NULL;

    //if list is empty
    if(dbllist_next(node) == NULL) {
        //its the only node so its also the tail of the list
        dbllist_tail(list) = node;
    } else {
        //making the previous head point to the new node
        dbllist_prev(dbllist_head(list)) = node;
    }

    dbllist_head(list) = node;
    dbllist_size(list)++;

    return 0;
}

//removes a node from the list
int dbllist_remove(dbllist_t *to, dbllist_node_t* pointer,dbllist_destroy_t dealloc) {
    if(to == NULL) return -1;
    if(dbllist_size(to) == 0) return -1;
    if(pointer == NULL) return -1;

    //detaching the node from the list
    if(dbllist_prev(pointer) != NULL) { dbllist_next(dbllist_prev(pointer)) = dbllist_next(pointer); }
    else { dbllist_head(to) = dbllist_next(pointer); }
    if(dbllist_next(pointer) != NULL) { dbllist_prev(dbllist_next(pointer)) = dbllist_prev(pointer); }
    else { dbllist_tail(to) = dbllist_prev(pointer); }

    //freeing its memory 
    if(dealloc == DBLLIST_FREE_DATA) free(dbllist_data(pointer));
    free(pointer);
    pointer = NULL;
    dbllist_size(to)--;
    return 0;
}


