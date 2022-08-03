//Author:   Ron Amar
//ID:       318479458
//Compile:  g++ main.cpp sim_mem.cpp -o main
//Run:      ./main

#include "sim_mem.h"

char main_memory[MEMORY_SIZE];

//contructor
sim_mem::sim_mem(char exe_file_name1[],char exe_file_name2[], char swap_file_name[], int
                text_size, int data_size, int bss_size, int heap_stack_size, int num_of_pages, int
                page_size, int num_of_process)
{
    this->pagesLoaded = 0;
    this->text_size = text_size;
    this->data_size = data_size;
    this->bss_size = bss_size;
    this->heap_stack_size = heap_stack_size;
    this->num_of_pages = num_of_pages;
    this->num_of_proc = num_of_process;
    this->page_size = page_size;

    //=========== O P E N I N G === P R O G R A M === F I L E S ===========
    this->program_fd[0] = open(exe_file_name1, O_RDONLY, 0);
    if(this->program_fd[0] == -1) {
        perror("Bad 1st file name");
        exit(EXIT_FAILURE);
    }
    if(num_of_process > 1) {
        this->program_fd[1] = open(exe_file_name2, O_RDONLY, 0);
        if(this->program_fd[1] == -1) {
            perror("Bad 2nd file name");
            exit(EXIT_FAILURE);
        }
    }

    //=============== O P E N I N G === S W A P ===============
    this->swapfile_fd = open(swap_file_name, O_RDWR | O_CREAT, 0644);
    if(this->swapfile_fd == -1) {
        perror("Bad swap file name");
        exit(EXIT_FAILURE);
    }

    //========== I N I T I A L I Z I N G === S W A P ==========
    int text_pages = text_size / page_size;
    int swap_size = (page_size * (num_of_pages - text_pages)) * num_of_process;
    for(int i = 0; i < swap_size; i++) {
        if(write(this->swapfile_fd, "0", 1) == -1) {
            perror("Write failed (init swap)");
            exit(EXIT_FAILURE);
        }
    }

    //========== I N I T I A L I Z I N G === M A I N === M E M O R Y ==========
    for(int i = 0; i < MEMORY_SIZE; i++) {
        main_memory[i] = '0';
    }

    //========== C R E A T I N G === P A G E === T A B L E ==========
    this->page_table = new page_descriptor*[num_of_process];
    this->page_table[0] = new page_descriptor[num_of_pages];
    if(num_of_process > 1)
        this->page_table[1] = new page_descriptor[num_of_pages];
    
    initPageTable(0);
    if(num_of_process > 1)
        initPageTable(1);
    
    //========== C R E A T I N G === C O N N E C T I O N === T A B L E S ==========
    this->page_connections = new page_connector[MEMORY_SIZE / page_size];
    this->swap_connections = new page_connector[swap_size / page_size];
}

//destructor
sim_mem::~sim_mem() {
    close(this->program_fd[0]);
    if(this->num_of_proc > 1)
        close(this->program_fd[1]);
    close(this->swapfile_fd);

    delete[] this->page_table[0];
    if(this->num_of_proc > 1)
        delete[] this->page_table[1];
    delete this->page_table;
    delete[] this->page_connections;
    delete[] this->swap_connections;
}

