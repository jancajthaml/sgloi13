/*
 * Material.h
 *
 *  Created on: 27.10.2013
 *      Author: jancajthaml
 */

#ifndef MATERIAL_H_
#define MATERIAL_H_

struct Material
{

	Color color;				// COLOR material component
	float diffuse;				// DIFFUSE material component
	float specular;				// SPECULAR material component
	float shine;				// cos power for highlights
	float T;					// TRANSMITTANCE
	float index_of_refraction;

	Material()
	{
		color.r				= 1.0f;
		color.g				= 1.0f;
		color.b				= 1.0f;
		diffuse				= 0.5f;
		specular			= 0.5f;
		shine				= 1.0f;
		T					= 0.5f;
		index_of_refraction	= 0.5f;
	}

	Material(float r, float g, float b, float diffuse, float specular, float shine, float T, float index_of_refraction)
	{
		this->color.r				= r;
		this->color.g				= g;
		this->color.b				= b;
		this->diffuse				= diffuse;
		this->specular				= specular;
		this->shine					= shine;
		this->T						= T;
		this->index_of_refraction	= index_of_refraction;
	}

};

#endif /* MATERIAL_H_ */
