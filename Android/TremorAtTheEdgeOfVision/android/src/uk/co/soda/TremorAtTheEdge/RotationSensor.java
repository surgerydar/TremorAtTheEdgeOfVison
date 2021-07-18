package uk.co.soda.TremorAtTheEdge;

// Qt
import org.qtproject.qt5.android.QtNative;
import org.qtproject.qt5.android.bindings.QtActivity;

// java
import java.io.*;

// android
import android.app.Activity;
import android.hardware.Sensor;
import android.hardware.SensorEvent;
import android.hardware.SensorEventListener;
import android.hardware.SensorManager;

public class RotationSensor {
    static RotationSensorListener mRotationSensorListener;
    public static void open() {
        mRotationSensorListener = new RotationSensorListener();
        System.out.println("RotationSensor.open");
    }
    public static void start() {
        mRotationSensorListener.start();
        System.out.println("RotationSensor.start");
    }
    public static void stop() {
        mRotationSensorListener.stop();
        System.out.println("RotationSensor.stop");
    }
    public static float[] getRotationMatrix() {
        return mRotationSensorListener.getRotationMatrix();
    }
    public static float[] getOrientation() {
        return mRotationSensorListener.getOrientation();
    }
    public static void setMapping(int x, int y ) {
        mRotationSensorListener.setMapping(x,y);
    }
    static class RotationSensorListener implements SensorEventListener {
        private SensorManager mSensorManager;
        private Sensor mRotationVectorSensor;
        private final float[] mRotationMatrix = new float[16];
        private final float[] mOrientation = new float[4];
        private int mappingX = 0;
        private int mappingY = 2;

        public RotationSensorListener() {
            //
            //
            //
            mSensorManager = (SensorManager)QtNative.activity().getSystemService(Activity.SENSOR_SERVICE);
            //
            //
            //
            mRotationVectorSensor = mSensorManager.getDefaultSensor(Sensor.TYPE_ROTATION_VECTOR);
            //
            // initialize the rotation matrix to identity
            //
            mRotationMatrix[ 0] = 1;
            mRotationMatrix[ 4] = 1;
            mRotationMatrix[ 8] = 1;
            mRotationMatrix[12] = 1;
        }
        public void start() {
            // enable our sensor when the activity is resumed, ask for
            // 10 ms updates.
            mSensorManager.registerListener(this, mRotationVectorSensor, 10000);
        }
        public void stop() {
            // make sure to turn our sensor off when the activity is paused
            mSensorManager.unregisterListener(this);
        }
        public float[] getRotationMatrix() {
            return mRotationMatrix;
        }
        public float[] getOrientation() {
            return mOrientation;
        }
        public void setMapping(int x, int y ) {
            mappingX = x;
            mappingY = y;
        }
        //
        // SensorEventListener methods
        //
        public void onSensorChanged(SensorEvent event) {
            // we received a sensor event. it is a good practice to check
            // that we received the proper event
            if (event.sensor.getType() == Sensor.TYPE_ROTATION_VECTOR) {
                // convert the rotation-vector to a 4x4 matrix. the matrix
                // is interpreted by Open GL as the inverse of the
                // rotation-vector, which is what we want.
                int[] mappingAxis = {
                    SensorManager.AXIS_X,
                    SensorManager.AXIS_Y,
                    SensorManager.AXIS_Z,
                    SensorManager.AXIS_MINUS_X,
                    SensorManager.AXIS_MINUS_Y,
                    SensorManager.AXIS_MINUS_Z
                };
                float[] rotationMatrix = new float[16];
                SensorManager.getRotationMatrixFromVector(rotationMatrix , event.values);
                SensorManager.remapCoordinateSystem(rotationMatrix, mappingAxis[mappingX], mappingAxis[mappingY], mRotationMatrix);
                SensorManager.getOrientation(mRotationMatrix, mOrientation);
                /*
                int yaw = (int) (Math.toDegrees(mOrientation[0]) ) ;
                int pitch = (int) (Math.toDegrees(mOrientation[1]) ) ;
                int roll = (int) (Math.toDegrees(mOrientation[2]) ) ;
                System.out.println( "yaw=" + yaw + " pitch=" + pitch + " roll=" + roll );
                */
            }
        }
        public void onAccuracyChanged(Sensor sensor, int accuracy) {
        }
    }
}
