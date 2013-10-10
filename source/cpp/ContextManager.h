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
class ContextManager
{
	public:
		int current;
		std::vector<Context> contexts;

	/**
	 * Default constructor. Sets currentID to -1 as there are no valid contexts yet
	 */
	ContextManager()
	{
		current = 0;
	}
	/**
	 * Checks if the id is in the range of available context ids
	 * and sets it as current context
	 * \param id 	the context id to be set as curent context id
	 */
	void setContext(int id)
	{
		if ((id >= 0) && (id < (int)contexts.size()))
			current = id;
		else throw std::exception();
	}

	/**
	 * Deletes all contexts
	 */
	void clearContexts()
	{
		contexts.clear();
	}

	/**
	 * Get current context
	 * \return returns reference to the current context
	 */
	//Context & getContext()
	//{
		//if (currentID >= 0)	return contexts[current];
			//				return contexts[0];
	//}

	void deleteContext(int id)
	{
		if (id >= 0 && id < (int)contexts.size())
			contexts.erase(contexts.begin() + id);
	}

	void addContext(Context context)
	{
		contexts.push_back(context);
	}

	int lastContextID()
	{
		return contexts.size() - 1;
	}

	//int getCurrentContextID()
	//{
		//return current;
	//}

};
#endif
