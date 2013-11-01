//
//  Material.h
//  libsgl
//
//  Created by Jan Brejcha on 01.11.13.
//  Copyright (c) 2013 brejchajan. All rights reserved.
//

#ifndef libsgl_Material_h
#define libsgl_Material_h

#include "../struct/Color.h"

/**
 Definition of material used on particular models.
 */
struct Material
{
	
	///Color of the material
	Color color;
	
	///diffuse coefficient
	float kd;
	
	///specular coefficient
	float ks;
	
	///Phong cosine power for highlights.
	float shine;
	
	///transmittance (fraction of contribution of the transmitting ray).
	float trn;
	
	///index of refraction
	float ior;
	
	/**
	 Material constructor
	 @param _color	color of the material
	 @param _kd		diffuse coefficient
	 @param _ks		specular coefficient
	 @param _shine	phong cosine power for highlights
	 @param _trn	transmittance (fraction of contribution of the transmitting ray).
	 @param _ior	index of refraction
	 */
	Material(Color _color, const float _kd, const float _ks, const float _shine, const float _trn, const float _ior)
	{
		this->color = _color;
		this->kd = _kd;
		this->ks = _ks;
		this->shine = _shine;
		this->trn = _trn;
		this->ior = _ior;
	}
	
	Material(){}
	
};

#endif