//reads from memory
char sim_mem::load(int process_id, int address) {
    int proc_id = process_id - 1;
    int pageNum = address / this->page_size;
    int offset = address % this->page_size;
    int frame = this->pagesLoaded % (MEMORY_SIZE / this->page_size);    //frame we want to insert the new page to
    //=================================== V A L I D ===================================
    //checks if the page needed is already in the memory (if its VALID)
    if(this->page_table[proc_id][pageNum].V == 1) {
        return main_memory[(this->page_table[proc_id][pageNum].frame * this->page_size) + offset];
    }

    //============================== P E R M I S S I O N ==============================
    //checks if the page needed has access rights (if its a TEXT file)
    if(this->page_table[proc_id][pageNum].P == 0) {
        //checks if the page we want to SWAP out is not empty (Memory Full)
        checkMemoryFull(frame);
        getExecPage(proc_id, pageNum, frame);
        return main_memory[(frame * this->page_size) + offset];
    }

    //=================================== D I R T Y ===================================
    //checks if the page is dirty (get it from SWAP file)
    if(this->page_table[proc_id][pageNum].D == 1) {
        //checks if the page we want to SWAP out is not empty (Memory Full)
        checkMemoryFull(frame);

        //copies page from swap
        int swap_index = this->page_table[proc_id][pageNum].swap_index;
        lseek(this->swapfile_fd, swap_index * this->page_size, SEEK_SET);
        const int pSize = this->page_size;
        char page[pSize];
        if(read(this->swapfile_fd, page, this->page_size) == -1) {
            perror("read() failed");
            exit(EXIT_FAILURE);
        }
        
        lseek(this->swapfile_fd, swap_index * this->page_size, SEEK_SET);
        for(int i = 0; i < this->page_size; i++) {
            //add page to main memory
            main_memory[(frame * this->page_size) + i] = page[i];
            //delete page from swap
            if(write(this->swapfile_fd, "0", 1) == -1) {
                perror("Write failed");
                exit(EXIT_FAILURE);
            }
        }
        //update page_connections array
        this->page_connections[frame].page_num = pageNum;
        this->page_connections[frame].proccess_num = proc_id;
        //update swap_connections array
        this->swap_connections[swap_index].page_num = -1;
        this->swap_connections[swap_index].proccess_num = -1;
        //update page table
        this->page_table[proc_id][pageNum].V = 1;
        this->page_table[proc_id][pageNum].frame = frame;
        this->page_table[proc_id][pageNum].swap_index = -1;
        this->pagesLoaded += 1;
        return main_memory[(frame * this->page_size) + offset];
    }

    //============================== N O T === D I R T Y ==============================
    //page not dirty (Data/BSS/heap stack)
    int heap_stack_loc = this->num_of_pages - (this->heap_stack_size / this->page_size);
    int BSS_loc = this->num_of_pages - ((this->bss_size + this->heap_stack_size) / this->page_size);
    //check if in BSS/heap stack
    if(pageNum >= BSS_loc) {
        //check if heap stack
        if(pageNum >= heap_stack_loc) {
            fprintf(stderr, "Trying to access non-allocated memory\n");
            return 0;
        }

        //in BSS
        //init new page
        checkMemoryFull(frame);

        for(int i = 0; i < this->page_size; i++)
            main_memory[(frame * this->page_size) + i] = '0';
        
        //update page_connections array
        this->page_connections[frame].page_num = pageNum;
        this->page_connections[frame].proccess_num = proc_id;
        //update page table
        this->page_table[proc_id][pageNum].V = 1;
        this->page_table[proc_id][pageNum].frame = frame;
        this->pagesLoaded += 1;
        return main_memory[(frame * this->page_size) + offset];
    }

    //in Data
    //checks if the page we want to SWAP out is not empty (Memory Full)
    checkMemoryFull(frame);
    //get needed page from exec file
    getExecPage(proc_id, pageNum, frame);
    return main_memory[(frame * this->page_size) + offset];
}

//writes to memory
void sim_mem::store(int process_id, int address, char value) {
    int proc_id = process_id - 1;
    int pageNum = address / this->page_size;
    int offset = address % this->page_size;
    int frame = this->pagesLoaded % (MEMORY_SIZE / this->page_size);    //frame we want to insert the new page to

    //============================== P E R M I S S I O N ==============================
    //checks if the page needed has access rights (if its a TEXT file)
    if(this->page_table[proc_id][pageNum].P == 0) {
        fprintf(stderr, "Cannot change text page values\n");
        return;
    }

    //=================================== V A L I D ===================================
    //checks if the page needed is already in the memory (if its VALID)
    if(this->page_table[proc_id][pageNum].V == 1) {
        this->page_table[proc_id][pageNum].D = 1;
        main_memory[(this->page_table[proc_id][pageNum].frame * this->page_size) + offset] = value;
        return;
    }

    //=================================== D I R T Y ===================================
    //checks if the page is dirty (get it from SWAP file)
    if(this->page_table[proc_id][pageNum].D == 1) {
        //checks if the page we want to SWAP out is not empty (Memory Full)
        checkMemoryFull(frame);

        //copies page from swap
        int swap_index = this->page_table[proc_id][pageNum].swap_index;
        lseek(this->swapfile_fd, swap_index * this->page_size, SEEK_SET);
        const int pSize = this->page_size;
        char page[pSize];
        if(read(this->swapfile_fd, page, this->page_size) == -1) {
            perror("read() failed");
            exit(EXIT_FAILURE);
        }
        
        lseek(this->swapfile_fd, swap_index * this->page_size, SEEK_SET);
        for(int i = 0; i < this->page_size; i++) {
            //add page to main memory
            main_memory[(frame * this->page_size) + i] = page[i];
            //delete page from swap
            if(write(this->swapfile_fd, "0", 1) == -1) {
                perror("Write failed");
                exit(EXIT_FAILURE);
            }
        }
        //update page_connections array
        this->page_connections[frame].page_num = pageNum;
        this->page_connections[frame].proccess_num = proc_id;
        //update swap_connections array
        this->swap_connections[swap_index].page_num = -1;
        this->swap_connections[swap_index].proccess_num = -1;
        //update page table
        this->page_table[proc_id][pageNum].V = 1;
        this->page_table[proc_id][pageNum].frame = frame;
        this->page_table[proc_id][pageNum].swap_index = -1;
        this->pagesLoaded += 1;
        main_memory[(frame * this->page_size) + offset] = value;
        return;
    }

    //============================== N O T === D I R T Y ==============================
    //page not dirty (Data/BSS/heap stack)
    int heap_stack_loc = this->num_of_pages - (this->heap_stack_size / this->page_size);
    int BSS_loc = this->num_of_pages - ((this->bss_size + this->heap_stack_size) / this->page_size);
    //check if in BSS/heap stack
    if(pageNum >= BSS_loc) {
        //init new page
        checkMemoryFull(frame);

        for(int i = 0; i < this->page_size; i++)
            main_memory[(frame * this->page_size) + i] = '0';
        
        //update page_connections array
        this->page_connections[frame].page_num = pageNum;
        this->page_connections[frame].proccess_num = proc_id;
        //update page table
        this->page_table[proc_id][pageNum].V = 1;
        this->page_table[proc_id][pageNum].D = 1;
        this->page_table[proc_id][pageNum].frame = frame;
        this->pagesLoaded += 1;
        main_memory[(frame * this->page_size) + offset] = value;
        return;
    }

    //in Data
    //checks if the page we want to SWAP out is not empty (Memory Full)
    checkMemoryFull(frame);
    //get needed page from exec file
    getExecPage(proc_id, pageNum, frame);
    this->page_table[proc_id][pageNum].D = 1;
    main_memory[(frame * this->page_size) + offset] = value;
}

