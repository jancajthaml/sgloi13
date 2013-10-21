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
#include "./../struct/EdgeStack.h"
#include "./../struct/VertexStack.h"
#include "./../core/ContextChunk.h"
#include "./Helpers.h"
#include "./../struct/Color.h"
#include "./../struct/Vertex.h"
#include "./../struct/Edge.h"
#include "./../struct/EdgeStack.h"
#include "./../struct/VertexStack.h"
#include "./../core/ContextChunk.h"

class DrawingLibraryInterface
{
	public:

		virtual void setPixel		( signed x, signed y, Chunk &context) = 0;
		virtual void setPixel_x		( Chunk &context ) = 0;
		virtual void setPixel_y		( Chunk &context) = 0;
		virtual void setPixel_xmy	( Chunk &context) = 0;
		virtual void setPixel_xy	( Chunk &context) = 0;
		virtual void setPixel_mxy	( Chunk &context) = 0;
		virtual void fillSymPixel	( signed x, signed y, signed center_x, signed center_y, Chunk &context) = 0;
		virtual void setSymPixel	( signed x, signed y, signed xs, signed ys, Chunk &context) = 0;
		//virtual void bresenham_x	( signed x1, signed y1, signed x2, signed y2, Chunk &context) = 0;
		//virtual void bresenham_y	( signed x1, signed y1, signed x2, signed y2, Chunk &context) = 0;
		virtual void drawLine2D		( Vertex a, Vertex b, Chunk &context) = 0;
		virtual void drawPoints		( Chunk &context ) = 0;
		virtual void drawLines		( Chunk &context ) = 0;
		virtual void drawLineStrip	( Chunk &context ) = 0;
		virtual void drawLineLoop	( Chunk &context ) = 0;
		virtual void drawPolygon	( Chunk &context ) = 0;
		virtual void drawCircle		( Vertex v,float r, Chunk &context) = 0;
		virtual void fillCircle		( Vertex v,float r,Chunk &context ) = 0;
		virtual void fillPolygon	( Chunk &context ) = 0;

	protected:

		DrawingLibraryInterface(){};
    	~DrawingLibraryInterface(){};

};

class DrawingLibraryBase
{
	public:
		DrawingLibraryBase () : state(){};

		void setPixel		( signed x, signed y, Chunk &context);
		void setPixel_x		( Chunk &context );
		void setPixel_y		( Chunk &context);
		void setPixel_xmy	( Chunk &context);
		void setPixel_xy	( Chunk &context);
		void setPixel_mxy	( Chunk &context);
		void fillSymPixel	( signed x, signed y, signed center_x, signed center_y, Chunk &context);
		void setSymPixel	( signed x, signed y, signed xs, signed ys, Chunk &context);
		void bresenham_x	( signed x1, signed y1, signed x2, signed y2, Chunk &context);
		void bresenham_y	( signed x1, signed y1, signed x2, signed y2, Chunk &context);
		void drawLine2D		( Vertex a, Vertex b, Chunk &context);
		void drawPoints		( Chunk &context );
		void drawLines		( Chunk &context );
		void drawLineStrip	( Chunk &context );
		void drawLineLoop	( Chunk &context );
		void drawPolygon	( Chunk &context );
		void drawCircle		( Vertex v,float r, Chunk &context);
		void fillCircle		( Vertex v,float r,Chunk &context );
		void fillPolygon	( Chunk &context );

		void set (DrawingLibraryInterface* newState){ state = newState; }

  	  private:
    	friend class DrawingLibraryInterface;
    	DrawingLibraryInterface* state;
};

void DrawingLibraryBase::setPixel		( signed x, signed y, Chunk &context)										{ state->setPixel(x,y,context);	}
void DrawingLibraryBase::setPixel_x		( Chunk &context)															{ state->setPixel_x(context);	}
void DrawingLibraryBase::setPixel_y		( Chunk &context)															{ state->setPixel_y(context);	}
void DrawingLibraryBase::setPixel_xy	( Chunk &context)															{ state->setPixel_xy(context);	}

void DrawingLibraryBase::setPixel_xmy	( Chunk &context)															{ state->setPixel_xy(context);	}
void DrawingLibraryBase::setPixel_mxy	( Chunk &context)															{ state->setPixel_xy(context);	}

void DrawingLibraryBase::fillSymPixel	( signed x, signed y, signed center_x, signed center_y, Chunk &context)		{ state->fillSymPixel(x, y, center_x, center_y, context);	}
void DrawingLibraryBase::setSymPixel	( signed x, signed y, signed xs, signed ys, Chunk &context)					{ state->setSymPixel(x, y, xs, ys, context);				}


//void DrawingLibraryBase::bresenham_x	( signed x1, signed y1, signed x2, signed y2, Chunk &context)				{ state->bresenham_x(x1, y1, x2, y2, context);	}
//void DrawingLibraryBase::bresenham_y	( signed x1, signed y1, signed x2, signed y2, Chunk &context)				{ state->bresenham_y(x1, y1, x2, y2, context);	}
void DrawingLibraryBase::drawLine2D		( Vertex a, Vertex b, Chunk &context)										{ state->drawLine2D(a, b, context);				}

void DrawingLibraryBase::drawPoints		( Chunk &context )															{ state->drawPoints( context );			}
void DrawingLibraryBase::drawLines		( Chunk &context )															{ state->drawLines( context );			}
void DrawingLibraryBase::drawLineStrip	( Chunk &context )															{ state->drawLineStrip(context);		}
void DrawingLibraryBase::drawLineLoop	( Chunk &context )															{ state->drawLineLoop(context);			}
void DrawingLibraryBase::drawPolygon	( Chunk &context )															{ state->drawPolygon(context);			}
void DrawingLibraryBase::drawCircle		( Vertex v,float r, Chunk &context)											{ state->drawCircle( v, r, context);	}
void DrawingLibraryBase::fillCircle		( Vertex v,float r,Chunk &context )											{ state->fillCircle( v, r, context );	}
void DrawingLibraryBase::fillPolygon	( Chunk &context )															{ state->fillPolygon( context );		}


#endif /* DRAWINGLIBRARYBASE_H_ */
