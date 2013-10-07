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
	{ return new Matrix(	1.0f, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f); }

	public static Matrix scale(float x, float y, float z, float w)
	{ return new Matrix(x, 0.0f, 0.0f, 0.0f, 0.0f, y, 0.0f, 0.0f, 0.0f, 0.0f, z, 0.0f, 0.0f, 0.0f, 0.0f, w); }

	public static Matrix translate(float x, float y, float z)
	{ return new Matrix(1.0f, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f, x, y, z, 1.0f); }

	public static Matrix rotate(float angle, float centerx, float centery)
	{ return new Matrix( (float)Math.cos(angle), -(float)Math.sin(angle), 0.0f, 0.0f, (float)Math.sin(angle), (float)Math.cos(angle), 0.0f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f ); }

	public static Matrix rotateY(float angle)
	{ return new Matrix( (float)Math.cos(angle), 0.0f, -(float)Math.sin(angle), 0.0f, 0.0f, 1.0f, 0.0f, 0.0f, (float)Math.sin(angle), 0.0f, (float)Math.cos(angle), 0.0f, 0.0f, 0.0f, 0.0f, 1.0f ); }

	public Matrix multiply(Matrix other)
	{
	
		return other;
	}

	public Vertex multiply(Vertex other)
	{
		
		return other;
	}

}