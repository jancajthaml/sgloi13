//---------------------------------------------------------------------------
// sgl.h
// Header file for SGL (Simple Graphics Library)
// Date:  2012/10/1
// Author: Jaroslav Krivanek, Jiri Bittner,  CTU Prague
//---------------------------------------------------------------------------

#ifndef _SGL_H_
#define _SGL_H_
#include <stdint.h>
//---------------------------------------------------------------------------
// Data types
//---------------------------------------------------------------------------

/// Error codes
enum sglEErrorCode {
  /// Operation succeeded
  SGL_NO_ERROR=0,
  /// Invalid argument(s) to a call
  SGL_INVALID_VALUE,
  /// Invalid enum argument(s) to a call
  SGL_INVALID_ENUM,
  /// Invalid call (most typical: sglBegin after another sglBegin,
  /// state change inside sglBegin/sglEnd pair, no context defined)
  SGL_INVALID_OPERATION,
  /// Quota of internal resources exceeded (e.g. number of drawing contexts)
  SGL_OUT_OF_RESOURCES,
  /// Internal library error
  SGL_INTERNAL_ERROR,
  /// Matrix stack overflow. Generated by sglPushMatrix().
  SGL_STACK_OVERFLOW,
  /// Matrix stack underflow - trying to pop matrix when there is only one matrix left on the stack.
  /// Generated by sglPopMatrix().
  SGL_STACK_UNDERFLOW,
  /// Insufficient memory to finish the requested operation
  SGL_OUT_OF_MEMORY
};

/// Types of graphic elements which are specified using the sglVertexXf command
enum sglEElementType {
  /// Points
  SGL_POINTS = 1,
  /// Lines
  SGL_LINES,
  /// Line strip
  SGL_LINE_STRIP,
  /// Closed line strip
  SGL_LINE_LOOP,
  /// Triangle list
  SGL_TRIANGLES,
  /// General, non-convex polygon
  SGL_POLYGON,
  /// Area light - restricted to a quad for simplicity
  SGL_AREA_LIGHT,
  SGL_LAST_ELEMENT_TYPE
};

/// Bits indicating what to clear with sglClear()
enum sglEClearBit {
  /// Clear the color buffer
  SGL_COLOR_BUFFER_BIT = 0x0001,
  /// Clear the depth buffer
  SGL_DEPTH_BUFFER_BIT = 0x0002
};

/// Enum for selecting matrix stacks to work with. Passed to sglMatrixMode().
enum sglEMatrixMode {
  /// Modelview matrix
  SGL_MODELVIEW  = 0,
  /// Projection matrix
  SGL_PROJECTION = 1
};

/// Enum for area fill modes. Passed to sglAreaMode().
enum sglEAreaMode {
  /// Draw only vertices
  SGL_POINT	= 0,
  /// Draw only borders of graphics elements (lines)
  SGL_LINE  = 1,
  /// Draw filled elements, default.
  SGL_FILL  = 2
};

/// Enum for sglEnable() / sglDisable()
enum sglEEnableFlags {
  /// enable/disable depth test
  SGL_DEPTH_TEST = 1
};

//---------------------------------------------------------------------------
// Error handling functions
//---------------------------------------------------------------------------

// Note: All functions should set the value of an internal error code variable,
// whose value can be accessed from the application through sglGetErrorCode and
// sglGetErrorString functions.  This will be randomly tested when checking the
// assignements.

/// Return the error information.
/**
  sglGetError returns the value of the error flag. Each detectable error is
  assigned a numeric code and symbolic name. When an error occurs, the error
  flag is set to the appropriate error code value. No other errors are recorded
  until sglGetError is called, the error code is returned, and the flag is reset
  to SGL_NO_ERROR. If a call to sglGetError returns SGL_NO_ERROR, there has been
  no detectable error since the last call to sglGetError, or since the SGL was
  initialized.
 */
sglEErrorCode sglGetError(void);

/// Return the string representing the current error code.
const char* sglGetErrorString(sglEErrorCode error);

//---------------------------------------------------------------------------
// Initialization functions
//---------------------------------------------------------------------------

