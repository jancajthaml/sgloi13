/*
 * Ward.h
 *
 *  Created on: Nov 17, 2013
 *      Author: jancajthaml
 */

#ifndef WARD_H_
#define WARD_H_

const float PI = 3.141592;

struct Ward
{
	Ward()
	{}

	~Ward()
	{}

	Color calculateColor(const Ray &ray, Model *model, const Vertex &i, const Vertex &normal,const std::vector< Light > &lights) const
	{
		Color color;
		Material material = model->getMaterial();

		// parametre anizotropickÈho odlesku
		float alfa_x, alfa_y;

		alfa_y = 0.35f;
		alfa_x = 0.05f;
		float ps = 0.75f;


		// X, Y - "two orthogonal vectors in the normal plane which specify the anisotropic directions"
		Vertex up(1,0,0);
		Vertex X = Vertex::crossNormalised(normal, up);
		Vertex Y = Vertex::crossNormalised(normal, X);

		// V - vektor k pozorovateli
		Vertex V =  ray.direction * (-1);
		V.normalise();

		for( std::vector< Light >::const_iterator light = lights.begin(); light != lights.end(); ++light )
		{

			// L - smer k svetlu
			Vertex L = light->position - i;
			L.normalise();

			// H - "halfangle direction" - na polovine cesty mezi vektorem a pozorovatelem (V) a vektorem ke svetlu (L)
			Vertex H = (V+L);
			H.normalise();

			// spekulární cložka
			float kspec = 0.0f;

			float NL = normal * L;
			float NV = normal * V;

			if(NL > 0.0f && NV>0.0f && X.length() >=0.0f && Y.length() >=0.0f)
			{
				kspec = (ps / (4.0f * PI * alfa_x * alfa_y * sqrtf(NV*NL))) * expf(-2.0f * ((powf((H*X) / alfa_x,2) + (powf((H*Y) / alfa_y,2)) / (1.0f + (H*normal)))));
			}

			// diffuzní složka

			Color Ld = (material.kd * material.color) * NL;
			Color Ls = Color(material.ks, material.ks, material.ks) * kspec;

			Ls.clamp();
			Ld.clamp();

			// výsledná barva
			Ld = Ld + Ls;
			Ld = Ld * light->color;

			color = color+Ld;
		}

		return color;
	}

	float max( float a, float b) const
	{
		if(a>b) return a;
		return b;
	}
};

#endif /* WARD_H_ */
