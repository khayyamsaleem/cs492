#ifndef PRODCONS_INCLUDE
#define PRODCONS_INCLUDE

#include <time.h>
#include <sstream>

typedef struct Product {
//	unsigned producer_id;
    unsigned product_id;
    clock_t timestamp;
    int life;
    explicit Product(const unsigned _product_id, const clock_t _timestamp, const int _life){
        product_id = _product_id;
        timestamp = _timestamp;
        life = _life%1024;
    }
    friend std::ostream& operator<<(std::ostream& os, const Product &prod){
        os << "Product "
           << "[id=" << prod.product_id
           << ", TS=" << prod.timestamp
           << ", life=" << prod.life << " ]";
        os.flush();
        return os;
    }
} Product;

namespace Color {
    enum Code {
        FG_RED      = 31,
        FG_GREEN    = 32,
        FG_BLUE     = 34,
		FG_LIGHT_BLUE = 94,
        FG_DEFAULT  = 39,
        BG_RED      = 41,
        BG_GREEN    = 42,
        BG_BLUE     = 44,
        BG_DEFAULT  = 49
    };
    class Modifier {
        Code code;
    public:
        Modifier(Code pCode) : code(pCode) {}
        friend std::ostream&
        operator<<(std::ostream& os, const Modifier& mod) {
            return os << "\033[" << mod.code << "m";
        }
    };
}

extern int num_producers ;
extern int num_consumers ;
extern int num_products  ;
extern int queue_size    ;
extern int s_algo        ;
extern int quantum       ;
extern int seed          ;

#endif
