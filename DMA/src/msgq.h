#ifndef __MSGQ_H_
#define __MSGQ_H_

#define QUEUESIZE	256

typedef struct _QUEUE{
	int front;
	int rear;
	int maxQSize;
	char* buf;
}queue;

void init_queue(queue *Queue, char *pQueueBuf, int maxQSize);
void clear_queue(void);
int queue_put(char data);
int queue_get(char *data);
int queue_isEmpty(void);

//void init_queue(queue *Queue, char *pQueueBuf, int maxQSize);
//void clear_queue(queue *q);
//int queue_put(queue *q, char data);
//int queue_get(queue *q, char *data);
//int queue_isEmpty(queue *q);


#endif
