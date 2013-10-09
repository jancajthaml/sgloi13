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
import static main.Main.sglClear;

import static main.Main.sglViewport;
import static main.Main.sglCreateContext;
import static main.Main.sglClearColor;

public class Test {

   static int w = 800;
   static int h = 800;

	static int scene = 0;
	
   static int repaint = 0;
	@SuppressWarnings("serial")
	static JComponent component = new JComponent()
	{
	
		Image img = createImage(Main.sglGetColorBufferPointer());
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
			Main.sglGetColorBufferPointer().newPixels(0, 0, w, h, true);

			Test.repaint();
			super.repaint();
		}
		
		public void revalidate()
		{
			super.invalidate();
			super.validate();
			//Rectangle bounds = super.getBounds();
			w=Math.max(1, getWidth());
			h=Math.max(1, getHeight());
			
			main.Main.sglDestroyContext(current_context);
			current_context=sglCreateContext(w, h);
			sglViewport(60,60,w,h);

			this.repaint();		
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
					repaint();
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
		
		sglClearColor(0,0,0,1);

		//set viewport
		sglViewport(60,60,w,h);
		//branch(10,1.2f);
		//box();
		
		frame.setVisible(true);
		
		frame.setDefaultCloseOperation(JFrame.EXIT_ON_CLOSE);
		
		SwingUtilities.invokeLater(new Runnable(){public void run()
		{
			component.validate();
			
			//System.out.println("done");
		}});
		
		boolean benchmark = true;
		
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
	
	
	static void repaint()
	{
	//	System.out.println(scene);
		sglClear();
		
		//for(int i=0; i<1000; i++)
		switch(scene)
		{
			case 0 : Test1.DrawTestScene1A(w, h); break;
			case 1 : Test0.DrawTestScene0B(h); break;
			case 2 : Test0.DrawTestScene0A(h); break;
			case 3 : Test1.DrawTestScene1B(); break;
			
		}
		//Main.sglTranslate(10,10,1);
		
		//Test1.DrawTestScene1A(w>>1,h>>1);
	//	test1();
	}

}
