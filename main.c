#include <stdio.h>
#include <stdlib.h>
#include <windows.h>
#include <time.h>

// Kernel functions
extern void daxpy_asm(int, double, double*, double*, double*);
void daxpy_c(int, double, double*, double*, double*);

// Helper functions
void print_array(int, double[]);
void initialize_values(unsigned int, int, double*, double*, double*);
double rand_double(int, int);
double time_run(void (*func)(int, double, double*, double*, double*),
	int, double, double*, double*, double*);

// Macros for n
#define TWO_TO_TWENTY 1048576
#define TWO_TO_TWENTY_FOUR 16777216
#define TWO_TO_TWENTY_EIGHT 268435456
#define TWO_TO_TWENTY_NINE 536870912
#define TWO_TO_THIRTY 1073741824

// Macro for total number of runs
#define RUNS 30

int main() {
	int n = TWO_TO_TWENTY_EIGHT;
	double A;

	double* X = (double*)malloc(n * sizeof(double));
	double* Y = (double*)malloc(n * sizeof(double));
	double* Z_c = (double*)malloc(n * sizeof(double));
	double* Z_asm = (double*)malloc(n * sizeof(double));	

	double exec_time_c[RUNS], exec_time_asm[RUNS];
	double exec_time_ave_c = 0, exec_time_ave_asm = 0;

	printf("Vector size n: %d\n", n);

	int curr_run;
	for (curr_run = 0; curr_run < RUNS; curr_run++) {
		printf("\n=== Run %02d of %d ===\n", curr_run + 1, RUNS);

		// Initialize values of A, X and Y randomly using (n + curr_run) as random seed
		initialize_values(n + curr_run, n, &A, X, Y);

		printf("A: %.2f\n", A);
		printf("X: ");
		print_array(10, X);
		printf("Y: ");
		print_array(10, Y);
		puts("");

		// Run and get execution time of each kernel function
		exec_time_c[curr_run] = time_run(daxpy_c, n, A, X, Y, Z_c);
		exec_time_asm[curr_run] = time_run(daxpy_asm, n, A, X, Y, Z_asm);
		exec_time_ave_c += exec_time_c[curr_run];
		exec_time_ave_asm += exec_time_asm[curr_run];

		printf("Z (C kernel)     : ");
		print_array(10, Z_c);
		printf("Z (x86-64 kernel): ");
		print_array(10, Z_asm);

		// Check if Z_c and Z_asm are equal
		// To speed up process, only check first 10 elements
		int equal = memcmp(Z_asm, Z_c, sizeof(double) * 10) == 0;
		if (equal) {
			puts("The C and x86-64 kernel outputs are equal for the first 10 values.\n");
		}
		else {
			puts("WARNING: The C and x86-64 kernel outputs are not equal.\n");
			return 1;
		}

		printf("Execution time (C kernel)     : %f s\n", exec_time_c[curr_run]);
		printf("Execution time (x86-64 kernel): %f s\n", exec_time_asm[curr_run]);
	}

	exec_time_ave_c /= RUNS;
	exec_time_ave_asm /= RUNS;

	puts("All runs finished successfully with equal output.");
	printf("\nAverage execution time (C kernel)     : %f s\n", exec_time_ave_c);
	printf("Average execution time (x86-64 kernel): %f s\n", exec_time_ave_asm);

	return 0;
}

// C DAXPY kernel function
void daxpy_c(int n, double A, double* X, double* Y, double* Z) {
	int i;
	for (i = 0; i < n; i++)
		*(Z + i) = A * (*(X + i)) + *(Y + i);
}

// Print the first n elements of arr
void print_array(int n, double arr[]) {
	int i;
	for (i = 0; i < n; i++)
		printf("%.1f ", arr[i]);
	printf("\n");
}

// Initialize the values of scalar A and vectors X and Y 
// to random double-precision floats between 0 and 100
void initialize_values(unsigned int seed, int n, double* A, double* X, double* Y) {
	srand(seed);
	
	*A = rand_double(0, 100);
	
	int i;
	for (i = 0; i < n; i++) {
		*(X + i) = rand_double(0, 100);
		*(Y + i) = rand_double(0, 100);
	}
}

// Return a random double-precision float between min and max
double rand_double(int min, int max) {
	int range = max - min;
	return ((double)rand() / RAND_MAX) * range + (double)min;
}

// Run the given function, and return the execution time in seconds
// Parameter "func" accepts kernel functions daxpy_c and daxpy_asm
double time_run(void (*func)(int, double, double*, double*, double*),
	int n, double A, double* X, double* Y, double* Z) {

	LARGE_INTEGER frequency, start, end;
	double exec_time;

	QueryPerformanceFrequency(&frequency);
	QueryPerformanceCounter(&start);

	func(n, A, X, Y, Z);
	
	QueryPerformanceCounter(&end);
	exec_time = (double)(end.QuadPart - start.QuadPart) / frequency.QuadPart;

	return exec_time;
}