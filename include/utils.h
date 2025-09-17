#pragma once
#include <stdio.h>
#include <stdbool.h>
#include "task.h"

bool schedulability(Task tasks[], int num_tasks, char _type);
int calculate_hyperperiod(Task tasks[], int num_tasks);
void plot_timeline(int schedule_log[][2], int total_time);
Task* clone_tasks_array(Task* tasks, int n);
void print_taskset(Task tasks[], int num_tasks);
Job* allocate_job(Task* task, int cur_time);