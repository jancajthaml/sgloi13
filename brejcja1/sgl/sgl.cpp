//---------------------------------------------------------------------------
// sgl.cpp
// Empty implementation of the SGL (Simple Graphics Library)
// Date:  2011/11/1
// Author: Jaroslav Krivanek, Jiri Bittner CTU Prague
//---------------------------------------------------------------------------
#include "sgl.h"	
#include "SglContext.h"
#include "Color.h"
#include "ContextManager.h"
#include "Matrix4.h"
#include <cstdio>
ContextManager ctx_mgr;

/// Current error code.
static sglEErrorCode _libStatus = SGL_NO_ERROR;

static inline void setErrCode(sglEErrorCode c) 
{
  if(_libStatus==SGL_NO_ERROR)
    _libStatus = c;
}

//---------------------------------------------------------------------------
// sglGetError()
//---------------------------------------------------------------------------
sglEErrorCode sglGetError(void) 
{
  sglEErrorCode ret = _libStatus;
  _libStatus = SGL_NO_ERROR;
  return ret;
}

//---------------------------------------------------------------------------
// sglGetErrorString()
//---------------------------------------------------------------------------
const char* sglGetErrorString(sglEErrorCode error)
{
  static const char *errStrigTable[] = 
  {
      "Operation succeeded",
      "Invalid argument(s) to a call",
      "Invalid enumeration argument(s) to a call",
      "Invalid call",
      "Quota of internal resources exceeded",
      "Internal library error",
      "Matrix stack overflow",
      "Matrix stack underflow",
      "Insufficient memory to finish the requested operation"
  };

  if((int)error<(int)SGL_NO_ERROR || (int)error>(int)SGL_OUT_OF_MEMORY ) {
    return "Invalid value passed to sglGetErrorString()"; 
  }

  return errStrigTable[(int)error];
}

//---------------------------------------------------------------------------
// Initialization functions
//---------------------------------------------------------------------------

void sglInit(void) {}

void sglFinish(void) 
{
	ctx_mgr.clearContexts();
}

int sglCreateContext(int width, int height) 
{
	ctx_mgr.addContext(SGLContext(width, height));
	return ctx_mgr.lastContextID();
}

void sglDestroyContext(int id)
{
	ctx_mgr.deleteContext(id);
}

void sglSetContext(int id)
{
	ctx_mgr.setContext(id);
}

int sglGetContext(void)
{
	return ctx_mgr.getCurrentContextID();
}
float *sglGetColorBufferPointer(void)
{
	sglEErrorCode err;
	float * fbuff = (float *)ctx_mgr.getContext(&err).getFramebuffer();
	setErrCode(err);	
	return fbuff;
}
//---------------------------------------------------------------------------
// Drawing functions
//---------------------------------------------------------------------------

void sglClearColor (float r, float g, float b, float alpha)
{
	sglEErrorCode err;
	ctx_mgr.getContext(&err).setClearColor(r, g, b);
	setErrCode(err);
}

void sglClear(unsigned what)
{
	sglEErrorCode err;
	ctx_mgr.getContext(&err).clear(what, &err);
	setErrCode(err);
}

void sglBegin(sglEElementType mode)
{
	sglEErrorCode err;
	ctx_mgr.getContext(&err).pushTypeState(mode);
	setErrCode(err);
}

void sglEnd(void)
{
	sglEErrorCode err;
	ctx_mgr.getContext(&err).draw();
	setErrCode(err);
}

void sglVertex4f(float x, float y, float z, float w) {}

void sglVertex3f(float x, float y, float z) {}

void sglVertex2f(float x, float y)
{
	sglEErrorCode err;
	ctx_mgr.getContext(&err).setVertex2f(x, y);
	setErrCode(err);
}

void sglCircle(float x, float y, float z, float radius)
{
	if (radius < 0)
	{
		setErrCode(SGL_INVALID_VALUE);
		return;
	}

	sglEErrorCode err;
	SGLContext c = ctx_mgr.getContext(&err);
	setErrCode(err);
	if (!c.beginEndCheck())
		setErrCode(SGL_INVALID_OPERATION);
	c.drawCircle(x, y, radius);
}

void sglEllipse(float x, float y, float z, float a, float b)
{
	if (a < 0 || b < 0)
	{
		setErrCode(SGL_INVALID_VALUE);
		return;
	}
	sglEErrorCode err;
	SGLContext c = ctx_mgr.getContext(&err);
	setErrCode(err);
	if (!c.beginEndCheck())
		setErrCode(SGL_INVALID_OPERATION);
	c.drawEllipse(x, y, z, a, b);
}

void sglArc(float x, float y, float z, float radius, float from, float to)
{
	if (radius < 0)
	{
		setErrCode(SGL_INVALID_VALUE);
		return;
	}
	sglEErrorCode err;
	ctx_mgr.getContext(&err).drawArc(x, y, z, radius, from, to);
	setErrCode(err);
}

//---------------------------------------------------------------------------
// Transform functions
//---------------------------------------------------------------------------

