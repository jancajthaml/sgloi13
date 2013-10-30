package rabbit.gl.scheme;

import java.awt.Color;
import java.awt.Graphics;
import javax.swing.JPanel;
import main.SGLTest;
import rabbit.gl.engine.HUB;

@SuppressWarnings("serial")
public class sglCanvas extends JPanel
{
	private long     currentUpdateSpeed  =  30;
	private boolean  repainting          =  false;
	private float    time                = 0;
    private boolean  loop                = false;
    
	public void addNotify()
	{
	    super.addNotify();
	    setIgnoreRepaint(true);
	}
	
	public final void paint(Graphics g)
	{}
	
	public final void update(Graphics g)
	{ super.paint(g); }
	
	public final void invalidate()
	{}
	
	public final void revalidate()
	{}
	
	public final void validate()
	{}
	
	public final boolean isValid()
	{ return true; }
	
	public void paint()
	{}
	
	public void paintComponent(Graphics g)
	{
		try
		{
			long t = System.nanoTime();
			
			paint();
			g.setClip(getBounds());
			g.drawImage(HUB.sglGetColorBufferPointer(), 0,0,null);
			
			time = (float) ((System.nanoTime()-t)/1000000000.0);
			
			g.setColor(Color.white);
			g.drawString("duration: "+time+"s", 10, 20);
			if(!SGLTest.movement.equals(""))
				g.drawString("moving: "+SGLTest.movement, 10, 40);
		}catch(Throwable t){}
	}
	
	public final void repaint()
	{
		if(repainting) return;
		
		repainting	= true;
		
        this.paintComponent(this.getGraphics());
        
        //Toolkit.getDefaultToolkit().sync();
        
        try { Thread.sleep(currentUpdateSpeed); }
		catch (InterruptedException e) { Thread.currentThread().interrupt(); }
        
        repainting=false;
        if(loop) repaint();
	}

	public void enterMainLoop()
	{
		loop=true;
		repaint();
	}
        
	public void leaveMainLoop()
	{ loop=false; }

	public void setUpdateDelay(int delay)
	{ currentUpdateSpeed = delay; }
}