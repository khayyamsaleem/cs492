#include <sys/time.h>
#include <stdio.h>
#include <unistd.h>
#include <pthread.h>
#include <errno.h>

#define NUMP 5
/*number of philosophers at the table*/

pthread_mutex_t fork_mutex[NUMP];

pthread_mutex_t attempt_to_eat;
int currently_eating; 
/*Tracking how many are eating at a time so that only one philospher is eating at a time*/

int main()
{
  int i;
  pthread_t diner_thread[NUMP];
  int dn[NUMP];
  void *diner();
  currently_eating = 0;

  pthread_mutex_init(&attempt_to_eat, NULL);


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
    pthread_mutex_lock(&attempt_to_eat);
    pthread_mutex_lock(&fork_mutex[v]); /*picking up a left fork */
    pthread_mutex_lock(&fork_mutex[(v+1)%NUMP]); /*picking up a right fork */
    printf("%d is eating\n", v);
    eating++;
    currently_eating++;
    printf("%d philosophers currently eating.\n",currently_eating);
    sleep(1);
    printf("%d is done eating\n", v);
    currently_eating--;
    pthread_mutex_unlock(&fork_mutex[v]); /*releasing a left fork */
    pthread_mutex_unlock(&fork_mutex[(v+1)%NUMP]); /*releasing a right fork */
    pthread_mutex_unlock(&attempt_to_eat);


  }
  pthread_exit(NULL);
}
