#ifndef CONTEXTMANAGER_H
#define CONTEXTMANAGER_H
#include <exception>
#include <vector>
#include <cstdio>
#include "sgl.h"
#include "Context.h"
/**
 * Context manager
 * \author Jan Brejcha
 *
 * edit and cleanup Jan Cajthaml
 */
struct ContextManager
{
	int current;
	std::vector<Context> contexts;

	ContextManager()
	{ current = 0; }

	void setContext(int id)
	{
		if ((id >= 0) && (id < (int)contexts.size()))
			current = id;
		else throw std::exception();
	}

	void deleteContext(int id)
	{
		if (id >= 0 && id < (int)contexts.size())
			contexts.erase(contexts.begin() + id);
	}


};
#endif
