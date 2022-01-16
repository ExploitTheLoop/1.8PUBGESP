package android.raincitygming.view;

import android.content.Context;
import android.view.LayoutInflater;
import android.view.View;

import com.rcheats.R;

public class FView {
    public static View getMainView(Context context){
        return LayoutInflater.from(context).inflate(R.layout.roverlay_layout, null);
    }
}
