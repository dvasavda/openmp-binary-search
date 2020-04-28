/*
    int i;
    int parallel_arr_size = input_arr/num_threads; // Get input from user for num_threads

# pragma omp parallel for
    for(i = 0; i < parallel_arr_size; i++) {
        int parallel_start = i * parallel_arr_size;
        int parallel_end = parallel_arr_size - 1;

        // binarySearch(int start, int end, int search_val)
        binarySearch(parallel_start, parallel_end, search_val);

    }

    if (search == search_val) {
        // trigger flag to let all threads know
    }

*/

/*

gcc -Wall -o binarySearch_openMP binarySearch_openMP.c
./binarySearch_openMP


clang -Xpreprocessor -fopenmp -lomp binarySearch_openmp.c -o binarySearch_openmp
./binarySearch_openmp

*/

#include "stdio.h"
#include "stdlib.h"
#include "string.h"
#include <sys/time.h> // High resolution timer
#include <omp.h>


const int ARR_SIZE = 200000000; // Add 2,000,000,000 elements into array
int randomNums[ARR_SIZE]; // Array of random integers
int parallel_search_found = 0;
double omp_get_wtime(void);


// High resolution timer
inline uint64_t rdtsc() {
    uint32_t lo, hi;
    __asm__ __volatile__ (
      "xorl %%eax, %%eax\n"
      "cpuid\n"
      "rdtsc\n"
      : "=a" (lo), "=d" (hi)
      :
      : "%ebx", "%ecx");
    return (uint64_t)hi << 32 | lo;
}


/*
    void readInputFile() -
        Takes contents from file input1.txt and places it into array randomNums[]
*/
void readInputFile() {
    FILE *inputFile;
    inputFile = fopen("input1.txt", "r");

    int i;

    if (inputFile == NULL){
        printf("Error Reading File input1.txt\n");
        exit(0);
    }

    printf("Populating randomNums[] with data from input1.txt...\n");
    printf("(This may take a while)\n");
    // Place file contents into array randomNums[]
    for (i = 0; i < ARR_SIZE; i++){
        fscanf(inputFile, "%d,", &randomNums[i]);
    }

    // // Print a few values from array to verify
    // for (int i = 0; i < 4; i++) {
    //     printf("%d\n", randomNums[i]);
    // }

    printf("Finished inserting %d elements into randomNums[]\n", ARR_SIZE);

    fclose(inputFile);

    printf("\n");

}



/*
    void binarySearch_serial() -
        Performs serial binary search on randomNums[]
*/
int binarySearch_serial(int searchVal) {

    int num_elements = ARR_SIZE;
    int first = 0;
    int last = num_elements - 1;

    while (first <= last) {
        int middle = first + (last - first) / 2;

        // Check if search value is present at mid
        if (randomNums[middle] == searchVal)
            return middle;

        // If search value greater, ignore left half
        if (randomNums[middle] < searchVal) {
            first = middle + 1;
        }

        // If search value is smaller, ignore right half
        else
            last = middle - 1;
    }

    // if we reach here, then element was
    // not present
    return -1;

}



/*
    void binarySearch_openmp() -
        Performs parallel binary search using OpenMP on randomNums[]
*/
int binarySearch_openmp(int first, int last, int searchVal) {

    // int tid = omp_get_thread_num();
    // printf("Hello World from thread = %d\n", tid);

    while (first <= last) {
        int middle = first + (last - first) / 2;

        // Check if search value is present at mid
        if (randomNums[middle] == searchVal)
            return middle;

        // If search value greater, ignore left half
        if (randomNums[middle] < searchVal) {
            first = middle + 1;
        }

        // If search value is smaller, ignore right half
        else
            last = middle - 1;
    }

    // if we reach here, then element was
    // not present
    return -1;
}


