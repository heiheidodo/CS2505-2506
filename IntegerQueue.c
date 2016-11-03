#include "IntergerQueue.h"
#include <stdbool.h>
#include <stdint.h>
#include "Queue.h"
#include "IntegerDT.h"

// Initialize empty IntegerQueue.
//
// Pre:  pIQ points to an IntegerQueue object
// Post: pIQ->Q has been initialized to an empty state
//       (see Queue.h for details)
//
void IntegerQueue_Init(IntegerQueue* const pIQ)
{
    Queue_Init(pIQ->Q);
}

// Return whether IntegerQueue is empty.
//
// Pre:  pIQ points to aqn IntegerQueue object
// Returns true if pIQ->Q is empty, false otherwise 
//
bool IntegerQueue_Empty(const IntegerQueue* const pIQ)
{
    return (Queue_Empty(pIQ->Q);
}

// Insert K at rear of IntegerQueue.
//
// Pre:  pIQ points to an IntegerQueue object
// Post: An IntegerDT object containing the value K has been inserted 
//       at the rear of pIQ->Q
//
void IntegerQueue_Push(IntegerQueue* const pIQ, int32_t K)
{
    IntegerDT* node = K;
	IntegerDT_Init(pIQ->node, K);
	Queue_Push(pIQ->Q, pIQ->node);
}

// Remove and return data value from front of IntegerQueue.
//
// Pre:  pIQ points to a nonempty IntegerQueue object
// Post: The IntegerDT object at the front of pIQ->Q has been removed
// Returns the user data value that was in the IntegerDT object at the 
// front of pIQ->Q
//
int32_t IntegerQueue_Pop(IntegerQueue* const pIQ)
{
    QNode* pop = Queue_Pop(pIQ->Q);
	IntergerDT = Queue_Entry(pop, IntergerDT, pIQ);
	IntegerDT_Init(pDT, K);
	return ;
}

// On my honor:
//
// - I have not discussed the C language code in my program with
//   anyone other than my instructor or the teaching assistants
//   assigned to this course.
//
// - I have not used C language code obtained from another student,
//   or any other unauthorized source, either modified or unmodified.
//
// - If any C language code or documentation used in my program
//   was obtained from another source, such as a text book or course
//   notes, that has been clearly noted with a proper citation in
//   the comments of my program.
//
//   <Sheng Zhou>