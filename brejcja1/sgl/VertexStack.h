#ifndef VERTEX_STACK_H
#define VERTEX_STACK_H

#include "Vertex.h"
#include <stdlib.h>
#define BASIC_STACK_SIZE 100000
class VertexStack
{
private:
	Vertex * stack;
	long currentSize;
	long index;
public:
	VertexStack()
	{
		currentSize = BASIC_STACK_SIZE;
		index = 0;
		stack = (Vertex *)(malloc(sizeof(Vertex) * currentSize));
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
		this->index = other.index;
		this->currentSize = other.currentSize;
		this->stack = (Vertex *)malloc(sizeof(Vertex) * currentSize);
		memcpy(this->stack, other.stack, sizeof(Vertex) * index);
		
	}

	VertexStack& operator=(const VertexStack& other)
	{
		this->index = other.index;
		this->currentSize = other.currentSize;
		this->stack = (Vertex *)malloc(sizeof(Vertex) * currentSize);
		memcpy(this->stack, other.stack, sizeof(Vertex) * index);
		return *this;
	}	

	void push_back(Vertex v)
	{
		if (index == currentSize)
		{
			realloc();
		}
		stack[index++] = v;
	}

	Vertex back()
	{
		return stack[index - 1];
	}

	void pop_back()
	{
		--index;
	}

	void clear()
	{
		index = 0;
	}

	Vertex operator[](const long i)const
	{
		return stack[i];
	}
	long size()
	{
		return index;
	}

	void realloc()
	{/*
		long newSize = currentSize << 1;
		Vertex * stack2 = (Vertex *)malloc(sizeof(Vertex) * newSize);
		memcpy(stack2, stack, sizeof(Vertex) * currentSize);
		currentSize = newSize;
		free(stack);
		stack = stack2;
		stack2 = NULL;
*/	}	




};

#endif
