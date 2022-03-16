#include <pthread.h>

#include <stdio.h>

#include <string.h>

#include <stdlib.h>

#define LENGTH 510
char ** creatArray(char * str, int);
int CountsTheOperation(char * str);
void * CalculationPolynomial(void * str);
int findTheLestNumcer(char * str);
void deleteTheWords(char** a,int t);
int value;
int result;
int main() {
    int status;
    char str[LENGTH];
    while (1) {
        result = 0;
        printf("enter a polinom:\n");
        fgets(str, LENGTH, stdin);
        int freeNum = findTheLestNumcer(str);
        char * token1;
        token1 = strtok(str, ","); //polinum
        token1 = strtok(NULL, ","); //value
        value = atoi(token1);
        int counter = CountsTheOperation(str);
        result += freeNum;
        char ** array = creatArray(str, counter);
        if ((strcmp(str, "done\n")) == 0) {
            deleteTheWords(array,counter);
            break;
        }
        pthread_t thread_data[counter];
        for (int i = 0; i < counter; ++i) { //creat thread
            status = pthread_create( & thread_data[i], NULL, CalculationPolynomial, (void * ) array[i]);
            if (status != 0) {
                fputs("pthread_creat failed in main\n", stderr);
                deleteTheWords(array,counter);
                exit(EXIT_FAILURE);
            }
        }
        for (int j = 0; j < counter; ++j) { //wait for all the threads
            pthread_join(thread_data[j], NULL);
        }
        printf("%d\n", result);
        deleteTheWords(array,counter);
    }

    return 0;
}
char ** creatArray(char * str, int counter) {//Crosses the polynomial into small parts for calculation and allocates memory
    int i = 0;
    char ** array = (char ** ) malloc(counter * sizeof(char * )); //Assign the array as large as the size of the words plus one
    if (array == NULL) { //Checks if there is place in the memory for allocation
        printf("Error:could not allocate memory");
        exit(1);
    }
    char * token;
    if (counter == 1) {
        token = strtok(str, "+"); //polinum
        int size = (int) strlen(token);
        array[i] = (char * ) malloc((size + 1) * sizeof(char)); //Dynamically assigns the small array
        if (array[i] == NULL) { //Checks if there is place in the memory for allocation
            printf("Error:could not allocate memory");
           deleteTheWords(array,size + 1);
            exit(1);
        }
        strcpy(array[i], token); //copy the words
    }
    if (counter == 2) {
        token = strtok(str, "+"); //polinum 1
        int size = (int) strlen(token);
        array[i] = (char * ) malloc((size + 1) * sizeof(char)); //Dynamically assigns the small array
        if (array[i] == NULL) { //Checks if there is place in the memory for allocation
            printf("Error:could not allocate memory");
            deleteTheWords(array,size + 1);
            exit(1);
        }
        strcpy(array[i], token); //copy the words
        i++;
        token = strtok(NULL, "+"); //polinum 2
        int size2 = (int) strlen(token);
        array[i] = (char * ) malloc((size2 + 1) * sizeof(char)); //Dynamically assigns the small array

        if (array[i] == NULL) { //Checks if there is place in the memory for allocation
            printf("Error:could not allocate memory");
            deleteTheWords(array,size2 + 1);
            exit(1);
        }
        strcpy(array[i], token); //copy the words
    }
    if (counter == 3) {
        token = strtok(str, "+"); //polinum 1
        int size = (int) strlen(token);
        array[i] = (char * ) malloc((size + 1) * sizeof(char)); //Dynamically assigns the small array

        if (array[i] == NULL) { //Checks if there is place in the memory for allocation
            printf("Error:could not allocate memory");
            deleteTheWords(array,size + 1);
            exit(1);
        }
        strcpy(array[i], token); //copy the words
        i++;
        token = strtok(NULL, "+"); //polinum 2
        int size2 = (int) strlen(token);
        array[i] = (char * ) malloc((size2 + 1) * sizeof(char)); //Dynamically assigns the small array

        if (array[i] == NULL) { //Checks if there is place in the memory for allocation
            printf("Error:could not allocate memory");
            deleteTheWords(array,size2 + 1);
            exit(1);
        }
        strcpy(array[i], token); //copy the words
        i++;
        token = strtok(NULL, "+"); //polinum 1
        int size3 = (int) strlen(token);
        array[i] = (char * ) malloc((size3 + 1) * sizeof(char)); //Dynamically assigns the small array
        if (array[i] == NULL) { //Checks if there is place in the memory for allocation
            printf("Error:could not allocate memory");
            deleteTheWords(array,size3 + 1);
            exit(1);
        }
        strcpy(array[i], token); //copy the words
        i++;
    }
    return array;
}
int CountsTheOperation(char * str) {//Count how many times an "X" appears
    int len, counter = 0;
    len = (int) strlen(str);
    for (int i = 0; i < len; ++i) {
        if (str[i] == 'x') {
            counter++;
        }
    }
    return counter;
}
void * CalculationPolynomial(void * str) {
    int rez = 1;
    char * newStr = (char * ) str;
    for (int i = 0; i < strlen(str); i++) {
        if (newStr[i] == 'x') {
            if (i == 0) { //If the first element in the array is X, the coefficient is equal to 1
                for (int j = 0; j < strlen(str); j++) {
                    if (newStr[j] == '^') {
                        int exponent = (int) newStr[j + 1] - 48;
                        for (int k = 0; k < exponent; k++) {
                            rez *= value;
                        }
                        result += rez;
                    }
                }
            }
            if (i != 0 && newStr[i - 1] == '+') { //If the organ is not first but in front of it there is a + the coefficient is equal to 1
                for (int m = 0; m < strlen(str); m++) {
                    if (newStr[m] == '^') {
                        int exponent = (int) newStr[m + 1] - 48;
                        for (int k = 0; k < exponent; k++) {
                            rez *= value;
                        }
                        result += rez;
                    }
                }
            }
            if (i != 0 && newStr[i - 1] != '+') { //If there is a coefficient
                for (int j = 0; j < strlen(str); j++) {
                    if (newStr[j] == '^') {
                        int exponent = (int) newStr[j + 1] - 48;
                        for (int k = 0; k < exponent; k++) {
                            rez *= value;
                        }
                        rez *= (int) newStr[i - 2] - 48;
                        result += rez;
                    }
                }
            }
        }
    }
    return NULL;
}
int findTheLestNumcer(char * str) {
    int len = (int) strlen(str);
    int numOfPlus = 0, counter = 0, freeNumner;
    for (int i = 0; i < len; ++i) {
        if (str[i] == '+') {
            numOfPlus++;
        }
    }
    for (int j = 0; j < len; ++j) {
        if (str[j] == '+') {
            counter++;
            if (counter == numOfPlus && str[j + 2] != '*') {
                freeNumner = (int) str[j + 1] - 48;
            } else
                freeNumner = 0;
        }
    }
    return freeNumner;
}
void deleteTheWords(char** a,int t){//free all the allocation of memory
    int m;
    for (m = 0; m<=t;m++) {
        free(a[m]);
    }
    free(a);
}

//gcc main.c -o main -lpthread
//2*x^3+3*x^2+2*x^1,2    //32

