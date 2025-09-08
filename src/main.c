#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "../include/task.h"
#include "../include/utils.h"
#include "../include/scheduler.h"

int main(int argc, char* argv[]) 
{
    if(argc < 2)
    {
        printf("usage %s <filename.txt>\n",argv[0]);
        return -1;
    }

    FILE* task_file = fopen(argv[1],"r");
    int num_tasks = 0;

    if(task_file == NULL)
    {
        printf("[ERROR]: Error opening %s\n",argv[1]);
        return -2;
    }


    char buffer[256];
    while (fgets(buffer, sizeof(buffer), task_file) != NULL) num_tasks++;

    num_tasks--; //the first line contains headings so subtract 1
    rewind(task_file); //rewind the file stream 

    Task tasks[num_tasks];
    fgets(buffer, sizeof(buffer), task_file); //move the stream to the next line
    for(int i = 0; i < num_tasks; i++)
    {

        int period,execution_time,deadline;

        fscanf(task_file,"%d\t%d\t%d\n",&period,&execution_time,&deadline);
        tasks[i].period = period;
        tasks[i].execution_time = execution_time;
        tasks[i].deadline = deadline;
    }

    print_taskset(tasks,num_tasks);
    printf("================================================================\n");
    if(schedulability(tasks,num_tasks,'F'))
    {
        int hyperperiod = calculate_hyperperiod(tasks, num_tasks);
        int timeline[hyperperiod];
        rate_monotonic_scheduler(tasks,num_tasks,timeline);
        plot_timeline(timeline,hyperperiod);
    }
    printf("================================================================\n");
    if(schedulability(tasks,num_tasks,'D'))
    {
        int hyperperiod = calculate_hyperperiod(tasks, num_tasks);
        int timeline[hyperperiod];
        earliest_deadline_first_scheduler(tasks,num_tasks,timeline);
        plot_timeline(timeline,hyperperiod);
    }

}
