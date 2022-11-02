#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>
#include <unistd.h>
#include <math.h>
#include <fcntl.h>
#include <errno.h>

void main(){
    for (int i = 0;i<20;i++){
        srand(time(NULL));
        int num = (rand() % (99 - 1));
        printf("%d\n",num);
    }
}