package android.raincitygming.utils;

import android.annotation.SuppressLint;
import android.app.Activity;
import android.app.AlertDialog;
import android.content.Context;
import android.content.DialogInterface;
import android.view.LayoutInflater;
import android.view.View;
import android.widget.ImageView;
import android.widget.TextView;
import android.widget.Toast;

import androidx.cardview.widget.CardView;

import com.rcheats.R;

public class RDialog {

        public static void message(Context context, CharSequence message, boolean setCancel, View.OnClickListener onClickListener){
            ((Activity)context).runOnUiThread(() -> {
                View layout = LayoutInflater.from(context).inflate(R.layout.rdialog_layout, null);
                ImageView image = layout.findViewById(R.id.image);
                image.setImageResource(R.mipmap.ic_launcher);
                TextView text = layout.findViewById(R.id.text);
                CardView cv_ok = layout.findViewById(R.id.cv_ok);
                text.setText(message);
                AlertDialog.Builder builder = new AlertDialog.Builder(context);
                AlertDialog alertDialog = builder.create();
                alertDialog.setCancelable(setCancel);
                alertDialog.getWindow().setBackgroundDrawableResource(android.R.color.transparent);
                alertDialog.setView(layout);
                alertDialog.show();
                if (onClickListener == null){
                    cv_ok.setOnClickListener(view -> alertDialog.dismiss());
                } else {
                    cv_ok.setOnClickListener(onClickListener);
                    alertDialog.dismiss();
                }
            });
        }

        public static void errorMessage(Context context, CharSequence message, boolean setCancel, View.OnClickListener onClickListener){
            ((Activity)context).runOnUiThread(() -> {
                View layout = LayoutInflater.from(context).inflate(R.layout.rdialog_layout, null);
                ImageView image = layout.findViewById(R.id.image);
                image.setImageResource(R.drawable.ic_baseline_error_24);
                TextView text = layout.findViewById(R.id.text);
                CardView cv_ok = layout.findViewById(R.id.cv_ok);
                text.setText(message);
                AlertDialog.Builder builder = new AlertDialog.Builder(context);
                AlertDialog alertDialog = builder.create();
                alertDialog.setCancelable(setCancel);
                alertDialog.getWindow().setBackgroundDrawableResource(android.R.color.transparent);
                alertDialog.setView(layout);
                alertDialog.show();
                if (onClickListener == null){
                    cv_ok.setOnClickListener(view -> alertDialog.dismiss());
                } else {
                    cv_ok.setOnClickListener(onClickListener);
                    alertDialog.dismiss();
                }
            });
        }

        public static void warningMessage(Context context, CharSequence message, boolean setCancel, View.OnClickListener onClickListener){
            ((Activity)context).runOnUiThread(() -> {
                View layout = LayoutInflater.from(context).inflate(R.layout.rdialog_layout, null);
                ImageView image = layout.findViewById(R.id.image);
                image.setImageResource(R.drawable.ic_baseline_warning_24);
                TextView text = layout.findViewById(R.id.text);
                CardView cv_ok = layout.findViewById(R.id.cv_ok);
                text.setText(message);
                AlertDialog.Builder builder = new AlertDialog.Builder(context);
                AlertDialog alertDialog = builder.create();
                alertDialog.setCancelable(setCancel);
                alertDialog.getWindow().setBackgroundDrawableResource(android.R.color.transparent);
                alertDialog.setView(layout);
                alertDialog.show();
                if (onClickListener == null){
                    cv_ok.setOnClickListener(view -> alertDialog.dismiss());
                } else {
                    cv_ok.setOnClickListener(onClickListener);
                }
            });
        }
        public static void checkMessage(Context context, CharSequence message, boolean setCancel, View.OnClickListener onClickListener){
            ((Activity)context).runOnUiThread(() -> {
                View layout = LayoutInflater.from(context).inflate(R.layout.rdialog_layout, null);
                ImageView image = layout.findViewById(R.id.image);
                image.setImageResource(R.drawable.ic_baseline_check_24);
                TextView text = layout.findViewById(R.id.text);
                CardView cv_ok = layout.findViewById(R.id.cv_ok);
                text.setText(message);
                AlertDialog.Builder builder = new AlertDialog.Builder(context);
                AlertDialog alertDialog = builder.create();
                alertDialog.setCancelable(setCancel);
                alertDialog.getWindow().setBackgroundDrawableResource(android.R.color.transparent);
                alertDialog.setView(layout);
                alertDialog.show();
                if (onClickListener == null){
                    cv_ok.setOnClickListener(view -> alertDialog.dismiss());
                } else {
                    cv_ok.setOnClickListener(onClickListener);
                }
            });
        }

}
