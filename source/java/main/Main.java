package main;

import java.awt.Color;
import java.awt.Graphics;
import java.awt.Graphics2D;
import java.awt.GridLayout;
import java.awt.Image;
import java.awt.event.ComponentAdapter;
import java.awt.event.ComponentEvent;
import java.awt.event.KeyEvent;
import java.awt.event.KeyListener;

import javax.swing.JComponent;
import javax.swing.JFrame;
import javax.swing.SwingUtilities;

import rabbit.gl.engine.HUB;
import tests.Test0;
import tests.Test1;

import static rabbit.gl.engine.HUB.sglBegin;
import static rabbit.gl.engine.HUB.sglClear;
import static rabbit.gl.engine.HUB.sglClearColor;
import static rabbit.gl.engine.HUB.sglColor3f;
import static rabbit.gl.engine.HUB.sglCreateContext;
import static rabbit.gl.engine.HUB.sglDestroyContext;
import static rabbit.gl.engine.HUB.sglEnd;
import static rabbit.gl.engine.HUB.sglLoadIdentity;
import static rabbit.gl.engine.HUB.sglRotate2D;
import static rabbit.gl.engine.HUB.sglScale;
import static rabbit.gl.engine.HUB.sglTranslate;
import static rabbit.gl.engine.HUB.sglVertex2f;
import static rabbit.gl.engine.HUB.sglViewport;
import static rabbit.gl.math.SimpleMath.PI;
import static rabbit.gl.math.SimpleMath.cos;
import static rabbit.gl.math.SimpleMath.sin;
import static rabbit.gl.type.sglEElementType.SGL_LINES;
import static rabbit.gl.type.sglEElementType.SGL_LINE_LOOP;


public class Main {

	
   static int w = 800;
   static int h = 600;
   
   static int scene = 0;
	
   static int repaint = 0;
   
	@SuppressWarnings("serial")
	static JComponent component = new JComponent()
	{
	
		Image img = createImage(HUB.sglGetColorBufferPointer());
		public void paintComponent(Graphics t)
		{
			Graphics2D g = (Graphics2D)t;
			
			g.drawImage(img, 0,0,null);

			g.setColor(Color.white);
			g.drawString(""+(++repaint), 20,20);
		}
		
		public void repaint()
		{
			if(!this.isValid() || !this.isVisible()) return;
			//img.getSource().
			
			//HUB.sglGetColorBufferPointer().newPixels(0, 0, w, h, true);

			if(Main.repaint())
				super.repaint();
		}
		
		public void revalidate()
		{
			super.invalidate();
			super.validate();
			//Rectangle bounds = super.getBounds();
			w = Math.max(1, getWidth());
			h = Math.max(1, getHeight());
			
			sglDestroyContext(current_context);
			sglCreateContext(w, h);
			sglViewport(0,0,800,600);
			
			HUB.sglGetColorBufferPointer().newPixels(0, 0, w, h, true);

			if(Main.repaint()) super.repaint();
		
		}
		
		{
			this.addComponentListener(new ComponentAdapter()
			{
				public void componentResized(ComponentEvent e)	{ revalidate();																		}
				public void componentHidden(ComponentEvent e)	{																								}
				public void componentMoved(ComponentEvent e)	{																								}
			});
			
			this.setFocusable(true);
			
			this.addKeyListener(new KeyListener(){
				@Override public void keyPressed(KeyEvent arg0)
				{
					
				}

				@Override public void keyReleased(KeyEvent e)
				{
					switch(e.getKeyCode())
					{
						case KeyEvent.VK_0	:	scene=0; break;
						case KeyEvent.VK_1	:	scene=1; break;
						case KeyEvent.VK_2	:	scene=2; break;
						case KeyEvent.VK_3	:	scene=3; break;
					}
					revalidate();
				}

				@Override public void keyTyped(KeyEvent arg0)
				{
					
				}
				
			});
		}
	};
	
	static int current_context = 0;
	
	public static void main(String[] args)
	{
		JFrame frame = new JFrame();
		frame.setLayout(new GridLayout(1,1));
		frame.add(component);
		frame.setSize(800,600);
		frame.setLocation(100, 100);
		
		current_context=sglCreateContext(w, h);
		sglViewport(0,0,800,600);
		sglClearColor(0,0,0,1);

		//set viewport
		 

		
		//branch(10,1.2f);
		//box();
		
		frame.setVisible(true);
		
		frame.setDefaultCloseOperation(JFrame.EXIT_ON_CLOSE);
		
		SwingUtilities.invokeLater(new Runnable(){public void run()
		{
			component.validate();
			
			//System.out.println("done");
		}});
		
		boolean benchmark = false;
		
		if(benchmark)for(;;)
		{
			try{ Thread.sleep(30); } catch (InterruptedException e) {}
			component.repaint();
		}
		//try {
		
	//for(int i=0; i<200; i++)
	//{
		//component.revalidate();
	//}
	}
	
	static int translate = 0;
	static boolean repaint()
	{
	//	System.out.println(scene);
		sglClear();
		
		switch(scene)
		{
			case 1 : Test1.DrawTestScene1A(800, 600)	; return true;
			//case 1 : Test0.DrawTestScene0B(s)			; return true;
			//case 2 : Test0.DrawTestScene0A(s)			; return true;
			case 2 : Test1.DrawTestScene1B()			; return true;
			case 3 : Test1.DrawTestScene1C()			; return true;
			default : return false;
			
		}
		//Test1.DrawTestScene1A(w>>1,h>>1);
	//	test1();
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
}
