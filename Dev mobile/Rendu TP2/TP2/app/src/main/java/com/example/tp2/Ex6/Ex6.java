package com.example.tp2.Ex6;

import android.annotation.SuppressLint;
import android.content.Context;
import android.content.Intent;
import android.hardware.Sensor;
import android.hardware.SensorEvent;
import android.hardware.SensorEventListener;
import android.hardware.SensorManager;
import android.media.AudioManager;
import android.os.Bundle;
import android.view.View;
import android.widget.TextView;
import android.widget.Toast;
import androidx.appcompat.app.AppCompatActivity;
import com.example.tp2.MainActivity;
import com.example.tp2.R;

public class Ex6 extends AppCompatActivity implements SensorEventListener {
    private Sensor proximitySensor;
    private SensorManager mSensorManager;
    private TextView txt;


    @SuppressLint("MissingInflatedId")
    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_ex6);
        txt = (TextView) findViewById(R.id.textView12);
    }


    @Override
    public void onSensorChanged(SensorEvent sensorEvent) {
        txt.setText(Float.toString( sensorEvent.values[0]));
    }

    @Override
    public void onAccuracyChanged(Sensor sensor, int i) {}

    @Override
    protected void onPause() {
        mSensorManager.unregisterListener(this, proximitySensor);
        super.onPause();
    }
    @Override
    protected void onResume() {
        super.onResume();

        mSensorManager = (SensorManager) getSystemService(Context.SENSOR_SERVICE);

        if (mSensorManager.getDefaultSensor(Sensor.TYPE_PROXIMITY) !=null){
            proximitySensor = mSensorManager.getDefaultSensor(Sensor.TYPE_PROXIMITY);
            mSensorManager.registerListener(this, proximitySensor, SensorManager.SENSOR_STATUS_ACCURACY_LOW);
        } else {
            Toast.makeText(this,"capteur de proximité manquant", Toast.LENGTH_LONG).show();
        }
    }

    public void back(View view){
        Intent intention= new Intent(Ex6.this, MainActivity.class);
        startActivity(intention);
    }

}