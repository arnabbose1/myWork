#define _GNU_SOURCE
#include<stdio.h>
#include<stdlib.h>
#include<time.h>
#include<pthread.h>
#include<string.h>
#include <errno.h>

struct th_struct{
  pthread_t threads;
};

struct data{
 int loopstart;
 int loopend;
 int tid;
};

double ops_avg;
int thread_count;
double loop_count;
char *arithmetic_performed;
float **A;
float **B;
float **C;
int MATSIZE;

// This declaration is for testing whether multiplication is correct.

//float a[16][16] = {{2.00,2.00,2.00,2.00,2.00,2.00,2.00,2.00,2.00,2.00,2.00,2.00,2.00,2.00,2.00,2.00},{2.00,2.00,2.00,2.00,2.00,2.00,2.00,2.00,2.00,2.00,2.00,2.00,2.00,2.00,2.00,2.00},{2.00,2.00,2.00,2.00,2.00,2.00,2.00,2.00,2.00,2.00,2.00,2.00,2.00,2.00,2.00,2.00},{2.00,2.00,2.00,2.00,2.00,2.00,2.00,2.00,2.00,2.00,2.00,2.00,2.00,2.00,2.00,2.00},{2.00,2.00,2.00,2.00,2.00,2.00,2.00,2.00,2.00,2.00,2.00,2.00,2.00,2.00,2.00,2.00},{2.00,2.00,2.00,2.00,2.00,2.00,2.00,2.00,2.00,2.00,2.00,2.00,2.00,2.00,2.00,2.00},{2.00,2.00,2.00,2.00,2.00,2.00,2.00,2.00,2.00,2.00,2.00,2.00,2.00,2.00,2.00,2.00},{2.00,2.00,2.00,2.00,2.00,2.00,2.00,2.00,2.00,2.00,2.00,2.00,2.00,2.00,2.00,2.00},{2.00,2.00,2.00,2.00,2.00,2.00,2.00,2.00,2.00,2.00,2.00,2.00,2.00,2.00,2.00,2.00},{2.00,2.00,2.00,2.00,2.00,2.00,2.00,2.00,2.00,2.00,2.00,2.00,2.00,2.00,2.00,2.00},{2.00,2.00,2.00,2.00,2.00,2.00,2.00,2.00,2.00,2.00,2.00,2.00,2.00,2.00,2.00,2.00},{2.00,2.00,2.00,2.00,2.00,2.00,2.00,2.00,2.00,2.00,2.00,2.00,2.00,2.00,2.00,2.00},{2.00,2.00,2.00,2.00,2.00,2.00,2.00,2.00,2.00,2.00,2.00,2.00,2.00,2.00,2.00,2.00},{2.00,2.00,2.00,2.00,2.00,2.00,2.00,2.00,2.00,2.00,2.00,2.00,2.00,2.00,2.00,2.00},{2.00,2.00,2.00,2.00,2.00,2.00,2.00,2.00,2.00,2.00,2.00,2.00,2.00,2.00,2.00,2.00},{2.00,2.00,2.00,2.00,2.00,2.00,2.00,2.00,2.00,2.00,2.00,2.00,2.00,2.00,2.00,2.00}};

