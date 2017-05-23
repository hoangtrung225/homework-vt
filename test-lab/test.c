#include <stdlib.h>
#include <stdio.h>
int global_variable_uninitualize;
int global_variable_initualized = 5;
const char text[] = "this is a constant string";

int recursion(int i);
int main(void){
	printf("global variable uninitualize at BSS segment: %p\n", &global_variable_uninitualize);
	printf("global variable initualized at : %p\n", &global_variable_initualized);
	printf("text \"%s\" in main at DATA segment and CODE segment ?: %p\n", text, text);
	printf("main function location at CODE segment: %p\n", (void*)main);
	printf("malloc function locate at header CODE segment from shared library: %p\n", (void*) malloc);	
	int local_at_main = 0;
	recursion(local_at_main);

	//dynamic allocate memory at heap segment
	int *heap_pointer = (int *)malloc(sizeof (int));
	printf("dynamic allocated memory at HEAP segment: %p\n", heap_pointer);
}

int recursion(int i){
	//local variable
	int local_on_stack;
	if(i > 5)	return 0;
	else{
		printf("local vaiable on STACK segment: %p\n", &local_on_stack);
		recursion(i+1);
	}
}
