#include "stdint.h"
#include "stdbool.h"
#include "stdlib.h"

#include "msg.h"



void Kernel_msgQ_init(void)
{
    for (uint32_t i = 0 ; i < KernelMsgQ_Num ; i++)
    {
        sMsgQ[i].front = 0; //Queue에 데이터를 읽으면++
        sMsgQ[i].rear = 0;  //Queue에 데이터가 들어오면++
        memclr(sMsgQ[i].Queue, MSG_Q_SIZE_BYTE);
    }
}

bool Kernel_msgQ_is_empty(KernelMsgQ_t Qname)
{
    if (Qname >= KernelMsgQ_Num)
    {
        return false;
    }

    if (sMsgQ[Qname].front == sMsgQ[Qname].rear)
    {
        return true;
    }

    return false;
} /* Q가 비어있는지 확인하는 함수이다. 비어있으면 true */

bool Kernel_msgQ_is_full(KernelMsgQ_t Qname)
{
    if (Qname >= KernelMsgQ_Num)
    {
        return false;
    }

    if (((sMsgQ[Qname].rear + 1) % MSG_Q_SIZE_BYTE) == sMsgQ[Qname].front) //Queue.rear 값이 Queue.front 값 바로 직전 index까지 커지면 full
    {
        return true;
    }

    return false;
}

bool Kernel_msgQ_enqueue(KernelMsgQ_t Qname, uint8_t data)
{
    if (Qname >= KernelMsgQ_Num)
    {
        return false;
    }

    if (Kernel_msgQ_is_full(Qname))
    {
        return false;
    }
    sMsgQ[Qname].rear++;
    sMsgQ[Qname].rear %= MSG_Q_SIZE_BYTE; //Queue size가 index byte를 넘지 않도록한다.

    uint32_t idx = sMsgQ[Qname].rear;
    sMsgQ[Qname].Queue[idx] = data;

    return true;
}

bool Kernel_msgQ_dequeue(KernelMsgQ_t Qname, uint8_t* out_data)
{
    if (Qname >= KernelMsgQ_Num)
    {
        return false;
    }

    if (Kernel_msgQ_is_empty(Qname))
    {
        return false;
    }

    sMsgQ[Qname].front++;
    sMsgQ[Qname].front %= MSG_Q_SIZE_BYTE;

    uint32_t idx = sMsgQ[Qname].front;
    *out_data = sMsgQ[Qname].Queue[idx];

    return true;
}

uint8_t MsgQ_recv_length(KernelMsgQ_t Qname)
{
    uint8_t data;
    
    if(Kernel_msgQ_is_full(Qname))
    {

        return 16;
    }
    if( sMsgQ[Qname].rear > sMsgQ[Qname].front )
    {
        data = (sMsgQ[Qname].rear - sMsgQ[Qname].front);
        if( data < 17 )
        {
            return data;
        }

        return 0;
    }   
    else
    {
        data = ((sMsgQ[Qname].rear)+16 - sMsgQ[Qname].front);
        if( data < 17 )
        {
            return data;
        }

        return 0;

    }
    
}


