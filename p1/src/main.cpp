//Khayyam Saleem and Faris Ibrahim
//ksaleem@stevens.edu, fibrahim@stevens.edu
//We pledge our honor that we have abided by the Stevens Honor System.
#include <iostream>
#include <cstdlib>
#include <string>
#include <pthread.h>
#include <time.h>
#include <unistd.h>
#include <queue>
#include <assert.h>
#include <ostream>

#include "prodcons.h"

using std::chrono::system_clock;

int num_producers, num_consumers, num_products, queue_size, s_algo, quantum,
    seed;
pthread_mutex_t access_queue;
bool first_thread;
std::queue<Product*> waiting_products;
int num_produced, num_consumed;
pthread_cond_t full_queue, queue_not_full;
system_clock::time_point start_time, end_producing, end_consuming;
//Turn-around times
std::chrono::duration<double> turn_min = std::chrono::system_clock::duration::max();
std::chrono::duration<double> turn_max = std::chrono::system_clock::duration::min();
std::chrono::duration<double> turn_total = std::chrono::system_clock::duration::zero();
//Wait times
std::chrono::duration<double> wait_min = std::chrono::system_clock::duration::max();
std::chrono::duration<double> wait_max = std::chrono::system_clock::duration::min();
std::chrono::duration<double> wait_total = std::chrono::system_clock::duration::zero();
Color::Modifier red(Color::FG_RED);
Color::Modifier def(Color::FG_DEFAULT);
Color::Modifier green(Color::FG_GREEN);
Color::Modifier blue(Color::FG_LIGHT_BLUE);

const char* usage() {
    return "Usage: ./prodcons p1 p2 p3 p4 p5 p6 p7\n"
        "        p1: Number of producer threads\n"
        "        p2: Number of consumer threads\n"
        "        p3: Total number of products to be generated by all producer threads\n"
        "        p4: Size of the queue to store products for both producer and consumer threads\n"
        "            (0 for unlimited queue size)\n"
        "        p5: Type of scheduling algorithm (0 for FCFS, 1 for RR)\n"
        "        p6: Value of quantum used for round-robin scheduling\n"
        "        p7: Seed for random number generator";
}

void analysis(Product *p){ //Function to calculate things required for Analysis
    //Turn-around time calculations
    std::chrono::duration<double> turn_diff = system_clock::now() - p->timestamp;
    turn_total += turn_diff;
    if (turn_diff < turn_min)  {
        turn_min = turn_diff;
    }
    if (turn_diff > turn_max) {
        turn_max = turn_diff;
    }
    //Wait time calculations
    std::chrono::duration<double> wait_diff = p->wait_time;
    wait_total += wait_diff;
    if (wait_diff < wait_min) {
        wait_min = wait_diff;
    }
    if (wait_diff > wait_max) {
        wait_max = wait_diff;
    }
    //    std::cout << "Time elapsed for id=" << p->product_id << ": " << turn_diff.count() << "s" << std::endl;
}

void producer(unsigned *args) {
    //getting id for producer
    unsigned my_id = *args;
    while (true) {
        //protect access to the queue
        pthread_mutex_lock(&access_queue);
        //only wait if we don't have the unlimited queue size config
        if (queue_size != 0)
            //only create new products if queue is not full and we have products left to create
            while (waiting_products.size() >= queue_size
                    && num_produced < num_products)
                pthread_cond_wait(&full_queue, &access_queue);
        //only create products if we have to
        if (num_produced < num_products) {
            //use product constructor
            Product *p = new Product(num_produced++, start_time);
            waiting_products.push(p);
            //std::cout << "push " << p << "\n";
            //print product details
            std::ostringstream os;
            os << green << "Producer " << my_id << " produced " << blue << *p << def << std::endl;
            std::cout << os.str();
            //let other threads know that queue is not yet full
            pthread_cond_broadcast(&queue_not_full);
            //release access to queue
            pthread_mutex_unlock(&access_queue);
            //100000 µs == 100ms
            usleep(100000);
        } else {
            //done with all products we needed to make, let other producers know
            pthread_cond_broadcast(&full_queue);
            //release queue
            pthread_mutex_unlock(&access_queue);
            break;
        }
    }
    end_producing = system_clock::now();
}

unsigned fn(unsigned n) {
    return (n == 0 || n == 1) ? n : fn(n - 1) + fn(n - 2);
}

