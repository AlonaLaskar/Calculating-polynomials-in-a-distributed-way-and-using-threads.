Calculating polynomials in a distributed way  and using threads.
== Description ==
Functions:
This program calculates a polynomial long to the top 3. The calculation is done in parallel with the help of threads and shared memory.
==The difference between the exercises==
In the first exercise, I used threads to calculate the polynomial and with the help of a global variable and threads, there was information about the calculation process the threads calculated everything and  the main thread summarized the final result.
In the second exercise, I used shared memory and no threads.
The calculation is done by the sons and the father summarizes everyone's results and returns the desired result.
==profram files==
char ** creatArray(char * str, int)-This function gets a string and several times in the polynomial an "X" appears and according to it crosses the array into small polynomials and assigns memory for each.
Finally returns a two-dimensional array.
int CountsTheOperation(char * str)-This function counts how many times the character "X" appears and returns its quantity.
void * CalculationPolynomial(void * str)-This function calculates all the small polynomials sent to it.
Basically in this function we send the thread that will calculate the polynomials at the same time.
int findTheLestNumcer(char * str)-This function finds us the free number (without X).
void deleteTheWords(char** a,int t)-Releases all the allocated memory.
==how to compile==
compile:gcc ex5a.c -o ex5a -lpthread
compile:gcc ex5b.c -o ex5b 
run: ./ex5a
run: ./ ex5b
input:
The user will enter a polynomial up to 3 that he wants to calculate.
output:
The result of the calculation.
