#include "../include/task.h"
#include "../include/utils.h"
#include "../include/sched_new.h"
#include <stdbool.h>
#include <stdio.h>
#include <math.h>
#include <stdlib.h>
#include <time.h>

//calculate gcd of num1 and num2
int gcd(int num1, int num2)
{
    while(num2 != 0)
    {
        int temp = num2;
        num2 = num1%num2;
        num1 = temp;
    }

    return num1;
}

//calculate lcm of num1 and num2
int lcm(int num1, int num2)
{ 
    return (num1 * num2)/(gcd(num1,num2));
}

//calculate lcm of all periods to get the hyperperiod
int calculate_hyperperiod(Task tasks[], int num_tasks)
{
    int _lcm = tasks[0].period;
    for(int i = 1; i < num_tasks; i++)
    {
        _lcm = lcm(_lcm, tasks[i].period);
    }

    return _lcm;
}

//check for schedulability for EDF and RM
bool schedulability(Task tasks[], int num_tasks, char _type)
{

    //calculate CPU utilization for the given taskset
    double cpu_utilization = 0.00;
    for(int i = 0; i < num_tasks; i++)
    {
        cpu_utilization += (double)tasks[i].execution_time/tasks[i].period;
    }

    
    //if its a fixed priority type then check for the upper bound with liu-layland RM upper bound
    if(_type == 'F')
    {
        double upper_bound = num_tasks * (pow(2,1.00/num_tasks) - 1);

        if(cpu_utilization <= upper_bound)
        {
            printf("%f>%f schedulable under RM\n",upper_bound,cpu_utilization);
            return true;
        }
        else
        {
            printf("%f<%f not schedulable under RM\n",upper_bound,cpu_utilization);
            return false;
        }
    }
    //upper bound for edf is 1 i.e 100% utilization
    else if(_type == 'D')
    {
        double upper_bound = 1;
        if(cpu_utilization <= upper_bound)
        {
            printf("%f>%f schedulable under EDF\n",upper_bound,cpu_utilization);
            return true;
        }
        else
        {
            printf("%f <%f not schedulable under EDF\n",upper_bound,cpu_utilization);
            return false;
        }
    }
    else
    {
        printf("[ERROR]: no valid algorithm found with type %c\n",_type);
        return false;
    }

}

void print_taskset(Task tasks[], int num_tasks)
{
    printf("T#\tA\tP\tC\tD\n");
    for(int i = 0; i < num_tasks; i++)
    {
        printf("T%d\t%d\t%d\t%d\t%d\n",i,tasks[i].arrival_time,tasks[i].period,tasks[i].execution_time,tasks[i].relative_deadline);
    }
}


void plot_timeline(int schedule_log[][2], int total_time)
{
    for(int i = 0; i < total_time; i++)
    {
        if(schedule_log[i][0] != -1)
        printf("J%d,%d\t",schedule_log[i][0],schedule_log[i][1]);
        else
        printf("IDLE\t");
    }
    printf("\n");
}


Job* allocate_job(Task* task, int cur_time)
{
    //srand(time(NULL));

    Job* job = (Job*) malloc(sizeof(Job));
    
    job->absolute_deadline = cur_time + task->relative_deadline;
    job->actual_execution_time = task->execution_time; //(rand() % (task.execution_time  + 1));
    job->job_task = task;
    job->remaining_execution_time = job->actual_execution_time;

    return job;
}