//Author:   Ron Amar
//ID:       318479458
//Compile:  g++ main.cpp sim_mem.cpp -o main
//Run:      ./main

#include <iostream>
#include "sim_mem.h"
#include <stdlib.h>
#include <time.h>

int main()
{
    srand(time(NULL));
    char val;
    int text_size = 100;
    int data_size = 200;
    int bss_size = 50;
    int heap_stack_size = 75;
    int page_size = 5;
    int file_size = text_size + data_size + bss_size + heap_stack_size;
    int num_of_pages = file_size / page_size;

    char* file1 = (char*)"exec_file_1.txt\0";
    char* file2 = (char*)"exec_file_2.txt\0";
    char* swap = (char*)"swap_file.txt\0";

    int num_of_proc = 1;
    if(!(file2 == NULL || file2[0] == 0)) num_of_proc++;

    sim_mem mem_sm(file1, file2, swap ,text_size, data_size, bss_size, heap_stack_size, num_of_pages, page_size, num_of_proc);
    /*User tester*/
    // int cmd_id;
    // int proc_id;
    // cout<<"Enter process number:"<<endl;
    // scanf("%d", &proc_id);
    // cout<<"Enter command number:"<<endl;
    // scanf("%d", &cmd_id);
    // do {
    //     int cmd = cmd_id % file_size;
    //     val = mem_sm.load (proc_id, cmd);
    //     mem_sm.print_memory();
    //     mem_sm.print_swap();
    //     mem_sm.print_page_table();
    //     cout<<"Val: "<<val<<endl;

    //     cout<<"Enter process number:"<<endl;
    //     scanf("%d", &proc_id);
    //     cout<<"Enter command number:"<<endl;
    //     scanf("%d", &cmd_id);
    // } while(cmd_id > -1);

    /*loop tester*/
    for(int i = 0; i < 200; i++) {
        int cmd = rand() % 2;
        int proc = (rand() % 2) + 1;
        int num = rand() % file_size;
        switch(cmd) {
            case 0: cout<<i + 1<<".\t"<<"Store  X  in  "<<"process "<<proc<<" ["<<num<<"]"<<endl;
                    mem_sm.store(proc, num, 'X');
                    break;
            case 1: char val = mem_sm.load(proc, num);
                    if(val == 0) continue;
                    cout<<i + 1<<".\t"<<"Loaded "<<val<<" from "<<"process "<<proc<<" ["<<num<<"]"<<endl;
        }
    }
    mem_sm.print_memory();
    mem_sm.print_swap();
    mem_sm.print_page_table();

    
}