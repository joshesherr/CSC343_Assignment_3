
#include <iostream> 
#include <sys/wait.h>
#include <ctime>
using namespace std; 
  
int main() { 
  
    int arr[20];

    // Random int array generation
    printf("Array elements: [ ");
    srand(time(0));
    for (int i = 0; i < 20; ++i) {
        arr[i] = (rand() % 200) + 1;
        printf("%d ",arr[i]);
    }
    printf("]\n");
    
    int start, end;
    int arrsize = sizeof(arr)/sizeof(int);
    printf("array size: %d\n",arrsize);
    int fd[2];
    
    if (pipe(fd)==-1) return 1;
    
    int id = fork();
    if (id==-1) return 2;

    if (id==0) {
        start = 0;
        end = arrsize/2;
        printf("child loop from index %d to %d",start,end);
    }
    else {
        start = (arrsize/2)+1;
        end = arrsize;
        printf("parent loop from index %d to %d",start,end);
    }

    int min=arr[0];
    int i;
    for (i=start;i<end;i++) {
        if (arr[i]<min) min = arr[i];
    }
    
    printf(" | calculated min: %d\n",min);
    
    if (id==0) {
        printf("child's process id is: %d\n",getpid());
        write(fd[1],&min,sizeof(min));
        close(fd[1]);
    }
    else {
        printf("parent's process id is: %d\n",getpid());
        int childMin;
        read(fd[0], &childMin, sizeof(int));
        close(fd[0]);
        int finalMin = (childMin<min) ? childMin : min;
        printf("Smallets integer is: %d\n",finalMin);
    }

    return 0;
}