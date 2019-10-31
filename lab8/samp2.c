#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <pthread.h>

#define SIZE 16

int num1;
int num2 = 5;

int my_func_pls(int x) {
	int temp[4] = {2, 1, 5, temp[0] * 2};
	return temp[3];
}

int main() {
	//int num1;
	//int num2 = 5;
	
	char *data1;
	char *data2 = malloc(10);

	data1 = malloc (SIZE);

	printf("address of run-time var (pointer) 'data1': %p\n", data1);
	printf("address of run-time var (pointer) 'data2': %p\n\n", data2);
	
	printf("address of uninitialized 'num1': %p\n", &num1);
	printf("address of initialized  'num2': %p\n\n", &num2);

	printf("address of library floor: %p\n", &floor);
	printf("address of library ceil: %p\n", &ceil);


	printf("address of lib asctime: %p\n", &asctime);
	printf("address of lib strtok: %p\n\n", &strtok);


	int arrTemp = my_func_pls(7);
	int another = my_func_pls(34);
	printf("address of stack func/var: %p\n", &arrTemp);
	printf("address of stack func/var2: %p\n\n", &another); 

	printf("address of program text/main: %p\n", &main);


	free(data1);
	free(data2);
	return 0;
}
