package uk.co.soda.TremorAtTheEdge;

// Qt
import org.qtproject.qt5.android.QtNative;
import org.qtproject.qt5.android.bindings.QtActivity;

// java
import java.io.*;

// android
import android.app.Activity;
import android.os.PowerManager;
import android.view.WindowManager;

public class PowerManagement {
    static PowerManager.WakeLock mWakeLock = null;

    public static void preventSleep() {
        /*
        if ( mWakeLock == null ) {
            PowerManager pm = (PowerManager) QtNative.activity().getSystemService(Activity.POWER_SERVICE);
            mWakeLock = pm.newWakeLock(PowerManager.SCREEN_DIM_WAKE_LOCK, "Tremor at the Edge");
            mWakeLock.acquire();
        }
        */
        QtNative.activity().runOnUiThread(new Runnable() {
            @Override
            public void run() {
                QtNative.activity().getWindow().addFlags(WindowManager.LayoutParams.FLAG_KEEP_SCREEN_ON);
            }
        });
    }

    public static void allowSleep() {
        /*
        if ( mWakeLock != null ) {
            mWakeLock.release();
            mWakeLock = null;
        }
        */
        QtNative.activity().runOnUiThread(new Runnable() {
            @Override
            public void run() {
                QtNative.activity().getWindow().clearFlags(WindowManager.LayoutParams.FLAG_KEEP_SCREEN_ON);
            }
        });
    }

}
