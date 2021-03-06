//Khayyam Saleem and Faris Ibrahim
//ksaleem@stevens.edu and fibrahim@stevens.edu
#ifndef PRODCONS_INCLUDE
#define PRODCONS_INCLUDE

#include <time.h>
#include <sstream>
#include <chrono>

struct Product {
    unsigned product_id;
    std::chrono::system_clock::time_point timestamp, start_time, last_update;
    int life;
    std::chrono::duration<double> wait_time;
    explicit Product(
            const unsigned _product_id,
            const std::chrono::system_clock::time_point _start_time
            ) {
        start_time = _start_time;
        product_id = _product_id;
        timestamp = std::chrono::system_clock::now();
        life = rand() % 1024;
        wait_time = std::chrono::system_clock::duration::zero();
        last_update = timestamp;
    }
    friend std::ostream& operator<<(std::ostream& os, const Product &prod) {
        os << "Product " << "[ id=" << prod.product_id << ", timestamp="
            << (prod.timestamp - prod.start_time).count() << ", life="
            << prod.life << " ]";
        os.flush();
        return os;
    }
};

namespace Color {
    enum Code {
        FG_RED = 31,
        FG_GREEN = 32,
        FG_BLUE = 34,
        FG_LIGHT_BLUE = 94,
        FG_DEFAULT = 39,
        BG_RED = 41,
        BG_GREEN = 42,
        BG_BLUE = 44,
        BG_DEFAULT = 49
    };
    class Modifier {
        Code code;
        public:
        Modifier(Code pCode) :
            code(pCode) {
            }
        friend std::ostream&
            operator<<(std::ostream& os, const Modifier& mod) {
                return os << "\033[" << mod.code << "m";
            }
    };
}

extern int num_producers, num_consumers, num_products, queue_size, s_algo,
       quantum, seed;

#endif
