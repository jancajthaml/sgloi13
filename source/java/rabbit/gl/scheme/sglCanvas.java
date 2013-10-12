package rabbit.gl.scheme;

import java.awt.Canvas;
import java.awt.Color;
import java.awt.Graphics;
import java.awt.Image;
import java.awt.Toolkit;
import java.awt.image.BufferStrategy;
import rabbit.gl.engine.HUB;

@SuppressWarnings("serial")
public class sglCanvas extends Canvas
{

	private Image img							= createImage(HUB.sglGetColorBufferPointer());
	private long currentUpdateSpeed				= 30;
	private long oldTime						= System.nanoTime();
	private long timeSinceLastFPSCalculation	= 0;
	private int frames							= 0;
	private int fps								= 0;
	private boolean repainting					= false;
	private int counter							= 0;
	private BufferStrategy strategy				= null;
    private boolean loop						= false;
    
	public void addNotify()
	{
	    super.addNotify();
	    setIgnoreRepaint(true);
	    createBufferStrategy(2);
		strategy = getBufferStrategy();
	}
	
	public final void paint(Graphics g)
	{}
	
	public final void update(Graphics g)
	{ super.paint(g); }
	
	public final void invalidate()
	{}
	
	public final void revalidate()
	{}
	
	public final void validate(){ }
	
	public final boolean isValid()
	{ return true; }
	
	public void paint()
	{}
	
	public final void repaint()
	{
		if(repainting) return;
		
		repainting	= true;
		
		long nanoTimeAtStartOfUpdate = System.nanoTime();

        update();
        Graphics g 	=  strategy.getDrawGraphics();
        paint();
    			
        g.drawImage(img, 0,0,null);
        
        g.setColor(Color.white);
        g.drawString("FPS: "+(fps), 10, 20);
        g.drawString("counter: "+(++counter), 10, 40);
    		
        if (!strategy.contentsLost()) strategy.show();
        
        Toolkit.getDefaultToolkit().sync();
        cool(nanoTimeAtStartOfUpdate);
        
        repainting=false;
        if(loop) repaint();
	}
	
	public void cool(long nanoTimeCurrentUpdateStartedOn)
	{
		long currentUpdateSpeed = getCurrentWaitTimeBetweenUpdates();
		if (currentUpdateSpeed > 0)
		{
			long timeToSleep = currentUpdateSpeed - ((System.nanoTime() - nanoTimeCurrentUpdateStartedOn) / 10000000);
			
			if (timeToSleep <= 0) return;
			
			try { Thread.sleep(timeToSleep); }
			catch (InterruptedException e) { Thread.currentThread().interrupt(); }
		}
	}

	public void enterMainLoop()
	{
		loop=true;
		repaint();
	}
        
	public void leaveMainLoop()
	{ loop=false; }
        
	public synchronized void resetTimeOfLastUpdate()
	{ oldTime = System.nanoTime(); }

	public synchronized long getCurrentWaitTimeBetweenUpdates()
	{ return currentUpdateSpeed; }

	public synchronized void update()
	{
		if (timeSinceLastFPSCalculation >= 1000000000)
		{
			fps = frames;
			timeSinceLastFPSCalculation = timeSinceLastFPSCalculation - 1000000000;
			frames = 0;
		}

		long elapsedTime = System.nanoTime() - oldTime;
		oldTime = oldTime + elapsedTime;
		timeSinceLastFPSCalculation = timeSinceLastFPSCalculation + elapsedTime;
		repaint();
		frames++;
	}

	public void setUpdateDelay(int delay)
	{ currentUpdateSpeed = delay; }
}