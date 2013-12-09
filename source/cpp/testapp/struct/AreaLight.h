/*
 * AreaLight.h
 *
 *  Created on: Dec 9, 2013
 *      Author: jancajthaml
 */

#ifndef AREALIGHT_H_
#define AREALIGHT_H_

#include "Light.h"

/// Point Light
class AreaLight : public Light
{
//friend class SceneBuilder;

private:
	const Vertex atten;		// utlum - x,y,z = c1, c2, c3
	Vertex normal;
	Vertex uVec,vVec;
public:

	Vertex v0,v1,v2;			// vrcholy svetla (staci 3 - obdelnik svetlo);
	const Vertex color;

	AreaLight(const Color &color, const Vertex &atten)
	{
		this->color = color;
		this->atten = atten;
	}

	void addVertices(Vertex iV0, Vertex iV1, Vertex iV2)
	{
		v0 = iV0;
		v1 = iV1;
		v2 = iV2;
	}

	/// Sample on the light
    /**
        Given a point on surface it generates shadow ray
        and contribution of the light along this ray.
        Shadow ray is fully prepared for casting
        (epsilons, normalised direction etc).
        Contribution does account for attenuation.
        The two float parameters u, v can be used for sampling
        area lights. The amount of samples and their distribution
        is up to the callee (Shader).

        @param aPoint   Point on a surface we want to shade
        @param oRay     Generated shadow ray
        @param oContrib Lights contribution
        @param u        Externally supplied random value
        @param v        Externally supplied random value
    */
	virtual void Sample( const Vertex& point, Ray& ray, Color& contribution, const float u = 0, const float v = 0)
	{

		Vertex edge1 = Vertex(this->v2 - this->v1);
		Vertex edge2 = Vertex(this->v0 - this->v1);
		Vertex normal = Vertex::cross(edge1,edge2);
		float area = normal.length();
		normal.normalise();

		ray.origin = v1 + edge1*u + edge2*v;
		ray.direction = point - ray.origin;

		float d = ray.direction.length();
		ray.direction.normalise();

			// set contribution of light (? maybe wrong here)
		contribution = color / (atten.x + atten.y*d + atten.z*d*d);
		contribution *= (ray.direction*normal)*area;

	}

	/// Get light intensity (R,G,B)
	virtual Color getLightColor() const{
		return color;
	}

	// light intersection
	virtual bool shadowIntersect(const Ray& aRay) const
	{
		/*
		float t;

		Vec3f uVector = Vec3f(this->v0 - this->v1);
		Vec3f vVector = Vec3f(this->v2 - this->v1);

		// we use PBRT to find intersection
		Vec3f s1 = cross(aRay.dir, vVector);

		float divisor = dot(s1, uVector);
		// divisor cannot be zero
		if (divisor == 0.f){
			t = FLT_INF;
		}else{
			float invDivisor = 1.f / divisor;
			// Compute first barycentric coordinate
			Vec3f d = aRay.org - this->v1;
			float b1 = dot(d, s1) * invDivisor;
			if (b1 < 0. || b1 > 1.){
				t = FLT_INF;
			}else{
				// Compute second barycentric coordinate
				Vec3f s2 = cross(d, uVector);
				float b2 = dot(aRay.dir, s2) * invDivisor;
				// here is change to square intersection
				if (b2 < 0. || b2 > 1.){
					return false;
				}else{
					// Compute _t_ to intersection point
					t = dot(vVector, s2) * invDivisor;
				}
			}
		}


		if((t > aRay.tMin)&&(t < aRay.tMax))
		{
			return true;
		}
		*/
		return false;
	}


    /// True when the light is a point
    /**
        Returns false when multiple samples on a light
        are desired (area light), true otherwise (point light)

        @return True when light is a point
    */
	virtual bool isPoint()
	{ return false; }

	/// Get light intensity (R,G,B)
	virtual Color getIntensity() const
	{ return color; }

};

#endif /* AREALIGHT_H_ */
