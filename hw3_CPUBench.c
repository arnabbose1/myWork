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

double ops_avg;
int thread_count;
double loop_count;
char *arithmetic_performed;

void ThreadFunction(void* args){

 double i;
 float a,x;
 float b,y;
 float c,z;
 float d,m;
 float e,n;
 long tid;
 tid = (long) args;

 if(strcmp(arithmetic_performed,"SP")==0){
   a=0.2,x=0.7;
   b=0.3,y=0.8;
   c=0.4,z=0.9;
   d=0.5,m=0.1;
   e=0.6,n=0.2;
 } else if(strcmp(arithmetic_performed,"DP")==0){
   a=0.22,x=0.77;
   b=0.33,y=0.88;
   c=0.44,z=0.99;
   d=0.55,m=0.11;
   e=0.66,n=0.22;
 }

 loop_count = 1000000000000/(thread_count*100);
 printf("Thread %ld is running on CPU %d\n", tid, sched_getcpu());

 for(i=0;i<loop_count;i++){

     a+b;a+c;a+d;a+a;a+e;
     b+a;b+b;b+c;b+d;b+e;
     c+a;c+b;c+d;c+c;c+e;
     d+a;d+b;d+c;d+d;d+e;
     e+a;e+b;e+c;e+d;e+e;
     a-b;a-c;a-d;a-a;a-e;
     b-a;b-b;b-c;b-d;b-e;
     c-a;c-b;c-d;c-c;c-e;
     d-a;d-b;d-c;d-d;d-e;
     e-a;e-b;e-c;e-d;e-e;

     x+y;x+x;x+z;x+m;x+n;
     y+x;y+y;y+z;y+m;y+n;
     z+x;z+y;z+z;z+m;z+n;
     m+x;m+y;m+z;m+m;m+n;
     n+x;n+y;n+z;n+m;n+n;
     x-y;x-x;x-z;x-m;x-n;
     y-x;y-y;y-z;y-m;y-n;
     z-x;z-y;z-z;z-m;z-n;
     m-x;m-y;m-z;m-m;m-n;
     n-x;n-y;n-z;n-m;n-n;
 }

 printf("loop done\n");

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

  // getting the current system time.
  gettimeofday(&t, NULL);
  start_time = t.tv_sec+(t.tv_usec/1000000.0);

  // starting thread
  for (tid=0; tid<thread_count;tid++) {
    printf("Inside thread create\n");
    rc = pthread_create(&ts[tid].threads, NULL, ThreadFunction, (void*) tid);
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
  end_time = t.tv_sec+(t.tv_usec/1000000.0); //Converting microseconds to seconds
  elapsed_time = end_time - start_time;
  ops_avg += (loop_count*100) / (elapsed_time * 1000000000); //Couting GFLOPS by deviding with 10^9
 
  
}

int main(int argc,char **argv){

  
  thread_count = atoi(argv[1]);
  arithmetic_performed = argv[2];
  int i=0;
  ops_avg=0;
  for(i=0;i<3;i++){
    ThreadCall();
  }

  // Calculating avg FLOPS after 3 try
  printf("ops_avg is %f\n", ops_avg/3);
  return 0;
}

