#ifndef PRODCONS_INCLUDE
#define PRODCONS_INCLUDE

#include <time.h>

typedef struct Product {
    int product_id;
    float timestamp;
    unsigned life;
    Product(int _product_id, clock_t _timestamp, unsigned _life){
        product_id = _product_id;
        timestamp = ((float)_timestamp)/CLOCKS_PER_SEC;
        life = _life%1024;
    }
} Product;

#endif
