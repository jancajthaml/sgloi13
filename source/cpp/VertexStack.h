/*
 * VertexStack.h
 *
 *  Created on: 9.10.2013
 *      Author: Jan Brejcha
 */

#ifndef VERTEXSTACK_H_
#define VERTEXSTACK_H_

#include "Edge.h"

//TODO COMMENT !!!!!!!
class VertexStack
{
	private:
		Vertex*			stack;
		int_fast16_t	currentSize;	//16bit = 65536

	public:
		int_fast16_t	index;			//16bit = 65536

	VertexStack()
	{
		currentSize  =  BASIC_STACK_SIZE;
		index        =  0;
		stack        =  (Vertex*)(malloc(sizeof(Vertex) * currentSize));
	}

	~VertexStack()
	{
		if (this->stack != NULL)
		{
			free(this->stack);
			stack = NULL;
		}
	}

	VertexStack(const VertexStack& other)
	{
		this->index			= other.index;
		this->currentSize	= other.currentSize;
		this->stack			= (Vertex*)malloc(sizeof(Vertex) * currentSize);

		memcpy(this->stack, other.stack, sizeof(Vertex) * index);
	}

	VertexStack& operator=(const VertexStack& other)
	{
		this->index			= other.index;
		this->currentSize	= other.currentSize;
		this->stack			= (Vertex *)malloc(sizeof(Vertex) * currentSize);

		memcpy(this->stack, other.stack, sizeof(Vertex) * index);

		return *this;
	}

	void push_back(Vertex v)
	{
		if (index == currentSize)	realloc();
		stack[index++] = v;
	}

	Vertex back()
	{ return stack[index - 1]; }

	void pop_back()
	{ --index; }

	Vertex operator[](const long i)const
	{ return stack[i]; }

	void realloc()
	{
		currentSize <<= 1;
		Vertex* tmp = this->stack;
		this->stack = (Vertex *)malloc(sizeof(Vertex) * currentSize);
		memcpy(this->stack, tmp, sizeof(Vertex) * index);
		free(tmp);
	}
};

#endif /* VERTEXSTACK_H_ */
