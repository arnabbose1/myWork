#include<stdio.h>
#include<stdlib.h>
#include <sys/time.h>
#include<string.h>
#include<pthread.h>

double throughput=0;
int thread_count;
int block_size;
struct th_struct{
 pthread_t threads;
};

struct data{
 int loopstart;
 int loopend;
 int tid;
};

char *srcptr;
char *dstptr;
int array_size = 1073741824; //1GB converted into bytes and then array size
//int array_size = 524288000;
//int array_size = 104857600;
pthread_mutex_t mutex1 = PTHREAD_MUTEX_INITIALIZER;                 
//int array_size = 10;
//char srcptr[1073741824]; 
//char dstptr[1073741824];
double time_diff_avg=0;

void createData(char *ptr, int array_size){

  int i=0;
  for(i=0;i<=array_size;i++){
    ptr[i]= (char)(rand()%10+97); //converting random number to character
  }
}

void *seqReadWrite(void *args){

   int i,tmp;
   //char *dstptr;
   //int dest_array_size = 1073741824; //1GB converted into array size for destination array
   //dstptr = (char*)malloc(dest_array_size*sizeof(char));
   struct data *d = args; //struct that has data 
   double start_time,end_time,elapsed_time;
   struct timeval t;
   char *src_ptr,*dst_ptr;
  
   src_ptr = srcptr + (d->loopstart) + block_size;
   dst_ptr = dstptr + (d->loopstart) + block_size;

   gettimeofday(&t, NULL);
   start_time = t.tv_sec+(t.tv_usec/1000000.0);

   /* 
   for (i=array_size; i >= block_size; i-=block_size, srcptr+=block_size){
     dstptr=mempcpy(dstptr, srcptr, block_size);
   }
   if(i) {
     dstptr=mempcpy(dstptr, srcptr, i);
   }
   */

   //printf("This is thread %d\n",d->tid); 
   //printf("Value of loopend %d\n",d->loopend);
   //printf("Value of loopstart %d\n",d->loopstart);
   //printf("Value of srcptr %u\n",*(&srcptr));
   //printf("Value of dstptr %u\n",*(&dstptr));
   //printf("Value of src_ptr %u\n",*(&src_ptr));
   //printf("Value of dst_ptr %u\n",*(&dst_ptr));

   //printf("Value of thread_count is %d\n",thread_count);
 
   //for (i=d->loopend; i>=(d->loopstart+2*block_size); i-=block_size, src_ptr+=block_size){
   //  dst_ptr=mempcpy(dst_ptr, src_ptr, block_size);
   //}

   for(i=d->loopstart; i<(d->loopend-block_size); i+=block_size, src_ptr+=block_size){
     dst_ptr=mempcpy(dst_ptr, src_ptr, block_size);
   }
   //if(i) {
   //  dst_ptr=mempcpy(dst_ptr, src_ptr, i);
   //}

   gettimeofday(&t, NULL);
   end_time = t.tv_sec+(t.tv_usec/1000000.0); // Convert microseconds to seconds.
   elapsed_time = end_time - start_time;
   tmp = array_size/thread_count;
   throughput += tmp/elapsed_time;

   printf("Throughput for seq readwrite in thread %d = %lf bytes/second\n", d->tid,(tmp/elapsed_time));
}

