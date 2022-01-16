package android.raincitygming.controller;

import android.content.Context;
import android.raincitygming.service.RService;

public class AppController {
    public static int STOP = 0;
    public static int START = 1;
    public static void InitService(Context context, int mode){
        switch (mode){
            case 0:
                RService.Stop();
                break;
            case 1:
                RService.Start(context);
                break;
        }
    }
}