/// Library initialization
/**
  Initialize the SGL, allocate internal data structures.

  ERRORS:
   - SGL_OUT_OF_MEMORY
*/
void sglInit(void);

/// Clean up the library, deallocate internal data structures.
/**
  This must destroy all the allocated drawing contexts.
  No memory leaking is acceptable! Memory leaks will be tested when
  returning the assignments!

  ERRORS:
   - none
*/
void sglFinish(void);

/// Create a new drawing context.
/**
  Create a new drawing context for a drawing window with dimensions width x height of
  RGB pixels. Allocates additional internal structures for the context (including the
  depth buffer).

  @param width [in] Desired canvas width.
  @param height [in] Desired canvas height.
  @return Unique identifier of the drawing context.

  ERRORS:
   - SGL_OUT_OF_MEMORY ... not enough memory
   - SGL_OUT_OF_RESOURCES ... too many contexts
     (Implementation dependent. At least 32 must be provided, though.)
*/
int sglCreateContext(int width, int height);

/// Destroy a drawing context.
/**
  Deallocate the canvas + internal structures of the context.
  No memory leaking is acceptable! Memory leaks will be tested when
  returning the assignments!

  @param id [in] Identified of the deleted drawing context.

  ERRORS:
    - SGL_INVALID_VALUE ... invalid context id
*/
void sglDestroyContext(int id);

/// Set the current drawing context (ie. drawing window for further drawing operations)
/**
  @param id [in] Identifier of the context to set.

  ERRORS:
    - SGL_INVALID_VALUE ... invalid context id
*/
void sglSetContext(int id);

/// Return the identifier of the current drawing context.
/**
  ERRORS:
    - SGL_INVALID_OPERATION ... no context has been allocated
*/
int sglGetContext(void);

/// Return the pointer to the color buffer of the current drawing context.
/**
  ERRORS:
    - none (0 returned if no context allocated. No error code set.)
*/
float *sglGetColorBufferPointer(void);

//---------------------------------------------------------------------------
// Drawing functions
//---------------------------------------------------------------------------

// Notice:
// All coordinates passed to the library undergo the modelling/projection/viewport
// transform.

/// Clear the buffer according to the specified bitmask
/**
  @param what is a bitmask composed of the following bits:
    - SGL_COLOR_BUFFER_BIT ... clear the color buffer (use the color specified by sglClearColor)
    - SGL_DEPTH_BUFFER_BIT ... clear the depth buffer (use a very large positive value)

  ERRORS:
    - SGL_INVALID_VALUE
       Is generated if any bit other than the two defined bits is set in mask.
    - SGL_INVALID_OPERATION
       No context has been allocated yet or sglClear is called between a
       call to sglBegin and the corresponding call to sglEnd.
 */
void sglClear(unsigned what);

/// Start drawing a specified element given by the elementType parameter.
/**
  ERRORS:
    - SGL_INVALID_ENUM
     is generated if mode is set to an unaccepted value.
    - SGL_INVALID_OPERATION
     is generated if sglEnd is called before the corresponding sglBegin is called,
     or if sglBegin is called within a sglBegin/sglEnd sequence.
 */
void sglBegin(sglEElementType mode);

/// Element completion.
/**
  ERRORS:
    - SGL_INVALID_OPERATION
     is generated if sglEnd is called before the corresponding sglBegin is called,
     or if sglBegin is called within a sglBegin/sglEnd sequence.
 */
void sglEnd(void);

/// Input of a vertex.
/**
  ERRORS:
   - none (Invoking sglVertex outside of a sglBegin/sglEnd pair results in undefined behavior.)
 */
void sglVertex4f(float x, float y, float z, float w);

/// Input of a vertex. Assumes w=1.
/**
  ERRORS:
   - none (Invoking sglVertex outside of a sglBegin/sglEnd pair results in undefined behavior.)
 */
void sglVertex3f(float x, float y, float z);

/// Input of a vertex. Assumes z=0, w=1.
/**
  ERRORS:
   - none (Invoking sglVertex outside of a sglBegin/sglEnd pair results in undefined behavior.)
 */
void sglVertex2f(float x, float y);

