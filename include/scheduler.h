#pragma once
#include "task.h"

void rate_monotonic_scheduler(Task tasks[], int num_tasks, int* timeline);
void earliest_deadline_first_scheduler(Task tasks[], int num_tasks, int* timeline);




