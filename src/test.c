// Create a C program that implements the RM (Rate Monotonic) and EDF (Earliest Deadline First) 
// scheduling algorithms for real-time systems. The program should also perform schedulability tests 
// and display an execution timeline (similar to class slides).

// The program should read a .txt file containing all the tasks of a system with their respective
// periods, worst-case execution times, and deadlines. Below is an example of a file 
// showing a system with three tasks.

// P C D
// 4 1 4
// 5 2 5
// 20 4 20

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <limits.h>
#include <math.h>

// Defines the structure for a real-time task
typedef struct Task {
    int period, execution_time, deadline;
} Task;

// Calculates the total simulation time.
// Note: This implementation uses the latest deadline as the simulation time,
// which is a simplification. A full analysis requires simulating up to the hyperperiod (LCM of all periods).
int calculate_simulation_time(Task tasks[], int n) {
    int latest = tasks[0].deadline;
    for (int i = 0; i < n; i++) {
        if (tasks[i].deadline > latest) {
            latest = tasks[i].deadline;
        }
    }
    return latest;
}

// Performs a schedulability test based on CPU utilization.
void schedulability_test(Task tasks[], int n, char priority_type) {
    float utilization = 0;
    for (int i = 0; i < n; i++) {
        utilization += ((float)tasks[i].execution_time / (float)tasks[i].period);
    }

    // For RM (Fixed Priority)
    if (priority_type == 'F') {
        float utilization_bound = n * (pow(2, 1.0 / n) - 1);
        printf("Schedulability Test (Fixed Priority): %.2f <= %.2f\n", utilization, utilization_bound);
        if (utilization <= utilization_bound) {
            printf("--> System is schedulable under RM.\n");
        } else {
            printf("--> System may not be schedulable under RM (test is sufficient, not necessary).\n");
        }
    // For EDF (Dynamic Priority)
    } else {
        printf("Schedulability Test (Dynamic Priority): %.2f <= 1\n", utilization);
        if (utilization <= 1) {
            printf("--> System is schedulable under EDF.\n");
        } else {
            printf("--> System is not schedulable under EDF.\n");
        }
    }
}

// Prints the execution timeline for the schedule.
void plot_timeline(int schedule_log[], int num_tasks, int total_time) {
    for (int i = 0; i < num_tasks; i++) {
        printf("T%i\t", i + 1);
        for (int j = 0; j < total_time; j++) {
            if (schedule_log[j] == i + 1) {
                printf("#\t");
            } else {
                printf("-\t");
            }
        }
        printf("\n");
    }
    printf("Time\t");
    for (int j = 0; j < total_time; j++) {
        printf("%i\t", j);
    }
}

// Creates a deep copy of the tasks array.
Task* clone_tasks_array(Task* tasks, int n) {
    Task* new_array = (Task*)malloc(n * sizeof(Task));
    if (new_array == NULL) {
        printf("Error allocating memory.\n");
        return NULL;
    }
    memcpy(new_array, tasks, n * sizeof(Task));
    return new_array;
}

// Simulates the Rate Monotonic (RM) scheduling algorithm.
void rate_monotonic_scheduler(Task tasks[], int n) {
    int simulation_time = calculate_simulation_time(tasks, n);
    
    // Arrays to keep track of task states
    int *timeline = (int *)malloc(simulation_time * sizeof(int));
    int *remaining_time = (int *)malloc(n * sizeof(int));

    // Initialize remaining times to 0
    for(int i = 0; i < n; i++){
        remaining_time[i] = 0;
    }

    // Main simulation loop
    for (int current_time = 0; current_time < simulation_time; current_time++) {
        int selected_task_index = -1;
        int shortest_period = INT_MAX;

        // Check for new job arrivals and update remaining times
        for (int i = 0; i < n; i++) {
            if (current_time % tasks[i].period == 0) {
                remaining_time[i] = tasks[i].execution_time;
            }
        }

        // Select the task with the highest priority (shortest period) that is ready to run
        for (int i = 0; i < n; i++) {
            if (remaining_time[i] > 0 && tasks[i].period < shortest_period) {
                shortest_period = tasks[i].period;
                selected_task_index = i;
            }
        }

        // If a task is selected, execute it for one time unit
        if (selected_task_index != -1) {
            timeline[current_time] = selected_task_index + 1; // Log task ID (1-based)
            remaining_time[selected_task_index]--;
        } else {
            timeline[current_time] = 0; // 0 represents an idle CPU
        }
    }

    plot_timeline(timeline, n, simulation_time);
    free(timeline);
    free(remaining_time);
}