void sglMatrixMode( sglEMatrixMode mode )
{
	
	sglEErrorCode err1, err2;
	ctx_mgr.getContext(&err1).setMatrixMode(mode, &err2);
	setErrCode(err1);
	setErrCode(err2);

}

void sglPushMatrix(void)
{
	sglEErrorCode err1, err2;
	ctx_mgr.getContext(&err1).pushMatrix(&err2);
	setErrCode(err1);
	setErrCode(err2);
}

void sglPopMatrix(void)
{
	sglEErrorCode err1, err2;
	ctx_mgr.getContext(&err1).popMatrix(&err2);
	setErrCode(err1);
	setErrCode(err2);
}

void sglLoadIdentity(void)
{
	sglEErrorCode err1, err2;
	ctx_mgr.getContext(&err1).setCurrentMatrix(Matrix4::makeIdentity(), &err2);
	setErrCode(err1);
	setErrCode(err2);
}

void sglLoadMatrix(const float *matrix)
{
	sglEErrorCode err1, err2;
	ctx_mgr.getContext(&err1).setCurrentMatrix(Matrix4(matrix), &err2);
	setErrCode(err1);
	setErrCode(err2);
}

void sglMultMatrix(const float *matrix)
{
	sglEErrorCode err1, err2;
	Matrix4 mat(matrix);
	ctx_mgr.getContext(&err1).multiplyCurrentMatrix(mat, &err2);
	setErrCode(err1);
	setErrCode(err2);
}

void sglTranslate(float x, float y, float z)
{
	sglEErrorCode err1, err2;
	Matrix4 translation = Matrix4::makeTranslation(x, y, z);
	ctx_mgr.getContext(&err1).multiplyCurrentMatrix(translation, &err2);
	setErrCode(err1);
	setErrCode(err2);
}

void sglScale(float scalex, float scaley, float scalez)
{
	sglEErrorCode err1, err2;
	Matrix4 scale = Matrix4::makeScale(scalex, scaley, scalez);
	ctx_mgr.getContext(&err1).multiplyCurrentMatrix(scale, &err2);
	setErrCode(err1);
	setErrCode(err2);
}


void sglRotate2D(float angle, float centerx, float centery)
{
	sglEErrorCode err1, err2;
	sglTranslate(centerx, centery, 0.0f);
	Matrix4 rotate = Matrix4::makeRotation2D(angle, centerx, centery);
	ctx_mgr.getContext(&err1).multiplyCurrentMatrix(rotate, &err2);
	sglTranslate(-centerx, -centery, 0.0f);
	setErrCode(err1);
	setErrCode(err2);

}

void sglRotateY(float angle) {}

void sglOrtho(float left, float right, float bottom, float top, float near, float far)
{
	//printf("multiplying with ortho matrix\n");
	Matrix4 ortho(2/(right - left), 0, 0, 0, 0, 2/(top-bottom), 0, 0, 0, 0, -2/(far-near), 0, -((right+left)/(right-left)), -((top+bottom)/(top-bottom)), -((far+near)/(far-near)), 1);
	//ortho.print();
	sglEErrorCode err1, err2;
	ctx_mgr.getContext(&err1).multiplyCurrentMatrix(ortho, &err2);
	//ctx_mgr.getContext(&err1).getCurrentMatrix().print();
	setErrCode(err1);
	setErrCode(err2);
}

void sglFrustum(float left, float right, float bottom, float top, float near, float far) {}

void sglViewport(int x, int y, int width, int height)
{
	sglEErrorCode err;
	sglEErrorCode err2;
	ctx_mgr.getContext(&err).setViewport(width, height, x, y, &err2);
	setErrCode(err);
	setErrCode(err2);
}

//---------------------------------------------------------------------------
// Attribute functions
//---------------------------------------------------------------------------

void sglColor3f(float r, float g, float b)
{
	sglEErrorCode err;
	ctx_mgr.getContext(&err).setColor(Color(r, g, b));
	setErrCode(err);
}

void sglAreaMode(sglEAreaMode mode) {}

void sglPointSize(float size)
{
	sglEErrorCode err;
	ctx_mgr.getContext(&err).setPointSize(size);
	setErrCode(err);
}

void sglEnable(sglEEnableFlags cap) {}

void sglDisable(sglEEnableFlags cap) {}

//---------------------------------------------------------------------------
// RayTracing oriented functions
//---------------------------------------------------------------------------

void sglBeginScene() {}

void sglEndScene() {}

void sglSphere(const float x,
			   const float y,
			   const float z,
			   const float radius) {}

void sglMaterial(const float r,
				 const float g,
				 const float b,
				 const float kd,
				 const float ks,
				 const float shine,
				 const float T,
				 const float ior) {}

void sglPointLight(const float x,
				   const float y,
				   const float z,
				   const float r,
				   const float g,
				   const float b) {}

void sglRayTraceScene() {}

void sglRasterizeScene() {}

void sglEnvironmentMap(const int width,
					   const int height,
					   float *texels)
{}

void sglEmissiveMaterial(
						 const float r,
						 const float g,
						 const float b,
						 const float c0,
						 const float c1,
						 const float c2
						 )
{}

