package struct;

import java.awt.*;
import java.awt.image.*;
import java.util.Hashtable;
import java.util.Vector;

import javax.swing.ImageIcon;

public class Buffer extends Image implements WritableRenderedImage, Transparency
{
	ColorModel colorModel;
	WritableRaster raster;
	OffScreenImageSource osis;

	public Buffer(int width, int height)
	{
		//super(1,1,BufferedImage.TYPE_INT_RGB);
		colorModel = new DirectColorModel(24,
                       0x00ff0000,   // Red
                       0x0000ff00,   // Green
                       0x000000ff,   // Blue
                       0x0           // Alpha
                       );
		raster = colorModel.createCompatibleWritableRaster(width, height);
	}

	public ColorModel getColorModel()
	{ return colorModel; }

	public WritableRaster getRaster()
	{ return raster; }

	public WritableRaster getAlphaRaster()
	{ return colorModel.getAlphaRaster(raster); }

	public int getRGB(int x, int y)
	{ return colorModel.getRGB(raster.getDataElements(x, y, null)); }

	public synchronized void setRGB(int x, int y, int rgb)
	{ raster.setDataElements(x, y, colorModel.getDataElements(rgb, null)); }

	public void setRGB(int startX, int startY, int w, int h, int[] rgbArray, int offset, int scansize)
	{
		int yoff  = offset;
		int off;
		Object pixel = null;

		for (int y = startY; y < startY+h; y++, yoff+=scansize)
		{
			off = yoff;
			for (int x = startX; x < startX+w; x++)
			{
				pixel = colorModel.getDataElements(rgbArray[off++], pixel);
				raster.setDataElements(x, y, pixel);
			}
		}
	}

	public int getWidth()
	{ return raster.getWidth(); }

	public int getHeight()
	{ return raster.getHeight(); }

	public int getWidth(ImageObserver observer) { return raster.getWidth(); }

	public int getHeight(ImageObserver observer)
	{ return raster.getHeight(); }

	public ImageProducer getSource()
	{
		if (osis == null) osis = new OffScreenImageSource(this);
		return osis;
	}

	public Object getProperty(String name, ImageObserver observer)
	{ return getProperty(name); }

	public Object getProperty(String name)
	{ return java.awt.Image.UndefinedProperty; }

	public java.awt.Graphics getGraphics()
	{ return null; }

	public String toString()
	{ return new String("BufferedImage@"+Integer.toHexString(hashCode())+": "+colorModel+" "+raster); }

	public Vector<RenderedImage> getSources() { return null; }

	public String[] getPropertyNames() { return null; }

	public int getMinX()
	{ return raster.getMinX(); }

	public int getMinY()
	{ return raster.getMinY(); }

	public SampleModel getSampleModel()
	{ return raster.getSampleModel(); }

	public int getNumXTiles()
	{ return 1; }

	public int getNumYTiles()
	{ return 1; }

	public int getMinTileX()
	{ return 0; }

	public int getMinTileY()
	{ return 0; }

	public int getTileWidth()
	{ return raster.getWidth(); }

	public int getTileHeight()
	{ return raster.getHeight(); }

	public int getTileGridXOffset()
	{ return raster.getSampleModelTranslateX(); }

	public int getTileGridYOffset()
	{ return raster.getSampleModelTranslateY(); }

	public Raster getTile(int tileX, int tileY)
	{
		if (tileX == 0 && tileY == 0)
			return raster;
		
		throw new ArrayIndexOutOfBoundsException("BufferedImages only have one tile with index 0,0");
	}

	public Raster getData()
	{
		// REMIND : this allocates a whole new tile if raster is a
		// subtile.  (It only copies in the requested area)
		// We should do something smarter.
		int width = raster.getWidth();
		int height = raster.getHeight();
		int startX = raster.getMinX();
		int startY = raster.getMinY();
		WritableRaster wr = Raster.createWritableRaster(raster.getSampleModel(), new Point(raster.getSampleModelTranslateX(), raster.getSampleModelTranslateY()));
		Object tdata = null;
		
		for (int i = startY; i < startY+height; i++)
		{
			tdata = raster.getDataElements(startX,i,width,1,tdata);
			wr.setDataElements(startX,i,width,1, tdata);
		}
		return wr;
	}

	public Raster getData(Rectangle rect)
	{
		SampleModel sm = raster.getSampleModel();
		SampleModel nsm = sm.createCompatibleSampleModel(rect.width, rect.height);
		WritableRaster wr = Raster.createWritableRaster(nsm, rect.getLocation());
		
		int width = rect.width;
		int height = rect.height;
		int startX = rect.x;
		int startY = rect.y;

		Object tdata = null;

		for (int i = startY; i < startY+height; i++)
		{
			tdata = raster.getDataElements(startX,i,width,1,tdata);
			wr.setDataElements(startX,i,width,1, tdata);
		}
		return wr;
	}

	public WritableRaster copyData(WritableRaster outRaster)
	{
		if (outRaster == null) return (WritableRaster) getData();

		int width = outRaster.getWidth();
		int height = outRaster.getHeight();
		int startX = outRaster.getMinX();
		int startY = outRaster.getMinY();

		Object tdata = null;

		for (int i = startY; i < startY+height; i++)
		{
			tdata = raster.getDataElements(startX,i,width,1,tdata);
			outRaster.setDataElements(startX,i,width,1, tdata);
		}

		return outRaster;
	}

	public void setData(Raster r)
	{
		int width = r.getWidth();
		int height = r.getHeight();
		int startX = r.getMinX();
		int startY = r.getMinY();
		int[] tdata = null;

		// Clip to the current Raster
		Rectangle rclip = new Rectangle(startX, startY, width, height);
		Rectangle bclip = new Rectangle(0, 0, raster.getWidth(), raster.getHeight());
		Rectangle intersect = rclip.intersection(bclip);
		if (intersect.isEmpty())
			return;
		
		width = intersect.width;
		height = intersect.height;
		startX = intersect.x;
		startY = intersect.y;

		// remind use get/setDataElements for speed if Rasters are
		// compatible
		for (int i = startY; i < startY+height; i++)
		{
			tdata = r.getPixels(startX,i,width,1,tdata);
			raster.setPixels(startX,i,width,1, tdata);
		}
	}

	public void addTileObserver (TileObserver to) {}

	public void removeTileObserver (TileObserver to) {}

	public boolean isTileWritable (int tileX, int tileY)
	{
		if (tileX == 0 && tileY == 0) return true;

		throw new IllegalArgumentException("Only 1 tile in image");
	}

	public Point[] getWritableTileIndices()
	{ return new Point[]{new Point(0, 0)}; }

	public boolean hasTileWriters ()
	{ return true; }

	public WritableRaster getWritableTile (int tileX, int tileY)
	{ return raster; }

	public void releaseWritableTile (int tileX, int tileY)
	{ }

	public int getTransparency()
	{ return colorModel.getTransparency(); }

	public Graphics2D createGraphics()
	{
		return null;
	}
}