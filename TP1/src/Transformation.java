
import algebra.*;

/**
 * author: cdehais
 */
public class Transformation {

    Matrix worldToCamera;
    Matrix projection;
    Matrix calibration;

    public Transformation() {
        try {
            worldToCamera = Matrix.createIdentity("W2C", 4);
            projection = new Matrix("P", 3, 4);
            calibration = Matrix.createIdentity("K", 3);
        } catch (InstantiationException e) {
            // should not reach
        }
    }

    public void setLookAt(Vector3 eye, Vector3 interestPoint, Vector3 up) {
        try {
            Vector3 ez = new Vector3(interestPoint);
            ez.subtract(eye);
            ez.normalize();

            Vector3 ex = up.cross(ez);
            ex.normalize();

            Vector3 ey = ez.cross(ex);

            Matrix N_t = new Matrix(3, 3);
            N_t.setCol(0, ex);
            N_t.setCol(1, ey);
            N_t.setCol(2, ez);
            Matrix N = N_t.transpose();

            Vector t = N.multiply(eye);
            t.scale(-1);

            for (int i=0; i<3; i++){
                worldToCamera.set(i, 3, t.get(i));
                for (int j=0; j<3; j++){
                    worldToCamera.set(i, j, N.get(i, j));
                }
            }
             
        } catch (Exception e) {
            /* unreached */ }
        ;

        System.out.println("Modelview matrix:\n" + worldToCamera);
    }

    public void setProjection() {
        try {
            Matrix p2 = worldToCamera.getSubMatrix(0, 0, 3, 4);
            projection = calibration.multiply(p2);
        } catch (Exception e) {
            // unhandled exception
        }



        System.out.println("Projection matrix:\n" + projection);
    }

    public void setCalibration(double focal, double width, double height) {

        calibration.set(0,0,focal);
        calibration.set(1,1,focal);
        calibration.set(0,2,width/2);
        calibration.set(1,2,height/2);

        System.out.println("Calibration matrix:\n" + calibration);
    }

    /**
     * Projects the given homogeneous, 4 dimensional point onto the screen.
     * The resulting Vector as its (x,y) coordinates in pixel, and its z coordinate
     * is the depth of the point in the camera coordinate system.
     */
    public Vector3 projectPoint(Vector p)
            throws SizeMismatchException, InstantiationException {
        Vector ps = new Vector(3);
        ps = projection.multiply(p);
        ps.set(0,ps.get(0)/ps.get(2));
        ps.set(1,ps.get(1)/ps.get(2));
        return new Vector3(ps);
    }

    /**
     * Transform a vector from world to camera coordinates.
     */
    public Vector3 transformVector(Vector3 v)
            throws SizeMismatchException, InstantiationException {
        // Doing nothing special here because there is no scaling
        Matrix R = worldToCamera.getSubMatrix(0, 0, 3, 3);
        Vector tv = R.multiply(v);
        return new Vector3(tv);
    }

}
