package rabbit.gl.io;

import java.io.File;
import java.io.FileNotFoundException;

public class ObjectLoader {


	public static NFFStore load(String filename)
	{
		int i = filename.lastIndexOf('.');
		String ext = (i > 0)?filename.substring(i+1):"";
		
		if(ext.equalsIgnoreCase("nff"))	return load_nff(filename);
		else throw new IllegalArgumentException("Unsupported format type \""+ext+"\"");
	}
	
	private static NFFStore load_nff(String filename)
	{
		System.out.println("LOADING NFF["+filename+"]");
		
		//Sphere -> triangles OK
		//Sphere -> primitive FAILED
		
		NFFStore store = new NFFStore(true);
		
		try
		{ NFFRead.read(new File(filename),store); }
		catch (FileNotFoundException e) { e.printStackTrace(); }
		
		System.out.println("FINISHED NFF["+filename+"]");
		
		return store;
	}

}
