//  Every thread must write its own number, overall nubmer of processes and "Hello, world!"
#include <omp.h>
#include <stdio.h>

int main(int argc, char *argv[])
{
  int max_threads;
  scanf("%d", &max_threads);
  omp_set_num_threads(max_threads);
  #pragma omp parallel
  {
    printf("Hello, world!, i am process nubmer %d, overall number %d\n", omp_get_thread_num(), omp_get_max_threads());
  }
  return 0;
}
