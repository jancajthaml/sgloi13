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
class PointModel : public Model
{
	
private:
	int size;
	
public:
	/**
	 Constructor of model
	 @param _g			drawing library
	 @param _context	graphics context
	 @param _size		size of the point
	*/
	PointModel(DrawingLibraryBase _g, Chunk _context, int _size, Material _material) : Model(_g, _context, _material)
	{
		this->size = _size;
	}
	
	
	/**
	 Rasterizes this model with lights affecting it
	 @param lights	lights affecting appearance of this model
	 @param mpv		model projection view matrix to be used when rasterizing
	 */
	virtual void rasterize(std::vector<Light> lights, Matrix mpv)
	{
		Model::multiplyVerticesWithMVP(mpv);
		
		int_fast32_t s = int_fast32_t(vertices.size());
		
		if(size==1)
		{
			for (int_fast32_t i = 0; i < s; i++)
				g.setPixel( (vertices)[i].x, (vertices)[i].y, context );
		}
		else
		{
			int_fast8_t thickness = size;
			
			if((thickness%2)==0) thickness++;
			
			thickness >>= 1;
			
			for (int_fast32_t i = 0; i < s; i++)
			{
				Vertex v = (vertices)[i];
				for(int_fast8_t i = -thickness; i < size-1; i++)
					for(int_fast8_t j = -thickness; j < size-1; j++)
						g.setPixel(v.x+j, v.y+i, context);
			}
		}

	}
};


#endif
