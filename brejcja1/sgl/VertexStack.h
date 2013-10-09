#ifndef VERTEX_STACK_H
#define VERTEX_STACK_H

#include "Vertex.h"
#include <cstdlib>
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
		//free(stack);
		//stack = NULL;
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
	{
		long newSize = currentSize << 1;
		Vertex * stack2 = (Vertex *)malloc(sizeof(Vertex) * newSize);
		memcpy(stack2, stack, sizeof(Vertex) * currentSize);
		currentSize = newSize;
		free(stack);
		stack = stack2;
		stack2 = NULL;
	}	




};

#endif
