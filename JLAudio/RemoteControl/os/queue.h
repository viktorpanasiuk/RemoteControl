#ifndef QUEUE_H_
#define QUEUE_H_

#include <stdbool.h>
#include <avr/io.h>

typedef uint16_t Cnt;
typedef void (* Func)(void);

typedef struct {
    Cnt cnt;
    Func func;
} QueueElement;

bool setQueueElement(QueueElement * element);
bool getQueueElement(QueueElement * element);

#endif /* QUEUE_H_ */