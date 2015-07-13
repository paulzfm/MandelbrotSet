#include <stdio.h>
#include <omp.h>

int main()
{
    int i, j;
    int cnt = 0;

    #pragma omp parallel for private(j)
    for (i = 0; i < 400; i++) {
        for (j = 0; j < 400; j++) {
            int k;
            for (k = 0; k < 10000; k++);
            printf("(%d, %d)\n", i, j);
            #pragma omp critical
            cnt++;
        }
    }

    printf("%d\n", cnt);
    return 0;
}
