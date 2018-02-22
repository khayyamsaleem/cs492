#include <sys/time.h>
#include <stdio.h>
#include <unistd.h>
#include <pthread.h>
#include <errno.h>

#define NUMP 5
/*number of philosophers at the table*/

pthread_mutex_t fork_mutex[NUMP];

pthread_mutex_t pickup_forks;
pthread_mutex_t drop_forks;

int main()
{
  int i;
  pthread_t diner_thread[NUMP];
  int dn[NUMP];
  void *diner();

  pthread_mutex_init(&pickup_forks, NULL);
  pthread_mutex_init(&drop_forks, NULL);


  for (i=0;i<NUMP;i++)
    pthread_mutex_init(&fork_mutex[i], NULL);

  for (i=0;i<NUMP;i++){
    dn[i] = i;
    pthread_create(&diner_thread[i],NULL,diner,&dn[i]);
  }

  for (i=0;i<NUMP;i++)
    pthread_join(diner_thread[i],NULL);

  for (i=0;i<NUMP;i++)
    pthread_mutex_destroy(&fork_mutex[i]);
  pthread_mutex_destroy(&pickup_forks);
  pthread_mutex_destroy(&drop_forks);
  pthread_exit(0);

}

void *diner(int *i)
{
  int v;
  int eating = 0;
  printf("I'm diner %d\n",*i);
  v = *i;
  while (eating < 5) {
    printf("%d is thinking\n", v);
    sleep( v/2);
    printf("%d is hungry\n", v);
    pthread_mutex_lock(&pickup_forks);
    pthread_mutex_lock(&fork_mutex[v]); /*picking up a left fork */
    pthread_mutex_lock(&fork_mutex[(v+1)%NUMP]); /*picking up a right fork */
    pthread_mutex_unlock(&pickup_forks);
    printf("%d is eating\n", v);
    eating++;
    sleep(1);
    printf("%d is done eating\n", v);
    pthread_mutex_lock(&drop_forks);
    pthread_mutex_unlock(&fork_mutex[v]); /*releasing a left fork */
    pthread_mutex_unlock(&fork_mutex[(v+1)%NUMP]); /*releasing a right fork */
    pthread_mutex_unlock(&drop_forks);
  }
  pthread_exit(NULL);
}
