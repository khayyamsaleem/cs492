/*
 * CS492 Programming Assignment #2
 * Khayyam Saleem and Faris Ibrahim
 */
#include <iostream>

enum Alg {CLOCK, FIFO, LRU};
std::string algs[] = {"CLOCK", "FIFO", "LRU"};

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

    /* program args */
    const unsigned NUM_LOCATIONS = 512;
    std::string f_plist;
    std::string f_ptrace;
    unsigned page_size;
    Alg r_alg;
    bool with_prepaging;

    if (argc != 6){
        std::cerr << "Incorrect number of args" << std::endl;
        std::cout << usage() << std::endl;
        std::exit(1);
    }

    f_plist = argv[1];
    f_ptrace = argv[2];
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

    std::cout << "num locations (fixed): " << NUM_LOCATIONS << std::endl;
    std::cout << "path to plist: " << f_plist << std::endl;
    std::cout << "path to ptrace: " << f_plist << std::endl;
    std::cout << "size of pages: " << page_size << std::endl;
    std::cout << "type of pr_alg: " << algs[r_alg] << std::endl;
    std::cout << "pre-paging flag: " << with_prepaging << std::endl;
}
