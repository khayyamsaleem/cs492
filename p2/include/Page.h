#ifndef PAGE_H
#define PAGE_H

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

    /* sets valid to true, toggles r_bit on, updates time last accessed */
    void load(unsigned long at){
        valid = true;
        r_bit = true;
        last_accessed = at;
    }
};

#endif