void consumer(unsigned *args) {
    unsigned my_id = *args;
    while (true) {
        pthread_mutex_lock(&access_queue);
        while (waiting_products.empty() && num_consumed < num_products)
            pthread_cond_wait(&queue_not_full, &access_queue);
        //check if there are any products left to consume
        //and which algorithm to use
        Product *p = waiting_products.front();
        //std::cout << "popped " << *p << "\n";
        if (num_consumed < num_products && s_algo == 0) { //FCFS
            waiting_products.pop();
            //Calculate wait time
            p->wait_time += (system_clock::now() - p->last_update);
            ++num_consumed;
            //let producers know that there's space
            pthread_cond_broadcast(&full_queue);
            pthread_mutex_unlock(&access_queue);
            for (int i = 0; i < p->life; ++i)
                fn(10);
            std::ostringstream os;
            os << red << "Consumer " << my_id << " consumed " << blue << *p << def << std::endl;
            std::cout << os.str();
            analysis(p);
            usleep(100000);
        } else if (num_consumed < num_products && s_algo == 1) { //ROUND ROBIN
            //round robin
            //if there is still some life left, we've gta keep running the func
            //and put the product back in the queue
            // "taking a bite"
            waiting_products.pop();
            //Calculate wait time
            std::chrono::system_clock::time_point exit_time = system_clock::now();
            p->wait_time += (exit_time - p->last_update);
            if (p->life >= quantum) {
                //reducing life
                p->life -= quantum;
                //simulating consumption of a product
                for (int i = 0; i < quantum; ++i) fn(10);
                //reinsert product in queue
                waiting_products.push(p);
                p->last_update = system_clock::now();
                //inform ONLY consumers that the product is back in the queue
                pthread_cond_broadcast(&queue_not_full);
            } else {
                //case where there is less than a quantum of life left,
                //finish consuming ("finishing the meal")
                ++num_consumed;
                //tell producers that we've made room in the queue
                pthread_cond_broadcast(&full_queue);
                //finish the remainder
                for (int i = 0; i < p->life; ++i) fn(10);
                std::ostringstream os;
                os << red << "Consumer " << my_id << " consumed " << blue << *p << def << std::endl;
                std::cout << os.str();
                analysis(p);
            }
            pthread_mutex_unlock(&access_queue);
            usleep(100000);
        } else {
            //consumed all products, tell consumers to chill out
            pthread_cond_broadcast(&queue_not_full);
            pthread_mutex_unlock(&access_queue);
            break;
        }
    }
    end_consuming = system_clock::now();
}

int main(int argc, char* argv[]) {
    if (argc != 8) {
        std::cerr << "Incorrect number of args" << std::endl;
        std::cout << usage() << std::endl;
        std::exit(1);
    }
    for (int i = 1; i < argc; ++i) {
        if ((i <= 4 || i >= 6) && std::atoi(argv[i]) < 0) {
            std::cerr << "Invalid Value " << argv[i] << " for p" << i
                << std::endl;
            std::exit(1);
        }
        if (i == 5 && (std::atoi(argv[i]) != 0 && std::atoi(argv[i]) != 1)) {
            std::cerr << "Invalid Value " << argv[i] << " for p5" << std::endl;
            std::exit(1);
        }
    }
    num_producers = std::atoi(argv[1]);
    num_consumers = std::atoi(argv[2]);
    num_products = std::atoi(argv[3]);
    queue_size = std::atoi(argv[4]);
    s_algo = std::atoi(argv[5]);
    quantum = std::atoi(argv[6]);
    seed = std::atoi(argv[7]);

    srand(seed);
    first_thread = true;
    num_produced = 0;
    num_consumed = 0;
    pthread_mutex_init(&access_queue, NULL);
    pthread_cond_init(&full_queue, NULL);
    pthread_cond_init(&queue_not_full, NULL);

    pthread_t producers[num_producers];
    pthread_t consumers[num_consumers];
    unsigned p_ids[num_producers];
    unsigned c_ids[num_consumers];

    //start monitoring time
    start_time = system_clock::now();

    for (int i = 0; i < num_producers; ++i) {
        p_ids[i] = i;
        pthread_create(&producers[i], NULL, (void*(*)(void*))(&producer), (void*)&p_ids[i]);
    }
    for (int i = 0; i < num_consumers; ++i) {
        c_ids[i] = i;
        pthread_create(&consumers[i], NULL, (void*(*)(void*))(&consumer), (void*)&c_ids[i]);
    }
    //join all threads
    for (int i = 0; i < num_producers; ++i)
        pthread_join(producers[i], NULL);
    for (int i = 0; i < num_consumers; ++i)
        pthread_join(consumers[i], NULL);

    pthread_mutex_destroy(&access_queue);
    pthread_cond_destroy(&full_queue);
    std::cout << "Products left in queue: " << waiting_products.size() << std::endl;
    //stop monitoring time

    std::chrono::duration<double> diff = system_clock::now() - start_time;
    std::cout << "--------------ANALYSIS---------------" << std::endl;
    std::cout << "Time elapsed: " << diff.count() << "s" << std::endl;
    std::cout << "Turn-around times:" << std::endl
        << "Min = " << turn_min.count() << "s" << std::endl
        << "Max = " << turn_max.count() << "s" << std::endl
        << "Average = " << turn_total.count()/num_products << "s per product" << std::endl;
    std::cout << "Wait times:" << std::endl
        << "Min = " << wait_min.count() << "s" << std::endl
        << "Max = " << wait_max.count() << "s" << std::endl
        << "Average = " << wait_total.count()/num_products << "s per product" << std::endl;
    std::cout << "Producer throughput: "
        << ((double)num_produced * 1000 * 1000 * 1000 * 60) /
            std::chrono::duration_cast<std::chrono::nanoseconds>(end_producing - start_time).count()
        << " products produced per minute" << std::endl;
    std::cout << "Consumer throughput: "
        << ((double)num_consumed * 1000 * 1000 * 1000 * 60) /
            std::chrono::duration_cast<std::chrono::nanoseconds>(end_consuming - start_time).count()
        << " products consumed per minute" << std::endl;
}
