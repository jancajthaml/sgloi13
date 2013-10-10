package struct;

public class Matrix
{
	public float[] matrix = new float[16];
	
	public Matrix()
	{
		
	}
	
	public Matrix(float[] f)
	{
		//System arraycopy
		this.matrix=f;
	}
	
	public Matrix(float m11, float m21, float m31, float m41, float m12, float m22, float m32, float m42, float m13, float m23, float m33, float m43,  float m14, float m24, float m34, float m44)
	{
		this.matrix[0] = m11;
		this.matrix[1] = m21;
		this.matrix[2] = m31;
		this.matrix[3] = m41;

		this.matrix[4] = m12;
		this.matrix[5] = m22;
		this.matrix[6] = m32;
		this.matrix[7] = m42;

		this.matrix[8] = m13;
		this.matrix[9] = m23;
		this.matrix[10] = m33;
		this.matrix[11] = m43;

		this.matrix[12] = m14;
		this.matrix[13] = m24;
		this.matrix[14] = m34;
		this.matrix[15] = m44;
	}

	public static Matrix identity()
	{
		return new Matrix(	1.0f, 0.0f, 0.0f, 0.0f,
				0.0f, 1.0f, 0.0f, 0.0f,
				0.0f, 0.0f, 1.0f, 0.0f,
				0.0f, 0.0f, 0.0f, 1.0f);
	}

	public static Matrix scale(float sc_x, float sc_y, float sc_z)
	{
		return new Matrix( sc_x, 0.0f, 0.0f, 0.0f,
			       	0.0f, sc_y, 0.0f, 0.0f,
			       	0.0f, 0.0f, sc_z, 0.0f,
				0.0f, 0.0f, 0.0f, 1.0f);
	}
	
	public static Matrix translate(float t_x, float t_y, float t_z)
	{
		return new Matrix(	1.0f, 0.0f, 0.0f, 0.0f ,
				0.0f, 1.0f, 0.0f, 0.0f ,
				0.0f, 0.0f, 1.0f, 0.0f ,
				t_x, t_y, t_z, 1.0f);
	};

	public static Matrix rotate2D(float angle, float c_x, float c_y)
	{
		float cos_a = (float)Math.cos(angle);
		float sin_a = (float)Math.sin(angle);
		return new Matrix(	cos_a, -sin_a, 0.0f, 0.0f ,
				sin_a, cos_a, 0.0f, 0.0f,
				0.0f, 0.0f, 1.0f, 0.0f ,
				0.0f, 0.0f, 0.0f, 1.0f);

	}

	public static Matrix rotateY(float angle)
	{
		return identity();
	}
	
	public Matrix multiply(Matrix other)
	{
		Matrix m = new Matrix();
		
		// Row 1
    	m.matrix[0]  = this.matrix[0]  * other.matrix[0]  +  this.matrix[1]  * other.matrix[4]  +  this.matrix[2] * other.matrix[8]   +  this.matrix[3]  * other.matrix[12];    // Column 1
    	m.matrix[1]  = this.matrix[0]  * other.matrix[1]  +  this.matrix[1]  * other.matrix[5]  +  this.matrix[2] * other.matrix[9]   +  this.matrix[3]  * other.matrix[13];    // Column 2
    	m.matrix[2]  = this.matrix[0]  * other.matrix[2]  +  this.matrix[1]  * other.matrix[6]  +  this.matrix[2] * other.matrix[10]  +  this.matrix[3]  * other.matrix[14];    // Column 3
    	m.matrix[3]  = this.matrix[0]  * other.matrix[3]  +  this.matrix[1]  * other.matrix[7]  +  this.matrix[2] * other.matrix[11]  +  this.matrix[3]  * other.matrix[15];    // Column 4

    	// Row 2
    	m.matrix[4]  = this.matrix[4]  * other.matrix[0]  +  this.matrix[5]  * other.matrix[4]  +  this.matrix[6] * other.matrix[8]   +  this.matrix[7]  * other.matrix[12];    // Column 1
    	m.matrix[5]  = this.matrix[4]  * other.matrix[1]  +  this.matrix[5]  * other.matrix[5]  +  this.matrix[6] * other.matrix[9]   +  this.matrix[7]  * other.matrix[13];    // Column 2
    	m.matrix[6]  = this.matrix[4]  * other.matrix[2]  +  this.matrix[5]  * other.matrix[6]  +  this.matrix[6] * other.matrix[10]  +  this.matrix[7]  * other.matrix[14];    // Column 3
    	m.matrix[7]  = this.matrix[4]  * other.matrix[3]  +  this.matrix[5]  * other.matrix[7]  +  this.matrix[6] * other.matrix[11]  +  this.matrix[7]  * other.matrix[15];    // Column 4

    	// Row 3
    	m.matrix[8]  = this.matrix[8]  * other.matrix[0]  +  this.matrix[9]  * other.matrix[4]  +  this.matrix[10] * other.matrix[8]  +  this.matrix[11] * other.matrix[12];    // Column 1
    	m.matrix[9]  = this.matrix[8]  * other.matrix[1]  +  this.matrix[9]  * other.matrix[5]  +  this.matrix[10] * other.matrix[9]  +  this.matrix[11] * other.matrix[13];    // Column 2
    	m.matrix[10] = this.matrix[8]  * other.matrix[2]  +  this.matrix[9]  * other.matrix[6]  +  this.matrix[10] * other.matrix[10] +  this.matrix[11] * other.matrix[14];    // Column 3
    	m.matrix[11] = this.matrix[8]  * other.matrix[3]  +  this.matrix[9]  * other.matrix[7]  +  this.matrix[10] * other.matrix[11] +  this.matrix[11] * other.matrix[15];    // Column 4

    	// Row 4
    	m.matrix[12] = this.matrix[12] * other.matrix[0]  +  this.matrix[13] * other.matrix[4]  +  this.matrix[14] * other.matrix[8]  +  this.matrix[15] * other.matrix[12];    // Column 1
    	m.matrix[13] = this.matrix[12] * other.matrix[1]  +  this.matrix[13] * other.matrix[5]  +  this.matrix[14] * other.matrix[9]  +  this.matrix[15] * other.matrix[13];    // Column 2
    	m.matrix[14] = this.matrix[12] * other.matrix[2]  +  this.matrix[13] * other.matrix[6]  +  this.matrix[14] * other.matrix[10] +  this.matrix[15] * other.matrix[14];    // Column 3
    	m.matrix[15] = this.matrix[12] * other.matrix[3]  +  this.matrix[13] * other.matrix[7]  +  this.matrix[14] * other.matrix[11] +  this.matrix[15] * other.matrix[15];    // Column 4

		return other;
	}

	public Vertex multiply(Vertex other)
	{
		Vertex res = new Vertex(0,0);
		res.x = (other.x*this.matrix[0]) + (other.y*this.matrix[4]) + (other.z*this.matrix[8]) + (other.w*this.matrix[12]);
		res.y = (other.x*this.matrix[1]) + (other.y*this.matrix[5]) + (other.z*this.matrix[9]) + (other.w*this.matrix[13]);
		res.z = (other.x*this.matrix[2]) + (other.y*this.matrix[6]) + (other.z*this.matrix[10]) + (other.w*this.matrix[14]);
		res.w = (other.x*this.matrix[3]) + (other.y*this.matrix[7]) + (other.z*this.matrix[11]) + (other.w*this.matrix[15]);

		return res;
	}

}