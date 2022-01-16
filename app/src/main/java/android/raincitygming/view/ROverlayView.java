package android.raincitygming.view;

import android.content.Context;
import android.graphics.Canvas;
import android.raincitygming.utils.RShared;
import android.raincitygming.utils.SingleTapConfirm;
import android.view.GestureDetector;
import android.view.MotionEvent;
import android.view.View;
import android.view.WindowManager;
import android.widget.CheckBox;
import android.widget.CompoundButton;
import android.widget.LinearLayout;
import android.widget.RelativeLayout;
import android.widget.SeekBar;
import android.widget.TextView;

import com.rcheats.R;

public class ROverlayView {
    static {
        System.loadLibrary("RCheatsBase");
    }
    public static native void setValueBoolean(int feature, boolean about);
    public static native void GetInitCanvasDrawing(RCanvasDraw rCanvasDraw, Canvas canvas);
    private static RShared rShared;
    private static Context context;
    private static View menuView;
    private static RCanvasDraw rCanvasDraw;
    private static WindowManager windowManager;
    private static WindowManager.LayoutParams params, paramsCanvas;

    public ROverlayView(Context ctx){
        context = ctx;
    }
    public static ROverlayView init(Context context){
        rShared = new RShared(context);
        windowManager = FWindowManager.getWindowManager(context);
        menuView = FView.getMainView(context);
        rCanvasDraw = new RCanvasDraw(context);
        rCanvasDraw.setFPS(getValueInt("FRAME_RATE"));
        rCanvasDraw.runEsp();
        params = FOverlayMenuWLParams.getParams();
        paramsCanvas = ROverlayCanvasWLParams.getParams();
        return new ROverlayView(context);
    }
    public void show(){
        windowManager.addView(menuView, params);
        windowManager.addView(rCanvasDraw, paramsCanvas);
        InitView();
    }
    private static LinearLayout controlView;
    private static RelativeLayout mainView;
    private static void InitView() {
        controlView = menuView.findViewById(R.id.layout_control);
        mainView = menuView.findViewById(R.id.layout_menu);
        menuView.findViewById(R.id.ic_close).setOnClickListener(v -> {
            controlView.setVisibility(View.VISIBLE);
            mainView.setVisibility(View.GONE);
        });
        final GestureDetector gestureDetector = new GestureDetector(context, new SingleTapConfirm());
        menuView.findViewById(R.id.base_layout).setOnTouchListener(new View.OnTouchListener() {
            private int initialX;
            private int initialY;
            private float initialTouchX;
            private float initialTouchY;
            @Override
            public boolean onTouch(View v, MotionEvent event) {
                if (gestureDetector.onTouchEvent(event)) {
                    mainView.setVisibility(View.VISIBLE);
                    controlView.setVisibility(View.GONE);
                    return true;
                } else {
                    switch (event.getAction()) {
                        case MotionEvent.ACTION_DOWN:
                            initialX = params.x;
                            initialY = params.y;
                            initialTouchX = event.getRawX();
                            initialTouchY = event.getRawY();
                            return true;
                        case MotionEvent.ACTION_MOVE:
                            params.x = initialX + (int) (event.getRawX() - initialTouchX);
                            params.y = initialY + (int) (event.getRawY() - initialTouchY);
                            windowManager.updateViewLayout(menuView, params);
                            return true;
                    }
                    return false;
                }
            }
        });
        InitMenuView();
    }
    private static SeekBar seekbar_frame_rate;

