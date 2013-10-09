package struct;

import java.awt.image.ColorModel;
import java.awt.image.ImageConsumer;
import java.awt.image.ImageProducer;
import java.awt.image.WritableRaster;

public class OffScreenImageSource implements ImageProducer {
    Buffer image;
    int width;
    int height;

    public OffScreenImageSource(Buffer image) {
        this.image = image;
        width  = image.getWidth();
        height = image.getHeight();
    }


    // We can only have one consumer since we immediately return the data...
    private ImageConsumer theConsumer;

    public synchronized void addConsumer(ImageConsumer ic) {
        theConsumer = ic;
        produce();
    }

    public synchronized boolean isConsumer(ImageConsumer ic) {
        return (ic == theConsumer);
    }

    public synchronized void removeConsumer(ImageConsumer ic) {
        if (theConsumer == ic) {
            theConsumer = null;
        }
    }

    public void startProduction(ImageConsumer ic) {
        addConsumer(ic);
    }

    public void requestTopDownLeftRightResend(ImageConsumer ic) {
    }

    private void sendPixels()
    {
        ColorModel cm = image.getColorModel();
        WritableRaster raster = image.getRaster();
        int numDataElements = raster.getNumDataElements();
        int[] scanline = new int[width*numDataElements];
        
        theConsumer.setColorModel(cm);
        
        for (int y=0; y < height; y++)
        {
        	raster.getDataElements(0, y, width, 1, scanline);
        	theConsumer.setPixels(0, y, width, 1, cm, scanline, 0, width);
        }
    }

    private void produce() {
        try {
            theConsumer.setDimensions(image.getWidth(), image.getHeight());
            //theConsumer.setProperties(properties);
            sendPixels();
            theConsumer.imageComplete(ImageConsumer.SINGLEFRAMEDONE);
        } catch (NullPointerException e) {
            if (theConsumer != null) {
                theConsumer.imageComplete(ImageConsumer.IMAGEERROR);
            }
        }
    }
}