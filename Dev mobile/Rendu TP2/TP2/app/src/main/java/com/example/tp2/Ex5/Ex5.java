package com.example.tp2.Ex5;

import android.annotation.SuppressLint;
import android.content.Context;
import android.content.Intent;
import android.hardware.Sensor;
import android.hardware.SensorEvent;
import android.hardware.SensorEventListener;
import android.hardware.SensorManager;
import android.hardware.camera2.CameraAccessException;
import android.hardware.camera2.CameraManager;
import android.os.Bundle;
import android.os.CountDownTimer;
import android.os.Handler;
import android.os.SystemClock;
import android.view.View;
import android.widget.ImageView;
import android.widget.TextView;
import android.widget.Toast;
import androidx.appcompat.app.AppCompatActivity;
import com.example.tp2.MainActivity;
import com.example.tp2.R;

public class Ex5 extends AppCompatActivity implements SensorEventListener{

    private Sensor accelerometre;

    private SensorManager mSensorManager;
    private CameraManager camManager;
    private String cameraId;

    private TextView switch_txt;
    private boolean lightSwitch = true;
    @SuppressLint("MissingInflatedId")
    @Override
    protected void onCreate(Bundle savedInstanceState){
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_ex5);
        switch_txt = (TextView)findViewById(R.id.textView11);
    }

    @Override
    public void onSensorChanged(SensorEvent sensorEvent) {
        float x = sensorEvent.values[0];
        float y = sensorEvent.values[1];

        if(Math.abs(x) + Math.abs(y) > 30) {

            try {
                camManager.setTorchMode(cameraId, lightSwitch);
            } catch (CameraAccessException ex) {
                throw new RuntimeException(ex);
            }

            if(lightSwitch){
                lightSwitch = false;
                switch_txt.setText("allumé");
            } else {
                lightSwitch = true;
                switch_txt.setText("éteint");
            }
        }
    }

    @Override
    public void onAccuracyChanged(Sensor sensor, int i) {}

    @Override
    protected void onPause() {
        mSensorManager.unregisterListener(this, accelerometre);
        super.onPause();
    }
    @Override
    protected void onResume() {
        super.onResume();
        mSensorManager = (SensorManager) getSystemService(Context.SENSOR_SERVICE);

        if (mSensorManager.getDefaultSensor(Sensor.TYPE_ACCELEROMETER) !=null){
            accelerometre = mSensorManager.getDefaultSensor(Sensor.TYPE_ACCELEROMETER);
            mSensorManager.registerListener(this, accelerometre, SensorManager.SENSOR_DELAY_NORMAL);
        } else {
            Toast.makeText(this,"pas d'accéléromètre", Toast.LENGTH_LONG).show();
        }

        camManager = (CameraManager) getSystemService(Context.CAMERA_SERVICE);
        try {
            cameraId = camManager.getCameraIdList()[0];
        } catch (CameraAccessException e) {
            Toast.makeText(this,"Flash error", Toast.LENGTH_LONG).show();
            e.printStackTrace();
        }
    }

    public void back(View view){
        Intent intention= new Intent(Ex5.this, MainActivity.class);
        startActivity(intention);
    }
}