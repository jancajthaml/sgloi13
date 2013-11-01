package rabbit.gl.io;

import java.util.List;
import java.util.Stack;

import rabbit.gl.math.SimpleMath;
import rabbit.gl.primitive.Material;
import rabbit.gl.primitive.Sphere;
import rabbit.gl.primitive.Triangle;
import rabbit.gl.struct.Color;
import rabbit.gl.struct.Vertex;
import rabbit.struct.common.GenericStack;

public class NFFStore implements NFFCallbacks
{
	public class PointLight
	{
		public Vertex position;
		public Color intensity;
		
		public PointLight()
		{
			this.position = new Vertex();
			this.intensity = new Color(1,1,1);
		}

		public PointLight( Vertex pos, Color inten)
		{
			this.position  = pos;
			this.intensity = inten;
		}
	};
	
	/// one material with a list of geometry that has the meteria assigned to it
	public class LightGroup
	{
		public Color intensity;
		public Vertex atten;
		
		public GenericStack<Triangle> geometry = new GenericStack<Triangle>();
		public GenericStack<Sphere> spheres = new GenericStack<Sphere>();
	  
		public LightGroup(Color i, Vertex a)
		{
			this.intensity = i;
			this.atten     = a;
		}
	};

	/// one material with a list of geometry that has the meteria assigned to it
	public class MaterialGroup
	  {
		  public Material        material ;
		  public GenericStack<Triangle>  geometry = new GenericStack<Triangle>() ;
		  public GenericStack<Sphere>    spheres  = new GenericStack<Sphere>();
		  
		  public MaterialGroup(Color xcol, float xkd, float xks, float xshine, float xT, float xior)
		  { this.material=new Material(xcol,xkd,xks,xshine,xT,xior); }
	  };

	  /// true if speheres should be tesselated into triangles
	  boolean tesselateSpheres;

	  boolean areaLightMode;
	  
	  
	  /// background
	  public Color bg_col;

	  //HDRLoaderResult envMap;
	  
	  /// camera
	  public Vertex from;
	  public Vertex at;
	  public Vertex up;
	  public float angle;
	  public float hither;
	  public int width;
	  public int height;
	  
	  public GenericStack<MaterialGroup> matgroups = new GenericStack<MaterialGroup>();
	  public GenericStack<LightGroup> lightgroups = new GenericStack<LightGroup>();
	  public GenericStack<PointLight> pointLights = new GenericStack<PointLight>();

	  public NFFStore(boolean ts)
	  {
		  areaLightMode = false;
		  tesselateSpheres=ts;
	  }
	  
	  public void Background(Color col)
	  { bg_col = col; }

	  // not implemented yet
	  public void HDRBackground(String hdr_name)
	  {
		//if (!HDRLoader.load(hdr_name, envMap))
		//{
			//System.out.println("Cound not read hdr env map ! "+hdr_name);
	//		envMap.cols = null;
		//}
		
	  }

	  public void Camera(Vertex f, Vertex a, Vertex u, float ang, float hit, int   w, int h)
	  {
		  from = f;
		  at = a;
		  up = u;
		  angle = ang;
		  hither = hit;
		  width  = w;
		  height = h;
	  }

	  // not implemented
	  public void AddPointLight(Vertex pos, Color intensity)
	  {
		  pointLights.push(new PointLight(pos, intensity));
	  }

	  public void SetMaterial(Color col, float kd, float ks, float shine, float T, float ior)
	  {
		  matgroups.push(new MaterialGroup(col,kd,ks,shine,T,ior));
	  }

	  /// convert spherical coordinates to a 3D vector in cartesian coordinates
	  public Vertex.Geometric sph2cart(float theta,float phi)
	  { 
	    float sin_theta = SimpleMath.sin(theta);
	    return new Vertex.Geometric( sin_theta*SimpleMath.cos(phi), sin_theta*SimpleMath.sin(phi), SimpleMath.cos(theta) );
	  }

	  /// helper class for sphere triangulation
	  public class TD {
	  
		  public Triangle t;
		  public int       depth;
	  
		  public TD(Vertex v0, Vertex v1, Vertex v2,int d)
		  {
			  this.t		= new Triangle(v0,v1,v2);
			  this.depth	= d;
		  }
	  };