//float b[16][16] = {{3.00,3.00,3.00,3.00,3.00,3.00,3.00,3.00,3.00,3.00,3.00,3.00,3.00,3.00,3.00,3.00},{3.00,3.00,3.00,3.00,3.00,3.00,3.00,3.00,3.00,3.00,3.00,3.00,3.00,3.00,3.00,3.00},{3.00,3.00,3.00,3.00,3.00,3.00,3.00,3.00,3.00,3.00,3.00,3.00,3.00,3.00,3.00,3.00},{3.00,3.00,3.00,3.00,3.00,3.00,3.00,3.00,3.00,3.00,3.00,3.00,3.00,3.00,3.00,3.00},{3.00,3.00,3.00,3.00,3.00,3.00,3.00,3.00,3.00,3.00,3.00,3.00,3.00,3.00,3.00,3.00},{3.00,3.00,3.00,3.00,3.00,3.00,3.00,3.00,3.00,3.00,3.00,3.00,3.00,3.00,3.00,3.00},{3.00,3.00,3.00,3.00,3.00,3.00,3.00,3.00,3.00,3.00,3.00,3.00,3.00,3.00,3.00,3.00},{3.00,3.00,3.00,3.00,3.00,3.00,3.00,3.00,3.00,3.00,3.00,3.00,3.00,3.00,3.00,3.00},{3.00,3.00,3.00,3.00,3.00,3.00,3.00,3.00,3.00,3.00,3.00,3.00,3.00,3.00,3.00,3.00},{3.00,3.00,3.00,3.00,3.00,3.00,3.00,3.00,3.00,3.00,3.00,3.00,3.00,3.00,3.00,3.00},{3.00,3.00,3.00,3.00,3.00,3.00,3.00,3.00,3.00,3.00,3.00,3.00,3.00,3.00,3.00,3.00},{3.00,3.00,3.00,3.00,3.00,3.00,3.00,3.00,3.00,3.00,3.00,3.00,3.00,3.00,3.00,3.00},{3.00,3.00,3.00,3.00,3.00,3.00,3.00,3.00,3.00,3.00,3.00,3.00,3.00,3.00,3.00,3.00},{3.00,3.00,3.00,3.00,3.00,3.00,3.00,3.00,3.00,3.00,3.00,3.00,3.00,3.00,3.00,3.00},{3.00,3.00,3.00,3.00,3.00,3.00,3.00,3.00,3.00,3.00,3.00,3.00,3.00,3.00,3.00,3.00},{3.00,3.00,3.00,3.00,3.00,3.00,3.00,3.00,3.00,3.00,3.00,3.00,3.00,3.00,3.00,3.00}};

//float c[16][16] = {0.00};

float **matrix_allocate(int row, int column) {
  float **matrix = malloc(row * sizeof(float*));
  int i;
  for (i = 0; i < row; i++) {
      matrix[i] = calloc(column, sizeof(*matrix[i]));
  }
  return matrix;
}

void Matmul(void* args){
 struct data *d = args; //struct that has data
 int i,j,x,K;
 
 printf("value of loop start %d\n",d->loopstart); 
 printf("Thread %d is running on CPU %d\n", d->tid, sched_getcpu()); 

  //********************************************************************************
 //This code can be uncommented to check if the matrix multiplication is correct
 //also uncomment above static array declaration.Also comment the for loop 
 //below this for loop in that case.
 //*********************************************************************************

 /*
 for (i = d->loopstart; i<MATSIZE; i++){
   for (j = 0; j < MATSIZE; j++){
     c[i][j] = 0;
     for (x = 0; x < MATSIZE; x++){
       *(*(C + i) + j) += *(*(A + i) + x) *
                           *(*(B + x) + j);
     } 
   } 
 }
 */

 for (i = d->loopstart; i<MATSIZE; i++){
   for (j = 0; j < MATSIZE; j++){
     C[i][j] = 0;
     for (x = 0; x < MATSIZE; x++){
       *(*(C + i) + j) += *(*(A + i) + x) *
                           *(*(B + x) + j);
     }
   }
 }
}

