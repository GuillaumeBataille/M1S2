package com.example.tp2.Ex4;

import android.annotation.SuppressLint;
import android.content.Context;
import android.content.Intent;
import android.hardware.Sensor;
import android.hardware.SensorEvent;
import android.hardware.SensorEventListener;
import android.hardware.SensorManager;
import android.os.Bundle;
import android.view.View;
import android.widget.ImageView;
import android.widget.TextView;
import android.widget.Toast;
import androidx.appcompat.app.AppCompatActivity;
import com.example.tp2.MainActivity;
import com.example.tp2.R;

import org.w3c.dom.Text;


public class Ex4 extends AppCompatActivity implements SensorEventListener {
    private Sensor accelerometre;
    private SensorManager mSensorManager;
    private TextView value_x;
    private TextView value_y;
    private TextView direction_txt;

    private ImageView image;
    @SuppressLint("MissingInflatedId")
    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_ex4);
        image = findViewById(R.id.imageView);

        value_x = (TextView) findViewById(R.id.textView5);
        value_y = (TextView) findViewById(R.id.textView9);
        direction_txt = (TextView) findViewById(R.id.textView10);
    }

    @Override
    public void onSensorChanged(SensorEvent sensorEvent) {
    float x = sensorEvent.values[0];
    float y = sensorEvent.values[1];
        

    if(Math.abs(x) + Math.abs(y) > 1) {
        if (y > 0 && x >= 0)
            image.setRotation((float) Math.toDegrees(Math.atan(x / y) - Math.PI));
        else if (y > 0 && x < 0)
            image.setRotation((float) Math.toDegrees(Math.atan(x / y) + Math.PI));
        else if (y < 0)
            image.setRotation((float) Math.toDegrees(Math.atan(x / y)));
    }

    if(x > 1){
        direction_txt.setText("Gauche");
    }
    if(x < -1){
        direction_txt.setText("Droite");
    }
    if(y > 1){
        direction_txt.setText("Bas");
    }
    if(y < -1){
        direction_txt.setText("Haut");
    }
    value_x.setText(Float.toString(x));
    value_y.setText(Float.toString(y));
    }

    @Override
    public void onAccuracyChanged(Sensor sensor, int i) {

    }
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
        Intent intention= new Intent(Ex4.this, MainActivity.class);
        startActivity(intention);
    }
}