void *randomReadWrite(void *args){

   int i,tmp;
   struct data *d = args; //struct that has data 
   double start_time,end_time,elapsed_time;
   struct timeval t;
   char *src_ptr,*dst_ptr;
   long index;
 
   src_ptr = srcptr + (d->loopstart);
   dst_ptr = (char*)malloc((d->loopend-d->loopstart)*sizeof(char));
   srand(time(NULL));
   printf("loop start %d\n",d->loopstart);
   printf("loop end %d\n",d->loopend);

   gettimeofday(&t, NULL);
   start_time = t.tv_sec+(t.tv_usec/1000000.0);
   
   //pthread_mutex_lock( &mutex1 );

   for(i=0;i<(d->loopend-d->loopstart); i+=block_size){
     index = rand()%200;
     mempcpy(&dst_ptr[index], &src_ptr[index], block_size);
   }
   //pthread_mutex_unlock( &mutex1 );

   gettimeofday(&t, NULL);
   end_time = t.tv_sec+(t.tv_usec/1000000.0); // Convert microseconds to seconds.
   elapsed_time = end_time - start_time;
   tmp = array_size/thread_count;
   throughput += tmp/elapsed_time;

   printf("Throughput for random readwrite in thread %d = %lf bytes/second\n", d->tid,(tmp/elapsed_time));
}

double get_avg(double a[]){
  int s;
  double sum=0;
  double avg;
  int length = sizeof(a)/sizeof(double);
  for(s = 0; s <length ; s++){
    sum+=a[length];
  }
  avg=sum/length;
  return avg;
}

long timediff(clock_t t1, clock_t t2) {
  long elapsed;
  elapsed = ((double)t2 - t1) / CLOCKS_PER_SEC * 1000000 ;
  return elapsed;
}

void *seqReadWriteLatency(void *args){

   int i,tmp;
   //char *dstptr;
   //int dest_array_size = 1073741824; //1GB converted into array size for destination array
   //dstptr = (char*)malloc(dest_array_size*sizeof(char));
   struct data *d = args; //struct that has data 
   char *src_ptr,*dst_ptr;
   int buffer_size =1;
   clock_t t1,t2;
   double elapsed[1000000];
   double thread_wise_latency; 

   src_ptr = srcptr + (d->loopstart);
   //dst_ptr = dstptr + (d->loopstart);
   dst_ptr = (char*)malloc((d->loopend-d->loopstart)*sizeof(char));

   //printf("loop start %d\n",d->loopstart);
   //printf("loop end %d\n",d->loopend);

  
   for(i=0; i<1000000; i+=buffer_size){
     t1 = clock();
     mempcpy(&dst_ptr[i], &src_ptr[i], buffer_size);
     t2 = clock();
     elapsed[i]=timediff(t1,t2);
   }

   thread_wise_latency=get_avg(elapsed);
   time_diff_avg+=thread_wise_latency;
   printf("Latency: %lf ns\n", thread_wise_latency);
 
}

void *randomReadWriteLatency(void *args){

   int i,tmp;
   struct data *d = args; //struct that has data 
   char *src_ptr,*dst_ptr;
   long index;
   int buffer_size =1;
   clock_t t1,t2;
   double elapsed[1000000];
   double thread_wise_latency; 

   src_ptr = srcptr + (d->loopstart);
   dst_ptr = (char*)malloc((d->loopend-d->loopstart)*sizeof(char));
   srand(time(NULL));
   //printf("loop start %d\n",d->loopstart);
   //printf("loop end %d\n",d->loopend);


   //pthread_mutex_lock( &mutex1 );

   for(i=0;i<1000000; i+=buffer_size){
     index = rand()%200;
     t1 = clock();
     mempcpy(&dst_ptr[index], &src_ptr[index], buffer_size);
     t2 = clock();
     elapsed[i]=timediff(t1,t2);
   }
   //pthread_mutex_unlock( &mutex1 );

   thread_wise_latency=get_avg(elapsed);
   time_diff_avg+=thread_wise_latency;
   printf("Latency: %lf ns\n", thread_wise_latency);

}