/*
    void serial_work() -
        Performs all work related to Serial code, including:
            - All print statements
            - Timing of Serial work
            - Binary search function calls
*/
void serial_work(int searchVal) {
    int result;
    double  start, end, total_time;

    printf("\n****** Now beginning Serial work ******\n\n");

    printf("Starting binary search...\n");

    start = omp_get_wtime();
    // Perform serial Binary search with timing
    // gettimeofday(&start, NULL);
    result = binarySearch_serial(searchVal); // Binary search here
    // gettimeofday(&end, NULL);

    end = omp_get_wtime();
    total_time = end - start;

    printf("Work took %f seconds\n", total_time);

    // printf("Time taken: %ld microseconds\n", ((end.tv_sec * 1000000 + end.tv_usec)
	// 	  - (start.tv_sec * 1000000 + start.tv_usec)));


    // Print results of serial Binary search
    if (result != -1) {
        printf("Element %d found! At index %d\n", searchVal, result);
    }
    else {
        printf("Element %d not found\n", searchVal);
    }
    printf("\n");

}


/*
    void parallel_work() -
        Performs all work related to Parallelized code, including:
            - All print statements
            - Timing of parallel work
            - Binary search function calls
*/
void parallel_work(int searchVal, int num_threads) {
    int result;
    // struct timeval start, end;
    double  start, end, total_time;

    // For use with Parallel search
    int first = 0;
    int last = ARR_SIZE - 1;
    int middle = first + (last - first) / 2;

    // Array will be sliced into sections
    int thread_one, thread_two, thread_three, thread_four;
    int quarter_slice = middle / 2;


    printf("\n****** Now beginning Parallel work with OpenMP ******\n\n");


    printf("Starting binary search...\n");

    // Perform serial Binary search with timing
    // gettimeofday(&start, NULL);

     start = omp_get_wtime();
#pragma omp parallel num_threads(num_threads)
    {
    #pragma omp sections
        {
            #pragma omp section
            thread_one = binarySearch_openmp(0, quarter_slice, searchVal); // Binary search here
            #pragma omp section
            thread_two = binarySearch_openmp(quarter_slice + 1, middle, searchVal); // Binary search here
            #pragma omp section
            thread_three = binarySearch_openmp(middle + 1, quarter_slice * 3, searchVal); // Binary search here
            #pragma omp section
            thread_four = binarySearch_openmp((quarter_slice * 3) + 1, last, searchVal); // Binary search here

            // result = binarySearch_openmp(first, last, searchVal); // Binary
        }
    }

    // start = omp_get_wtime();
// #pragma omp parallel num_threads(num_threads)
//     {

//         // thread_one = binarySearch_openmp(0, quarter_slice, searchVal);
//         // thread_two = binarySearch_openmp(quarter_slice + 1, middle, searchVal);
//         // thread_three = binarySearch_openmp(middle + 1, quarter_slice * 3, searchVal);
//         // thread_four = binarySearch_openmp((quarter_slice * 3) + 1, last, searchVal);

//         result = binarySearch_openmp(first, last, searchVal); // Binary Search
//     }

    end = omp_get_wtime();
    total_time = end - start;

    printf("Work took %f seconds\n", total_time);

    // gettimeofday(&end, NULL);

    // printf("Time taken: %ld microseconds\n", ((end.tv_sec * 1000000 + end.tv_usec)
	// 	  - (start.tv_sec * 1000000 + start.tv_usec)));


    // // Print results of serial Binary search
    if (result != -1) {
        printf("Element %d found! At index %d\n", searchVal, result);
    }
    else {
        printf("Element %d not found\n", searchVal);
    }
    printf("\n");


}



int main(int argc, char* argv[]) {
    int searchVal, num_threads;

    printf("Enter value to search for: \n");
    scanf("%d", &searchVal);

    printf("How many threads to run on: \n");
    scanf("%d", &num_threads);

    // Read contents of input file and populate array
    readInputFile();

    // Perform all serial work
    serial_work(searchVal);

    // Rewrite contents to array for consistency purposes
    readInputFile();


    // Perform all parallelized work
    parallel_work(searchVal, num_threads);


    return 0;
}