package rabbit.gl.io;

import java.util.List;
import rabbit.gl.struct.Color;
import rabbit.gl.struct.Vertex;

public interface NFFCallbacks
{

	/// background color
	public void Background(Color col);

	/// background HDR map
	public void HDRBackground(String hdr_name);

	/// camera specification
	public void Camera(Vertex from, Vertex at, Vertex up, float angle, float hither, int width, int height);

	/// ppint light specification
	public void AddPointLight(Vertex pos, Color intensity);

	/// matyerial specification - the following geometry should have this material assigned to it
	public void SetMaterial(Color col, float kd, float ks, float shine, float T, float ior);

	/// sphere
	public void AddSphere(Vertex c, float r);

	/// polygonal patch. if 'normals' are empty, they were not specified in the input file
	public void PolyPatch(List<Vertex> vertices, List<Vertex> normals);

	/// area light specification. the following geometry emits light
	public void AreaLightBegin(Color intensity, Vertex atten);

	/// the following geometry does not emit light
	public void AreaLightEnd();

}