/// Circle drawing
/**
  @param x   [in] circle center x
  @param y   [in] circle center y
  @param z   [in] circle center z (depth)
  @param radius [in] circle radius

  Current transformation is applied _only_ to the coordinates of the center.

  Radius is scaled by the overall scale [x,y] of the concatenated
   (modelview o projection o viewport) tranformation. This scaling
   factor is computed as the square root of the determinant of the 2x2 upper left
   block of the contatenated transformation matrix.

  A call to sglCircle always draws a circle, no matter the
  possible non-uniform scale or squash in the current transform matrices.
  In addition, the depth of all the generated fragments is constant
  and equal to the depth of the transformed circle center.

  The fill mode is given by the last call to sglAreaMode().
  If SGL_POINT is specified, a point in the circle center is generated.

  ERRORS:
   - SGL_INVALID_VALUE
    Is generated if the radius not positive.
   - SGL_INVALID_OPERATION
    No context has been allocated yet or sglCircle is called between a
    call to sglBegin and the corresponding call to sglEnd.
*/
void sglCircle(float x, float y, float z, float radius);

/// Ellipse drawing (ellipse axes aligned with coordinate axes).
/**
 @param x   [in] ellipse center x
 @param y   [in] ellipse center y
 @param z   [in] ellipse center z (depth)
 @param a   [in] major axis length
 @param b   [in] minor axis length

 The whole ellipse must be appropriately transformed!
 (The simplest way to do this is to approximate the ellipse with a
  polyline / polygon (depending on the fill mode) and transform each
  vertex separately.

 Use approximation by a lineloop/polygon of exactly 40 vertices
  (so that the speed measurements are fair).

 NOTE:
  For a bonus of 3 points, you may use adaptive subdivision up to a 1 pixel tolerance (in screen space).

 The fill mode is given by the last call to sglAreaMode().
 If SGL_POINT is specified, a point in the circle center is generated.

 ERRORS:
  - SGL_INVALID_VALUE
    Is generated if the a or b is negative.
  - SGL_INVALID_OPERATION
    No context has been allocated yet or sglEllipse is called between a
    call to sglBegin and the corresponding call to sglEnd.
*/
void sglEllipse(float x, float y, float z, float a, float b);

/// Circular arc drawing.
/**
  @param x   [in] circle center x
  @param y   [in] circle center y
  @param z   [in] circle center z
  @param radius [in] circle radius
  @param from   [in] starting angle (measured counterclockwise from the x-axis)
  @param to     [in] ending angle (measured counterclockwise from the x-axis)

  Use approximation by a linestrip / polygon of exactly 40*(to-from)/(2*PI) vertices
  (so that the speed measurements are fair).

  See sglEllipse() for more info.
 */
void sglArc(float x, float y, float z, float radius, float from, float to);

//---------------------------------------------------------------------------
// Transform functions
//---------------------------------------------------------------------------

// Note:
// Points are column vectors and transformation matrices are 4x4 matrices stored as
// in OpenGL [column-by-column]. Transformed vector is computed by vt = M*v.

/// Specifies which matrix stack is the target for subsequent matrix operations.
/**
  @param mode
    Two values are accepted: SGL_MODELVIEW and SGL_PROJECTION.

  ERRORS:
   - SGL_INVALID_ENUM is generated if mode is not an accepted value.
   - SGL_INVALID_OPERATION
    No context has been allocated yet or sglMatrixMode is called between a
    call to sglBegin and the corresponding call to sglEnd.
 */
void sglMatrixMode( sglEMatrixMode mode );

/// Store the current transform matrix on the stack.
/**
  ERRORS:
   - SGL_STACK_OVERFLOW
    is generated if sglPushMatrix is called while the current
    matrix stack is full.
   - SGL_INVALID_OPERATION
    No context has been allocated yet or sglPushMatrix is called between a
    call to sglBegin and the corresponding call to sglEnd.
 */
void sglPushMatrix(void);

/// Pop the matrix from the current matrix stack.
/**
  ERRORS:
   - SGL_STACK_UNDERFLOW
    is generated if sglPopMatrix is called while the current matrix stack
    contains only a single matrix.
   - SGL_INVALID_OPERATION
    No context has been allocated yet or sglPopMatrix is called between a
    call to sglBegin and the corresponding call to sglEnd.
 */
