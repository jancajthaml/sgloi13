#ifndef CONTEXTMANAGER_H
#define CONTEXTMANAGER_H

#include "CrossReferenceDispatcher.h"
#include "Context.h"

//TODO COMMENT !!!!!!!

/**
 * Context manager
 * \author Jan Brejcha
 *
 * edit and cleanup Jan Cajthaml
 */
struct ContextManager
{
	int_fast8_t current;			// Maximum of 256 contexts
	std::vector<Context> contexts;

	ContextManager()
	{ current = 0; }

	inline void setContext(int_fast8_t id)
	{
		if ((id >= 0) && (id < int_fast32_t(contexts.size())))
			current = id;
		else throw std::exception();
	}

	inline void deleteContext(int_fast8_t id)
	{
		if (id >= 0 && id < (int_fast32_t)contexts.size())
			contexts.erase(contexts.begin() + id);
	}

};
#endif
