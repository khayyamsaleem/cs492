#ifndef PROC_H
#define PROC_H

#include <vector>
#include <iostream>
#include <sstream>
#include "Page.h"

struct Proc {
    std::string pid;
    unsigned mem;
    std::vector<Page*> table;
    std::vector<unsigned> indices;
    unsigned time_ind;

    Proc(){
        mem = 0;
        time_ind = 0;
    };

    friend std::ostream& operator<<(std::ostream& os, const Proc &p){
        os << "Process [ pid=" << p.pid << ", mem=" << p.mem << " ]";
        os.flush();
        return os;
    }

    void replace_FIFO(int ind){
        unsigned remove_ind = indices.front();
        table[remove_ind]->valid = false;
        table[ind]->valid = true;
        indices.erase(indices.begin());
        indices.push_back(ind);
    }

    void replace_LRU(int ind, unsigned long at){
        unsigned long min = -1;
        int min_ind = -1;
        for (unsigned i = 0; i < indices.size(); ++i){
            if (table[indices[i]]->last_accessed < min){
                min = table[indices[i]]->last_accessed;
                min_ind = i;
            }
        }

        /* unload old page */
        table[indices[min_ind]]->valid = false;

        /* load new page, update last_accessed */
        table[ind]->valid = true;
        table[ind]->last_accessed = at;

        /* replace old index with new */
        indices[min_ind] = ind;

    }

    void replace_CLOCK(int ind){
        while(table[indices[time_ind]]->r_bit){
            table[indices[time_ind]]->r_bit = false;
            time_ind = (time_ind + 1) % indices.size();
        }
        table[indices[time_ind]]->valid = false;
        table[ind]->valid = true;
        table[ind]->r_bit = true;

        indices[time_ind] = ind;
        time_ind = (time_ind + 1) % indices.size();
    };

    /* used for pre-paging */
    int find_next_invalid(unsigned start){
        unsigned i;
        for (i = (start + 1) % table.size(); table[i]->valid; i = (i+1)%table.size()){
            if (i == start) return -1;
        }
        return i;
    }
};

#endif
