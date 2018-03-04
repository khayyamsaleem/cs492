#ifndef PRODCONS_INCLUDE
#define PRODCONS_INCLUDE

#include <time.h>
#include <sstream>

typedef struct Product {
	unsigned producer_id;
    unsigned product_id;
    clock_t timestamp;
    unsigned life;
    explicit Product(const unsigned _producer_id, const unsigned _product_id, const clock_t _timestamp, const unsigned _life){
    	    producer_id = _producer_id;
        product_id = _product_id;
        timestamp = _timestamp;
        life = _life%1024;
    }
    friend std::ostream& operator<<(std::ostream& os, const Product &prod){
        os << "Product "
           << prod.product_id
		   << " produced by Producer "
		   << prod.producer_id
           << " has timestamp "
           << prod.timestamp
           << " and life "
           << prod.life << "." << std::endl;
        os.flush();
        return os;
    }
} Product;

extern int num_producers ;
extern int num_consumers ;
extern int num_products  ;
extern int queue_size    ;
extern int s_algo        ;
extern int quantum       ;
extern int seed          ;

void *producer(void *);
//void *consumer(void *);

#endif