void ThreadCall(){

  int i=0,rc;
  
  struct th_struct ts[thread_count];
  double start_time,end_time,elapsed_time;
  int tid;
  struct timeval t;
  
  cpu_set_t cpu1, cpu2, cpu3, cpu4;
  int tid1,tid2,tid3,tid4;
  int temp;

  gettimeofday(&t, NULL);
  start_time = t.tv_sec+(t.tv_usec/1000000.0);

  //*************************INLINE COMMENT ***************************************
  // The threads are distributed along the rows. Meaning if there are 1000 rows
  // and 10 threads then 1 row will handle first 10 rows, then the 2nd thread will
  // handle next 10. No distribution has been done along the cols as that is not
  // possible with this approach.
  //*******************************************************************************

  for (tid=0; tid<thread_count;tid++) {
    struct data *d = (struct data *)malloc(sizeof(struct data));
    d->loopstart = tid*(MATSIZE/thread_count);
    d->loopend   = d->loopstart + (MATSIZE/thread_count);
    d->tid = tid; 
    rc = pthread_create(&ts[tid].threads, NULL, Matmul, d);
    if (rc) {
      printf("Could not create thread %d\n", i);
    }
  }
  //**********************INLINE COMMENT*******************************************
  // This is a try to distribute the threads to multiple cores.It is
  // successful in doing that but no significant improvement in execution
  // time has been seen even after doing that.
  //*******************************************************************************

  CPU_ZERO(&cpu1);
  CPU_SET(1, &cpu1);
  for (tid1 = 0; tid1 < thread_count/4; tid1++) {
   temp = pthread_setaffinity_np(ts[tid1].threads, sizeof(cpu_set_t), &cpu1);
  }

  CPU_ZERO(&cpu2);
  CPU_SET(2, &cpu2);
  for (tid2 = tid1; tid2 < thread_count/2; tid2++) {
    temp = pthread_setaffinity_np(ts[tid2].threads, sizeof(cpu_set_t), &cpu2);
  }

  CPU_ZERO(&cpu3);
  CPU_SET(3, &cpu3);
  for (tid3 = tid2; tid3 < thread_count/3; tid3++) {
    temp = pthread_setaffinity_np(ts[tid3].threads, sizeof(cpu_set_t), &cpu3);
  }

  CPU_ZERO(&cpu4);
  CPU_SET(4, &cpu4);
  for (tid4 = tid3; tid4 < thread_count; tid4++) {
    temp = pthread_setaffinity_np(ts[tid4].threads, sizeof(cpu_set_t), &cpu4);
  }


  for (tid=0;tid<thread_count;tid++) {
    rc = pthread_join(ts[tid].threads, NULL);
    if (rc) {
      printf("Could not join thread %d\n", i);
    }
  }

  gettimeofday(&t, NULL);
  end_time = t.tv_sec+(t.tv_usec/1000000.0); // Connvert microseconds to seconds.
  elapsed_time = end_time - start_time;
  ops_avg += (2*MATSIZE*MATSIZE*MATSIZE) / (elapsed_time * 1000000000); // convert into GFlops
   
  printf("ops_avg is %f GFLOPS\n", ops_avg);

}

int main(int argc,char **argv){

   
  thread_count = atoi(argv[2]);
  MATSIZE = argc < 2 ? 8 : atoi(argv[1]);
  A = matrix_allocate(MATSIZE,MATSIZE);
  B = matrix_allocate(MATSIZE,MATSIZE);
  C = matrix_allocate(MATSIZE,MATSIZE);

  int i,j,r,count=0;
  //int row=16;
  //int col=16;
  //printf("Max random number is %d\n", RAND_MAX);
  srand(time(NULL));

   
  for(i=0;i<MATSIZE;i++){
    for(j=0;j<MATSIZE;j++){

      r = rand();
      if (j%2==0){
        A[i][j]= B[i][j] = -r/RAND_MAX;
      }else{
        B[i][j]= B[i][j] =  r/RAND_MAX;
      }
    }
  }
  
  /*
  for(i=0;i<MATSIZE;i++){
    for(j=0;j<MATSIZE;j++){
      printf("%f ", *(*(A + i) + j));
    }
    printf("\n");
  }
  printf("\n\n");

  for(i=0;i<MATSIZE;i++){
    for(j=0;j<MATSIZE;j++){
      printf("%f ", *(*(B + i) + j));
    }
    printf("\n");
  }
  */

  for(i=0;i<3;i++){
    ThreadCall();
  }
  printf("ops_avg is %f GFLOPS\n", ops_avg/3); 
  
 /* 
  for (i = 0; i < MATSIZE; i++){ 
    for (j = 0; j < MATSIZE; j++){ 
      printf("%f ", *(*(C + i) + j)); 
    } 
    printf("\n"); 
  } 
*/ 
  return 0;
}

