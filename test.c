#include <stdio.h>
#include <omp.h>

int main()
{
    int i, j;

    #pragma omp parallel for private(j)
    for (i = 0; i < 10; i++) {
        for (j = 0; j < 10; j++) {
            int k;
            for (k = 0; k < 10000; k++);
            printf("(%d, %d)\n", i, j);
        }
    }

    return 0;
}
