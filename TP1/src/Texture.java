
import java.awt.*;
import java.awt.image.*;
import java.io.*;
import javax.imageio.ImageIO;

/**
 * 2D Texture class.
 */
public class Texture {
    int width;
    int height;
    BufferedImage image;

    /**
     * Constructs a new Texture with the content of the image at @path.
     */
    public Texture(String path) throws Exception {
        image = ImageIO.read(new File(path));
        width = image.getWidth();
        height = image.getHeight();
    }

    /**
     * Samples the texture at texture coordinates (u,v), using nearest neighboor
     * interpolation
     * u and v are wrapped around to [0,1].
     */
    public Color sample(double u, double v) {
        // TODO
        int x = Math.round((float) u * width) % width;
        int y = Math.round((float) v * height) % height;
        int c = image.getRGB(x, y);

        return new Color(c);
    }
}
