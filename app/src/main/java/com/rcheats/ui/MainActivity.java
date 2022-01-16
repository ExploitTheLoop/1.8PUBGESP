package com.rcheats.ui;

import androidx.annotation.Nullable;
import androidx.appcompat.app.AppCompatActivity;
import androidx.cardview.widget.CardView;

import android.app.AlertDialog;
import android.content.DialogInterface;
import android.content.Intent;
import android.net.Uri;
import android.os.Build;
import android.os.Bundle;
import android.os.Handler;
import android.os.Looper;
import android.provider.Settings;
import android.raincitygming.controller.AppController;
import android.raincitygming.utils.FileCommons;
import android.raincitygming.utils.RDialog;
import android.raincitygming.utils.RShared;
import android.util.Log;
import android.view.View;
import android.widget.RadioButton;
import android.widget.TextView;

import com.rcheats.R;
import com.topjohnwu.superuser.Shell;

import java.io.File;
public class MainActivity extends AppCompatActivity {

    private RShared rShared;
    public static int REQUEST = 5565;

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_main);
        /** Check root access **/
        if (Shell.rootAccess()){
            Log.d("RCG", "Root granted");
        } else {
            Log.d("RCG", "Root not granted");
        }
        RShared.InitShared(this);
        rShared = new RShared(this);
        requestOverlayPermission();
        FileCommons.copyFromAssets(this, getFilesDir().getPath(), "rd1");
        CardView cv_start_cheat = findViewById(R.id.cv_start_cheat);
        CardView cv_stop_cheat = findViewById(R.id.cv_stop_cheat);
        RadioButton rb_global = findViewById(R.id.rb_global);
        RadioButton rb_vietnam = findViewById(R.id.rb_vietnam);
        RadioButton rb_korea = findViewById(R.id.rb_korea);
        TextView tv_contact_me = findViewById(R.id.tv_contact_me);
        /** PUBGM Version **/
        /**
         *  Global 1
         *  Vietnam 2
         *  Korea 3
         **/
        rb_global.setOnClickListener(view -> {
            rShared.setInt("PUBGM_VERSION", 1);
        });
        rb_vietnam.setOnClickListener(view -> {
            rShared.setInt("PUBGM_VERSION", 2);
        });
        rb_korea.setOnClickListener(view -> {
            rShared.setInt("PUBGM_VERSION", 3);
        });

        if (rShared.getInt("PUBGM_VERSION") == 1){
            rb_global.setChecked(true);
        }
        if (rShared.getInt("PUBGM_VERSION") == 2){
            rb_vietnam.setChecked(true);
        }
        if (rShared.getInt("PUBGM_VERSION") == 3){
            rb_korea.setChecked(true);
        }
        tv_contact_me.setOnClickListener(view -> {
            String str =
                    "Email: raincityhack@gmail.com\n" +
                            "Telegram: t.me/RAIN_CITY_GAMING\n" +
                            "Website: https://raincitygaming.com";
            RDialog.message(MainActivity.this, str, true, null);
        });
        File file = new File(getFilesDir().getPath() + "/rd1");
        cv_start_cheat.setOnClickListener(view -> {
            Shell.sh("chmod +x " + file.toString()).exec();
            AppController.InitService(this, AppController.START);
        });
        cv_stop_cheat.setOnClickListener(view -> {
            AppController.InitService(this, AppController.STOP);
        });
    }
    private void requestOverlayPermission() {
        if (Build.VERSION.SDK_INT >= Build.VERSION_CODES.M) {
            if(!Settings.canDrawOverlays(this)){
                new AlertDialog.Builder(MainActivity.this, R.style.Theme_AppCompat_Light_Dialog)
                        .setMessage("Please enable overlay permission first")
                        .setPositiveButton("OK", (dialogInterface, i) -> {
                            Intent intent = new Intent(Settings.ACTION_MANAGE_OVERLAY_PERMISSION, Uri.parse("package:" + MainActivity.this.getPackageName()));
                            startActivityForResult(intent, REQUEST);
                        })
                        .setNegativeButton("Cancel", (dialogInterface, i) -> finish())
                        .setCancelable(false)
                        .show();
            }
        }
    }
    @Override
    protected void onActivityResult(int requestCode, int resultCode, @Nullable Intent data) {
        super.onActivityResult(requestCode, resultCode, data);
        if (requestCode == REQUEST){
            if (Build.VERSION.SDK_INT >= Build.VERSION_CODES.M) {
                requestOverlayPermission();
            }
        }
    }
}