int main(int argc, char **argv){

  //char *srcptr;
  //char *dstptr;
  //int array_size = 1073741824; //1GB converted into bytes and then array size
  srcptr = (char*)malloc(array_size*sizeof(char));
  dstptr = (char*)malloc(array_size*sizeof(char));

  int number_times = 100;
  int count;
  int tid,rc;
  struct th_struct ts[thread_count];
  thread_count = atoi(argv[2]);
  block_size =   atoi(argv[1]);

  //fill up the array.
  createData(srcptr, array_size);
 
   
  for(count=1; count<=number_times;count++){  

    //seqReadWrite
    printf("*************%d time************\n", count); 

    for (tid=0; tid<thread_count;tid++) {
      //printf("Inside thread create thread %d \n",tid);
      struct data *d = (struct data *)malloc(sizeof(struct data));
      d->loopstart = tid*(array_size/thread_count);
      d->loopend   = d->loopstart + (array_size/thread_count);
      d->tid = tid;

      rc = pthread_create(&ts[tid].threads, NULL, seqReadWrite, d);

      if (rc) {
        printf("Could not create thread %d\n", tid);
      }
    }

    for (tid=0;tid<thread_count;tid++) {
      rc = pthread_join(ts[tid].threads, NULL);
      if (rc) {
        printf("Could not join thread %d\n", tid);
      }
    }
   
  }
  
  printf("Avg throughput for seq readwrite = %lf  bytes/second\n", (throughput/number_times*thread_count));
  
 
  throughput=0.00; // setting throughput to 0 again
  
  for(count=1; count<=number_times;count++){

    //randomReadWrite
    printf("*************%d time************\n", count);

    for (tid=0; tid<thread_count;tid++) {
      //printf("Inside thread create thread %d \n",tid);
      struct data *d = (struct data *)malloc(sizeof(struct data));
      d->loopstart = tid*(array_size/thread_count);
      d->loopend   = d->loopstart + (array_size/thread_count);
      d->tid = tid;

      rc = pthread_create(&ts[tid].threads, NULL, randomReadWrite, d);

      if (rc) {
        printf("Could not create thread %d\n", tid);
      }
    }

    for (tid=0;tid<thread_count;tid++) {
      rc = pthread_join(ts[tid].threads, NULL);
      if (rc) {
        printf("Could not join thread %d\n", tid);
      }
    }

  }

  printf("Avg throughput for randomreadwrite = %lf  bytes/second\n", (throughput/number_times*thread_count));
  
  
  for(count=1; count<=number_times;count++){

    printf("*************%d time************\n", count);

    for (tid=0; tid<thread_count;tid++) {
      //printf("Inside thread create thread %d \n",tid);
      struct data *d = (struct data *)malloc(sizeof(struct data));
      d->loopstart = tid*(array_size/thread_count);
      d->loopend   = d->loopstart + (array_size/thread_count);
      d->tid = tid;

      rc = pthread_create(&ts[tid].threads, NULL, randomReadWriteLatency, d);

      if (rc) {
        printf("Could not create thread %d\n", tid);
      }
    }

    for (tid=0;tid<thread_count;tid++) {
      rc = pthread_join(ts[tid].threads, NULL);
      if (rc) {
        printf("Could not join thread %d\n", tid);
      }
    }
  }
  
  printf("Overall latency for randomReadWrite %lf ns\n",(time_diff_avg/(number_times*thread_count)));
  time_diff_avg = 0;

  
  for(count=1; count<=number_times;count++){

    printf("*************%d time************\n", count);

    for (tid=0; tid<thread_count;tid++) {
      //printf("Inside thread create thread %d \n",tid);
      struct data *d = (struct data *)malloc(sizeof(struct data));
      d->loopstart = tid*(array_size/thread_count);
      d->loopend   = d->loopstart + (array_size/thread_count);
      d->tid = tid;

      rc = pthread_create(&ts[tid].threads, NULL, seqReadWriteLatency, d);

      if (rc) {
        printf("Could not create thread %d\n", tid);
      }
    }

    for (tid=0;tid<thread_count;tid++) {
      rc = pthread_join(ts[tid].threads, NULL);
      if (rc) {
        printf("Could not join thread %d\n", tid);
      }
    }
  }
  
  printf("Overall latency for seqReadWrite %lf ns\n",(time_diff_avg/(number_times*thread_count)));
  

  return 0;
}

