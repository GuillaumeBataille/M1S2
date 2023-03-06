package com.example.tp2.Ex3;

import android.annotation.SuppressLint;
import android.content.Context;
import android.content.Intent;
import android.graphics.Color;
import android.hardware.Sensor;
import android.hardware.SensorEvent;
import android.hardware.SensorEventListener;
import android.hardware.SensorManager;
import android.os.Bundle;
import android.view.View;
import android.widget.TextView;
import android.widget.Toast;
import androidx.appcompat.app.AppCompatActivity;
import androidx.constraintlayout.widget.ConstraintLayout;
import com.example.tp2.MainActivity;
import com.example.tp2.R;

public class Ex3 extends AppCompatActivity implements SensorEventListener {

    private float value;
    private ConstraintLayout color;
    private TextView avg_text;
    private TextView value_x;
    private TextView value_y;
    private TextView value_z;

    private Sensor accelerometre;
    private SensorManager mSensorManager;


    private float vert,noir,rouge;

    @SuppressLint("MissingInflatedId")
    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_ex3);

        avg_text = (TextView) findViewById(R.id.textView);
        value_x = (TextView) findViewById(R.id.textView2);
        value_y = (TextView) findViewById(R.id.textView3);
        value_z = (TextView) findViewById(R.id.textView4);

        color = (ConstraintLayout) findViewById(R.id.fond) ;
        vert = 15f;
        noir = 20f;
        rouge = 30f;
    }

    @Override
    public void onSensorChanged(SensorEvent sensorEvent) {
        float x = sensorEvent.values[0];
        float y = sensorEvent.values[1];
        float z = sensorEvent.values[2];

        value = Math.abs(x)+Math.abs(y) +Math.abs(z);

        if(value < vert )
            color.setBackgroundColor(Color.GREEN);
        else if(value < noir)
            color.setBackgroundColor(Color.BLACK);
        else if(value < rouge)
            color.setBackgroundColor(Color.RED);

        avg_text.setText(Float.toString(value));
        value_x.setText(Float.toString(x));
        value_y.setText(Float.toString(y));
        value_z.setText(Float.toString(z));
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
    }

    public void back(View view){
        Intent intention= new Intent(Ex3.this, MainActivity.class);
        startActivity(intention);
    }
}