package rabbit.gl.primitive;

import rabbit.gl.struct.Color;

public class Material implements Primitive
{
	public Color col;
	public float kd, ks, shine, T, ior;

	public Material(Color xcol, float xkd, float xks, float xshine, float xT, float xior)
	{
		this.col	= xcol;
		this.kd		= xkd;
		this.ks		= xks;
		this.shine	= xshine;
		this.T		= xT;
		this.ior	= xior;
	}

};
