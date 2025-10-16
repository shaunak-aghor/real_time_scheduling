#pragma once
#include "task.h"

void rate_monotonic_scheduler(Task tasks[], int num_tasks, int timeline[][2]);
void earliest_deadline_first_scheduler(Task tasks[], int num_tasks, int timeline[][2]);
void least_laxity_first(Task tasks[], int num_tasks, int timeline[][2]);



