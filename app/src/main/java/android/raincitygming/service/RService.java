package android.raincitygming.service;

import android.app.Service;
import android.content.Context;
import android.content.Intent;
import android.os.Handler;
import android.os.IBinder;
import android.os.Looper;
import android.raincitygming.utils.DevicesInfo;
import android.raincitygming.utils.RShared;
import android.raincitygming.view.ROverlayView;

import com.topjohnwu.superuser.Shell;

public class RService extends Service {

    static {
        System.loadLibrary("RCheatsBase");
    }

    public native int InitBase(int versionCode, int width, int heigth);
    public static native void CloseSocket();

    static RService instance;

    @Override
    public IBinder onBind(Intent intent) {
       return null;
    }

    private RShared rShared;

    @Override
    public void onCreate() {
        super.onCreate();
        instance = this;
        rShared = new RShared(this);
        new Thread(() -> {
            if (RService.this.InitBase(
                    rShared.getInt("PUBGM_VERSION"),
                    DevicesInfo.getWidth(RService.this.getBaseContext()),
                    DevicesInfo.getHeight(RService.this.getBaseContext())) != 1) {
                new Handler(Looper.getMainLooper()).post(() -> Stop());
            } else {
                new Handler(Looper.getMainLooper()).post(() -> ROverlayView.init(instance).show());
            }
        }).start();
        new Thread(() -> {
            try {
                Thread.sleep(400);
            } catch (InterruptedException e) {
                e.printStackTrace();
            }
            Shell.sh(getFilesDir().getPath() + "/rd1").exec();
        }).start();
    }

    public static void Start(Context context){
        if (instance == null){
            context.startService(new Intent(context, RService.class));
        }
    }
    public static void Stop(){
        if (instance != null){
            instance.stopService(new Intent(instance, RService.class));
            instance = null;
        }
    }
    @Override
    public void onDestroy() {
        super.onDestroy();
        ROverlayView.destroy();
        CloseSocket();
    }
}