/*
 * VertexStack.h
 *
 *  Created on: 9.10.2013
 *      Author: jancajthaml
 */

#ifndef VERTEXSTACK_H_
#define VERTEXSTACK_H_
#include "Vertex.h"
#include <stdlib.h>
#include <cstdio>

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
				realloc();
			stack[index++] = v;
		}

		Vertex back()
		{ return stack[index - 1]; }

		void pop_back()
		{ --index; }

		void clear()
		{ index = 0; }

		Vertex operator[](const long i)const
		{ return stack[i]; }

		long size()
		{ return index; }

		void realloc()
		{}
};

#endif /* VERTEXSTACK_H_ */
