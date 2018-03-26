#include<stdio.h>
#include<stdlib.h>
#include<time.h>

#define MAXPROCESS 50

/* Structure for process */
typedef struct Process {
    int max_cycle;
    unsigned char *memory;
    int id;
    int cycle;
    int status;
} process;

/* Generate random number */
int gen_random(int bot,int top);

/* Main function */
int main(void){

    clock_t t_start,t_end;
    t_start = clock();
    srand(time(NULL));
    int t,i,cp,p;
    int memory_sum = 0;
    p = 0;
    cp = 0;//total number of completed processes
    process *process_list[MAXPROCESS];
    t = 0;//CPU time
    while(1){
        if(t%50==0 && p<MAXPROCESS){ // Creating new process for every 50 cycles
            process_list[p] = (process *) malloc(sizeof (process));
            process_list[p]->id = p+1;// process id

            // Setting cycles for a process between 200 to 2500 
            int cycle = gen_random(200,2500);
            process_list[p]->max_cycle = cycle;
            process_list[p]->cycle = 0;

            // Setting memory of process between 10KB to 2MB 
            int memory = gen_random(10000,2000000);
            process_list[p]->memory = malloc(memory);
            memory_sum += memory;//total memory
            process_list[p]->status = 0;
            printf("New process ID: %2d Memory: %7d[Bytes] Cycles: %4d\n",p,memory,cycle);
            p++;
        }

        // Loading new process into CPU 
        for(i=0;i<p;i++){
            if(process_list[i]->status == 0){
                process_list[i]->status = 1;
                printf("Process %d enters CPU.\n",process_list[i]->id);
            }
        }

         // Running all processes with status = 1 
        for(i=0;i<p;i++){
            if(process_list[i]->status == 1){
                process_list[i]->cycle++;
            }
        }

        // Stop process if reaches max cycles 
        for(i=0;i<p;i++){
            if(process_list[i]->cycle == process_list[i]->max_cycle && process_list[i]->status==1){
                // Free process memory
                free(process_list[i]->memory);
                process_list[i]->status = 2;
                printf("Process %d has finished\n",process_list[i]->id);
                cp++;
            }
        }
        if(cp==MAXPROCESS) break;

        t++;
    }

    // Free all process 
    for(i=0;i<p;i++){
        free(process_list[i]);
    }
    

    t_end = clock();
    printf("Total CPU cycles:%d\n",t);
    printf("Total memory usage:%d\n",memory_sum);
    printf("System time:%lf [ms]\n",(double)(t_end-t_start)*1000/CLOCKS_PER_SEC); 

    return 0;
}




// Function to generate random number for certain range 
int gen_random(int bot,int top){
    int r,seed = 0x7FB8; while(seed) r = (r + rand()+seed--) % (top-bot) + bot; return r;
}
