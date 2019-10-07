#include "msgq.h"

char serialMsgQueueBuf[QUEUESIZE];

queue serialMsgQueue =
		{
				0,	//front
				0,	//rear
				QUEUESIZE,	//maxQsize
				serialMsgQueueBuf,	//buf
		};

void clear_queue(void){
	serialMsgQueue.front = serialMsgQueue.rear;
}

int queue_put(char data){
    // 큐가 꽉차있는지 확인
    if ((serialMsgQueue.rear + 1) % serialMsgQueue.maxQSize == serialMsgQueue.front){
        printf("\n   Queue overflow.");
        return -1;
    }

    serialMsgQueue.buf[serialMsgQueue.rear] = data;
    serialMsgQueue.rear = ++(serialMsgQueue.rear) % serialMsgQueue.maxQSize;
    return 1;
}

int queue_get(char *data){
    if (serialMsgQueue.front == serialMsgQueue.rear){
        printf("\n  Queue underflow.");
        return -1;
    }

    *data = serialMsgQueue.buf[serialMsgQueue.front];
    serialMsgQueue.front = ++(serialMsgQueue.front) % serialMsgQueue.maxQSize;
    return 1;
}

int queue_isEmpty(void)
{
	if (serialMsgQueue.front == serialMsgQueue.rear) return 1;
	else return 0;
}

//void init_queue(queue *q, char *Qbuf, int queueSize){
//    q->front = q->rear = 0;
//    q->buf = Qbuf;
//    q->maxQSize = queueSize;
//}
//
//void clear_queue(queue *q){
//    q->front = q->rear;
//}
//
//int queue_put(queue *q, char data){
//    // 큐가 꽉차있는지 확인
//    if ((q->rear + 1) % q->maxQSize == q->front){
//        printf("\n   Queue overflow.");
//        return -1;
//    }
//
//    q->buf[q->rear] = data;
//    q->rear = ++(q->rear) % q->maxQSize;
//    return 1;
//}
//
//int queue_get(queue *q, char *data){
//    if (q->front == q->rear){
//        printf("\n  Queue underflow.");
//        return -1;
//    }
//
//    *data = q->buf[q->front];
//    q->front = ++(q->front) % q->maxQSize;
//    return 1;
//}
//
//int queue_isEmpty(queue *q)
//{
//	if (q->front == q->rear) return 1;
//	else return 0;
//}
