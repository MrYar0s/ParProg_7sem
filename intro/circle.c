#include <omp.h>
#include <stdio.h>

int main(int argc, char *argv[])
{
  int val = 1939;
  #pragma omp parallel shared(val)
  {
    int thread_id = omp_get_thread_num();
    for (int i = 0; i < omp_get_max_threads(); i++) {
      if (i == thread_id) {
        val++;
        printf("Increased value by process %d equals to %d\n", thread_id, val);
      }
      #pragma omp barrier
    }
  }
  return 0;
}
