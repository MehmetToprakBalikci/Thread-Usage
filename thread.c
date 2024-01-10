#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<unistd.h>
#include<math.h>
#include<pthread.h>

//global variables 
double globalSqrtSum = 0.0;
pthread_mutex_t mutex;

//creating a struct to store end , start of all threads
struct threadData{
    long long int  start;
    long long int  end;
};

int isHaveAnyError(int argc,char const *argv[]);

// this function gonna work if you enter d = 1  
void * threadFunction(void * threadPtr){

    struct threadData * ptr = (struct threadData *) threadPtr;

    long long int  start = ptr->start;
    long long int  end = ptr->end;

    // calculating globalSqrtSum 
    long long int x;
    for(x = start;x<=end;x++){
        globalSqrtSum += sqrt(x);
    }
    // deallocating a memory
    free(ptr);
    return 0;
}

// this function gonna work if you enter d = 2  
void * threadFunction2(void * threadPtr){

    struct threadData * ptr = (struct threadData *) threadPtr;

    long long int  start = ptr->start;
    long long int  end = ptr->end;

    //calculating globalSqrtSum with using mutex 
    long long int x;
    pthread_mutex_lock(&mutex);
    for(x = start;x<=end;x++){
        globalSqrtSum += sqrt(x);
    }
     pthread_mutex_unlock(&mutex);
    // deallocating memory 
    free(ptr);

    return 0;
}
// this function gonna work if you enter d = 3
void * threadFunction3(void * threadPtr){

    struct threadData * ptr = (struct threadData *) threadPtr;

    // getting start - end value of thread 
    long long int  start = ptr->start;
    long long int  end = ptr->end;
    
   //calculating globalSqrtSum
    long long int x;
    double localSqrtSum = 0;
    for(x = start;x<=end;x++){
        localSqrtSum += sqrt(x); 
    }
    // protecting critical section(globalSqrtSum)
    pthread_mutex_lock(&mutex);
    globalSqrtSum = localSqrtSum + globalSqrtSum;
    pthread_mutex_unlock(&mutex);

    free(ptr);
    return 0;
}
int main(int argc, char const *argv[])
{
    //checking whether there is any error or not
    if(isHaveAnyError(argc,argv) == 1 ){
        return 1;
    }

    //getting and parsing value 
    long long int a = atoll(argv[1]); // start number 
    long long int b = atoll(argv[2]); // end number 
    int c = atoi(argv[3]);// number of threads 
    int d = atoi(argv[4]);// function number 
    
    // creating thread arrays
    pthread_t myThreads[c];

    // range give us range that we have in a thread
    long long int  range =((b - a + 1 ) / c); 

    if(d == 1){
        // creating multple thread 
        for(int i = 0;i<c;i++){

            struct threadData *ptr = (struct threadData*) malloc(sizeof(struct threadData));
            ptr->start = a + i * range;
            ptr->end= (i == c - 1 ? b : a + (i+1) * range - 1);
            if(pthread_create(&myThreads[i],NULL,&threadFunction,ptr) != 0){
                printf("error has occured while thread creating\n");
                return 1;
            }      
        } 
    }else if( d == 2){
        // creating multple thread
        pthread_mutex_init(&mutex,NULL);
        for(int i = 0;i<c;i++){

            struct threadData *ptr = (struct threadData*) malloc(sizeof(struct threadData));
            ptr->start = a + i * range;
            ptr->end= (i == c - 1 ? b : a + (i+1) * range - 1);
            if(pthread_create(&myThreads[i],NULL,&threadFunction2,ptr) != 0){
                printf("error has occured while thread creating\n");
                return 1;
            }      
        } 
    }else{ // d = 3 
        //creating multiple thread 
        pthread_mutex_init(&mutex,NULL);
        for(int i = 0;i<c;i++){

            struct threadData *ptr = (struct threadData*) malloc(sizeof(struct threadData));
            ptr->start = a + i * range;
            ptr->end= (i == c - 1 ? b : a + (i+1) * range - 1);
            if(pthread_create(&myThreads[i],NULL,&threadFunction3,ptr) != 0){
                printf("error has occured while thread creating\n");
                return 1;
            }      
        } 
    }

    for(int i = 0;i < c;i++){
        // error checkhing while joining 
         if(pthread_join(myThreads[i],NULL) != 0){
            printf("error in join \n");
            return 1;
        }   
    } 
    //destroy mutex 
    pthread_mutex_destroy(&mutex);

    printf("Sum : %.6e \n",globalSqrtSum);

}

// checking there is an any error or not and return 1 or 0 
int isHaveAnyError(int argc,char const *argv[]){
    if (argc != 5) {
        printf("you entered missing arguments\n");
        return 1;
    } 
    // Convert command line arguments to appropriate types
    long long int a = atoll(argv[1]);
    long long int b = atoll(argv[2]);
    int c = atoi(argv[3]);// number of threads 
    int d = atoi(argv[4]);
    // checking arguments according to be numeric or not 
    if(a==0 || b==0 || c==0 || d == 0 ){
        printf("enter numeric input\n");
        return 1;
    }

    // checking commandline arguments condition
    if (a > b) {
        printf("Error: (a) must be less than or equal to (b)\n");
        return 1;
    }

    if (c  < 1) {
        printf("Error: Number of threads (c) must be at least 1\n");
        return 1;
    }
    if (d < 1 || d > 3) {
        printf("Error: Invalid method for (d) d must be 1 or 2 or 3 \n");
        return 1;
    }
    return 0; // there is no error
}
