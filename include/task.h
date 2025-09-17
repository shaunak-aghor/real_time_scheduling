#pragma once

typedef struct task {
    int task_id;
    int arrival_time;
    int period;
    int execution_time;
    int relative_deadline;
    int next_arrival_time;
    int instance_counter;
} Task;


typedef struct job {
    Task* job_task;
    int actual_execution_time;
    int absolute_deadline;
    int remaining_execution_time;
} Job;