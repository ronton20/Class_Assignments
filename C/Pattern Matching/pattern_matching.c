#include "pattern_matching.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

//Initializing a new tree and its zero state
int pm_init(pm_t * tree) {
    if(tree == NULL) return -1;
    //Initializing Tree
    tree->newstate = 0;
    tree->zerostate = (pm_state_t*)malloc(sizeof(pm_state_t));
    if(tree->zerostate == NULL) {
        fprintf(stderr, "malloc() failed\n");
        return -1;
    }
    //Initializing S0
    tree->zerostate->id = 0;
    tree->zerostate->depth = 0;
    //Creating output list
    tree->zerostate->output = (dbllist_t*)malloc(sizeof(dbllist_t));
    if(tree->zerostate->output == NULL) {
        fprintf(stderr, "malloc() failed\n");
        free(tree->zerostate);
        return -1;
    }
    dbllist_init(tree->zerostate->output);

    //Setting S0's fail to be itself
    tree->zerostate->fail = tree->zerostate;

    //Creating transitions list
    tree->zerostate->_transitions = (dbllist_t*)malloc(sizeof(dbllist_t));
    if(tree->zerostate->_transitions == NULL) {
        fprintf(stderr, "malloc() failed\n");
        free(tree->zerostate);
        free(tree->zerostate->output);
        return -1;
    }
    dbllist_init(tree->zerostate->_transitions);
    tree->newstate++;

    return 0;
}

//Adds a pattern to the pattern tree
int pm_addstring(pm_t *tree, unsigned char *pattern, _size_t n) {
    if(tree == NULL) return -1;
    if(pattern == NULL) return -1;
    if(n <= 0) return -1;

    int depth = -1;
    pm_state_t* curr = NULL;
    pm_state_t* next = tree->zerostate;
    //checking if the char is already mapped
    while(next != NULL) {
        curr = next;
        next = pm_goto_get(curr, pattern[++depth]);
    }
    //creating the rest of the pattern
    while(depth < n) {
        //Creating the new state
        pm_state_t* newState = (pm_state_t*)malloc(sizeof(pm_state_t));
        if(newState == NULL) {
            fprintf(stderr, "malloc() failed\n");
            return -1;
        }
        //Initializing the new state
        newState->id = tree->newstate++;
        newState->depth = curr->depth + 1;
        newState->fail = NULL;
        newState->_transitions = (dbllist_t*)malloc(sizeof(dbllist_t));
        if(newState->_transitions == NULL) {
            fprintf(stderr, "malloc() failed\n");
            free(newState);
            return -1;
        }
        dbllist_init(newState->_transitions);
        newState->output = (dbllist_t*)malloc(sizeof(dbllist_t));
        if(newState->output == NULL) {
            fprintf(stderr, "malloc() failed\n");
            free(newState);
            free(newState->_transitions);
            return -1;
        }
        dbllist_init(newState->output);

        //Creating the edge
        printf("Allocating state: %d\n", newState->id);
        pm_goto_set(curr, pattern[depth], newState);
        curr = newState;
        depth++;
    }
    //checking if the pattern already exists in the tree
    dbllist_node_t* temp = dbllist_head(curr->output);
    while(temp != NULL) {
        if(strcmp(dbllist_data(temp), pattern) == 0) return -1;
        temp = dbllist_next(temp);
    }
    //adding the pattern
    dbllist_append(curr->output, pattern);
    return 0;
}

//Linking 2 states with a labled edge
int pm_goto_set(pm_state_t *from_state, unsigned char symbol,pm_state_t *to_state) {
    if(from_state == NULL) return -1;
    if(to_state == NULL) return -1;
    //Creating the new edge
    pm_labeled_edge_t* newEdge = (pm_labeled_edge_t*)malloc(sizeof(pm_labeled_edge_t));
        if(newEdge == NULL) {
            fprintf(stderr, "malloc() failed\n");
            return -1;
        }
        newEdge->label = symbol;
        newEdge->state = to_state;
        //Adding the new edge to the old state
        dbllist_append(from_state->_transitions, newEdge);
        printf("%d -> %c -> %d\n", from_state->id, symbol, to_state->id);
        return 0;
}

//Finding the state with the desired symbol from the current state
pm_state_t* pm_goto_get(pm_state_t *state, unsigned char symbol) {
    if(state == NULL) return NULL;
    pm_state_t* next = NULL;
    dbllist_node_t* curr = state->_transitions->head;
    while(curr != NULL) {
        pm_labeled_edge_t* nextEdge = (pm_labeled_edge_t*)dbllist_data(curr);
        if(nextEdge->label == symbol) {
            //Found the mapped char - continue running on the tree
            next = nextEdge->state;
            break;
        }
        curr = dbllist_next(curr);
    }
    return next;
}

