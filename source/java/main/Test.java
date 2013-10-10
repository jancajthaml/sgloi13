package main;

import java.awt.Graphics;
import java.awt.Graphics2D;
import java.awt.GridLayout;
import java.awt.image.BufferedImage;

import javax.swing.JFrame;
import javax.swing.JPanel;

import struct.Vertex;
import static main.Main.sglBegin;
import static main.Main.sglVertex2f;
import static main.Main.sglEnd;
import static main.Main.sglPushMatrix;
import static main.Main.sglPopMatrix;
import static main.Main.sglScale;
import static main.Main.sglRotate2D;
import static main.Main.sglTranslate;
import static main.Main.sglColor3f;
import static main.Main.sglClear;

import static main.Main.sglMatrixMode;
import static main.Main.sglLoadIdentity;
import static main.Main.sglViewport;
import static main.Main.sglOrtho;
import static main.Main.sglCreateContext;
import static main.Main.sglClearColor;
import static main.Main.sglEllipse;
import static main.Main.sglArc;
import static main.Main.sglAreaMode;

import static type.sglEMatrixMode.SGL_PROJECTION;
import static type.sglEMatrixMode.SGL_MODELVIEW;

import static type.sglEElementType.SGL_LINES;
import static type.sglEAreaMode.SGL_LINE;

import static type.sglEElementType.SGL_LINE_LOOP;

public class Test {

   static int w = 800;
   static int h = 600;

	static JPanel component = new JPanel()
	{
	
		public void paintComponent(Graphics t)
		{
			Graphics2D g = (Graphics2D)t;
			g.drawImage(Main.sglGetColorBufferPointer(),null,0,0);
		}
	};
	
	public static void main(String[] args)
	{
		JFrame frame = new JFrame();
		frame.setLayout(new GridLayout(1,1));
		frame.add(component);
		frame.setSize(w,h);

		sglCreateContext(w, h);
		sglClearColor(0,0,0,1);

		sglClear();
		sglColor3f(0, 1, 0);
		  sglAreaMode(SGL_LINE);

		  // set viewport
		  sglViewport(0,0,w>>2,h>>2);

		  // set the projection matrix
		  sglMatrixMode(SGL_PROJECTION);
		  sglLoadIdentity();
		  sglOrtho(-8, 8, -6, 6, -1, 1);

		  // set the modelview matrix
		  sglMatrixMode(SGL_MODELVIEW);
		  sglLoadIdentity();


		  // draw coord axes
		  sglColor3f(0.5f,0.5f,0.5f);
		  sglBegin(SGL_LINES);
		  sglVertex2f(-7,0);
		  sglVertex2f(7,0);
		  sglVertex2f(0,-5);
		  sglVertex2f(0,5);
		  sglEnd();

		  // BOX
		  sglColor3f(1,1,1);
		  box();

		  sglColor3f(0,1,0);
		  sglTranslate(-3,0,0);
		  box();

		  sglColor3f(1,0,0);
		  sglTranslate(-3,0,0);
		  sglRotate2D((float)(Math.PI*1/8),0,0);
		  box();

		  sglColor3f(0,0,1);
		  sglLoadIdentity();
		  sglTranslate(3,0,0);
		  sglScale(0.5f,2,1);
		  box();

		  sglColor3f(0,1,1);
		  sglLoadIdentity();
		  sglTranslate(6,0,0);
		  sglRotate2D((float)(Math.PI*1/8),0,0);
		  sglScale(0.5f,2,1);
		  box();

		  // ARC
		  sglLoadIdentity(); sglTranslate(0,4,0);

		  sglColor3f(1,1,1);
		  sglArc(0,0,0,1,0,4);

		  sglColor3f(0,1,0);
		  sglTranslate(-3,0,0);
		  sglArc(0,0,0,1,0,4);

		  sglColor3f(1,0,0);
		  sglTranslate(-3,0,0);
		  sglRotate2D((float)(Math.PI*1/8),0,0);
		  sglArc(0,0,0,1,0,4);

		  sglColor3f(0,0,1);
		  sglLoadIdentity(); sglTranslate(0,4,0);
		  sglTranslate(3,0,0);
		  sglScale(0.5f,2,1);
		  sglArc(0,0,0,1,0,4);

		  sglColor3f(0,1,1);
		  sglLoadIdentity(); sglTranslate(0,4,0);
		  sglTranslate(6,0,0);
		  sglRotate2D((float)(Math.PI*1/8),0,0);
		  sglScale(0.5f,2,1);
		  sglArc(0,0,0,1,0,4);

		  // ELLIPSE
		  sglLoadIdentity(); sglTranslate(0,-4,0);

		  sglColor3f(1,1,1);
		  sglEllipse(0,0,0,1.5f,0.7f);

		  sglColor3f(0,1,0);
		  sglTranslate(-3,0,0);
		  sglEllipse(0,0,0,1.5f,0.7f);

		  sglColor3f(1,0,0);
		  sglTranslate(-3,0,0);
		  sglRotate2D((float)(Math.PI*1/8),0,0);
		  sglEllipse(0,0,0,1.5f,0.7f);

		  sglColor3f(0,0,1);
		  sglLoadIdentity(); sglTranslate(0,-4,0);
		  sglTranslate(3,0,0);
		  sglScale(0.5f,2,1);
		  sglEllipse(0,0,0,1.5f,0.7f);

		  sglColor3f(0,1,1);
		  sglLoadIdentity(); sglTranslate(0,-4,0);
		  sglTranslate(6,0,0);
		  sglRotate2D((float)(Math.PI*1/8),0,0);
		  sglScale(0.5f,2,1);
		  sglEllipse(0,0,0,1.5f,0.7f);
		  
		//branch(10,1.2f);
		//box();
		
		frame.setVisible(true);
		
		frame.setDefaultCloseOperation(JFrame.EXIT_ON_CLOSE);		
	}
	
	static void branch(int depth,float rot)
	{
	  sglPushMatrix();
	  for(int i=0; i<3; i++) {
	    sglScale(0.87f,0.87f,1);
	    sglRotate2D(rot*0.15f,0,1);
	    sglTranslate(0,2,0);
	    box();
	  }
	  if(depth<3) {
	    branch(depth+1,1);
	    sglColor3f(0,1,0);
	    branch(depth+1,-1);
	  }
	  sglPopMatrix();
	}
	static void box()
	{
		
		sglBegin(SGL_LINE_LOOP);
		  sglVertex2f(-1,-1);
		  sglVertex2f(-1,1);
		  sglVertex2f(1,1);
		  sglVertex2f(1,-1);
		  sglEnd();
		  sglBegin(SGL_LINES);
		  sglVertex2f(0,-1);
		  sglVertex2f(0,1);
		  sglEnd();
	}
	
	
	static void draw()
	{
		
	}

}
