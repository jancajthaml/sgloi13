package main;

import java.awt.Graphics;
import java.awt.Graphics2D;
import java.awt.GridLayout;
import java.awt.image.BufferedImage;

import javax.swing.JFrame;
import javax.swing.JPanel;

import struct.Vertex;

public class Test {

    

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
		frame.setSize(400,400);

		Main.sglCreateContext(400, 400);
		Main.sglClearColor(0,0,0,1);

		Vertex v1 = new Vertex(10,10);
		Vertex v2 = new Vertex(100,100);
		
		Main.sglClear();
		Main.sglColor3f(0, 1, 0);
		Main.sglDrawLine(v1, v2);
		
		
		frame.setVisible(true);
		
		frame.setDefaultCloseOperation(JFrame.EXIT_ON_CLOSE);		
	}
	
	
	static void draw()
	{
		
	}

}
