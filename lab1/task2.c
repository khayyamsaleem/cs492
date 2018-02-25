#include <sys/time.h>
#include <stdio.h>
#include <pthread.h>
#include <errno.h>
#include <unistd.h>
#include <assert.h>

#define NUMP 5

pthread_mutex_t fork_mutex[NUMP];

pthread_cond_t eating_condition;
pthread_mutex_t grab_forks;
pthread_mutex_t drop_forks;

int currently_eating;
const int allowed_to_eat = 4;

int main(){
    int i;
    pthread_t diner_thread[NUMP];
    int dn[NUMP];
    void *diner();
    currently_eating = 0;

    for(i=0; i<NUMP; ++i) pthread_mutex_init(&fork_mutex[i],NULL);
    pthread_mutex_init(&grab_forks,NULL);
    pthread_mutex_init(&drop_forks,NULL);
    pthread_cond_init(&eating_condition,NULL);

    for(i=0; i < NUMP; ++i){
        dn[i] = i;
        pthread_create(&diner_thread[i], NULL, diner, &dn[i]);
    }
    for(i=0; i < NUMP; ++i) pthread_join(diner_thread[i], NULL);
    for(i=0;i<NUMP;++i) pthread_mutex_destroy(&fork_mutex[i]);
    pthread_mutex_destroy(&grab_forks);
    pthread_mutex_destroy(&drop_forks);
    pthread_cond_destroy(&eating_condition);
    pthread_exit(0);
}

void *diner(int *i){
    int v;
    int eating = 0;
    printf("I'm diner %d\n",*i);
    v = *i;
    while (eating < 5){
        printf("%d is thinking\n",v);
        sleep(v/2);
        printf("%d is hungry\n",v);

        pthread_mutex_lock(&grab_forks);
        while(currently_eating >= allowed_to_eat) pthread_cond_wait(&eating_condition,&grab_forks);
        pthread_mutex_lock(&fork_mutex[v]);
        pthread_mutex_lock(&fork_mutex[(v+1)%NUMP]);
        ++currently_eating;
        pthread_mutex_unlock(&grab_forks);
        printf("%d is eating\n",v);
        ++eating;
        assert(currently_eating < 5);
        printf("%d philosophers currently eating.\n",currently_eating);
        sleep(1);
        printf("%d is done eating\n",v);
        pthread_mutex_lock(&drop_forks);
        --currently_eating;
        pthread_mutex_unlock(&fork_mutex[v]);
        pthread_mutex_unlock(&fork_mutex[(v+1)%NUMP]);
        pthread_cond_broadcast(&eating_condition);
        pthread_mutex_unlock(&drop_forks);
    }
    pthread_exit(NULL);
}
