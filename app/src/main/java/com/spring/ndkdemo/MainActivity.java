package com.spring.ndkdemo;

import androidx.appcompat.app.AppCompatActivity;

import android.annotation.SuppressLint;
import android.content.Context;
import android.content.pm.PackageInfo;
import android.content.pm.PackageManager;
import android.content.pm.Signature;
import android.os.Bundle;
import android.util.Log;

public class MainActivity extends AppCompatActivity {
    public native String getStrFromJNI();

    //验证包名
    public native String verificationPackage(Object context);

    //验证签名
    public native String verificationSign(Object context);

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_main);
        Log.i("****", "sign:" + getSignature(MainActivity.this));
        Log.i("****", "jni:" + getStrFromJNI());
        Log.i("****", "Package:" + verificationPackage(MainActivity.this));
        Log.i("****", "Sign:" + verificationSign(MainActivity.this));
    }

    static {
        System.loadLibrary("my_lib");
    }

    public static String getSignature(Context context) {
        try {
            //获得签名
            @SuppressLint("PackageManagerGetSignatures")
            PackageInfo packageInfo = context.getPackageManager().getPackageInfo(context.getPackageName(), PackageManager.GET_SIGNATURES);
            Signature[] signatures = packageInfo.signatures;
            return signatures[0].toCharsString();
        } catch (PackageManager.NameNotFoundException e) {
            e.printStackTrace();
        }
        return null;
    }

}
