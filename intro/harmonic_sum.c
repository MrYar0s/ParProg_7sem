#include <omp.h>
#include <stdio.h>
#include <stdlib.h>

int main(int argc, char *argv[])
{
  double sum = 0.0;
  size_t num = atoi(argv[1]);
  double* thread_sum = (double*) calloc(omp_get_max_threads(), sizeof(double));
  #pragma omp parallel
  {
    double partial_sum = 0.0;
    #pragma omp for
      for (int i = 1; i <= num; i++) {
        partial_sum += 1.0 / i;
      }
      thread_sum[omp_get_thread_num()] += partial_sum;
  }
  for (int i = 0; i < omp_get_max_threads(); i++) {
    sum += thread_sum[i];
  }
  free(thread_sum);
  printf("%lg\n", sum);
  return 0;
}