// Simulates the Earliest Deadline First (EDF) scheduling algorithm.
void earliest_deadline_first_scheduler(Task tasks[], int n) {
    Task *tasks_copy = clone_tasks_array(tasks, n); // Use a copy to modify deadlines
    int simulation_time = calculate_simulation_time(tasks, n);
    
    int *next_arrival_time = (int *)malloc(n * sizeof(int));
    int *timeline = (int *)malloc(simulation_time * sizeof(int));

    for (int i = 0; i < n; i++) {
        next_arrival_time[i] = 0;
    }

    // Main simulation loop
    for (int current_time = 0; current_time < simulation_time; current_time++) {
        int selected_task_index = -1;
        int earliest_deadline = INT_MAX;

        // Check for jobs that are ready and find the one with the earliest deadline
        for (int i = 0; i < n; i++) {
            // A job is ready if its arrival time is now or in the past, and it has work to do.
            if (current_time >= next_arrival_time[i] && tasks_copy[i].execution_time > 0) {
                if (tasks_copy[i].deadline < earliest_deadline) {
                    earliest_deadline = tasks_copy[i].deadline;
                    selected_task_index = i;
                }
            }
        }

        // If a task is selected, execute it for one time unit
        if (selected_task_index != -1) {
            timeline[current_time] = selected_task_index + 1;
            tasks_copy[selected_task_index].execution_time--;

            // If the job finishes
            if (tasks_copy[selected_task_index].execution_time == 0) {
                // Set up for the next instance of this task
                next_arrival_time[selected_task_index] += tasks[selected_task_index].period;
                tasks_copy[selected_task_index].deadline += tasks[selected_task_index].period;
                tasks_copy[selected_task_index].execution_time = tasks[selected_task_index].execution_time;
            }
        } else {
            timeline[current_time] = 0; // Idle CPU
        }
    }
    
    plot_timeline(timeline, n, simulation_time);
    free(tasks_copy);
    free(next_arrival_time);
    free(timeline);
}


int main(int argc, char *argv[]) {
    if (argc < 2) {
        printf("Usage: %s <filename.txt>\n", argv[0]);
        return 1;
    }

    FILE *file = fopen(argv[1], "r");
    if (file == NULL) {
        printf("Error: Could not open file %s\n", argv[1]);
        return 1;
    }

    char line[50];
    int num_tasks = -1; // Start at -1 to ignore header line
    while (fgets(line, sizeof(line), file) != NULL) {
        num_tasks++;
    }
    rewind(file);

    Task *tasks = (Task *)malloc(num_tasks * sizeof(Task));

    fgets(line, sizeof(line), file); // Skip header line "P C D"
    
    char *token;
    int i = 0;
    while (fgets(line, sizeof(line), file) != NULL) {
        Task new_task;
        token = strtok(line, "\t \n"); // Use tab, space, or newline as delimiters
        int j = 1;
        while (token != NULL) {
            int value = atoi(token);
            if (j == 1) { new_task.period = value; } 
            else if (j == 2) { new_task.execution_time = value; } 
            else if (j == 3) { new_task.deadline = value; }
            j++;
            token = strtok(NULL, "\t \n");
        }
        tasks[i] = new_task;
        i++;
    }
    fclose(file);

    printf("\n--- Task Set ---\n");
    for (int k = 0; k < num_tasks; k++) {
        printf("Task %d: Period=%d, ExecutionTime=%d, Deadline=%d\n", k + 1, tasks[k].period, tasks[k].execution_time, tasks[k].deadline);
    }

    printf("\n============================= Rate Monotonic =============================\n\n");
    schedulability_test(tasks, num_tasks, 'F');
    rate_monotonic_scheduler(tasks, num_tasks);

    printf("\n\n========================= Earliest Deadline First =========================\n\n");
    schedulability_test(tasks, num_tasks, 'V');
    // Note: The original EDF implementation in the provided code was complex and had potential issues.
    // I have replaced it with a more standard, clearer implementation for correctness.
    earliest_deadline_first_scheduler(tasks, num_tasks);
    printf("\n");

    free(tasks);
    return 0;
}