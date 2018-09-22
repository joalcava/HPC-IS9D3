#include <iostream>
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/mman.h>
#include <sys/types.h>
#include <sys/wait.h>

using namespace std;


int main(int argc, char ** argv) {
    int *arr = (int *)malloc(20 * sizeof(int));

    arr = (int *) mmap(NULL, sizeof(*arr), PROT_READ | PROT_WRITE, MAP_SHARED | MAP_ANONYMOUS, -1, 0);
    if (arr == MAP_FAILED) {
        perror("Cannot do the mmap.");
        exit(EXIT_FAILURE);
    }


    for(size_t i = 0; i < 20; i++)
    {
        arr[i] = 99;
    }

    for(size_t i = 0; i < 20; i++)
    {
        cout << arr[i] << endl;
    }

    munmap(arr, sizeof(*arr));

    return 0;
}
