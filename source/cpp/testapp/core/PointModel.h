//
//  PointModel.h
//  libsgl
//
//  Created by Jan Brejcha on 04.11.13.
//  Copyright (c) 2013 brejchajan. All rights reserved.
//

#ifndef libsgl_PointModel_h
#define libsgl_PointModel_h

#include "Model.h"
#include "./../helpers/Helpers.h"

class PointModel : public Model
{
	
private:
	
public:
	/**
	 Constructor of model
	 @param _g			drawing library
	 @param _context	graphics context
	 @param _size		size of the point
	*/
	PointModel( Chunk _context, Material _material) : Model( _context, _material)
	{}

	/**
	 Rasterizes this model with lights affecting it
	 @param lights	lights affecting appearance of this model
	 @param mpv		model projection view matrix to be used when rasterizing
	 */
	virtual void rasterize(std::vector<Light> lights, Matrix mpv)
	{
		Model::multiplyVerticesWithMVP(mpv);
		const uint16 s = vertices.size();
		
		if( context.size==1 )
		{
			uint16 i = -1;
			while( ++i<s )
				setPixel3D( (vertices)[i].x(), (vertices)[i].y(),(vertices)[i].z(), context );
		}
		else
		{
			uint8 thickness = context.size;
			
			if( (thickness%2)==0 ) thickness++;
			
			thickness	>>= 1;
			int i		= -1;

			while( ++i<s )
			{
				Vertex v = (vertices)[i];
				for( uint8 i = -thickness; i < context.size-1; i++ )
				for( uint8 j = -thickness; j < context.size-1; j++ )
					setPixel3D(v.x()+j, v.y()+i,v.z(), context);
			}
		}

	}
};


#endif
