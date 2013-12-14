#ifndef CONTEXTMANAGER_H
#define CONTEXTMANAGER_H

#include "Context.h"
#include "types.h"

//TODO COMMENT !!!!!!!

/**
 * Context manager
 * \author Jan Brejcha
 *
 * edit and cleanup Jan Cajthaml
 */
struct ContextManager
{
	uint8 current;			// Maximum of 256 contexts
	std::vector<Context> contexts;

	ContextManager()
	{ current = 0; }

	inline void setContext(uint8 id)
	{
		if ( id>=0 && id<contexts.size() )
			current = id;
		else throw std::exception();
	}

	inline void deleteContext(uint8 id)
	{
		if( id>=0 && id<contexts.size() )
			contexts.erase(contexts.begin() + id);
	}

};
#endif
