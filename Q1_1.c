#include <stdio.h>
#include <stdlib.h>
#include <strings.h>
#include <unistd.h>
#include <sys/wait.h>
#include <limits.h>
#include <pthread.h>
#include <ctype.h>
#include<sys/resource.h>
struct timespec t1, t2, t3, t4, t5, t6;
int priority_rr;
int priority_fifo;
int Other;
FILE *file;
struct sched_param parameter_fifo, parameter_rr, parameter_other;
void* countA(){
    pthread_t pid = pthread_self();
    setpriority(PRIO_PROCESS, 0, Other);
    pthread_setschedparam(pid ,SCHED_OTHER, &parameter_other);
    clock_gettime(CLOCK_REALTIME, &t1);
    for(long long int i = 1; i<=4294967296; i++){
        continue;
    }
    clock_gettime(CLOCK_REALTIME, &t2);
    char str[100];
    sprintf(str, "%f OTH\n", (double)((t2.tv_sec-t1.tv_sec)+((double)(t2.tv_nsec-t1.tv_nsec))/(double)1000000000L));
    fputs(str, file);
}

void* countB(){
    pthread_t pid = pthread_self();
    pthread_setschedparam(pid, SCHED_RR, &parameter_rr);
    clock_gettime(CLOCK_REALTIME, &t3);
    for(long long int i = 1; i<=4294967296; i++){
        continue;
    }
    clock_gettime(CLOCK_REALTIME, &t4);
    char str[100];
    sprintf(str, "%f RR\n", (double)((t4.tv_sec-t3.tv_sec)+((double)(t4.tv_nsec-t3.tv_nsec))/(double)1000000000L));
    fputs(str, file);
}

void* countC(){
    pthread_t pid = pthread_self();
    pthread_setschedparam(pid, SCHED_FIFO, &parameter_fifo);
    clock_gettime(CLOCK_REALTIME, &t5);
    for(long long int i = 1; i<=4294967296; i++){
        continue;
    }
    clock_gettime(CLOCK_REALTIME, &t6);
    char str[100];
    sprintf(str, "%f FIFO\n", (double)((t6.tv_sec-t5.tv_sec)+((double)(t6.tv_nsec-t5.tv_nsec))/(double)1000000000L));
    fputs(str, file);
}

int main(int argc, char *argv[]){

    file = fopen("Graph.txt", "a");
    pthread_t Thr_A, Thr_B, Thr_C;
    parameter_other.sched_priority = 0;
    parameter_rr.sched_priority = atoi(argv[2]);
    parameter_fifo.sched_priority = atoi(argv[1]);
    Other = atoi(argv[3]);
    pthread_create(&Thr_A,NULL,countA,NULL);
    pthread_create(&Thr_B,NULL,countB,NULL);
    pthread_create(&Thr_C,NULL,countC,NULL);
    pthread_join(Thr_A,NULL);
    pthread_join(Thr_B,NULL);
    pthread_join(Thr_C,NULL);   
    fclose(file);
    return 0;
}
