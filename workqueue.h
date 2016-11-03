#include <pthread.h>
#include <stdlib.h>
#include <assert.h>
#include "threadpool.h"

struct job {
struct future * job;
struct job * next;
};

struct work_queue {
struct job * head;
struct job * tail;
};

static void add_job(struct work_queue * queue, struct future * newFuture) {
struct job * newJob = malloc(sizeof(struct job));
newJob->job = newFuture;
newJob->next = NULL;

if (queue->head == NULL)
{
queue->head = newJob;
}
else
{
queue->tail->next = newJob;
}
queue->tail = newJob;
}

static struct future * poll_job(struct work_queue * queue)
{
struct future * toReturn = queue->head->job;
struct job * oldHead = queue->head;
queue->head = queue->head->next;
free(oldHead);
return toReturn;
}

static int is_empty(struct work_queue * queue)
{
return queue->head == NULL;
}

static void queue_init(struct work_queue * queue) {
assert(queue != NULL);
queue->head = NULL;
queue->tail = queue->head;
}

static void free_queue(struct work_queue * queue)
{
struct job * current = queue->head;
struct job * prev = NULL;
while (current != NULL)
{
prev = current;
current = current->next;
free(prev);
}
}