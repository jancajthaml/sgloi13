package main;

import java.awt.GridLayout;
import java.awt.event.KeyEvent;
import java.awt.event.KeyListener;

import javax.swing.JFrame;
import javax.swing.SwingUtilities;

import rabbit.gl.scheme.sglCanvas;
import tests.Test1;

import static rabbit.gl.engine.HUB.sglBegin;
import static rabbit.gl.engine.HUB.sglClear;
import static rabbit.gl.engine.HUB.sglClearColor;
import static rabbit.gl.engine.HUB.sglCreateContext;
import static rabbit.gl.engine.HUB.sglEnd;
import static rabbit.gl.engine.HUB.sglVertex2f;
import static rabbit.gl.engine.HUB.sglViewport;
import static rabbit.gl.type.sglEElementType.SGL_LINES;
import static rabbit.gl.type.sglEElementType.SGL_LINE_LOOP;
import static rabbit.gl.type.sglEClearBit.SGL_COLOR_BUFFER_BIT;

public class SGLTest {


	static int current_context = 0;
	
   static int w = 800;
   static int h = 600;
   
   static int scene = 1;
	
   static int repaint = 0;
   
	@SuppressWarnings("serial")
	static sglCanvas component = new sglCanvas()
	{
	
		public void paint()
		{
			sglClear(SGL_COLOR_BUFFER_BIT);
			
			switch(scene)
			{
				case 1 : Test1.DrawTestScene1A(800, 600)	; return;
				//case 1 : Test0.DrawTestScene0B(s)			; return true;
				//case 2 : Test0.DrawTestScene0A(s)			; return true;
				case 2 : Test1.DrawTestScene1B()			; return;
				case 3 : Test1.DrawTestScene1C()			; return;
				
			}
		}
		
		{
			
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
					repaint();
				}

				@Override public void keyTyped(KeyEvent arg0)
				{
					
				}
				
			});
		}
	};
	
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
		
		component.setUpdateDelay(300);
		//component.enterMainLoop();
		//try {
		
	//for(int i=0; i<200; i++)
	//{
		//component.revalidate();
	//}
		
		SwingUtilities.invokeLater(new Runnable(){
			public void run()
			{
				component.repaint();
			}
		});
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
