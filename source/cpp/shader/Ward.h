/*
 * Ward.h
 *
 *  Created on: Nov 17, 2013
 *      Author: jancajthaml
 */

#ifndef WARD_H_
#define WARD_H_

const float PI4 = 12.5663706144;

#include "./../helpers/Helpers.h"

struct Ward
{
	Ward()
	{}

	~Ward()
	{}

	Color calculateColor(const Ray &ray, Model *model, const Vertex &i, const Vertex &N,const std::vector< Light > &lights) const
	{
		Color color;
		Material material = model->getMaterial();

		//Parametry antromorfisovaneho odlesku
		float alfa_y	= 0.35f;
		float alfa_x	= 0.05f;
		float ps		= 0.75f;
		float alfa_xy	= alfa_x * alfa_y * PI4;

		alfa_x = 1.0f/alfa_x;
		alfa_y = 1.0f/alfa_y;

		// X, Y - "two orthogonal vectors in the normal plane which specify the anisotropic directions"
		Vertex up(1,0,0);
		Vertex X = Vertex::crossNormalised(N, up);
		Vertex Y = Vertex::crossNormalised(N, X);

		// V - vektor k pozorovateli
		Vertex V =  -1.0f * ray.direction;
		V.normalise();

		for( std::vector< Light >::const_iterator light = lights.begin(); light != lights.end(); ++light )
		{
			// L - smer k svetlu
			Vertex L = light->position - i;
			L.normalise();

			// H - "halfangle direction" - na polovine cesty mezi vektorem a pozorovatelem (V) a vektorem ke svetlu (L)
			Vertex H = (V+L);
			H.normalise();

			// spekulární složka
			float kspec = 0.0f;

			float NL = N * L;
			float NV = N * V;

			if( NL>0.0f && NV>0.0f && X.length()>=0.0f && Y.length()>=0.0f )
			{
				//FIXME here
				kspec = (ps / (alfa_xy * Helper::q3sqrt(NV*NL))) * expf(-2.0f * ((powf((H*X) * alfa_x,2) + (powf((H*Y) * alfa_y,2)) / (1.0f + (H*N)))));
			}

			//---------------[ AMBIENT
			// none

			//---------------[ DIFFUSE

			Color Ld = (material.kd * material.color) * Helper::max(0.0f, NL);

			//---------------[ SPECULAR

			Color Ls = Color(material.ks, material.ks, material.ks) * Helper::max(0.0f, kspec);

			// výsledná barva
			Ld = Ld + Ls;
			//Ld = Ld * light->color;

			color = color+Ld;
		}

		return color;
	}
};

#endif /* WARD_H_ */
