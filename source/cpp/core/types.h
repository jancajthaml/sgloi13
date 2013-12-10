/*
 * types.h
 *
 *  Created on: Dec 9, 2013
 *      Author: jancajthaml
 */

#ifndef TYPES_H_
#define TYPES_H_

#include <limits>

#define FLOAT_MAX std::numeric_limits<float>::max()
#define PI 3.14159265359

//Custom type definitions

typedef struct { float f1; float f2; float f3; } __color;

typedef unsigned char      uint8  ;
typedef unsigned short     uint16 ;
typedef unsigned int       uint32 ;
typedef unsigned long long uint64 ;

typedef          char      int8   ;
typedef          short     int16  ;
typedef          int       int32  ;
typedef          long long int64  ;

#endif /* TYPES_H_ */