	  /// truangulate and store a sphere
	  public  void AddSphere(Vertex ctr,float r) 
	  {
	    // default materials (in case material not specified)
	    if(matgroups.isEmpty())
	    	//push_back
	      matgroups.push(new MaterialGroup(new Color(1,1,1),0.7f,0.0f,32,0,1));

		if (!tesselateSpheres)
		{
			matgroups.peek().spheres.push(new Sphere(ctr,r));
		}
		else
		{
			GenericStack<Triangle> dest = matgroups.peek().geometry;
			Stack<TD> stack = new Stack<TD>();

	    // generate octahedron faces
			
			Vertex _1 = new Vertex();
			Vertex _2 = new Vertex();
			Vertex _3 = new Vertex();
			Vertex _4 = new Vertex();
			Vertex _5 = new Vertex();
			Vertex _6 = new Vertex();
			Vertex _7 = new Vertex();
			
			_2.v.x=1;
			_3.v.x=-1;
			_4.v.y=1;
			_5.v.y=-1;
			_6.v.z=1;
			_7.v.z=-1;
			
	    Vertex[] v = { _1,_2,_3,_4,_5,_6,_7 };

	    stack.push(new TD(v[1],v[3],v[5],0));
	    stack.push(new TD(v[3],v[1],v[6],0));
	    stack.push(new TD(v[4],v[1],v[5],0));
	    stack.push(new TD(v[1],v[4],v[6],0));
	    stack.push(new TD(v[3],v[2],v[5],0));
	    stack.push(new TD(v[2],v[3],v[6],0));
	    stack.push(new TD(v[2],v[4],v[5],0));
	    stack.push(new TD(v[4],v[2],v[6],0));

	    while(!stack.empty()){
	      TD c = stack.pop();
	      //Vertex[] 
	      v = c.t.vertices;

	      if(c.depth>2) {
	        // generate triangle
	        for(int i=0; i<3; i++)
	        {
	        	v[i] = (v[i].times(r)).plus(ctr);
	        }
	        //push_back
	        dest.push(c.t);
	      }
	      else
	      {
	        // subdivide
	        Vertex v01 = (v[0].plus(v[1])).normalize();
	        Vertex v12 = (v[1].plus(v[2])).normalize();
	        Vertex v20 = (v[2].plus(v[0])).normalize();
	        stack.push(new TD(v[0],v01,v20,c.depth+1));
	        stack.push(new TD(v[1],v12,v01,c.depth+1));
	        stack.push(new TD(v[2],v20,v12,c.depth+1));
	        stack.push(new TD(v01,v12,v20,c.depth+1));
	      }
	    }
		}
	  }

	  public  void PolyPatch(List<Vertex> vertices, List<Vertex> normals)
	  {
		  
		  if(vertices.size()<3) return;
		  
		  //areaLightMode=false;
		  if (areaLightMode)
		  {

			  _triangulateInto(lightgroups.peek().geometry, vertices);
		  }
		  else
		  {
			  if(matgroups.isEmpty())
				  matgroups.push(new MaterialGroup(new Color(1,1,1),0.7f,0.0f,32,0,1));

			  _triangulateInto(matgroups.peek().geometry,vertices);		  
		  }
		  
	  }

	  public  void AreaLightBegin(Color intensity, Vertex atten)
	  {
		  lightgroups.push(new LightGroup(intensity, atten));
		  areaLightMode = true;
	  }

	  public  void AreaLightEnd()
	  {
		  areaLightMode = false;
	  }

	  protected void _triangulateInto(GenericStack<Triangle> dest, List<Vertex> vertices)
	  {
		  
		  if(vertices.size()>4)
		  {
			  System.out.println("Cannot triangulate polygons with more than 4 vertices.");
			  System.exit(2);
		  }

		  if(vertices.size()==3)
		  {
			  dest.push(new Triangle(vertices.get(0),vertices.get(1),vertices.get(2)));
			  return;
		  }

	    // split along the shorter diagonal
	    float d1 = vertices.get(0).sqrdist(vertices.get(2));
	    float d2 = vertices.get(1).sqrdist(vertices.get(3));
	    if(d1<d2) {
	    	//push_back
	      dest.push(new Triangle(vertices.get(0), vertices.get(1), vertices.get(2) ));
	      dest.push(new Triangle(vertices.get(0), vertices.get(2), vertices.get(3) ));
	    }
	    else {

	    	//push_back
	      dest.push(new Triangle(vertices.get(0), vertices.get(1), vertices.get(3) ));
	      dest.push(new Triangle(vertices.get(1), vertices.get(2), vertices.get(3) ));
	    }
	    

	  }
    
}