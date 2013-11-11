/*
 * DrawingLibraryBase.h
 *
 *  Created on: 21.10.2013
 *      Author: jancajthaml
 */

#ifndef DRAWINGLIBRARYBASE_H_
#define DRAWINGLIBRARYBASE_H_

#include <stdint.h>
#include "./Helpers.h"
#include "./../struct/Color.h"
#include "./../struct/Vertex.h"
#include "./../struct/Edge.h"
#include "./../struct/VertexStack.h"
#include "./../core/ContextChunk.h"

class DrawingLibraryInterface
{
public:
	
	virtual void drawLine2D			( Vertex a, Vertex b, Chunk &context)	= 0;
	virtual void drawPoints			( Chunk &context )						= 0;
	virtual void drawLines			( Chunk &context )						= 0;
	virtual void drawLineStrip		( Chunk &context )						= 0;
	virtual void drawLineLoop		( Chunk &context )						= 0;
	virtual void drawPolygon		( Chunk &context )						= 0;
	virtual void drawCircle			( Vertex v, float r, Chunk &context)	= 0;
	virtual void fillCircle			( Vertex v, float r, Chunk &context )	= 0;
	virtual void fillTrianglesFan	( Chunk &context )						= 0;
	virtual void fillTrianglesStrip	( Chunk &context )						= 0;
	virtual void fillPolygon		( Chunk &context )						= 0;
	virtual void setPixel			( float x, float y, Chunk &context)		= 0;
	virtual void drawTriangle(const Vertex &v0, const Vertex &v1, const Vertex &v2, Chunk &context) = 0;
	
protected:
	DrawingLibraryInterface()	{};
	~DrawingLibraryInterface()	{};
	
};

class DrawingLibraryBase
{
public:
	DrawingLibraryBase () : state(){};
	
	void drawLine2D ( Vertex a, Vertex b, Chunk &context)
	{ state->drawLine2D(a, b, context); }
	
	void drawPoints ( Chunk &context )
	{ state->drawPoints( context ); }
	
	void drawLines ( Chunk &context )
	{ state->drawLines( context ); }
	
	void drawLineStrip ( Chunk &context )
	{ state->drawLineStrip(context); }
	
	void drawLineLoop ( Chunk &context )
	{ state->drawLineLoop(context); }
	
	void drawPolygon ( Chunk &context )
	{ state->drawPolygon(context); }
	
	void drawCircle ( Vertex v,float r, Chunk &context)
	{ state->drawCircle( v, r, context);	}
	
	void fillTrianglesFan ( Chunk &context )
	{ state->fillTrianglesFan( context);	}
	
	void fillTrianglesStrip	( Chunk &context )
	{ state->fillTrianglesStrip( context);	}
	
	void fillCircle	 ( Vertex v,float r, Chunk &context )
	{ state->fillCircle( v, r, context );	}
	
	void fillPolygon ( Chunk &context )
	{ state->fillPolygon( context ); }
	
	void set (DrawingLibraryInterface* newState)
	{ state = newState; }
	
	void setPixel(float x, float y, Chunk &context)
	{
		state->setPixel(x, y, context);
	}
	
	void drawTriangle(const Vertex &v0, const Vertex &v1, const Vertex &v2, Chunk &context)
	{
		state->drawTriangle(v0, v1, v2, context);
	}

		
	private:
    	friend class DrawingLibraryInterface;
    	DrawingLibraryInterface* state;
	};
	
	
#endif /* DRAWINGLIBRARYBASE_H_ */
