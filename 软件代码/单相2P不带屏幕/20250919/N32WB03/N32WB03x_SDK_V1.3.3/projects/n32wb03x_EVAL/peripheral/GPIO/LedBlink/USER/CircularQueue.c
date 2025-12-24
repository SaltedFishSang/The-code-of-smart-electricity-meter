#include "BSP.h"
#include <stdio.h>
#include <stdlib.h>
ScirQueue *MyQueue;
/*创建循环队列*/
ScirQueue *CreatQueue(int k)
{
	ScirQueue *scirQueue = (ScirQueue *)malloc(sizeof(ScirQueue));
	if (scirQueue != NULL)
	{
		scirQueue->num = k + 1; // 队列长度(这个要比定义的值+1)
		scirQueue->p = (u8 *)malloc(scirQueue->num * sizeof(u8));
		scirQueue->front = scirQueue->rear = 0;
		return scirQueue;
	}
	else
	{
		return NULL;
	}
}

/*删除队列*/
void Free_Queue(ScirQueue *queue)
{
	free(queue->p);
	queue->p = NULL;
	free(queue);
	queue = NULL;
}

/*元素入队*/
void In_Queue(ScirQueue *queue, u8 value)
{
	if (((queue->rear + 1) % queue->num) == queue->front)
	{
		// printf("Queue is full\r\n");
	}
	else
	{
		queue->p[queue->rear] = value;
		queue->rear = (queue->rear + 1) % (queue->num);
	}
}
/*元素出队*/
int Out_Queue(ScirQueue *queue)
{
	u8 data = 0;
	if (queue->rear == queue->front)
	{
		return 0;
		// printf("Queue is empty\r\n");
	}
	else
	{
		data = queue->p[queue->front];
		// printf("%c\r\n", queue->p[queue->front]);
		queue->front = (queue->front + 1) % (queue->num);
		return data;
	}
}
/*获取队中元素个数  入队个数*/
int Queue_len(ScirQueue *queue)
{
	int len = queue->rear - queue->front;
	len = (len >= 0) ? len : (queue->num) + len;
	return len;
}
/*获取队中剩余元素个数  */
int Queue_len_Left(ScirQueue *queue)
{
	int len = queue->rear - queue->front;
	len = (len >= 0) ? len : (queue->num) + len;
	len = queue->num - len - 1;
	return len;
}
/*获取队列总长度*/
int Queue_len_Total(ScirQueue *queue)
{
	return (queue->num - 1);
}

/*读队列数据*/
void Fiforead(u8 *buf, unsigned int len, ScirQueue *queue)
{
	int i = 0;
	for (i = 0; i < len; i++)
	{
		buf[i] = Out_Queue(queue);
	}
}

/*写队列数据*/
void Fifowrite(u8 *buf, unsigned int len, ScirQueue *queue)
{
	int i = 0;
	for (i = 0; i < len; i++)
	{
		In_Queue(queue, buf[i]);
	}
}

void QueueInit(void)
{
	MyQueue = CreatQueue(1500);
}
