package main;

import java.awt.GridLayout;
import java.awt.event.ComponentAdapter;
import java.awt.event.ComponentEvent;
import java.awt.event.KeyEvent;
import java.awt.event.KeyListener;
import javax.swing.JFrame;
import javax.swing.SwingUtilities;

import rabbit.gl.helpers.Movement;
import rabbit.gl.io.NFFStore;
import rabbit.gl.io.ObjectLoader;
import rabbit.gl.io.TestNFF;
import rabbit.gl.scheme.sglCanvas;
import rabbit.gl.type.sglEClearBit;
import tests.Test1;
import tests.Test2;
import tests.TestAnimation;
import static rabbit.gl.engine.HUB.sglClear;
import static rabbit.gl.engine.HUB.sglClearColor;
import static rabbit.gl.engine.HUB.sglCreateContext;
import static rabbit.gl.type.sglEClearBit.SGL_COLOR_BUFFER_BIT;

public class SGLTest
{
	static int current_context = 0;
	static NFFStore store = null;
	static int w = 800;
	static int h = 600;
   
	static int scene = 7;
	public static String movement = "";
	static int repaint = 0;

	static boolean LEFT  = false;
	static boolean RIGHT = false;
	static boolean UP    = false;
	static boolean DOWN  = false;

	@SuppressWarnings("serial")
	static sglCanvas component = new sglCanvas()
	{
		{
			this.addComponentListener
			(
				new ComponentAdapter()
				{
					@Override public void componentResized(ComponentEvent e)
					{
						w=getWidth();
						h=getHeight();
						current_context=sglCreateContext(w, h);
					}
				}
			);
		}

		public void paint()
		{
			sglClear(SGL_COLOR_BUFFER_BIT);
			sglClear(sglEClearBit.SGL_DEPTH_BUFFER_BIT);
			
			switch(scene)
			{
				case 0 : TestAnimation.DrawAnimation(w,h)	; return;
				
				case 1 : Test1.DrawTestScene1A(w, h)		; return;
				case 2 : Test1.DrawTestScene1B()			; return;
				case 3 : Test1.DrawTestScene1C()			; return;
				
				case 4 : Test2.DrawTestScene2A(w, h)		; return;
				case 5 : Test2.DrawTestScene2B(w, h)		; return;
				case 6 : Test2.DrawTestScene2C(w, h)		; return;
				case 7 : TestNFF.draw(w, h, store)			; return;
				
			}
		}
		
		{
			
			//store=ObjectLoader.load("./cornell-blocks.nff");	//<--- OK
			store=ObjectLoader.load("./cornell-spheres.nff");	//<--- OK
			//store=TestNFF.load("./butan.nff");	//<--- FAILED
			
			this.setFocusable(true);
			
			this.addKeyListener(new KeyListener(){
				@Override public void keyPressed(KeyEvent e)
				{
					switch(e.getKeyCode())
					{
						case KeyEvent.VK_LEFT   :
						{
							if(UP)         { Movement.leftUp(); movement = "LEFT-UP"   ; }
							else if(DOWN)  { Movement.leftDown();movement = "LEFT-DOWN" ; }
							else if(RIGHT) { movement = ""          ; }
							else           { Movement.left(); movement = "LEFT"      ; }
							LEFT=true;
						}
						break;
						
						case KeyEvent.VK_RIGHT  :
						{
							if(UP)        { Movement.rightUp(); movement = "RIGHT-UP";   }
							else if(DOWN) { Movement.rightDown(); movement = "RIGHT-DOWN"; }
							else if(LEFT) { movement = "";           }
							else          { Movement.right(); movement = "RIGHT"    ;  }
							RIGHT=true;
						}
						break;
						
						case KeyEvent.VK_UP     :
						{
							if(LEFT)       { Movement.leftUp(); movement = "LEFT-UP";  }
							else if(RIGHT) { Movement.rightUp(); movement = "RIGHT-UP"; }
							else if(DOWN)  { movement = "";         }
							else           { Movement.up(); movement = "UP"    ;   }
							UP=true;
						}
						break;
						
						case KeyEvent.VK_DOWN   :
						{
							if(LEFT)       { Movement.leftDown(); movement = "LEFT-DOWN";  }
							else if(RIGHT) { Movement.rightDown(); movement = "RIGHT-DOWN"; }
							else if(UP)    { movement = "";           }
							else           { Movement.down(); movement = "DOWN"    ;   }
							DOWN=true;
						}
						break;
					}
					repaint();
				}

				@Override public void keyReleased(KeyEvent e)
				{
					movement = "";
					
					switch(e.getKeyCode())
					{
						case KeyEvent.VK_UP    : UP    = false; break;
						case KeyEvent.VK_DOWN  : DOWN  = false; break;
						case KeyEvent.VK_LEFT  : LEFT  = false; break;
						case KeyEvent.VK_RIGHT : RIGHT = false; break;
					}
					if(UP)    movement = "UP";
					if(DOWN)  movement = "DOWN";
					if(LEFT)  movement = "LEFT";
					if(RIGHT) movement = "RIGHT";
					
					switch(e.getKeyCode())
					{
						case KeyEvent.VK_0	:	scene=0; break;
						case KeyEvent.VK_1	:	scene=1; break;
						case KeyEvent.VK_2	:	scene=2; break;
						case KeyEvent.VK_3	:	scene=3; break;
						case KeyEvent.VK_4	:	scene=4; break;
						case KeyEvent.VK_5	:	scene=5; break;
						case KeyEvent.VK_6	:	scene=6; break;
						case KeyEvent.VK_7  :   scene=7; break;
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
		
		sglClearColor(0,0,0,1);

		//set viewport
		 		
		//branch(10,1.2f);
		//box();
		
		frame.setVisible(true);
		
		frame.setDefaultCloseOperation(JFrame.EXIT_ON_CLOSE);
		
		component.setUpdateDelay(30);
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
	

}
