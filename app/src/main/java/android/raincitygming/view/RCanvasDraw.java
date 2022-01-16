package android.raincitygming.view;

import android.content.Context;
import android.graphics.Canvas;
import android.graphics.Color;
import android.graphics.Paint;
import android.graphics.PorterDuff;
import android.graphics.Rect;
import android.graphics.drawable.GradientDrawable;
import android.os.Handler;
import android.os.Looper;
import android.raincitygming.utils.ESPEngine;
import android.view.View;

public class RCanvasDraw extends View {
    int FPS = 60;
    Paint mTextPaint, mStrokePaint, mFilledPaint, mRectPaint;
    Rect eRect;
    GradientDrawable eGradientDrawable;
    int eColor[] = {Color.TRANSPARENT,Color.GREEN,Color.TRANSPARENT};
    public RCanvasDraw(Context context) {
        super(context, null, 0);
        InitializePaints();
        setFocusableInTouchMode(false);
        setBackgroundColor(Color.TRANSPARENT);
    }
    private void InitializePaints() {
        eRect = new Rect(-100,60,100,100);
        eGradientDrawable = new GradientDrawable();
        eGradientDrawable.setOrientation(GradientDrawable.Orientation.LEFT_RIGHT);
        eGradientDrawable.setColors(eColor);
        eGradientDrawable.setShape(GradientDrawable.RECTANGLE);
        eGradientDrawable.setBounds(eRect);

        mStrokePaint = new Paint();
        mStrokePaint.setStyle(Paint.Style.STROKE);
        mStrokePaint.setAntiAlias(true);
        mStrokePaint.setColor(Color.rgb(0, 0, 0));

        mTextPaint = new Paint();
        mTextPaint.setStyle(Paint.Style.FILL_AND_STROKE);
        mTextPaint.setAntiAlias(true);
        mTextPaint.setColor(Color.RED);
        mTextPaint.setTextAlign(Paint.Align.CENTER);
        mTextPaint.setStrokeWidth(1.1f);

        mFilledPaint = new Paint();
        mFilledPaint.setStyle(Paint.Style.FILL);
        mFilledPaint.setAntiAlias(true);

        mRectPaint = new Paint();
        mRectPaint.setStyle(Paint.Style.STROKE);
        mRectPaint.setAntiAlias(true);
    }

    Handler handler = new Handler(Looper.getMainLooper());
    Runnable runnable = new Runnable() {
        @Override
        public void run() {
            try {
                if (true)
                {
                    invalidate();
                }
            } catch (Exception e){
            }
            handler.postDelayed(this, FPS);
        }
    };
    public void runEsp() {
        runnable.run();
    }
    public void setFPS(int fps){
        FPS = 60 - fps;
    }
    @Override
    protected void onDraw(Canvas canvas) {
        if (canvas != null) {
            ClearCanvas(canvas);
            ROverlayView.GetInitCanvasDrawing(this, canvas);
        }
    }
    public void ClearCanvas(Canvas cvs) {
        cvs.drawColor(Color.TRANSPARENT, PorterDuff.Mode.CLEAR);
    }
    public void DrawEnemyCount(Canvas cvs, int colors, float dx, float dy) {
        if (colors == 1){ // Enemy = 0
            eColor[1] = Color.GREEN;
        }
        if (colors == 2){ // Enemy > 5
            eColor[1] = Color.YELLOW;
        }
        if (colors == 3){ // Enemy > 10
            eColor[1] = Color.parseColor("#FF6D00");
        }
        if (colors == 4){ // Enemy > 15
            eColor[1] = Color.RED;
        }
        cvs.save();
        cvs.translate(dx,dy);
        eGradientDrawable.setGradientType(GradientDrawable.LINEAR_GRADIENT);
        eGradientDrawable.draw(cvs);
        cvs.restore();
    }
    public void DrawText(Canvas cvs, int a, int r, int g, int b, String txt, float posX, float posY, float size) {
        mTextPaint.setShadowLayer(3, 0,0, Color.BLACK);
        mTextPaint.setColor(Color.rgb(r, g, b));
        mTextPaint.setAlpha(a);
        if (getRight() > 1920 || getBottom() > 1920)
            mTextPaint.setTextSize(4 + size);
        else if (getRight() == 1920 || getBottom() == 1920)
            mTextPaint.setTextSize(2 + size);
        else
            mTextPaint.setTextSize(size);
        cvs.drawText(txt, posX, posY, mTextPaint);
    }
    public void DrawText2(Canvas cvs, int a, int r, int g, int b, String txt, float posX, float posY, float size) {
        mTextPaint.clearShadowLayer();
        mTextPaint.setColor(Color.rgb(r, g, b));
        mTextPaint.setAlpha(a);
        if (getRight() > 1920 || getBottom() > 1920)
            mTextPaint.setTextSize(4 + size);
        else if (getRight() == 1920 || getBottom() == 1920)
            mTextPaint.setTextSize(2 + size);
        else
            mTextPaint.setTextSize(size);
        cvs.drawText(txt, posX, posY, mTextPaint);
    }
    public void DrawLine(Canvas cvs, int a, int r, int g, int b, float lineWidth, float fromX, float fromY, float toX, float toY) {
        mStrokePaint.setColor(Color.rgb(r, g, b));
        mStrokePaint.setAlpha(a);
        mStrokePaint.setStrokeWidth(lineWidth);
        cvs.drawLine(fromX, fromY, toX, toY, mStrokePaint);
    }
    public void DrawCircle(Canvas cvs, int a, int r, int g, int b, float cx, float cy, float radius){
        mFilledPaint.setColor(Color.rgb(r, g, b));
        mFilledPaint.setAlpha(a);
        cvs.drawCircle(cx, cy, radius, mFilledPaint);
    }
    public void DrawRect(Canvas canvas, int a, int r, int g, int b, float strokeSize, float startX, float startY, float endX, float endY){
        mRectPaint.setColor(Color.rgb(r, g, b));
        mTextPaint.setAlpha(a);
        mRectPaint.setStrokeWidth(strokeSize);
        canvas.drawRect(startX, startY, endX, endY, mRectPaint);
    }
    public void DrawEnemyWeapon(Canvas cvs, int a, int r, int g, int b, int id, int ammo, float posX, float posY, float size) {
        mTextPaint.setARGB(a,r, g, b);
        mTextPaint.clearShadowLayer();
        String wName = ESPEngine.getEnemyWeapon(id);
        if(wName != null){
            mTextPaint.setTextSize(size);
            mTextPaint.setColor(Color.WHITE);
            cvs.drawText(wName + " / " + ammo, posX, posY, mTextPaint);
        }
    }
}