void sglPopMatrix(void);

/// Set the current transformation matrix to identity.
/**
  ERRORS:
   - SGL_INVALID_OPERATION
    No context has been allocated yet or sglLoadIdentity is called between a
    call to sglBegin and the corresponding call to sglEnd.
 */
void sglLoadIdentity(void);

/// Replace the current matrix by the given matrix (elements are stored column-by-column).
/**
  ERRORS:
   - SGL_INVALID_OPERATION
    No context has been allocated yet or sglLoadMatrix is called between a
    call to sglBegin and the corresponding call to sglEnd.
 */
void sglLoadMatrix(const float *matrix);

/// Multiply the given matrix by the given matrix.
/**
  The multiplication is done so that the tranformation represented by the
  parameter 'matrix' is applied BEFORE the transformation represented by the
  current transform matrix. The operation applied is C := C*M where C is the
  current tranformation matrix and M is the matrix passed in the 'matrix'
  parameter. This is the standard behavior in OpenGL.

  ERRORS:
   - SGL_INVALID_OPERATION
    No context has been allocated yet or sglMultMatrix is called between a
    call to sglBegin and the corresponding call to sglEnd.
*/
void sglMultMatrix(const float *matrix);

/// Multiply current matrix by a translation matrix.
/**
  ERRORS:
   - SGL_INVALID_OPERATION
    No context has been allocated yet or sglTranslate is called between a
    call to sglBegin and the corresponding call to sglEnd.
*/
void sglTranslate(float x, float y, float z);

/// Multiply the current matrix by a non-uniform scale matrix
/**
  ERRORS:
   - SGL_INVALID_OPERATION
    No context has been allocated yet or sglScale is called between a
    call to sglBegin and the corresponding call to sglEnd.
*/
void sglScale(float scalex, float scaley, float scalez);

/// Multiply the current matrix by a matrix of rotation by the given angle
/// around a given center of rotation.
/**
  ERRORS:
   - SGL_INVALID_OPERATION
    No context has been allocated yet or sglRotate2D is called between a
    call to sglBegin and the corresponding call to sglEnd.
*/
void sglRotate2D(float angle, float centerx, float centery);

/// Multiply the current matrix by a matrix of rotation by the given angle
/// around the Y axis.
/**
  ERRORS:
   - SGL_INVALID_OPERATION
    No context has been allocated yet or sglRotateY is called between a
    call to sglBegin and the corresponding call to sglEnd.
*/
void sglRotateY(float angle);

/// Multiply the current matrix by an orthographic matrix.
/**
  @param left, right
    Specify the coordinates for the left and right vertical clipping planes.
  @param bottom, top
    Specify the coordinates for the bottom and top horizontal clipping planes.
  @param near, far
    Specify the distances to the nearer and farther depth clipping planes.
    These distances are negative if the plane is to be behind the viewer.

  See the OpenGL reference manual (the blue book) for the exact form of the
  orthographic matrix.

  ERRORS:
   - SGL_INVALID_OPERATION
    No context has been allocated yet or sglOrtho is called between a
    call to sglBegin and the corresponding call to sglEnd.
*/
void sglOrtho(float left, float right, float bottom, float top, float near, float far);

/// Multiply the current matrix by a perspective matrix
/**
  @param left, right
    Specify the coordinates for the left and right vertical clipping planes.
  @param bottom, top
    Specify the coordinates for the bottom and top horizontal clipping planes.
  @param near, far
   Specify the distances to the near and far depth clipping planes.
   Both distances must be positive.

  See the OpenGL reference manual (the blue book) for the exact form of the
  perspective matrix.

ERRORS
 - SGL_INVALID_VALUE
   is generated if near or far is not positive.
 - SGL_INVALID_OPERATION
   No context has been allocated yet or sglFrustum is called between a
   call to sglBegin and the corresponding call to sglEnd.
*/
void sglFrustum(float left, float right, float bottom, float top, float near, float far);

