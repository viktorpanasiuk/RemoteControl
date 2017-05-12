#ifndef QUEUE_H_
#define QUEUE_H_

#include <stdbool.h>
#include <avr/io.h>

typedef uint16_t Cnt;
typedef void (* Func)(uint8_t param);
typedef uint8_t Param;

typedef struct {
    Cnt cnt;
    Func func;
    Param param;
} QueueElement;

bool setQueueElement(QueueElement * element);
bool getQueueElement(QueueElement * element);

#endif /* QUEUE_H_ */