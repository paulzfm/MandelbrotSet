#include <stdio.h>
#include <omp.h>

int main()
{
    #pragma omp parallel for
    for (i = 0; i < 10; i++) {
        for (j = 0; j < 10; j++) {
            printf("(%d, %d)\n", i, j);
        }
    }
    
    return 0;
}
