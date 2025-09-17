#include "../include/task.h"
#include "../include/utils.h"
#include "../include/scheduler.h"
#include <limits.h>

//simulate rate monotonic for a hyperperiod and put the timeline in the the array in the arguments
void rate_monotonic_scheduler(Task tasks[], int num_tasks, int* timeline) {
    int hyperperiod = calculate_hyperperiod(tasks, num_tasks);
    int remaining_time[num_tasks];

    for (int tick = 0; tick < hyperperiod; tick++) 
    {
        for (int j = 0; j < num_tasks; j++) 
        {
            if (tick % tasks[j].period == 0) 
            {
                remaining_time[j] = tasks[j].execution_time;
            }
        }

        int selected_task_index = -1;
        int shortest_period = INT_MAX;

    
        for (int k = 0; k < num_tasks; k++) 
        {
            if (remaining_time[k] > 0 && tasks[k].period < shortest_period) 
            {
                shortest_period = tasks[k].period;
                selected_task_index = k;
            }
        }

        if (selected_task_index != -1) 
        {
            remaining_time[selected_task_index]--;
            timeline[tick] = selected_task_index; 
        } else 
        {
            timeline[tick] = -1; 
        }
    }
}

void earliest_deadline_first_scheduler(Task tasks[], int num_tasks, int* timeline)
{
    int hyperperiod = calculate_hyperperiod(tasks, num_tasks);
    int remaining_time[num_tasks];
    int next_deadlines[num_tasks];

    // Initialize all tasks with 0 remaining time initially
    for(int i = 0; i < num_tasks; i++)
    {
        remaining_time[i] = 0;
        next_deadlines[i] = tasks[i].relative_deadline;
    }

    // Main simulation loop
    for(int tick = 0; tick < hyperperiod; tick++)
    {
        
        for (int i = 0; i < num_tasks; i++) {
            if (tick % tasks[i].period == 0) {
                remaining_time[i] = tasks[i].execution_time;
                next_deadlines[i] = tick + tasks[i].relative_deadline;
            }
        }

        
        int selected_task_index = -1;
        int earliest_deadline = INT_MAX;
        for(int i = 0; i < num_tasks; i++)
        {
           
            if(remaining_time[i] > 0 && next_deadlines[i] < earliest_deadline)
            {
                earliest_deadline = next_deadlines[i];
                selected_task_index = i;
            }
        }

        
        if(selected_task_index != -1)
        {
            remaining_time[selected_task_index]--;
            timeline[tick] = selected_task_index;
        }
        else
        {
            timeline[tick] = -1;
        }
    }
}


