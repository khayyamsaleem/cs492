/*
 * CS492 Programming Assignment #2
 * Khayyam Saleem and Faris Ibrahim
 */
#include <iostream>
#include <fstream>
#include <math.h>
#include "Page.h"
#include "Proc.h"

enum Alg {CLOCK, FIFO, LRU};

const char* usage() {
    return "Usage: ./VMsimulator p1 p2 p3 p4 p5 p6 p7\n"
        "        p1: path to plist.txt\n"
        "        p2: path to ptrace.txt\n"
        "        p3: # of memory locations per page\n"
        "        p4: page replacement algorithm (CLOCK, FIFO, LRU)\n"
        "        p5: toggle pre-paging (+: on, -: off)";
}

int main(int argc, char **argv){
    /* number of mem locations in main memory */

    const std::string algs[] = {"CLOCK", "FIFO", "LRU"};
    /* program args */
    const unsigned NUM_LOCATIONS = 512;
    std::string p_plist;
    std::string p_ptrace;
    unsigned page_size;
    Alg r_alg;
    bool with_prepaging;

    std::vector<Proc*> programs;

    if (argc != 6){
        std::cerr << "Incorrect number of args" << std::endl;
        std::cout << usage() << std::endl;
        std::exit(1);
    }

    p_plist = argv[1];
    p_ptrace = argv[2];

    /* checking if file paths are valid */
    std::ifstream f_plist(p_plist);
    std::ifstream f_ptrace(p_ptrace);
    if(!f_plist.good()){
        std::cerr << "UNABLE TO OPEN " << p_plist << std::endl;
        std::exit(1);
    }
    if(!f_ptrace.good()){
        std::cerr << "UNABLE TO OPEN " << p_ptrace << std::endl;
        std::exit(1);
    }

    if (std::atoi(argv[3]) <= 0 ||
        std::atoi(argv[3]) > 32 ||
        !((std::atoi(argv[3]) & (std::atoi(argv[3]) -1)) == 0)){
        std::cerr << std::atoi(argv[3]) << " invalid as page size." << std::endl;
        std::cout << usage() << std::endl;
        std::exit(1);
    }
    page_size = std::atoi(argv[3]);
    if (std::string(argv[4]) == "CLOCK") r_alg = CLOCK;
    else if (std::string(argv[4]) == "FIFO") r_alg = FIFO;
    else if (std::string(argv[4]) == "LRU") r_alg = LRU;
    else {
        std::cerr << argv[4] << " is not a valid page replacement algorithm." << std::endl;
        std::cout << usage() << std::endl;
        std::exit(1);
    }

    if (std::string(argv[5]) == "+") with_prepaging = true;
    else if (std::string(argv[5]) == "-") with_prepaging = false;
    else {
        std::cerr << argv[5] << " invalid as arg, must be either '+' or '-'." << std::endl;
        std::cout << usage() << std::endl;
        std::exit(1);
    }

    /* read from plist */
    unsigned long p_count = 0;
    std::string line;

    while(std::getline(f_plist, line)){
        if (line.size() != 0){
            std::string process;
            std::string mem;
            std::string test;

            /* delimit by space */
            unsigned spc = line.find(' ');
            process = line.substr(0, spc);
            mem = line.substr(spc + 1);

            /* init page table */
            Proc *p = new Proc();
            p->pid = process.c_str();
            p->mem = atoi(mem.c_str());

            /* add to vector of processes */
            programs.push_back(p);
            p_count++;
        }
    }

    /* populate process page tables */
    unsigned mem_per_process = NUM_LOCATIONS / p_count;

    /* page id counter */
    for (unsigned i = 0, page_id_counter = 1; i < programs.size(); ++i, ++page_id_counter){
        unsigned pages = ceil(programs[i]->mem / ((float)page_size));
        for(unsigned j = 0; j < pages; ++j){
            Page *p = new Page(page_id_counter);
            programs[i]->table.push_back(p);
            page_id_counter++;
        }
    }

    /* initial loading */
    unsigned to_load = mem_per_process / page_size;

    for (unsigned i = 0; i < programs.size(); ++i){
        for (unsigned j = 0; j < to_load && j < programs[i]->table.size(); ++j){
            /* load with last_accessed = 0 */
            programs[i]->table[j]->load(0);
            /* add to indices */
            programs[i]->indices.push_back(j);
        }
    }

    /* read from ptrace */
    unsigned long num_accesses = 0;
    unsigned replacements = 0;
    while(std::getline(f_ptrace, line)){
        if (line.size() != 0){
            ++num_accesses;

            std::string process;
            std::string mem;
            unsigned spc = line.find(' ');
            process = line.substr(0, spc);
            mem = line.substr(spc + 1);

            /* fetch process page table */
            int table_ind = -1;
            unsigned i;
            for (i = 0; i < programs.size() && programs[i]->pid != process; ++i);
            table_ind = (i == programs.size()) ? -1 : i;

            int check_page = ceil(atoi(mem.c_str())/((float) page_size));

            if (programs[table_ind]->table[check_page-1]->valid) {
                programs[table_ind]->table[check_page-1]->load(num_accesses);
            } else {
                switch (r_alg) {
                    case FIFO:  programs[table_ind]->replace_FIFO(check_page-1);
                                break;

                    case LRU:   programs[table_ind]->replace_LRU(check_page-1, num_accesses);
                                break;

                    case CLOCK: programs[table_ind]->replace_CLOCK(check_page - 1);
                                break;
                }
                ++replacements;
                if (with_prepaging) {
                    int next_invalid_page_ind = programs[table_ind]->find_next_invalid(check_page - 1);
                    if (next_invalid_page_ind != -1){
                        ++num_accesses;
                        switch (r_alg) {
                            case FIFO:  programs[table_ind]->replace_FIFO(next_invalid_page_ind);
                                        break;

                            case LRU:   programs[table_ind]->replace_LRU(next_invalid_page_ind, num_accesses);
                                        break;

                            case CLOCK: programs[table_ind]->replace_CLOCK(next_invalid_page_ind);
                                        break;
                        }
                    }
                }
            }
        }
    }
    std::cout << "Number of Page replacements: " << replacements << std::endl;
    std::cout << "Number of Memory accesses: " << num_accesses << std::endl;
    std::exit(0);


    // std::cout << "num locations (fixed): " << NUM_LOCATIONS << std::endl;
    // std::cout << "path to plist: " << f_plist << std::endl;
    // std::cout << "path to ptrace: " << f_plist << std::endl;
    // std::cout << "size of pages: " << page_size << std::endl;
    // std::cout << "type of pr_alg: " << algs[r_alg] << std::endl;
    // std::cout << "pre-paging flag: " << with_prepaging << std::endl;
}
