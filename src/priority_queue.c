#include "../include/priority_queue.h"
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>

Node* new_Node(Job* data, int priority)
{
    Node* new = (Node*) malloc(sizeof(Node));
    new->data = data;
    new->priority = priority;
    new->nextnode = NULL;
    return new;
}

Node* push(Node* Head, Job* data, int priority)
{
    Node* new = new_Node(data,priority);

    if(Head == NULL)
    {
        Head = new;
    }
    else if(Head->priority > new->priority)
    {
        new->nextnode = Head;
        Head = new; 
    }
    else
    {
        Node* cur = Head;
        while(cur->nextnode != NULL && cur->nextnode->priority  < new->priority)
        {
            cur = cur->nextnode;
        }

        new->nextnode = cur->nextnode;
        cur->nextnode = new;
    }

    return Head;
}

Node* pop(Node* Head)
{
    if (Head == NULL) return NULL;
    Node* new_Head = Head->nextnode;
    free(Head->data);
    free(Head);
    return new_Head;
}


Job* peek(Node* Head)
{
    return Head ? (Head->data) : NULL;
}

bool isEmpty(Node* Head)
{
    return(Head == NULL);
}

Node* rebuild_with_laxity(Node* Head, int current_time)
{

    // laxity = absolute_deadline - current_time - remaining_execution_time

    Node* cur_node = Head;
    Node* new_queue_head = NULL;

    while(cur_node != NULL)
    {
        Job* cur_job = cur_node->data;
        Node* next_node = cur_node->nextnode;
        int laxity = cur_job->absolute_deadline - cur_job->remaining_execution_time - current_time;
        new_queue_head = push(new_queue_head,cur_job,laxity);
        free(cur_node);
        cur_node = next_node;
    }

    return new_queue_head;
}

