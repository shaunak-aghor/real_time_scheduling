#include "../include/priority_queue.h"
#include <stdlib.h>
#include <stdbool.h>

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



