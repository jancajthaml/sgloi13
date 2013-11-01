package rabbit.gl.struct;

import rabbit.gl.math.SimpleMath;

public class Matrix implements Cloneable
{

	static Matrix identity	= new Matrix( 1.0f, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f );
	static Matrix scale		= new Matrix( 1.0f, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f );
	static Matrix translate	= new Matrix( 1.0f, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f );
	static Matrix rotate_2D	= new Matrix( 1.0f, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f );
	static Matrix rotate_X	= new Matrix( 1.0f, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f );
	static Matrix rotate_Y	= new Matrix( 1.0f, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f );
	static Matrix rotate_Z	= new Matrix( 1.0f, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f );
	static Matrix frustum	= new Matrix( 1.0f, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f );
	static Matrix ortho	= new Matrix( 1.0f, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f );
	
	public float[] matrix	= new float[16];
	
	Matrix()
	{}
	
	Matrix(float[] f)
	{ System.arraycopy(f, 0, matrix, 0, 16); }
	
	Matrix(float m11, float m21, float m31, float m41, float m12, float m22, float m32, float m42, float m13, float m23, float m33, float m43,  float m14, float m24, float m34, float m44)
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
	
	public static Matrix frustum(float left, float right, float bottom, float top, float near, float far)
	{
		float invDiffRL = 1.0f / (right - left);
		float invDiffTB = 1.0f / (top - bottom);
		float invDiffFN = 1.0f / (far - near);

		frustum.matrix[0]	= 2.0f * near * invDiffRL;
		frustum.matrix[5]	= 2.0f * near * invDiffTB;
		frustum.matrix[8]	= (right + left) * invDiffRL;
		frustum.matrix[9]	= (top + bottom) * invDiffTB;
		frustum.matrix[10]	= -((far + near) * invDiffFN);
		frustum.matrix[11]	= -1.0f;
		frustum.matrix[14]	= -2.0f * far * near * invDiffFN;
		frustum.matrix[15]   = 0.0f;

		return frustum;
	}

	public static Matrix ortho(float left, float right, float bottom, float top, float near, float far)
	{
		float invDiffRL = 1.0f / (right - left);
		float invDiffTB = 1.0f / (top - bottom);
		float invDiffFN = 1.0f / (far - near);

		ortho.matrix[0]		= 2.0f * invDiffRL;
		ortho.matrix[5]		= 2.0f * invDiffTB;
		ortho.matrix[10]	= -2.0f * invDiffFN;
		ortho.matrix[12]	= -((right+left)*invDiffRL);
		ortho.matrix[13]	= -((top+bottom)*invDiffTB);
		ortho.matrix[14]	= -((far+near)*invDiffFN);

		
		return ortho;
	}

	public static Matrix rotateY(float angle)
	{
		float sin = SimpleMath.sin(-angle);
		float cos = SimpleMath.cos(-angle);

		rotate_Y.matrix[0] = cos;
		rotate_Y.matrix[2] = -sin;
		rotate_Y.matrix[8] = sin;
		rotate_Y.matrix[10] = cos;

		return rotate_Y;
	}
	
	public static Matrix rotateX(float angle)
	{
		float sin = SimpleMath.sin(-angle);
		float cos = SimpleMath.cos(-angle);

		rotate_X.matrix[5] = cos;
		rotate_X.matrix[6] = -sin;
		rotate_X.matrix[9] = sin;
		rotate_X.matrix[10] = cos;

		return rotate_X;
	}
	
	public static Matrix rotateZ(float angle)
	{
		float sin = SimpleMath.sin(-angle);
		float cos = SimpleMath.cos(-angle);

		rotate_Z.matrix[0] = cos;
		rotate_Z.matrix[1] = -sin;
		rotate_Z.matrix[4] = sin;
		rotate_Z.matrix[5] = cos;

		return rotate_Z;
	}
	
	public Matrix multiply(Matrix other)
	{
		Matrix res = new Matrix();

		for (int r = 0; r < 4; r++)
		for (int c = 0; c < 4; c++)
		{
			res.matrix[r + (c<<2)] = 0.0f;
				
			for (int i = 0; i < 4; i++)
				res.matrix[r + (c<<2)] += matrix[r + (i<<2)] * other.matrix[i + (c<<2)];
		}

		return res;
	}

	public Vertex multiply(Vertex other)
	{
		return VertexStack.create
		(
			other.v.x * matrix[0] + other.v.y * matrix[4] + other.v.z * matrix[8] + other.v.w * matrix[12],
			other.v.x * matrix[1] + other.v.y * matrix[5] + other.v.z * matrix[9] + other.v.w * matrix[13],
			other.v.x * matrix[2] + other.v.y * matrix[6] + other.v.z * matrix[10] + other.v.w * matrix[14],
			other.v.x * matrix[3] + other.v.y * matrix[7] + other.v.z * matrix[11] + other.v.w * matrix[15]
		);
		//FIXME recycle/pool this instance
		/*
		return new Vertex
		(
			other.x * matrix[0] + other.y * matrix[4] + other.z * matrix[8] + other.w * matrix[12],
			other.x * matrix[1] + other.y * matrix[5] + other.z * matrix[9] + other.w * matrix[13],
			other.x * matrix[2] + other.y * matrix[6] + other.z * matrix[10] + other.w * matrix[14],
			other.x * matrix[3] + other.y * matrix[7] + other.z * matrix[11] + other.w * matrix[15]
		);
		*/
	}

	public Matrix clone()
	{ return MatrixStack.create(matrix); }

	public boolean equals(Object other)
	{
		if(!(other instanceof Matrix)) return false;
		Matrix v = (Matrix)other;
		for (int i = 0; i < 16; i++)
		{
			if (v.matrix[i] != matrix[i]) return false;    
        }
		return true;
	}
}