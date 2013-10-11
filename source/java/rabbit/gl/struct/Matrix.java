package rabbit.gl.struct;

public class Matrix implements Cloneable
{
	static Matrix identity	= new Matrix( 1.0f, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f );
	static Matrix scale		= new Matrix( 1.0f, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f );
	static Matrix translate	= new Matrix( 1.0f, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f );
	static Matrix rotate_2D	= new Matrix( 1.0f, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f );
	
	public float[] matrix	= new float[16];
	
	public Matrix()
	{}
	
	public Matrix(float[] f)
	{ System.arraycopy(f, 0, matrix, 0, 16); }
	
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
	{ return identity; }

	public static Matrix scale(float sc_x, float sc_y, float sc_z)
	{
		scale.matrix[0]		= sc_x;
		scale.matrix[5]		= sc_y;
		scale.matrix[10]	= sc_z;
		return scale;
	}
	
	public static Matrix translate(float t_x, float t_y, float t_z)
	{
		translate.matrix[12] = t_x;
		translate.matrix[13] = t_y;
		translate.matrix[14] = t_z;
		return translate;
	};

	public static Matrix rotate2D(float angle, float c_x, float c_y)
	{
		float cos_a = (float)Math.cos(angle);
		float sin_a = (float)Math.sin(angle);
		
		rotate_2D.matrix[0]=cos_a;
		rotate_2D.matrix[1]=-sin_a;
		rotate_2D.matrix[4]=sin_a;
		rotate_2D.matrix[5]=cos_a;
		return rotate_2D;

	}

	public static Matrix rotateY(float angle)
	{
		return identity();
	}
	
	public Matrix multiply(Matrix other)
	{
		Matrix res = new Matrix();

		res.matrix[0] = matrix[0] * other.matrix[0] + matrix[4] * other.matrix[1] + matrix[8] * other.matrix[2] + matrix[12] * other.matrix[3];
		res.matrix[1] = matrix[1] * other.matrix[0] + matrix[5] * other.matrix[1] + matrix[9] * other.matrix[2] + matrix[13] * other.matrix[3];
		res.matrix[2] = matrix[2] * other.matrix[0] + matrix[6] * other.matrix[1] + matrix[10] * other.matrix[2] + matrix[14] * other.matrix[3];
		res.matrix[3] = matrix[3] * other.matrix[0] + matrix[7] * other.matrix[1] + matrix[11] * other.matrix[2] + matrix[15] * other.matrix[3];

		res.matrix[4] = matrix[0] * other.matrix[4] + matrix[4] * other.matrix[5] + matrix[8] * other.matrix[6] + matrix[12] * other.matrix[7];
		res.matrix[5] = matrix[1] * other.matrix[4] + matrix[5] * other.matrix[5] + matrix[9] * other.matrix[6] + matrix[13] * other.matrix[7];
		res.matrix[6] = matrix[2] * other.matrix[4] + matrix[6] * other.matrix[5] + matrix[10] * other.matrix[6] + matrix[14] * other.matrix[7];
		res.matrix[7] = matrix[3] * other.matrix[4] + matrix[7] * other.matrix[5] + matrix[11] * other.matrix[6] + matrix[15] * other.matrix[7];

		res.matrix[8] = matrix[0] * other.matrix[8] + matrix[4] * other.matrix[9] + matrix[8] * other.matrix[10] + matrix[12] * other.matrix[11];
		res.matrix[9] = matrix[1] * other.matrix[8] + matrix[5] * other.matrix[9] + matrix[9] * other.matrix[10] + matrix[13] * other.matrix[11];
		res.matrix[10] = matrix[2] * other.matrix[8] + matrix[6] * other.matrix[9] + matrix[10] * other.matrix[10] + matrix[14] * other.matrix[11];
		res.matrix[11] = matrix[3] * other.matrix[8] + matrix[7] * other.matrix[9] + matrix[11] * other.matrix[10] + matrix[15] * other.matrix[11];

		res.matrix[12] = matrix[0] * other.matrix[12] + matrix[4] * other.matrix[13] + matrix[8] * other.matrix[14] + matrix[12] * other.matrix[15];
		res.matrix[13] = matrix[1] * other.matrix[12] + matrix[5] * other.matrix[13] + matrix[9] * other.matrix[14] + matrix[13] * other.matrix[15];
		res.matrix[14] = matrix[2] * other.matrix[12] + matrix[6] * other.matrix[13] + matrix[10] * other.matrix[14] + matrix[14] * other.matrix[15];
		res.matrix[15] = matrix[3] * other.matrix[12] + matrix[7] * other.matrix[13] + matrix[11] * other.matrix[14] + matrix[15] * other.matrix[15];

		return res;
	}

	public Vertex multiply(Vertex other)
	{
		Vertex res = new Vertex(0,0);
		
		res.x = matrix[0] * other.x + matrix[4] * other.y + matrix[8] * other.z + matrix[12] * other.w;
		res.y = matrix[1] * other.x + matrix[5] * other.y + matrix[9] * other.z + matrix[13] * other.w;
		res.z = matrix[2] * other.x + matrix[6] * other.y + matrix[10] * other.z + matrix[14] * other.w;
		res.w = matrix[3] * other.x + matrix[7] * other.y + matrix[11] * other.z + matrix[15] * other.w;

		return res;
	}

	public Matrix clone()
	{ return new Matrix(matrix); }
}