//Linking all the states to their fail states
int pm_makeFSM(pm_t *tree) {
    if(tree == NULL) return -1;
    if(tree->newstate <= 1) return -1;
    //creating the queue
    dbllist_t* queue = (dbllist_t*)malloc(sizeof(dbllist_t));
    if(queue == NULL) {
        fprintf(stderr, "malloc() failed\n");
        return -1;
    }
    dbllist_init(queue);

    //Adding all the states that are of 1 depth
    dbllist_node_t* next = dbllist_head(tree->zerostate->_transitions);
    while(next != NULL) {
        pm_labeled_edge_t* currEdge = (pm_labeled_edge_t*)dbllist_data(next);
        pm_state_t* currState = currEdge->state;
        currState->fail = tree->zerostate;  //fails are S0
        // printf("Setting f(%d) = %d\n",currState->id, currState->fail->id);
        dbllist_append(queue, currState);
        next = dbllist_next(next);
    }
    //Creating all the fails for each state
    while(dbllist_size(queue) > 0) {
        pm_state_t* curr = (pm_state_t*)dbllist_data(dbllist_head(queue));
        next = dbllist_head(curr->_transitions);
        //for each edge in current state
        while(next != NULL) {
            pm_labeled_edge_t* nextEdge = (pm_labeled_edge_t*)dbllist_data(next);
            pm_state_t* nextState = nextEdge->state;
            //add all its 'sons' to the queue
            dbllist_append(queue, nextState);
            pm_state_t* state = curr->fail;
            //find the longest suffix of the current node, with an edge of the same symbol
            while((pm_goto_get(state, nextEdge->label) == NULL) && state->id != 0) state = state->fail;
            //and add it as the "son's" fail
            nextState->fail = pm_goto_get(state, nextEdge->label);
            if(nextState->fail == NULL) nextState->fail = tree->zerostate;
            printf("Setting f(%d) = %d\n",nextState->id, nextState->fail->id);

            //lastly add the son's fail's output to the son's output
            dbllist_node_t* out = dbllist_head(nextState->fail->output);
            for(int pointer = 0; pointer < dbllist_size(nextState->fail->output); pointer++) {
                char* pattern = dbllist_data(out);
                dbllist_append(nextState->output, pattern);
                out = dbllist_next(out);
            }
            dbllist_node_t* temp = dbllist_head(nextState->output);
            next = dbllist_next(next);
        }
        if(dbllist_remove(queue, dbllist_head(queue), DBLLIST_LEAVE_DATA) == -1) {
            fprintf(stderr, "dbllist_remove() failed\n");
            free(queue);
            return -1;
        }
    }
    //freeing the queue
    // dbllist_destroy(queue, DBLLIST_LEAVE_DATA);
    free(queue);
    return 0;
}

//Searching for patterns in the given text
dbllist_t* pm_fsm_search(pm_state_t *head, unsigned char *text, _size_t n){
    //creating the output list
    dbllist_t* output = (dbllist_t*)malloc(sizeof(dbllist_t));
    if(output == NULL) {
        fprintf(stderr, "malloc() failed\n");
        return NULL;
    }
    dbllist_init(output);
    if(head == NULL) return output;

    //going over the whole text
    pm_state_t* curr = head;
    for(int i = 0; i < n; i++) {
        //finding the next place to go on the tree
        while(pm_goto_get(curr, text[i]) == NULL) {
            curr = curr->fail;
            if(curr->id == 0) break;
        }
        if(pm_goto_get(curr, text[i]) != NULL) curr = pm_goto_get(curr, text[i]);
        
        //adding the outputs in the output list of the accepting state to the output list
        dbllist_node_t* out = dbllist_head(curr->output);
        for(int pointer = 0; pointer < dbllist_size(curr->output); pointer++) {
            pm_match_t* match = (pm_match_t*)malloc(sizeof(pm_match_t));
            if(match == NULL) {
                fprintf(stderr, "malloc() failed\n");
                return output;
            }
            match->pattern = dbllist_data(out);
            match->end_pos = i;
            match->start_pos = i - strlen(dbllist_data(out)) + 1;
            match->fstate = curr;
            dbllist_append(output, match);
            out = dbllist_next(out);
        }
    }
    return output;
}

//Destroying every part of the tree - NOT freeing the pointer (in case of static allocation) - free by user
void pm_destroy(pm_t *tree) {
    if(tree == NULL) return;
    if(tree->newstate == 0) {
        return;
    }
    //creating the queue for destroying all the states
    dbllist_t* queue = (dbllist_t*)malloc(sizeof(dbllist_t));
    if(queue == NULL) {
        fprintf(stderr, "malloc() failed\n");
        return;
    }
    dbllist_init(queue);
    dbllist_append(queue, tree->zerostate);

    while(dbllist_size(queue) > 0) {
        pm_state_t* curr = dbllist_data(dbllist_head(queue));
        dbllist_node_t* nextNode = dbllist_head(curr->_transitions);
        //adding all the 'son' states to the queue
        while(nextNode != NULL) {
            pm_labeled_edge_t* nextEdge = dbllist_data(nextNode);
            dbllist_append(queue, nextEdge->state);
            nextNode = dbllist_next(nextNode);
        }
        //destroying all the edges of the current node
        dbllist_destroy(curr->_transitions, DBLLIST_FREE_DATA);
        free(curr->_transitions);
        curr->_transitions = NULL;
        //destroying the output list
        dbllist_destroy(curr->output, DBLLIST_LEAVE_DATA);
        free(curr->output);
        curr->output = NULL;
        //destroying the node itself and removing it from the queue
        dbllist_remove(queue, dbllist_head(queue), DBLLIST_FREE_DATA);
    }
    //destroying the queue that was used to destroy the tree
    dbllist_destroy(queue, DBLLIST_LEAVE_DATA);
    free(queue);
}

