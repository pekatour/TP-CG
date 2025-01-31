
import algebra.*;

/**
 * The Rasterizer class is responsible for the discretization of geometric
 * primitives
 * (edges and faces) over the screen pixel grid and generates Fragment (pixels
 * with
 * interpolated attributes). Those Fragment are then passed to a Shader object,
 * which will produce the final color of the fragment.
 *
 * @author morin, chambon, cdehais
 */
public class Rasterizer {

    Shader shader;

    public Rasterizer(Shader shader) {
        this.shader = shader;
    }

    public void setShader(Shader shader) {
        this.shader = shader;
    }

    /**
     * Linear interpolation of a Fragment f on the edge defined by Fragment's v1 and
     * v2
     */
    private void interpolate2(Fragment v1, Fragment v2, Fragment f) {
        int x1 = v1.getX();
        int y1 = v1.getY();
        int x2 = v2.getX();
        int y2 = v2.getY();
        int x = f.getX();
        int y = f.getX();

        double alpha;
        if (Math.abs(x2 - x1) > Math.abs(y2 - y1)) {
            alpha = (double) (x - x1) / (double) (x2 - x1);
        } else {
            if (y2 != y1) {
                alpha = (double) (y - y1) / (double) (y2 - y1);
            } else {
                alpha = 0.5;
            }
        }
        int numAttributes = f.getNumAttributes();
        for (int i = 0; i < numAttributes; i++) {
            f.setAttribute(i, (1.0 - alpha) * v1.getAttribute(i) + alpha * v2.getAttribute(i));
        }
    }

    /**
     * Swaps x and y coordinates of the fragment. Used by the Bresenham algorithm.
     */
    private static void swapXAndY(Fragment f) {
        f.setPosition(f.getY(), f.getX());
    }

    /**
     * Rasterizes the edge between the projected vectors v1 and v2.
     * Generates Fragment's and calls the Shader::shade() metho on each of them.
     */
    public void rasterizeEdge(Fragment v1, Fragment v2) {
        // This is basically Bresenham's algorithm
        int x1 = v1.getX();
        int y1 = v1.getY();
        int x2 = v2.getX();
        int y2 = v2.getY();

        // For now : just display the vertices
        Fragment f = new Fragment(0, 0);
        int size = 2;
        for (int i = 0; i < v1.getNumAttributes(); i++) {
            f.setAttribute(i, v1.getAttribute(i));
        }
        for (int i = -size; i <= size; i++) {
            for (int j = -size; j <= size; j++) {
                f.setPosition(x1 + i, y1 + j);
                shader.shade(f);
            }
        }

        // Uncomment the following block of code for drawing the wireframe
//         // int numAttributes = v1.getNumAttributes ();
//         Fragment fragment = new Fragment(0, 0); // , numAttributes);
// 
//         boolean sym = (Math.abs(y2 - y1) > Math.abs(x2 - x1));
//         if (sym) {
//             int temp;
//             temp = x1;
//             x1 = y1;
//             y1 = temp;
//             temp = x2;
//             x2 = y2;
//             y2 = temp;
//             // swapXAndY (v1);
//             // swapXAndY (v2);
//         }
//         if (x1 > x2) {
//             Fragment ftemp;
//             int temp;
//             temp = x1;
//             x1 = x2;
//             x2 = temp;
//             temp = y1;
//             y1 = y2;
//             y2 = temp;
//             ftemp = v1;
//             v1 = v2;
//             v2 = ftemp;
//         }
// 
//         int ystep;
//         if (y1 < y2) {
//             ystep = 1;
//         } else {
//             ystep = -1;
//         }
// 
//         int err = (x1 - x2) / 2;
//         int dx = (x2 - x1);
//         int dy = Math.abs(y2 - y1);
// 
//         int x = x1;
//         int y = y1;
// 
//         while (x <= x2) {
// 
//             fragment.setPosition(x, y);
// 
//             if (!shader.isClipped(fragment)) {
// 
//                 interpolate2(v1, v2, fragment);
//                 if (sym) {
//                     swapXAndY(fragment);
//                 }
//                 shader.shade(fragment);
//             }
// 
//             x += 1;
//             err = err + dy;
//             if (err > 0) {
//                 y += ystep;
//                 err -= dx;
//             }
//         }
    }

    static double triangleArea(Fragment v1, Fragment v2, Fragment v3) {
        return (double) v2.getX() * v3.getY() - v2.getY() * v3.getX()
                + v3.getX() * v1.getY() - v1.getX() * v3.getY()
                + v1.getX() * v2.getY() - v2.getX() * v1.getY();
    }

    static protected Matrix makeBarycentricCoordsMatrix(Fragment v1, Fragment v2, Fragment v3) {
        Matrix C = null;
        try {
            C = new Matrix(3, 3);
        } catch (InstantiationException e) {
            // unreached
        }

        double area = triangleArea(v1, v2, v3);
        int x1 = v1.getX();
        int y1 = v1.getY();
        int x2 = v2.getX();
        int y2 = v2.getY();
        int x3 = v3.getX();
        int y3 = v3.getY();
        C.set(0, 0, (x2 * y3 - x3 * y2) / area);
        C.set(0, 1, (y2 - y3) / area);
        C.set(0, 2, (x3 - x2) / area);
        C.set(1, 0, (x3 * y1 - x1 * y3) / area);
        C.set(1, 1, (y3 - y1) / area);
        C.set(1, 2, (x1 - x3) / area);
        C.set(2, 0, (x1 * y2 - x2 * y1) / area);
        C.set(2, 1, (y1 - y2) / area);
        C.set(2, 2, (x2 - x1) / area);

        return C;
    }

    /**
     * Rasterizes the triangular face made of the Fragment v1, v2 and v3
     */
    public void rasterizeFace(Fragment v1, Fragment v2, Fragment v3) {

        Matrix C = makeBarycentricCoordsMatrix(v1, v2, v3);

        // iterate over the triangle's bounding box
        // TODO
































    }
}
