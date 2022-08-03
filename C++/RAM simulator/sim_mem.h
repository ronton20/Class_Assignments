//Author:   Ron Amar
//ID:       318479458
//Compile:  g++ main.cpp sim_mem.cpp -o main
//Run:      ./main

#include <iostream>
using namespace std;
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#ifndef Sim_Mem
#define Sim_Mem

#define MEMORY_SIZE 30
extern char main_memory[MEMORY_SIZE];

typedef struct page_descriptor {
    int V; // valid
    int D; // dirty
    int P; // permission
    int frame; //the number of a frame if it is page-mapped
    int swap_index; // where the page is located in the swap file. (indicates *PAGE* index as: page index 0 = swap memory 0-4, page index 1 = swap memory 5-9 etc)
} page_descriptor;

//struct to connect the main memory
typedef struct page_connector{
    int proccess_num = -1;
    int page_num = -1;
} page_connector;

class sim_mem {
    int swapfile_fd; //swap file fd
    int program_fd[2]; //executable file fd
    int text_size;
    int data_size;
    int bss_size;
    int heap_stack_size;
    int num_of_pages;
    int page_size;
    int num_of_proc;
    page_descriptor **page_table; //pointer to page table
    int pagesLoaded;
    page_connector* page_connections;
    page_connector* swap_connections;

public:
    sim_mem(char exe_file_name1[],char exe_file_name2[], char swap_file_name[], int
                    text_size, int data_size, int bss_size, int heap_stack_size, int num_of_pages, int
                    page_size, int num_of_process);
    ~sim_mem();
    char load(int process_id, int address);
    void store(int process_id, int address, char value);
    void print_memory();
    void print_swap ();
    void print_page_table();

protected:
    void initPageTable(int proc);
    void checkMemoryFull(int frame);
    void getExecPage(int proc_id, int pageNum, int frame);
};

#endif //Sim_Mem