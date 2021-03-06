#ifndef libsgl_PointLightNode_h
#define libsgl_PointLightNode_h

#include "Light.h"

class PointLight : public Light
{

public:
	PointLight(const Vertex &origin, const Color &color)
	{
		this->position	= origin;
		this->color		= color;
	}

	void Sample( const Vertex& point, Ray& ray, Color& contribution, const float u = 0, const float v = 0)
	{
		ray.origin		= position;
		ray.direction	= point - position;

		ray.direction.normalise();

		contribution	= color;
	}

	bool isPoint()
	{ return true; }

	Color getIntensity() const
	{ return color; }

};

#endif
