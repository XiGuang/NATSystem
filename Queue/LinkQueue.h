#ifndef __LK_QUEUE_H__
#define __LK_QUEUE_H__

#include "../Assistance.h"			// ���������
#include "../LinkList/Node.h"				// �����

// ��������

template<class ElemType>
class LinkQueue 
{
protected:
//  ������ʵ�ֵ����ݳ�Ա:
	Node<ElemType> *front, *rear;					// ��ͷ��βָָ

public:
	LinkQueue();									// �޲����Ĺ��캯��
	virtual ~LinkQueue();							// ��������
	int size() const;								// ����г���
	bool empty() const;								// �ж϶����Ƿ�Ϊ��
	void Clear();									// ���������
	void Traverse(void (*Visit)(const ElemType &)) const ;	// ��������
	Status pop(ElemType &e);				     // ���Ӳ���
	Status GetHead(ElemType &e) const;			     // ȡ��ͷ����
	Status push(ElemType e);			     // ��Ӳ���
	LinkQueue(const LinkQueue<ElemType> &q);		// ���ƹ��캯��
	LinkQueue<ElemType> &operator =(const LinkQueue<ElemType> &q);// ��ֵ�������
};

// ���������ʵ�ֲ���

template<class ElemType>
LinkQueue<ElemType>::LinkQueue()
// �������������һ���ն���
{
	rear = front = new Node<ElemType>;	// ����������ͷ���
}

template<class ElemType>
LinkQueue<ElemType>::~LinkQueue()
// ������������ٶ���
{
	Clear();
    delete front;		
}

template<class ElemType>
int LinkQueue<ElemType>::size() const
// ������������ض��г���			 
{
	int count = 0;		// ��ʼ�������� 
	for (Node<ElemType> *p = front->next; p != NULL; p = p->next)
		count++;		// ͳ���������еĽ���� 
	return count;
}

template<class ElemType>
bool LinkQueue<ElemType>::empty() const
// ��������������Ϊ�գ��򷵻�true�����򷵻�false
{
   return rear == front;
}

template<class ElemType>
void LinkQueue<ElemType>::Clear() 
// �����������ն���
{
	Node<ElemType> *p = front->next;
    while (p != NULL)	{	// ����ɾ�������е�Ԫ�ؽ��
        front->next = p->next;
		delete p;        
		p = front->next;
	}
	rear = front;
}

template <class ElemType>
void LinkQueue<ElemType>::Traverse(void (*Visit)(const ElemType &)) const 
// ������������ζԶ��е�ÿ��Ԫ�ص��ú���(*visit)
{
	for (Node<ElemType> *p = front->next; p != NULL; p = p->next)
	// �Զ���ÿ��Ԫ�ص��ú���(*visit)���� 
		(*Visit)(p->data);
}


template<class ElemType>
Status LinkQueue<ElemType>::pop(ElemType &e)
// ���������������зǿգ���ôɾ����ͷԪ�أ�����e������ֵ����������SUCCESS,
//	����������UNDER_FLOW��
{
	if (!empty()) 	{	// ���зǿ�
		Node<ElemType> *p = front->next;	// ָ�����ͷ��
		e = p->data;						// ��e���ض�ͷԪ��
		front->next = p->next;				// frontָ����һԪ��
		if (rear == p)	// ����ǰ������ֻ��һ��Ԫ�أ����Ӻ�Ϊ�ն���
			rear = front;
		delete p;							// �ͷų��ӵ�Ԫ�ؽ��
		return SUCCESS;
	}
	else
		return UNDER_FLOW;
}

template<class ElemType>
Status LinkQueue<ElemType>::GetHead(ElemType &e) const
// ���������������зǿգ���ô��e���ض�ͷԪ�أ���������SUCCESS,
//	����������UNDER_FLOW��
{
	if (!empty()) 	{	            // ���зǿ�
		e = front->next->data;		// ��e���ض�ͷԪ��
		return SUCCESS;
	}
	else
		return UNDER_FLOW;
}

template<class ElemType>
Status LinkQueue<ElemType>::push(const ElemType e)
// ������������ϵͳ�ռ䲻��������OVER_FLOW,
//	�������Ԫ��eΪ�µĶ�β������SUCCESS
{
	Node<ElemType> *p; 
    p = new Node<ElemType>(e);	        // ����һ���½��
    if (p) { 
       rear->next = p;	                // ���½����ڶ�β
	   rear = rear->next;				// rearָ���¶�β
	   return SUCCESS;
     }
     else                               //ϵͳ�ռ䲻��������OVER_FLOW 
		return OVER_FLOW;        
}

template<class ElemType>
LinkQueue<ElemType>::LinkQueue(const LinkQueue<ElemType> &q)
// ����������ɶ���q�����¶���--���ƹ��캯��
{
	rear = front = new Node<ElemType>;	// ���ɶ���ͷ���
	for (Node<ElemType> *p = q.front->next; p != NULL; p = p->next)
	// ȡq����ÿ��Ԫ�ص�ֵ,�����ڵ�ǰ������������в���
        push(p->data);
}

template<class ElemType>
LinkQueue<ElemType> &LinkQueue<ElemType>::operator =(const LinkQueue<ElemType> &q)
// ���������������q��ֵ����ǰ����--��ֵ�������
{
	if (&q != this)	{
		Clear();       //���ԭ�ж��� 
		for (Node<ElemType> *p = q.front->next; p != NULL; p = p->next)
		// ȡq����ÿ��Ԫ�ص�ֵ,�����ڵ�ǰ������������в���
            push(p->data);
	}
	return *this;
}

#endif
