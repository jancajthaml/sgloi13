package rabbit.gl.io;

import java.io.File;
import java.io.FileInputStream;
import java.io.FileNotFoundException;
import java.util.LinkedList;
import fio.Fio;
import rabbit.gl.math.SimpleMath;
import rabbit.gl.struct.Color;
import rabbit.gl.struct.Vertex;

public class NFFRead
{
	
	public static void main(String ... args)
	{
		NFFStore store = new NFFStore(false);
		try
		{ read(new File("./cornell-blocks.nff"),store); }
		catch (FileNotFoundException e) { e.printStackTrace(); }
	}

	public static void read(File fin, NFFCallbacks callbacks) throws FileNotFoundException
	{
        Fio fio = new Fio(new FileInputStream(fin));
        try
        {
		  String nexttok	=  "";
	
		  reader:while( (nexttok=fio.nextLine())!=null )
		  {
			  if(nexttok.length()==0 || nexttok.startsWith("#"))
			  {
				  continue;
			  }

		    switch(nexttok.charAt(0))
		    {
		    
		    case 'v': 
		      // ------------------------------
		      //  'v' camera
		      // ------------------------------
		      {
		    
		    	  
		    	  Vertex from = new Vertex();
		    	  Vertex at = new Vertex();
		    	  Vertex up = new Vertex();
		    	  float    angle  = 0;
		    	  float    hither = 0;
		    	  int      width  = 0;
		    	  int height      = 0;

		    	  fio.next();
		    	  from.x=Float.valueOf(fio.next());
		        from.y=Float.valueOf(fio.next());
		        from.z=Float.valueOf(fio.next());
		        
		        fio.next();
		        
		        at.x=Float.valueOf(fio.next());
		        at.y=Float.valueOf(fio.next());
		        at.z=Float.valueOf(fio.next());

		        fio.next();
		        
		        up.x=Float.valueOf(fio.next());
		        up.y=Float.valueOf(fio.next());
		        up.z=Float.valueOf(fio.next());
		        
		        fio.next();
		        angle=Float.valueOf(fio.next());
		        
		        fio.next();
		        
		        hither=Float.valueOf(fio.next());
		        
		        fio.next();
		        
		        width=Integer.valueOf(fio.next());
		        height=Integer.valueOf(fio.next());
		     
		        callbacks.Camera(from,at,up,angle,hither,width,height);
		        
		    	  
		      }
		      break;

		    case 'b':
		      // ------------------------------
		      //  'b' background
		      // ------------------------------
		      {
		    	  fio.back();
		        Color col = new Color();
		        col.r=Float.valueOf(fio.next());
		        col.g=Float.valueOf(fio.next());
		        col.b=Float.valueOf(fio.next());
		        callbacks.Background(col);
		        
		      }
		      break;

		    case 'l':
		      // ------------------------------
		      //  'l' point light
		      // ------------------------------
		      {
		    	  
		    	  
		    	  String[] L = nexttok.split(" ");
		    	  fio.back();
		        Vertex pos = new Vertex();
		        Color i = new Color();

		        pos.x=Float.valueOf(fio.next());
		        pos.y=Float.valueOf(fio.next());
		        pos.z=Float.valueOf(fio.next());
		        if(L.length<5)
		        	i.r = i.g = i.b = 1.0f;
		        else
		        {
		        	i.r=Float.valueOf(fio.next());
		        	i.g=Float.valueOf(fio.next());
		        	i.b=Float.valueOf(fio.next());
		        }
//		        ret = fscanf(fin," %g %g %g %g %g %g\n",
	//	          &(pos.x),&(pos.y),&(pos.z),&(i.r),&(i.g),&(i.b));


		        callbacks.AddPointLight(pos,i);
		      }
		      break;

			case 'B':
		      // ------------------------------
		      //  'B' HDR env map
		      // ------------------------------
		      {
		    	  System.out.println("HDR evn map");
				//char[] s = new char[256];
		    	  
		    	 String s=fio.next();
		        //ret = fscanf(fin," %s\n", s);
		        //if ( ret != 1 ) {
		        	//System.err.println("Could not read env map name.");
		          //return -1;
		        //}
				
		        callbacks.HDRBackground(s);
		        
		      }
		      break;

			case 'L':
		      // ------------------------------
		      //  'L' emmisive material (area lights)
		      // ------------------------------
		      {
		    	  System.out.println("L emmisive material");
		        Color i = new Color();
		        Vertex atten = new Vertex();
				float m = 0.0f;
				
				i.r = fio.nextDecimal();
				i.g = fio.nextDecimal();
				i.b = fio.nextDecimal();
				
				m = fio.nextDecimal();
				
				if(fio.next().contains("a"))
				{					
					atten.x = fio.nextDecimal();
					atten.y = fio.nextDecimal();
					atten.z = fio.nextDecimal();

					i.r *= m/SimpleMath.PI;
					i.g *= m/SimpleMath.PI;
					i.b *= m/SimpleMath.PI;
					
					callbacks.AreaLightBegin(i, atten);
				}
				else
				{
					atten.x = 0;
					atten.y = 0;
					atten.z = 0;
					
					
					i.r *= m/SimpleMath.PI;
					i.g *= m/SimpleMath.PI;
					i.b *= m/SimpleMath.PI;

					if (i.r == 0 && i.g == 0 && i.b == 0 && m == 0)
					  callbacks.AreaLightEnd();
					else
					  callbacks.AreaLightBegin(i, atten);
				}
		        /*
				ret = fscanf(fin," %g %g %g %g atten %g %g %g\n",
							 &(i.r),&(i.g),&(i.b),
							 &m,
							 &atten.x, &atten.y, &atten.z
							 );
*/
	//	        if ( ret != 4 && ret != 7 ) {
		//        	System.err.println("Could not read area light.");
		  //        return -1;
		    //    }

				System.out.println("DONE\n");
		      }
		      break;

		    case 'f': 
		      // ------------------------------
		      //  'f' phong material
		      // ------------------------------
		      {
		    	  
		        Color col = new Color();
		        float kd = 0.0f;
		        float ks = 0.0f;
		        float shine = 0.0f;
		        float T = 0.0f;
		        float ior = 0.0f;

		        fio.back();
		        col.r = Float.valueOf(fio.next());
		        col.g = Float.valueOf(fio.next());
		        col.b = Float.valueOf(fio.next());
		        
		        
		        kd = Float.valueOf(fio.next());
		        ks = Float.valueOf(fio.next());
		        shine = Float.valueOf(fio.next());
		        T = Float.valueOf(fio.next());
		        ior = Float.valueOf(fio.next());
		        /*
		        ret = fscanf(fin," %g %g %g %g %g %g %g %g",
		          &(col.r),&(col.g),&(col.b),
		          &(kd),&(ks),&(shine),&(T),&(ior));
*/
	

		        callbacks.SetMaterial(col,kd,ks,shine,T,ior);
		        
		      }
		      break;

		    case 's':
		      // ------------------------------
		      //  's' sphere
		      // ------------------------------
		      {
		    	  
		        Vertex c = new Vertex();
		        float    r = 0.0f;

		        c.x = fio.nextDecimal();
		        c.y = fio.nextDecimal();
		        c.z = fio.nextDecimal();
		        
		        r = fio.nextDecimal();
		        //ret = fscanf(fin, " %g %g %g %g\n", &(c.x), &(c.y), &(c.z), &r);

		        callbacks.AddSphere(c,r);
		        
		      }
		      break;

		    case 'p': 
		      {
		    	  
		        // ------------------------------
		        //  'p'/'pp' polygon/patch
		        // ------------------------------
		        boolean patch	= nexttok.charAt(1)=='p';
		        int num			= 0;
		        
		        LinkedList<Vertex> vertices	= new LinkedList<Vertex>();
		        LinkedList<Vertex> normals	= new LinkedList<Vertex>();

		        fio.back();
		        
		        num = Integer.valueOf(fio.next());
		        
		        
		        //ret = fscanf(fin, " %d\n", &num);

		        //if ( ret !=  1 ) {
		        	//System.err.println("Could not read number of polygon / patch vertices.");
		          //return -1;
		        //}

		        for(int i=0; i<num; i++)
		        {

			        Vertex  vertex	= new Vertex();
			        Vertex normal		= new Vertex();
		        	vertex.x = Float.valueOf(fio.next());
		        	vertex.y = Float.valueOf(fio.next());
		        	vertex.z = Float.valueOf(fio.next());
		          //ret = fscanf(fin, " %g %g %g", &(vertex.x),&(vertex.y),&(vertex.z) );
		          //if(ret != 3 ) {
		        	//  System.err.println("Could not read polygon / patch vertex #"+i+".");
		            //return -1;
		          //}
		        //	System.out.println(i+"/"+num+" --> "+vertex);
		          vertices.addLast(vertex);

		          if(!patch) continue;
		          
		          normal.x = Float.valueOf(fio.next());
		          normal.y = Float.valueOf(fio.next());
		          normal.z = Float.valueOf(fio.next());
		          
		          //ret = fscanf(fin, " %g %g %g", &(normal.x),&(normal.y),&(normal.z) );
		          //if(ret != 3 ) {
		        	//  System.err.println("Could not read polygon / patch normal #"+i+".");
		            //return -1;
		          //}
		          normals.addLast(normal);
		        }
		        //FIXME
		        if( num > 0 )	callbacks.PolyPatch(vertices, normals);
		        
		      }
		      break;

		    	default:
		    	System.err.println("Unknown nff command: "+nexttok);
		    	break reader;

		    }
		    
		  }
		  
        }
        catch(Throwable t)
        {
        	
        }
        finally
        { fio.close();}
	}
}
