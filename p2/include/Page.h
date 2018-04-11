#ifndef PAGE_H
#define PAGE_H
#include <sstream>

struct Page {
    unsigned long page_no;
    bool valid;
    bool r_bit;
    unsigned long last_accessed;

    /* constructs a page, and gives it an id */
    Page(unsigned long id){
        page_no = id;
        valid = false;
        r_bit = false;
        last_accessed = 0;
    }

    friend std::ostream& operator<<(std::ostream& os, const Page &p){
        os << "Page [ page_no=" << p.page_no <<
                     ", valid=" << p.valid <<
                         ", R=" << p.r_bit <<
             ", last_accessed=" << p.last_accessed <<
             " ]";
        os.flush();
        return os;
    }

    /* sets valid to true, toggles r_bit on, updates time last accessed */
    void load(unsigned long at){
        valid = true;
        r_bit = true;
        last_accessed = at;
    }
};

#endif
