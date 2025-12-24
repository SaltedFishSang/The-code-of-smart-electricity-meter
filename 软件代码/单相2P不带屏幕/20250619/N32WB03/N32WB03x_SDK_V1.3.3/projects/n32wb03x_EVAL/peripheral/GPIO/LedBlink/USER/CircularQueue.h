#include "BSP.h"
typedef struct
{
	u8 *p;
	int num;   // 队列长度
	int front; // 对头指针
	int rear;  // 队尾指针
}ScirQueue;
extern ScirQueue *MyQueue;
extern ScirQueue *CreatQueue(int k);
extern void QueueInit(void);
extern int Queue_len(ScirQueue *queue);
extern void Fiforead(u8 *buf, unsigned int len, ScirQueue *queue);
extern void In_Queue(ScirQueue *queue, u8 value);
extern int Out_Queue(ScirQueue *queue);

