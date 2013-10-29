package main;

import java.awt.GridLayout;
import java.awt.event.KeyEvent;
import java.awt.event.KeyListener;
import javax.swing.JFrame;
import javax.swing.SwingUtilities;

import rabbit.gl.engine.HUB;
import rabbit.gl.scheme.sglCanvas;
import rabbit.gl.type.sglEClearBit;
import tests.Test1;
import tests.Test2;
import tests.TestAnimation;
import static rabbit.gl.engine.HUB.sglClear;
import static rabbit.gl.engine.HUB.sglClearColor;
import static rabbit.gl.engine.HUB.sglCreateContext;
import static rabbit.gl.engine.HUB.sglViewport;
import static rabbit.gl.type.sglEClearBit.SGL_COLOR_BUFFER_BIT;

public class SGLTest
{

	static int current_context = 0;
	
   static int w = 800;
   static int h = 600;
   
   static int scene = 0;
   public static String movement = "";
   static int repaint = 0;

   static boolean LEFT  = false;
   static boolean RIGHT = false;
   static boolean UP    = false;
   static boolean DOWN  = false;

	@SuppressWarnings("serial")
	static sglCanvas component = new sglCanvas()
	{
	
		public void paint()
		{
			sglClear(SGL_COLOR_BUFFER_BIT);
			sglClear(sglEClearBit.SGL_DEPTH_BUFFER_BIT);
			
			switch(scene)
			{
				case 0 : TestAnimation.DrawAnimation(800,600); return;
				case 1 : Test1.DrawTestScene1A(800, 600)	; return;
				//case 1 : Test0.DrawTestScene0B(s)			; return true;
				//case 2 : Test0.DrawTestScene0A(s)			; return true;
				case 2 : Test1.DrawTestScene1B()			; return;
				case 3 : Test1.DrawTestScene1C()			; return;
				
				case 4 : Test2.DrawTestScene2A(800,600)		; return;
				case 5 : Test2.DrawTestScene2B(800,600)		; return;
				case 6 : Test2.DrawTestScene2C(800,600)		; return;
				
			}
		}
		
		{
			
			this.setFocusable(true);
			
			this.addKeyListener(new KeyListener(){
				@Override public void keyPressed(KeyEvent e)
				{
					switch(e.getKeyCode())
					{
						case KeyEvent.VK_LEFT   :
						{
							if(UP)        { movement = "LEFT-UP";   }
							else if(DOWN) { movement = "LEFT-DOWN"; }
							else          { movement = "LEFT"    ;  }
							LEFT=true;
						}
						break;
						
						case KeyEvent.VK_RIGHT  :
						{
							if(UP)        { movement = "RIGHT-UP";   }
							else if(DOWN) { movement = "RIGHT-DOWN"; }
							else          { movement = "RIGHT"    ;  }
							RIGHT=true;
						}
						break;
						
						case KeyEvent.VK_UP     :
						{
							if(LEFT)       { movement = "LEFT-UP";  }
							else if(RIGHT) { movement = "RIGHT-UP"; }
							else           { movement = "UP"    ;   }
							UP=true;
						}
						break;
						
						case KeyEvent.VK_DOWN   :
						{
							if(LEFT)       { movement = "LEFT-DOWN";  }
							else if(RIGHT) { movement = "RIGHT-DOWN"; }
							else           { movement = "DOWN"    ;   }
							DOWN=true;
						}
						break;
					}
				}

				@Override public void keyReleased(KeyEvent e)
				{
					movement = "";
					LEFT=RIGHT=UP=DOWN=false;
					switch(e.getKeyCode())
					{
						case KeyEvent.VK_0	:	scene=0; break;
						case KeyEvent.VK_1	:	scene=1; break;
						case KeyEvent.VK_2	:	scene=2; break;
						case KeyEvent.VK_3	:	scene=3; break;
						case KeyEvent.VK_4	:	scene=4; break;
						case KeyEvent.VK_5	:	scene=5; break;
						case KeyEvent.VK_6	:	scene=6; break;
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
		
		component.setUpdateDelay(30);
		component.enterMainLoop();
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
