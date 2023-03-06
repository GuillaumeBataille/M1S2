package com.example.tp2.Ex2;

import android.content.Context;
import android.content.Intent;
import android.hardware.Sensor;
import android.hardware.SensorManager;
import android.os.Bundle;
import android.view.View;
import android.widget.ListView;
import androidx.appcompat.app.AppCompatActivity;
import com.example.tp2.MainActivity;
import com.example.tp2.R;

import java.util.ArrayList;
import java.util.HashMap;
import java.util.List;
import java.util.Map;

import static android.hardware.Sensor.*;

public class Ex2 extends AppCompatActivity {
    private SensorManager mSensorManager;
    private List<Sensor> sensorsList;
    private List<String> sensorList;
    private Map<Integer,String> absentSensors;
    private ListView listView ;

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_ex2);

        listView = (ListView)findViewById(R.id.listView);

        sensorList = new ArrayList<>();
        sensorList.add("Capteurs absents");

        absentSensors = new HashMap<>();
        absentSensors.putIfAbsent(TYPE_ACCELEROMETER,"TYPE_ACCELEROMETER");
        absentSensors.putIfAbsent(TYPE_AMBIENT_TEMPERATURE,"TYPE_AMBIENT_TEMPERATURE");
        absentSensors.putIfAbsent(TYPE_GRAVITY,"TYPE_GRAVITY");
        absentSensors.putIfAbsent(TYPE_GYROSCOPE,"YPE_GYROSCOPE");
        absentSensors.putIfAbsent(TYPE_LIGHT,"TYPE_LIGHT");
        absentSensors.putIfAbsent(TYPE_LINEAR_ACCELERATION,"TYPE_LINEAR_ACCELERATION");
        absentSensors.putIfAbsent(TYPE_MAGNETIC_FIELD,"TYPE_MAGNETIC_FIELD");
        absentSensors.putIfAbsent(TYPE_PRESSURE,"TYPE_PRESSURE");
        absentSensors.putIfAbsent(TYPE_PROXIMITY,"TYPE_PROXIMITY");
        absentSensors.putIfAbsent(TYPE_RELATIVE_HUMIDITY,"TYPE_RELATIVE_HUMIDITY");
        absentSensors.putIfAbsent(TYPE_ROTATION_VECTOR,"TYPE_ROTATION_VECTOR");


        SensorManager mSensorManager = (SensorManager) getSystemService(Context.SENSOR_SERVICE);
        sensorsList = mSensorManager.getSensorList(Sensor.TYPE_ALL);

        for ( Integer i : absentSensors.keySet() ){
            int j = 0;
            boolean check = false;

            while (!check && j < sensorsList.size() ){
                if (sensorsList.get(j).getType()==i){
                    check = true;
                } else {
                    j++;
                }
            }

            if (!check){
                sensorList.add(absentSensors.get(i));
            }
        }

        Ex2_adaptator adapter = new Ex2_adaptator(Ex2.this, sensorList);
        listView.setAdapter(adapter);
    }

    public void back(View view){
        Intent intention= new Intent(Ex2.this, MainActivity.class);
        startActivity(intention);
    }

}