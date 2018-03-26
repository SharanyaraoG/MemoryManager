#include<stdio.h>
#include<stdlib.h>
#include<time.h>

#define MAXPROCESS 50
#define MAXINDEX 50
#define MAXMEMORY 100000000

int Free_Available[MAXINDEX][2];

// process structure
typedef struct Process {
    int id;
    int max_cycle;
    int cycle;
    int memory_size;
    unsigned char* memory;
    int status;
    int address;

} process;


// my_malloc
int my_malloc(process *cprocess, unsigned char *main_memory){
    int j;
    for(j=0;j<MAXINDEX;j++){
        if(Free_Available[j][0]==-1) continue;
        // Found empty block
        if(Free_Available[j][1] - Free_Available[j][0] >= cprocess->memory_size){
            // Set memory address on Free_Available 
            cprocess->address = Free_Available[j][0];
            // Set memory buffer 
            cprocess->memory = cprocess->address + main_memory;
            // Shrink current memory block
            Free_Available[j][0] += cprocess->memory_size;
            return 1;
        }

    }

    return 0; // No available memory
}

//my_free 
void my_free(process *cprocess){
    int j;
    int pR=-1,pL=-1;
    
    // Find pR and pL
    for(j=0;j<MAXINDEX;j++){
        if(Free_Available[j][0] == cprocess->address + cprocess->memory_size) pR = j;
        if(Free_Available[j][1] == cprocess->address -1) pL = j;
    }

    // Resize Free_Available according to freed memory 
    if(pL != -1 && pR != -1){
        // Both left and right blocks are empty 
        Free_Available[pL][j] = Free_Available[pR][1];
        Free_Available[pR][0] = -1; Free_Available[pR][1] = -1;
    }else if(pL != -1 && pR == -1){
        // Left block is empty 
        Free_Available[pL][1] += cprocess->memory_size;
    }else if(pL == -1 && pR != -1){
        //Right block is empty 
        Free_Available[pR][0] += cprocess->address;
    }else{
        // Find empty Free_Available 
        for(j=0;j<MAXINDEX;j++){
            if(Free_Available[j][0]==-1){
                Free_Available[j][0] = cprocess->address;
                Free_Available[j][1] = cprocess->address + cprocess->memory_size -1;
                break;
            }
        }

    }

}

// Generatint random number 
int gen_random(int bot,int top);

int main(void){

    clock_t t_start,t_end;
    t_start = clock();

    srand(time(NULL));
    int t,i,cp,p;
    int memory_sum = 0;
    p = 0;
    cp = 0; //Total number of completed processes

    process *process_list[MAXPROCESS]; 

    for(i=0;i<MAXINDEX;i++){
        Free_Available[i][0] = -1;
        Free_Available[i][1] = -1;

    }
    Free_Available[0][0] = 0;
    Free_Available[0][1] = MAXMEMORY - 1;

    unsigned char *main_memory = malloc(MAXMEMORY);//allocating 100mb memory

    t = 0;//CPU time

    while(1){

        if(t%50==0 && p<MAXPROCESS){ // Creating new process for every 50 cycles

            process_list[p] = (process *)malloc(sizeof (process));

            process_list[p]->id = p+1;// process id

            // Setting cycles for a process between 200 to 2500 
            int cycle = gen_random(200,2500);
            process_list[p]->max_cycle = cycle;
            process_list[p]->cycle = 0;

            // Setting memory of process between 10KB to 2MB 
            int memory_size = gen_random(10000,2000000);
            process_list[p]->memory_size = memory_size;

            memory_sum += memory_size;
            process_list[p]->status = 0;
            printf("New process ID: %2d Memory: %7d[Bytes] Cycles: %4d\n",p,memory_size,cycle);
            p++;
        }

        // Loading new process into CPU 
        for(i=0;i<p;i++){
            process *cprocess = process_list[i];
            if(cprocess->status == 0){
                // Finding and allocating free memory block (first fit) 
                if(my_malloc(cprocess,main_memory)){
                    /* Run process */
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

        // Stop process if reaches max cycles 
        for(i=0;i<p;i++){
            process *cprocess = process_list[i];
            if(cprocess->cycle == cprocess->max_cycle && cprocess->status==1){
                // Free process memory 
                my_free(cprocess);
                // Updating process status 
                process_list[i]->status = 2;
                printf("Process %d has finished\n",process_list[i]->id);
                cp++;
            }
        }

        if(cp==MAXPROCESS) break;

        t++;
    }

    // Free all processes
    for(i=0;i<p;i++){
        free(process_list[i]);
    }
    free(main_memory);//deallocating main memory
    t_end = clock();

    printf("Total CPU cycles:%d\n",t);
    printf("Total memory usage:%d\n",memory_sum);
    printf("System time:%lf [ms]\n",((double)(t_end-t_start))*1000/CLOCKS_PER_SEC); 
   
    return 0;
}

// Function to generate random number for certain range 
int gen_random(int bot,int top){
    int r,seed = 0x2C1B; while(seed) r = (r + rand()+seed--) % (top-bot) + bot; return r;
}
