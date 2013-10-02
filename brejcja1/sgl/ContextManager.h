#ifndef CONTEXTMANAGER_H
#define CONTEXTMANAGER_H
#include <exception>
#include <vector>
#include <cstdio>
#include "sgl.h"
/**
 * Context manager
 * \author Jan Brejcha
 */
class ContextManager
{
private:
	int currentID;
	std::vector<SGLContext> contexts;
public:
	/**
	 * Default constructor. Sets currentID to -1 as there are no valid contexts yet
	 */
	ContextManager()
	{
		currentID = -1;
	}
	/**
	 * Checks if the id is in the range of available context ids
	 * and sets it as current context
	 * \param id 	the context id to be set as curent context id
	 */ 	
	void setContext(int id)
	{
		if ((id >= 0) && (id < (int)contexts.size()))
			currentID = id;
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
	SGLContext & getContext(sglEErrorCode * err)
	{
		*err = SGL_NO_ERROR; 
		if (currentID >= 0)
			return contexts[currentID];
		*err = SGL_INVALID_OPERATION;	

	}

	void deleteContext(int id)
	{
		if (id >= 0 && id < (int)contexts.size())
			contexts.erase(contexts.begin() + id);
	}

	void addContext(SGLContext context)
	{
		contexts.push_back(context);
	}

	int lastContextID()
	{
		return contexts.size() - 1;
	}

	int getCurrentContextID()
	{
		return currentID;
	}

};
#endif