    private static void InitMenuView() {
        seekbar_frame_rate = mainView.findViewById(R.id.seekbar_frame_rate);
        TextView tv_seekbar_frame_rate = mainView.findViewById(R.id.tv_seekbar_frame_rate);
        seekbar_frame_rate.setOnSeekBarChangeListener(new SeekBar.OnSeekBarChangeListener() {
            @Override
            public void onProgressChanged(SeekBar seekBar, int i, boolean b) {
                tv_seekbar_frame_rate.setText("" + i);
                setValueInt("FRAME_RATE", i);
                rCanvasDraw.setFPS(i);
            }
            @Override
            public void onStartTrackingTouch(SeekBar seekBar) {}
            @Override
            public void onStopTrackingTouch(SeekBar seekBar) {}
        });
        seekbar_frame_rate.setProgress(getValueInt("FRAME_RATE"));
        tv_seekbar_frame_rate.setText(String.valueOf(seekbar_frame_rate.getProgress()));

        final CheckBox cb_name = mainView.findViewById(R.id.cb_name);
        final CheckBox cb_distance = mainView.findViewById(R.id.cb_distance);
        final CheckBox cb_health = mainView.findViewById(R.id.cb_health);
        final CheckBox cb_team_id = mainView.findViewById(R.id.cb_team_id);
        final CheckBox cb_box = mainView.findViewById(R.id.cb_box);
        final CheckBox cb_radar_line = mainView.findViewById(R.id.cb_radar_line);
        final CheckBox cb_skeleton = mainView.findViewById(R.id.cb_skeleton);
        final CheckBox cb_weapon = mainView.findViewById(R.id.cb_weapon);
        final CheckBox cb_enemy_count = mainView.findViewById(R.id.cb_enemy_count);
        final CheckBox cb_360_warning = mainView.findViewById(R.id.cb_360_warning);
        final CheckBox cb_head_dots = mainView.findViewById(R.id.cb_head_dots);
        final CheckBox cb_grenade_alert = mainView.findViewById(R.id.cb_grenade_alert);
        CompoundButton.OnCheckedChangeListener tab1OnChecked = (buttonView, isChecked) -> {
            switch (buttonView.getId()){
                case R.id.cb_name:
                    setValueBooelan(cb_name.getText().toString(), isChecked);
                    setValueBoolean(1, isChecked);
                    break;
                case R.id.cb_distance:
                    setValueBooelan(cb_distance.getText().toString(), isChecked);
                    setValueBoolean(2, isChecked);
                    break;
                case R.id.cb_health:
                    setValueBooelan(cb_health.getText().toString(), isChecked);
                    setValueBoolean(3, isChecked);
                    break;
                case R.id.cb_team_id:
                    setValueBooelan(cb_team_id.getText().toString(), isChecked);
                    setValueBoolean(4, isChecked);
                    break;
                case R.id.cb_box:
                    setValueBooelan(cb_box.getText().toString(), isChecked);
                    setValueBoolean(5, isChecked);
                    break;
                case R.id.cb_radar_line:
                    setValueBooelan(cb_radar_line.getText().toString(), isChecked);
                    setValueBoolean(6, isChecked);
                    break;
                case R.id.cb_skeleton:
                    setValueBooelan(cb_skeleton.getText().toString(), isChecked);
                    setValueBoolean(7, isChecked);
                    break;
                case R.id.cb_weapon:
                    setValueBooelan(cb_weapon.getText().toString(), isChecked);
                    setValueBoolean(8, isChecked);
                    break;
                case R.id.cb_enemy_count:
                    setValueBooelan(cb_enemy_count.getText().toString(), isChecked);
                    setValueBoolean(9, isChecked);
                    break;
                case R.id.cb_360_warning:
                    setValueBooelan(cb_360_warning.getText().toString(), isChecked);
                    setValueBoolean(10, isChecked);
                    break;
                case R.id.cb_head_dots:
                    setValueBooelan(cb_head_dots.getText().toString(), isChecked);
                    setValueBoolean(11, isChecked);
                    break;
                case R.id.cb_grenade_alert:
                    setValueBooelan(cb_grenade_alert.getText().toString(), isChecked);
                    setValueBoolean(12, isChecked);
                    break;
            }
        };
        cb_name.setOnCheckedChangeListener(tab1OnChecked);
        cb_distance.setOnCheckedChangeListener(tab1OnChecked);
        cb_health.setOnCheckedChangeListener(tab1OnChecked);
        cb_team_id.setOnCheckedChangeListener(tab1OnChecked);
        cb_box.setOnCheckedChangeListener(tab1OnChecked);
        cb_radar_line.setOnCheckedChangeListener(tab1OnChecked);
        cb_skeleton.setOnCheckedChangeListener(tab1OnChecked);
        cb_weapon.setOnCheckedChangeListener(tab1OnChecked);
        cb_enemy_count.setOnCheckedChangeListener(tab1OnChecked);
        cb_360_warning.setOnCheckedChangeListener(tab1OnChecked);
        cb_head_dots.setOnCheckedChangeListener(tab1OnChecked);
        cb_grenade_alert.setOnCheckedChangeListener(tab1OnChecked);

        cb_name.setChecked(getValueBoolean(cb_name.getText().toString()));
        cb_distance.setChecked(getValueBoolean(cb_name.getText().toString()));
        cb_health.setChecked(getValueBoolean(cb_health.getText().toString()));
        cb_team_id.setChecked(getValueBoolean(cb_team_id.getText().toString()));
        cb_box.setChecked(getValueBoolean(cb_box.getText().toString()));
        cb_radar_line.setChecked(getValueBoolean(cb_radar_line.getText().toString()));
        cb_skeleton.setChecked(getValueBoolean(cb_skeleton.getText().toString()));
        cb_weapon.setChecked(getValueBoolean(cb_weapon.getText().toString()));
        cb_enemy_count.setChecked(getValueBoolean(cb_enemy_count.getText().toString()));
        cb_360_warning.setChecked(getValueBoolean(cb_360_warning.getText().toString()));
        cb_head_dots.setChecked(getValueBoolean(cb_head_dots.getText().toString()));
        cb_grenade_alert.setChecked(getValueBoolean(cb_grenade_alert.getText().toString()));

        setValueBoolean(1, cb_name.isChecked());
        setValueBoolean(2, cb_distance.isChecked());
        setValueBoolean(3, cb_health.isChecked());
        setValueBoolean(4, cb_team_id.isChecked());
        setValueBoolean(5, cb_box.isChecked());
        setValueBoolean(6, cb_radar_line.isChecked());
        setValueBoolean(7, cb_skeleton.isChecked());
        setValueBoolean(8, cb_weapon.isChecked());
        setValueBoolean(9, cb_enemy_count.isChecked());
        setValueBoolean(10, cb_360_warning.isChecked());
        setValueBoolean(11, cb_head_dots.isChecked());
        setValueBoolean(12, cb_grenade_alert.isChecked());

        CheckBox cb_less_recoil = mainView.findViewById(R.id.cb_less_recoil);
        CheckBox cb_instant_hit = mainView.findViewById(R.id.cb_instant_hit);
        CheckBox cb_fast_parachute = mainView.findViewById(R.id.cb_fast_parachute);
        CompoundButton.OnCheckedChangeListener onCheckedOtherFitur = (compoundButton, isChecked) -> {
            switch (compoundButton.getId()){
                case R.id.cb_less_recoil:
                    setValueBooelan(cb_less_recoil.getText().toString(), isChecked);
                    setValueBoolean(13, isChecked);
                    break;
                case R.id.cb_instant_hit:
                    setValueBooelan(cb_instant_hit.getText().toString(), isChecked);
                    setValueBoolean(14, isChecked);
                    break;
                case R.id.cb_fast_parachute:
                    setValueBooelan(cb_fast_parachute.getText().toString(), isChecked);
                    setValueBoolean(15, isChecked);
                    break;
            }
        };

        cb_less_recoil.setOnCheckedChangeListener(onCheckedOtherFitur);
        cb_instant_hit.setOnCheckedChangeListener(onCheckedOtherFitur);
        cb_fast_parachute.setOnCheckedChangeListener(onCheckedOtherFitur);

        cb_less_recoil.setChecked(getValueBoolean(cb_less_recoil.getText().toString()));
        cb_instant_hit.setChecked(getValueBoolean(cb_instant_hit.getText().toString()));
        cb_fast_parachute.setChecked(getValueBoolean(cb_fast_parachute.getText().toString()));

        setValueBoolean(13, cb_less_recoil.isChecked());
        setValueBoolean(14, cb_instant_hit.isChecked());
        setValueBoolean(15, cb_fast_parachute.isChecked());
    }

    public static void setValueBooelan(String name, boolean apply){
        rShared.setBoolean(name, apply);
    }

    public static boolean getValueBoolean(String name){
        return rShared.getBoolean(name);
    }
    public static void setValueInt(String name, int value){
        rShared.setInt(name, value);
    }
    public static int getValueInt(String name){
        return rShared.getInt(name);
    }

    public static void destroy(){
        if (menuView != null){
            windowManager.removeView(menuView);
            menuView = null;
        }
        if (rCanvasDraw != null){
            windowManager.removeView(rCanvasDraw);
            rCanvasDraw = null;
        }
    }
}
