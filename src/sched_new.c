#include "../include/task.h"
#include "../include/utils.h"
#include "../include/priority_queue.h"
#include <math.h>
#include <stdlib.h>
#include <limits.h>


#define MIN(a,b) ((a) < (b) ? (a) : (b))

//rate monotonic scheduler
//task level fixed priority => higher the time period, lower the priority
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
        int next_decision_point = MIN(next_job_arrival,cur_finish_execution);
        

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
//job level fixed priority => farther the absolute deadline, lower the priority
//decision points: arrival of new job | finish execution of current job
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
        int next_decision_point = MIN(next_job_arrival, cur_finish_execution);
        

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

//least laxity first scheduler
//job level dynamic priority => higher the slack time, lower the priority
//decision points: arrival of new job | finish execution of current job | a job in ready queue gets laxity lesser than the currently running job
void least_laxity_first(Task tasks[], int num_tasks, int timeline[][2])
{
    int hyperperiod = calculate_hyperperiod(tasks, num_tasks);
    int time = 0;
    Node* Head = NULL;
    Job* executing_job = NULL;

    while (time < hyperperiod)
    {
        // Rebuild the ready queue at the start of a decision point
        // This recalculates laxity for all waiting jobs
        Head = rebuild_with_laxity(Head, time);

        // Add the jobs that have arrived into the ready queue
        for (int i = 0; i < num_tasks; i++)
        {
            if (tasks[i].next_arrival_time == time)
            {
                Job *cur_job = allocate_job(&tasks[i], time);
                
                // FIX 3: Push new jobs with their initial LAXITY, not deadline.
                int initial_laxity = cur_job->absolute_deadline - time - cur_job->remaining_execution_time;
                Head = push(Head, cur_job, initial_laxity);

                tasks[i].instance_counter++;
                tasks[i].next_arrival_time += tasks[i].period;
            }
        }

        executing_job = peek(Head);
        
        // Find decision points
        int cur_finish_execution = (executing_job != NULL) ? (time + executing_job->remaining_execution_time) : hyperperiod;
        
        int next_job_arrival = hyperperiod;
        for (int i = 0; i < num_tasks; i++)
        {
            next_job_arrival = next_job_arrival < tasks[i].next_arrival_time ? next_job_arrival : tasks[i].next_arrival_time;
        }

        // calculate the time where minimum laxity change occurs
        int preemption_time = hyperperiod;
        if (executing_job != NULL && Head->nextnode != NULL)
        {
            int laxity_diff = Head->nextnode->priority - Head->priority;
            preemption_time = time + laxity_diff + 1; 
        }

        int next_decision_point = MIN(cur_finish_execution, MIN(next_job_arrival, preemption_time));
        
        // Fill the timeline array
        for (int i = time; i < next_decision_point; i++)
        {
            if (executing_job != NULL)
            {
                timeline[i][0] = executing_job->job_task->task_id;
                timeline[i][1] = executing_job->job_task->instance_counter;
            }
            else
            {
                timeline[i][0] = -1; // CPU Idle
                timeline[i][1] = -1;
            }
        }

        // Update the state of the executing job
        if (executing_job != NULL)
        {
            executing_job->remaining_execution_time -= (next_decision_point - time);
            if (executing_job->remaining_execution_time <= 0)
            {
               
                Head = pop(Head);
            }
        }
        
      
        time = next_decision_point;
    }
}

