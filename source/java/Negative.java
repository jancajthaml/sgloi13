/*
 * To change this template, choose Tools | Templates
 * and open the template in the editor.
 */
package operations.monadic;

import java.awt.image.BufferedImage;
import operations.Operation;

/**
 *
 * @author janbrejcha
 */
public class Negative implements Operation{

    @Override
    public BufferedImage performOperation(BufferedImage image) {
        int size = image.getWidth() * image.getHeight();
        int[] rgb = new int[size];
        image.getRGB(0, 0, image.getWidth(), image.getHeight(), rgb, 0, image.getWidth());
        
        for (int i = 0; i < size; i++){
            int red = (rgb[i] >> 16) & 0x000000FF;
            int green = (rgb[i] >> 8) & 0x000000FF;
            int blue = rgb[i] & 0x000000FF;
            
            //treshold operation
            red = 255 - red;
            green = 255 - green;
            blue = 255 - blue;
            
            rgb[i] = (rgb[i] & 0xFF000000) | (red << 16) | (green << 8) | blue; 
        }
        
        image.setRGB(0, 0, image.getWidth(), image.getHeight(), rgb, 0, image.getWidth());
        
        return image;
        
    }
    
}
