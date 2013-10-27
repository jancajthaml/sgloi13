/*
 * Triangle.h
 *
 *  Created on: 27.10.2013
 *      Author: jancajthaml
 */

#ifndef TRIANGLE_H_
#define TRIANGLE_H_

class Triangle : public Primitive
{
	public:
		Triangle(Vertex a, Vertex b, Vertex c, Material* mat)
		{
			material = *mat;
		}

		Triangle(float a_x, float a_y, float a_z, float b_x, float b_y, float b_z, float c_x, float c_y, float c_z, Material* mat)
		{
			material = *mat;
		}

		virtual Vertex getNormal(const Vertex &v) const
		{
			Vertex e = Vertex(0.0f,0.0f);
			return e;
		}

	private:
		~Triangle(){}
};


#endif /* TRIANGLE_H_ */
