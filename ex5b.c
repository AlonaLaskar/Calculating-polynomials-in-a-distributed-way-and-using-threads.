#include <stdio.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>

#define LENGTH 510
char ** creatArray(char * str, int);
int CountsTheOperation(char * str);
int CalculationPolynomial(char * str);
int findTheLestNumcer(char * str);
void deleteTheWords(char** a,int t);
int value;
int main() {
    char str [LENGTH];

    while(1){
        fgets(str, LENGTH, stdin);
        if(strcmp("done" , str)==0){
            continue;
        }
        int freeNum = findTheLestNumcer(str);
        char * token1;
        token1 = strtok(str, ","); //polinum
        token1 = strtok(NULL, ","); //value
        value = atoi(token1);
        int counter = CountsTheOperation(str);
        char** array= creatArray(str,counter);
        key_t key ;
        if ((key = ftok("/tmp", 'y')) == -1)//Create an external key
        {
            perror("ftok() failed") ;
            deleteTheWords(array,counter);
            exit(EXIT_FAILURE) ;
        }

        int shm_id ;
        shm_id = shmget ( key, counter*sizeof (int), IPC_CREAT| IPC_EXCL | 0600 );//Create an internal key
        if (shm_id == -1){
            perror("shmget failed") ;
            exit(EXIT_FAILURE) ;
        }

        int *shm_ptr ;//Create a pointer that points to the memory space
        shm_ptr = (int *) shmat ( shm_id, NULL, 0 ) ;
        if (shm_ptr == (int *) -1)
        {
            perror( "shmat failed" ) ;
            exit( EXIT_FAILURE ) ;
        }

        pid_t son;
        int i;
        for(i=0; i<counter; i++){//Creating sons as the number of small polynomials that need calculation
            son =fork();
            if(son==-1){
                perror("the fork is failed");
                exit( EXIT_FAILURE ) ;
            }
            if(son==0) {
                shm_ptr[i] = CalculationPolynomial(array[i]);//Calculating the polynomial and writing it in the sharing memory
                exit(0);
            }
            wait(NULL);

        }
        int k, result=0;
        for(k=0; k< counter; k++){
            result+=shm_ptr[k];//Composing the results and writing them in the shared memory
        }
        result += freeNum;
        printf("%d\n" , result);
        deleteTheWords(array, counter);

        shmdt(shm_ptr) ;
        if (shmctl(shm_id,IPC_RMID, NULL) == -1) {//Release the pointer
            perror("shmctl failed");
            exit(EXIT_FAILURE);
        }

            result=0;


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
int CalculationPolynomial(char * str) {
    int rez = 1;
    for (int i = 0; i < strlen(str); i++) {
        if (str[i] == 'x') {
            if (i == 0) { //If the first element in the array is X, the coefficient is equal to 1
                for (int j = 0; j < strlen(str); j++) {
                    if (str[j] == '^') {
                        int exponent = (int) str[j + 1] - 48;
                        for (int k = 0; k < exponent; k++) {
                            rez *= value;
                        }
                        return rez;
                    }
                }
            }
            if (i != 0 && str[i - 1] == '+') { //If the organ is not first but in front of it there is a + the coefficient is equal to 1
                for (int m = 0; m < strlen(str); m++) {
                    if (str[m] == '^') {
                        int exponent = (int) str[m + 1] - 48;
                        for (int k = 0; k < exponent; k++) {
                            rez *= value;
                        }
                        return rez;
                    }
                }
            }
            if (i != 0 && str[i - 1] != '+') { //If there is a coefficient
                for (int j = 0; j < strlen(str); j++) {
                    if (str[j] == '^') {
                        int exponent = (int) str[j + 1] - 48;
                        for (int k = 0; k < exponent; k++) {
                            rez *= value;
                        }
                        rez *= (int) str[i - 2] - 48;
                        return rez;
                    }
                }
            }
        }
    }
    return rez;
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