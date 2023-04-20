#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "pattern_matching.h"

int main(int argc, char const *argv[])
{
    pm_t *pm = (pm_t*)malloc(sizeof(pm_t));
    // pm_t pms;
    // pm_t *pm = &pms;
    pm_init(pm);
    pm_addstring(pm, "she", 3);
    pm_addstring(pm, "hshse", 5);
    pm_addstring(pm, "hsh", 3);
    pm_addstring(pm, "shshsee", 7);
    pm_addstring(pm, "!", 1);
    pm_addstring(pm, "!", 1);
    pm_addstring(pm, "first", 5);
    pm_addstring(pm, "second", 6);
    pm_addstring(pm, "hhird", 5);
    pm_addstring(pm, "hshda", 5);

    pm_makeFSM(pm);
    char* text = "ahshseshhhsheehshe!firs!second!firsthhhirdiisheeshshsee";
    dbllist_t* output = pm_fsm_search(pm->zerostate, text, strlen(text));
    dbllist_node_t* temp = dbllist_head(output);
    printf("Patterns found:\n");
    while(temp != NULL) {
        pm_match_t* data = dbllist_data(temp);
        printf("%s at [%d, %d], state: S%d\n", data->pattern, data->start_pos, data->end_pos, data->fstate->id);
        temp = dbllist_next(temp);
    }
    //destroying the output list
    dbllist_destroy(output, DBLLIST_FREE_DATA);
    free(output);
    //destroying the tree
    pm_destroy(pm);
    free(pm);
    return 0;
}