/// Set the viewport tranformation.
/**
  sglViewport specifies the affine transformation of x and y from normalized device
  coordinates to window (canvas) coordinates.
  Let (xnd, ynd) be normalized device coordinates (computed by transforming the vertices by
  [modelview o projection] matrix). Then the window coordinates (xw, yw) are computed as follows:
  xw = (xnd + 1)*width/2  + x
  yw = (ynd + 1)*height/2 + y

  Parameters x, y specify the viewport position with respect to the drawing
  canvas. Parameters width and height specify the viewport size in pixels.

  ERRORS:
   - SGL_INVALID_VALUE
    is generated if either width or height is negative.
   - SGL_INVALID_OPERATION
    No context has been allocated yet or sglViewport is called between a
    call to sglBegin and the corresponding call to sglEnd.
*/
void sglViewport(int x, int y, int width, int height);

//---------------------------------------------------------------------------
// Attribute functions
//---------------------------------------------------------------------------

/// Set the color for clearing the color buffer. Aplpha is ignored.
/**
  ERRORS:
    - SGL_INVALID_OPERATION
     No context has been allocated yet or sglClearColor is called between a
     call to sglBegin() and the corresponding call to sglEnd().
*/
void sglClearColor (float r, float g, float b, float alpha);

/// Set current color in RGB model.
/**
 Current color is used for all subsequent graphics primitives, until
 another sglColor3f command is issued.
*/
void sglColor3f(float r, float g, float b);

/// Drawing mode of closed areas.
/**
 @param mode
   Three values are accepted:
    - SGL_POINT
     Draw only vertices (or center for sglCircle, sglEllipse, sglArc)
    - SGL_LINE
     Draw only borders of graphics elements (lines)
    - SGL_FILL
     Draw filled elements, default.

 ERRORS:
  - SGL_INVALID_ENUM
   is generated if mode is not an accepted value.
  - SGL_INVALID_OPERATION
    No context has been allocated yet or sglAreaMode is called between a
    call to sglBegin and the corresponding call to sglEnd.
*/
void sglAreaMode(sglEAreaMode mode);

/// Set the size of points in pixels.
/**
 Points are rendered as squares size x size

 ERRORS:
  - SGL_INVALID_VALUE
   is generated if size is less than or equal to zero.
  - SGL_INVALID_OPERATION
    No context has been allocated yet or sglPointSize is called between a
    call to sglBegin and the corresponding call to sglEnd.
*/
void sglPointSize(float size);


/// enable SGL capabilities
/**
 @param cap:
   SGL_DEPTH_TEST ... depth test is off by default

 ERRORS:
  - SGL_INVALID_ENUM
   is generated if cap is not one of the values listed above.
  - SGL_INVALID_OPERATION
    No context has been allocated yet or sglEnable is called between a
    call to sglBegin and the corresponding call to sglEnd.
 */
void sglEnable(sglEEnableFlags cap);

/// disable SGL capabilities
/**
 @param cap:
   SGL_DEPTH_TEST

 ERRORS:
  - SGL_INVALID_ENUM
   is generated if cap is not one of the values listed above.
  - SGL_INVALID_OPERATION
    No context has been allocated yet or sglDisable is called between a
    call to sglBegin and the corresponding call to sglEnd.
 */
void sglDisable(sglEEnableFlags cap);

/// Starts a definition of the scene.
/// Any primitves specified by GL_POLYGON (in begin/end) or sglSphere
/// will be added to the scene primitive lists
/// If the scene is not empty it is cleared.
/// During the scene definition no transformations applied to the
/// primitive vertices!
/**

 ERRORS:
  - SGL_INVALID_OPERATION
    No context has been allocated yet or sglBeginScene is called between a
    call to sglBegin and the corresponding call to sglEnd.
 */
void sglBeginScene();

/// Ends a definition of the scene.
/**
 ERRORS:
  - SGL_INVALID_OPERATION
    No context has been allocated yet or sglEndScene is called between a
    call to sglBegin and the corresponding call to sglEnd.
 */
void sglEndScene();

/// Definition of the sphere primitve.
/// This function can only be used with scene definition, where it adds
/// the sphere to the primitive list

