/*
 * Chunk.h
 *
 *  Created on: 17.10.2013
 *      Author: jancajthaml
 */

#ifndef CHUNK_H_
#define CHUNK_H_


#include "./../struct/Color.h"
#include "types.h"

//A brige between DrawingLibrary.h and Context.h
//C++ does not allow cross referencing and forwarding more than 3 parameters in a method call is evil
struct Chunk
{

	Color* buffer					;	// COLOR BUFFER
	float* depth					;	// DEPTH BUFFER
	Color* clear					;	// CLEAR COLOR CACHE

	uint16 w						;	// CONTEXT WIDTH  (maximum 65536)
	uint16 h						;	// CONTEXT HEIGHT (maximum 65536)
	uint32 w_h						;	// PRE-MULTIPLIED WIDTH * HEGIHT, must be 32bit (16bit is too small)

	Color color						;	// CURRENT ACTIVE COLOR

	//VertexStack vertices			;	//STACK OF VERTICIES

	uint8 size						;	//SIZE OF A PIXEL FOR DRAWING POINTS (maximum 256)

	uint32 lastSetPixelIndex		;	//CACHE OF setPixel(int,int) METHOD

	bool depthTest					;	//DEPTH TEST


};

#endif /* CHUNK_H_ */
