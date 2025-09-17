#include "../include/task.h"
#include "../include/utils.h"
#include "../include/priority_queue.h"
#include <math.h>
#include <stdlib.h>
#include <limits.h>

//decision points: arrival of new job | finish execution of current job
void rate_monotonic_scheduler(Task tasks[], int num_tasks, int timeline[][2])
{
    int hyperperiod = calculate_hyperperiod(tasks,num_tasks);
    int time = 0;
    Node* Head = NULL;
    Job* executing_job = NULL;

    
    while (time < hyperperiod)
    {
        //add the jobs that have arrived into the ready queue
        for (int i = 0; i < num_tasks; i++)
        {
            if (tasks[i].next_arrival_time == time)
            {
                // allocate memory for the new incoming job and initialize its variables
                Job *cur_job = allocate_job(&tasks[i], time);
                // push the current job onto the priority queue and keep track of the head
                Head = push(Head, cur_job, tasks[i].period);

                //increment the instance counter
                tasks[i].instance_counter++;

                // update the next arrival time in task struct
                tasks[i].next_arrival_time += tasks[i].period;
            }
        }
        
        // the job that has the highest priority and is executing
        executing_job = peek(Head);
        
        // finding the time when the current job finishes executing
        int cur_finish_execution = executing_job != NULL ? time + executing_job->remaining_execution_time : hyperperiod;
        
        // finding when the next job arrives
        int next_job_arrival = hyperperiod;
        for (int i = 0; i < num_tasks; i++)
        {
            next_job_arrival = next_job_arrival < tasks[i].next_arrival_time ? next_job_arrival : tasks[i].next_arrival_time;
        }
        
        //find the next decision point and move time to the next decision point
        int next_decision_point = cur_finish_execution < next_job_arrival ? cur_finish_execution : next_job_arrival ;
        

        //setup the timline array to print out the variables
        for(int i = time; i < next_decision_point; i++)
        {
            if(executing_job != NULL)
            {
                //set it to the task_id if the cpu 
                timeline[i][0] = executing_job->job_task->task_id;
                timeline[i][1] = executing_job->job_task->instance_counter; 
            }
            else
            {
                timeline[i][0] = -1;
                timeline[i][1] = -1;
            }
        }

        //if the CPU was not idle, upadte the remaining time for the current running task
        //run the task till that time and pop the queue if the task finishes executing
        if (executing_job != NULL)
        {
            executing_job->remaining_execution_time -= (next_decision_point - time);
            if(executing_job->remaining_execution_time <=0)
            {
                Head = pop(Head);
            }
        }
        

        //move time to next decision point
        time = next_decision_point;
    }
}


//earliest deadline first scheduler 
void earliest_deadline_first_scheduler(Task tasks[], int num_tasks, int timeline[][2])
{
    int hyperperiod = calculate_hyperperiod(tasks,num_tasks);
    int time = 0;
    Node* Head = NULL;
    Job* executing_job = NULL;

    
    while (time < hyperperiod)
    {
        //add the jobs that have arrived into the ready queue
        for (int i = 0; i < num_tasks; i++)
        {
            if (tasks[i].next_arrival_time == time)
            {
                // allocate memory for the new incoming job and initialize its variables
                Job *cur_job = allocate_job(&tasks[i], time);
                // push the current job onto the priority queue and keep track of the head
                Head = push(Head, cur_job, cur_job->absolute_deadline);

                //increment the instance counter
                tasks[i].instance_counter++;

                // update the next arrival time in task struct
                tasks[i].next_arrival_time += tasks[i].period;
            }
        }
        
        // the job that has the highest priority and is executing
        executing_job = peek(Head);
        
        // finding the time when the current job finishes executing
        int cur_finish_execution = executing_job != NULL ? time + executing_job->remaining_execution_time : hyperperiod;
        
        // finding when the next job arrives
        int next_job_arrival = hyperperiod;
        for (int i = 0; i < num_tasks; i++)
        {
            next_job_arrival = next_job_arrival < tasks[i].next_arrival_time ? next_job_arrival : tasks[i].next_arrival_time;
        }
        
        //find the next decision point and move time to the next decision point
        int next_decision_point = cur_finish_execution < next_job_arrival ? cur_finish_execution : next_job_arrival ;
        

        //setup the timline array to print out the variables
        for(int i = time; i < next_decision_point; i++)
        {
            if(executing_job != NULL)
            {
                //set it to the task_id if the cpu 
                timeline[i][0] = executing_job->job_task->task_id;
                timeline[i][1] = executing_job->job_task->instance_counter; 
            }
            else
            {
                timeline[i][0] = -1;
                timeline[i][1] = -1;
            }
        }

        //if the CPU was not idle, upadte the remaining time for the current running task
        //run the task till that time and pop the queue if the task finishes executing
        if (executing_job != NULL)
        {
            executing_job->remaining_execution_time -= (next_decision_point - time);
            if(executing_job->remaining_execution_time <=0)
            {
                Head = pop(Head);
            }
        }
        

        //move time to next decision point
        time = next_decision_point;
    }
}



