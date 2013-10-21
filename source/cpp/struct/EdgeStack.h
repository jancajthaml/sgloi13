/*
 * EdgeStack.h
 *
 *  Created on: 9.10.2013
 *      Author: Jan Brejcha & Jan Cajthaml
 */

#ifndef EDGESTACK_H
#define EDGESTACK_H

#include "./Edge.h"

//TODO COMMENT !!!!!!!
class EdgeStack
{
	private:
		Edge*			stack;
		int_fast16_t	currentSize;	//16bit = 65536

	public:
		int_fast16_t	index;			//16bit = 65536

		EdgeStack()
	{
		currentSize	= BASIC_STACK_SIZE;
		index		= 0;
		stack		= (Edge*)(malloc(sizeof(Edge) * currentSize));
	}

	~EdgeStack()
	{
		if (this->stack != NULL)
		{
			free(this->stack);
			stack = NULL;
		}
	}

	EdgeStack(const EdgeStack& other)
	{
		this->index			= other.index;
		this->currentSize	= other.currentSize;
		this->stack			= (Edge*)malloc(sizeof(Edge) * currentSize);

		memcpy(this->stack, other.stack, sizeof(Edge) * index);
	}

	EdgeStack& operator=(const EdgeStack& other)
	{
		this->index			= other.index;
		this->currentSize	= other.currentSize;
		this->stack			= (Edge *)malloc(sizeof(Edge) * currentSize);

		memcpy(this->stack, other.stack, sizeof(Edge) * index);

		return *this;
	}

	void push_back(Edge v)
	{
		if (index == currentSize)	realloc();
		stack[index++] = v;
	}

	Edge back()
	{ return stack[index - 1]; }

	void pop_back()
	{ --index; }

	Edge operator[](const long i)const
	{ return stack[i]; }

	void realloc()
	{
		currentSize <<= 1;
		Edge* tmp = this->stack;
		this->stack = (Edge *)malloc(sizeof(Edge) * currentSize);
		memcpy(this->stack, tmp, sizeof(Edge) * index);
		free(tmp);
	}
};

#endif /* VERTEXSTACK_H_ */