/**

 ERRORS:
  - SGL_INVALID_OPERATION
    No context has been allocated yet or sglSphere is called between a
    call to sglBegin and the corresponding call to sglEnd or sglSphere is
	called out side sglBeginScene/sglEndScene pair.
 */
void sglSphere(const float x,
			   const float y,
			   const float z,
			   const float radius);



/// Input of a material using a Phong model.
/**
 Current material is used for all subsequent graphics primitives, until
 another sglColor3f command is issued.
*/
/**
   @param r [in] color component.
   @param g [in] color component.
   @param b [in] color component.
   @param kd [in] diffuse coef.
   @param ks [in] specular coef.
   @param shine [in] Phong cosine power for highlights.
   @param T [in] transmittance (fraction of contribution of the
    transmitting ray).
   @param ior [in] index of refraction.

   Usually, 0 <= Kd <= 1 and 0 <= Ks <= 1, though it is
   not required that Kd + Ks == 1.  Note that transmitting objects ( T > 0 )
   are considered to have two sides for algorithms that need these (normally
   objects have one side).

  ERRORS:
  - SGL_INVALID_OPERATION
    No context has been allocated yet or sglMaterial is called between a
    call to sglBegin and the corresponding call to sglEnd.
 */
void sglMaterial(const float r,
				 const float g,
				 const float b,
				 const float kd,
				 const float ks,
				 const float shine,
				 const float T,
				 const float ior);


/// Input of a point light.
/**
Adds a point light to the scene.
*/
/**
   @param x [in] x coordinate of the position of the light.
   @param y [in] y coordinate of the position of the light.
   @param z [in] z coordinate of the position of the light.
   @param r [in] light intensity in the red channel.
   @param g [in] light intensity in the green channel.
   @param b [in] light intensity in the blue channel.

  ERRORS:
  - SGL_INVALID_OPERATION
    No context has been allocated yet or sglPointLight not called between a
    call to sglBeginScene and the corresponding call to sglEndScene.
 */
void sglPointLight(const float x,
				   const float y,
				   const float z,
				   const float r,
				   const float g,
				   const float b);



/// Compute an image using ray tracing
/**
Compute an image using ray tracing. The already supported functionality depends on
the actual task being implemented.
*/
void sglRayTraceScene();

/// Compute an image using ray tracing
/**
Compute an image using rasterization.
*/
void sglRasterizeScene();


/// Sets emissive material (for definition of area lights).
/**
Subsequent geometry (SGL_POLYGONS) will represent area light patches
until a call to sglEmissiveMaterial occurs. Only triangle patches need
to be supported. The total energy emitted from one area light patch
is (r,g,b)*patch_area. The attenuationm with the distance
d is 1/(c0 + c1*d + c2*d^2).
*/
/**
   @param r [in] light intensity per unit area.
   @param g [in] light intensity per unit area.
   @param b [in] light intensity per unit area.
   @param c0 [in] attentuation constant c0.
   @param c1 [in] attentuation constant c1.
   @param c2 [in] attentuation constant c2.


  ERRORS:
  - SGL_INVALID_OPERATION
     No context has been allocated yet or sglEmissiveMaterial is called between a
     call to sglBegin() and the corresponding call to sglEnd().
 */
void sglEmissiveMaterial(
						 const float r,
						 const float g,
						 const float b,
						 const float c0,
						 const float c1,
						 const float c2
						 );

/// Set the HDR environment map defining the "background"
/**
   If defined the environment map replaces the background color (set with sglClearColor) for both primary as well as secondary rays.
*/
/**
   @param width [in] texture width.
   @param height [in] texture height.
   @param texels [in] 3*width*height RGB tripplets corresponding to texels (in floats).*/
/**
  ERRORS:
    - SGL_INVALID_OPERATION
     No context has been allocated yet or sglEnvironmentMap is called between a
     call to sglBegin() and the corresponding call to sglEnd().
*/
void sglEnvironmentMap(const int width,
					   const int height,
					   float *texels);


void sglTexture(const int width, const int height, float *image);


#endif /* of _SGL_H_ */
