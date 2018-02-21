#include <iostream>
#include <unistd.h>
int main(){
    int x =1, y=2;
    x = fork();
    y = fork();
    if(x!=0) printf("THREAD (%d, %d): Type 1\n",x,y);
    if(y!=0) printf("THREAD (%d, %d): Type 2\n",x,y);
    if(x > 0 || y > 0) printf("THREAD (%d, %d): Type 3\n",x,y);
    if(x == 0 && y == 0) printf("THREAD (%d, %d): Type 4\n",x,y);
    if(x != 0 && y != 0) printf("THREAD (%d, %d): Type 5\n",x,y);
    return 0;
}