//checks if the page we want to SWAP out is not empty (Memory Full)
void sim_mem::checkMemoryFull(int frame) {
    if(this->page_connections[frame].page_num > -1) {
        //checks if the page we want to swap out is dirty
        if(this->page_table[this->page_connections[frame].proccess_num][this->page_connections[frame].page_num].D == 1) {
            //swap main memory to SWAP file
            int swap_index = 0;
            while(this->swap_connections[swap_index].page_num > -1) swap_index++;   //find empty frame in swap file
            lseek(this->swapfile_fd, swap_index * this->page_size, SEEK_SET);       //go there
            if(write(this->swapfile_fd, &main_memory[frame * page_size], this->page_size) == -1) {   //write there
                perror("Write failed (in swap)");
                exit(EXIT_FAILURE);
            }
            //update page table of removed page
            this->page_table[this->page_connections[frame].proccess_num][this->page_connections[frame].page_num].swap_index = swap_index;
            //update swap_connections array
            this->swap_connections[swap_index].page_num = this->page_connections[frame].page_num;
            this->swap_connections[swap_index].proccess_num = this->page_connections[frame].proccess_num;
        }
        //update page table of removed page
        this->page_table[this->page_connections[frame].proccess_num][this->page_connections[frame].page_num].V = 0;
        this->page_table[this->page_connections[frame].proccess_num][this->page_connections[frame].page_num].frame = -1;
    }
}

//puts the needed exec page in the main memory
void sim_mem::getExecPage(int proc_id, int pageNum, int frame) {
    //copies text page to main memory
    const int pSize = this->page_size;
    char page[pSize];
    lseek(this->program_fd[proc_id], pageNum * this->page_size, SEEK_SET);
    if(read(this->program_fd[proc_id], page, this->page_size) == -1) {
        perror("read() failed");
        exit(EXIT_FAILURE);
    }
    //add page to main memory
    for(int i = 0; i < this->page_size; i++) {
        main_memory[(frame * this->page_size) + i] = page[i];
    }
    //update page_connections array
    this->page_connections[frame].page_num = pageNum;
    this->page_connections[frame].proccess_num = proc_id;
    //update page table
    this->page_table[proc_id][pageNum].V = 1;
    this->page_table[proc_id][pageNum].frame = frame;
    this->pagesLoaded += 1;
}

//initializing a page table for a process
void sim_mem::initPageTable(int proc) {
    for(int i = 0; i < this->num_of_pages; i++) {
        this->page_table[proc][i].D = 0;
        if(i < ((this->text_size) / this->page_size))
            this->page_table[proc][i].P = 0;
        else
            this->page_table[proc][i].P = 1;
        this->page_table[proc][i].V = 0;
        this->page_table[proc][i].frame = -1;
        this->page_table[proc][i].swap_index = -1;
    }
}

//print main memory
void sim_mem::print_memory() {
    int i;
    printf("\n Physical memory\n");
    for(i = 0; i < MEMORY_SIZE; i++) {
        printf("[%c]\t", main_memory[i]);
        if((i + 1) % this->page_size == 0) printf("\n");
    }
}

//print swap file
void sim_mem::print_swap ()
{
  char* str = (char*)malloc (this->page_size * sizeof (char));
  int i;
  printf ("\n Swap memory\n");
  lseek (swapfile_fd, 0, SEEK_SET);    // go to the start of the file
  while (read (swapfile_fd, str, this->page_size) == this->page_size)
    {
      for (i = 0; i < page_size; i++)
    {
      printf ("%d - [%c]\t", i, str[i]);
    }
      printf ("\n");
    }
    free(str);
}

//print page table(s)
void sim_mem::print_page_table() {
    int i;
    for (int j = 0; j < num_of_proc; j++) {
        printf("\n page table of process: %d \n", j);
        printf("Valid\t Dirty\t Permission \t Frame\t Swap index\n");
        for(i = 0; i < num_of_pages; i++) {
            printf("[%d]\t[%d]\t[%d]\t[%d]\t[%d]\n",
            page_table[j][i].V,
            page_table[j][i].D,
            page_table[j][i].P,
            page_table[j][i].frame ,
            page_table[j][i].swap_index);
        }
    }
}