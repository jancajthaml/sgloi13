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

class Ward
{
private:
	Ward(){}

	static inline Color calculateColor(const Ray &ray, Model *model, const Vertex &i, const Vertex &N,const std::vector< Light > &lights, std::vector< SceneNode* > &children, const Color& clear)
	{
		Material material = model->getMaterial();
		const int size = lights.size();
		int pointer		= -1;
		Color color;

		//Parametry antromorfisovaneho odlesku
		float alfa_y	= 0.5f;
		float alfa_x	= 0.1f;
		float ps		= 0.75f;
		float alfa_xy	= alfa_x * alfa_y * PI4;

		alfa_x = 1.0f/alfa_x;
		alfa_y = 1.0f/alfa_y;

		// X, Y - "two orthogonal vectors in the normal plane which specify the anisotropic directions"

		Vertex X = Vertex::cross(N,Vertex(1,0,0));
		X.normalise();

		Vertex Y = Vertex::cross(N,X);
		Y.normalise();

		// V - vektor k pozorovateli
		Vertex V =  -1.0f * ray.direction;
		V.normalise();

		while( ++pointer<size )
		{
			// L - smer k svetlu
			Vertex L = lights[pointer].position - i;
			L.normalise();

			// H - "halfangle direction" - na polovine cesty mezi vektorem a pozorovatelem (V) a vektorem ke svetlu (L)
			Vertex H = (V+L);
			H.normalise();

			// spekulární složka
			float kspec = 0.0f;

			float NL = N * L;
			float NV = N * V;

			if( NL>0.0f && NV>0.0f && X.length()>=0.0f && Y.length()>=0.0f )
				kspec = (ps / (alfa_xy * Helper::q3sqrt(NV*NL))) * expf(-2.0f * ((powf((H*X) * alfa_x,2) + (powf((H*Y) * alfa_y,2)) / (1.0f + (H*N)))));

			//---------------[ AMBIENT
			// none

			//---------------[ DIFFUSE

			Color Ld = (material.kd * material.color) * Helper::max(0.0f, NL);

			//---------------[ SPECULAR

			Color Ls = Color(material.ks, material.ks, material.ks) * Helper::max(0.0f, kspec);

			// výsledná barva

			Ld = Ld + Ls;
			Ld = Ld * lights[pointer].color;

			color = color+Ld;
		}

		return color;
	}

public:
	static inline Color castAndShade(const Ray &ray,std::vector< SceneNode* > &children,const std::vector< Light > &lights, const Color &clear)
	{
			float tmin = FLOAT_MAX;
			Model *model;
			float t	 = FLOAT_MAX;

			for( std::vector< SceneNode* >::iterator child = children.begin(); child != children.end(); ++child )
			{
				Model* m = (*child)->getModel();
				//find nearest object
				if( m->findIntersection(ray, t) )
				{
					if( t<tmin )
					{
						tmin  = t;
						model = m;
					}
				}
			}
			if( tmin<FLOAT_MAX )
			{
				Vertex i		= ray.extrapolate(tmin);
				Vertex normal	= model->getNormal(i);

				return calculateColor(ray, model, i, normal, lights, children, clear);
			}
			return clear;
		}

};

#endif /* WARD_H_ */
