#include "os/queue.h"
#include <stddef.h>

#define SIZE_OF_QUEUE ((uint8_t) 8)

static QueueElement queue[SIZE_OF_QUEUE];

bool setQueueElement(QueueElement * element)
{
    for (uint8_t i = 0; i < SIZE_OF_QUEUE; ++i) {
        if (queue[i].func == NULL) {
            queue[i] = *element;
            return true;
        }
    }
    return false;
}

bool getQueueElement(QueueElement * element)
{
    if (queue[0].func != NULL) {
        *element = queue[0];
        for (uint8_t i = 1; i < SIZE_OF_QUEUE; ++i) {
            queue[i-1] = queue[i];
        }
        return true;
    }
    return false;
}
