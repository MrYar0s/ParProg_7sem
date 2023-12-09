#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <time.h>
#include <mpi.h>

#define ISIZE 1000
#define JSIZE 1000

void printInFile(double *a, const char *path)
{
    FILE *ff = fopen(path, "w");
    for(int i= 0; i < ISIZE; i++){
        for (int j= 0; j < JSIZE; j++){
            fprintf(ff,"%f ", a[i*ISIZE + j]);
        }
        fprintf(ff,"\n");
    }
    fclose(ff);
}

int sequence_case(const char *path)
{
    double* a = (double*) calloc(ISIZE*JSIZE, sizeof(double));
    for (int i = 0; i < ISIZE; i++){
        for (int j = 0; j < JSIZE; j++){
            a[i*ISIZE + j] = 10*i +j;
        }
    }

    clock_t start, end;
    double cpu_time_used;

    start = clock();
    for (int i = 2; i < ISIZE; i++){
        for (int j = 4; j < JSIZE; j++){
            a[i*ISIZE + j] = sin(4*a[(i-2)*ISIZE + j-4]);
        }
    }
    end = clock();

    cpu_time_used = ((double) (end - start)) / CLOCKS_PER_SEC;

    printf("Time spent on cycle in sequence mode: %lg", cpu_time_used);

    printInFile(a, path);
    return 0;
}

int parallel_case(int argc, char **argv, const char *path)
{
    double* a = (double*) calloc(ISIZE*JSIZE, sizeof(double));
    for (int i = 0; i < ISIZE; i++){
        for (int j = 0; j < JSIZE; j++){
            a[i*ISIZE + j] = 10*i +j;
        }
    }
    int size = 0;
    int rank = 0;
    MPI_Init(&argc, &argv);
    MPI_Comm_size(MPI_COMM_WORLD, &size);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);

    int begin = (rank == 0) ? 4 : JSIZE / size * rank, end = (rank == size - 1) ? JSIZE : JSIZE / size * (rank + 1);

    int* num_of_data = (int*) calloc(size, sizeof(int));
    int* offset = (int*) calloc(size, sizeof(int));
    for (int i = 0; i < size; i++) {
        int begin = JSIZE/size*i;
        int end = (i != size - 1) ? JSIZE/size*(i+1) : JSIZE;
        num_of_data[i] = end - begin;
        offset[i] = (i != 0) ? offset[i-1]+num_of_data[i-1] : 0;
    }
    double* solution = (double*) calloc(ISIZE*JSIZE, sizeof(double));

    double start_time = MPI_Wtime();

    for (int i = 2; i < ISIZE; i++) {
        for (int j = begin; j < end; j++) {
            a[i*ISIZE + j] = sin(4*a[(i-2)*ISIZE + j-4]);
        }
        double tmp[JSIZE];
        MPI_Allgatherv(a + i * JSIZE + offset[rank], num_of_data[rank], MPI_DOUBLE, tmp, num_of_data, offset, MPI_DOUBLE, MPI_COMM_WORLD);
        for (int j = 0; j < JSIZE; j++) {
            a[i*ISIZE + j] = tmp[j];
        }
        MPI_Barrier(MPI_COMM_WORLD);
    }

    MPI_Barrier(MPI_COMM_WORLD);
    double end_time = MPI_Wtime();

    if (!rank) {
        printf("Time spent on cycle in parallel mode: %lg", end_time - start_time);
    }

    for (int i = 0; i < ISIZE; i++) {
        MPI_Gatherv(a + i * JSIZE + offset[rank], num_of_data[rank], MPI_DOUBLE, solution + i * JSIZE, num_of_data, offset, MPI_DOUBLE, 0, MPI_COMM_WORLD);
    }

    if (!rank) {
        printInFile(solution, path);
    }

    MPI_Finalize();

    return 0;
}

int main(int argc, char **argv)
{
    if (argc != 3) {
        printf("Wrong number of arguments\n");
        printf("First argument must be a mode: [sequence|parallel]\n");
        printf("Second argument must be a path to output file\n");
        return -1;
    }
    const char* mode = argv[1];
    const char* path = argv[2];
    if (!strcmp(mode, "sequence")) {
        sequence_case(path);
    } else if (!strcmp(mode, "parallel")) {
        parallel_case(argc, argv, path);
    } else {
        printf("First argument must be a mode: [sequence|parallel]\n");
    }
}