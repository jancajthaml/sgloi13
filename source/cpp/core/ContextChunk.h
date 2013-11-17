/*
 * Chunk.h
 *
 *  Created on: 17.10.2013
 *      Author: jancajthaml
 */

#ifndef CHUNK_H_
#define CHUNK_H_

#include "./CrossReferenceDispatcher.h"
#include "./../struct/Color.h"
#include "./../struct/VertexStack.h"

//A brige between DrawingLibrary.h and Context.h
//C++ does not allow cross referencing and forwarding more than 3 parameters in a method call is evil
struct Chunk
{

	Color* buffer					;	// COLOR BUFFER
	float* depth					;	// DEPTH BUFFER
	Color* clear					;	// CLEAR COLOR CACHE

	int_fast16_t w					;	// CONTEXT WIDTH  (maximum 65536)
	int_fast16_t h					;	// CONTEXT HEIGHT (maximum 65536)
	uint_fast32_t w_h				;	// PRE-MULTIPLIED WIDTH * HEGIHT, must be 32bit (16bit is too small)

	Color color						;	// CURRENT ACTIVE COLOR

	//VertexStack vertices			;	//STACK OF VERTICIES

	int_fast8_t size				;	//SIZE OF A PIXEL FOR DRAWING POINTS (maximum 256)

	uint_fast32_t lastSetPixelIndex	;	//CACHE OF setPixel(int,int) METHOD

	bool depthTest						;	//DEPTH TEST

};

#endif /* CHUNK_H_ */
