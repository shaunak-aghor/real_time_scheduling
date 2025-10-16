#pragma once

#include "task.h"
#include <stdbool.h>
typedef struct node
{
    Job* data;
    int priority;
    struct node* nextnode;
} Node;


Node* new_Node(Job* data, int priority);
Node* push(Node* Head, Job* data, int priority);
Job* peek(Node* Head);
Node* pop(Node* Head);
bool isEmpty(Node* Head);
Node* rebuild_with_laxity(Node* Head, int current_time);

