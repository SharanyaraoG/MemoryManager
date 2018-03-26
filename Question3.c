#include<stdio.h>
#include<stdlib.h>
#include<time.h>

#define MAXPROCESS 50
#define MAXPARTITION 20
#define BLOCKSIZE 5000000
#define MAXMEMORY 100000000

int partition[MAXPARTITION];//creating partitions

//structure for process having cycles and memory footprint
typedef struct Process {
    int id;
    int max_cycle;
    int cycle;
    int memory_size;
    unsigned char* memory;
    int status;
    int address;

} process;


int my_malloc(process *cprocess, unsigned char *main_memory){
    int j;
    for(j=0;j<MAXPARTITION;j++){
        if(partition[j]==0){
            cprocess->address = j;//setting memory address partition
            cprocess->memory = main_memory + j * BLOCKSIZE;
            partition[j] = 1;// Set current memory block as used
            return 1;
        }
    }

    return 0;
}

int gen_random(int bot,int top);

// Function to free memory 
void my_free(process *cprocess){
    partition[cprocess->address]=0;// Setting current partition as free 
}

int main(void){

    clock_t t_start,t_end;
    t_start = clock();

    srand(time(NULL));
    int t,i,cp,p;
    int memory_sum = 0;
    p = 0;///number of processes 
    cp = 0;// number of completed processes 

    process *process_list[MAXPROCESS];   // Creating a list of processes

    for(i=0;i<MAXPARTITION;i++){
        partition[i] = 0; // Creating empty partitions

    }

    unsigned char *main_memory = malloc(MAXMEMORY);//allocating 100mb memory

    t = 0;
    while(1){

        if(t%50==0 && p<MAXPROCESS){	// Creating new process 
            process_list[p] = (process *)malloc(sizeof (process));
            process_list[p]->id = p+1;//process id

            // Setting cycles for a process between 200 to 2500 
            int cycle = gen_random(200,2500);
            process_list[p]->max_cycle = cycle;
            process_list[p]->cycle = 0;

            //Setting memory for a process between 10KB to 2MB 
            int memory_size = gen_random(10000,2000000);
            process_list[p]->memory_size = memory_size;

            memory_sum += memory_size;

            process_list[p]->status = 0;//setting process state to be zero

            printf("New process ID: %2d Memory: %7d[Bytes] Cycles: %4d\n",p,memory_size,cycle);

            p++;
        }

        // Loading new process into CPU 
        for(i=0;i<p;i++){
            process *cprocess = process_list[i];
            if(cprocess->status == 0){
                if(my_malloc(cprocess,main_memory)){
                    cprocess->status = 1;
                    printf("Process %d enters CPU.\n",cprocess->id);
                }
            }
        }


        // Running all processes with status = 1 
        for(i=0;i<p;i++){
            if(process_list[i]->status == 1){
                process_list[i]->cycle++;
            }
        }

        // Stop process if it reaches max cycles 
        for(i=0;i<p;i++){
            process *cprocess = process_list[i];
            if(cprocess->cycle == cprocess->max_cycle && cprocess->status==1){
                // Free process memory 
                my_free(cprocess);
                // Update process status 
                process_list[i]->status = 2;
                printf("Process %d has finished\n",process_list[i]->id);
            }
        }

        // Check if all process finished 
        if(p==MAXPROCESS){
            int finish = 1;
            for(i=0;i<p;i++){
                if(process_list[i]->status!=2){
                    finish = 0;
                    break;
                }
            }
            if(finish) break;
        }
	t++;
    }

    // Free all process 
    for(i=0;i<p;i++){
        free(process_list[i]);
    }

    // Free main memory
    free(main_memory);
    t_end = clock();

    printf("Total CPU cycles:%d\n",t);
    printf("Total memory usage:%d\n",memory_sum);
    printf("System time:%lf [ms]\n",((double)(t_end-t_start))*1000/CLOCKS_PER_SEC); 
   
    return 0;
}

// Function to generate random number for certain range 
int gen_random(int bot,int top){
    int r,seed = 0x3A6F; while(seed) r = (r + rand()+seed--) % (top-bot) + bot; return r;
}
