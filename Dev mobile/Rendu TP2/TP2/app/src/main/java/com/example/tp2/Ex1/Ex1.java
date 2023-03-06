package com.example.tp2.Ex1;

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
import java.util.List;

public class Ex1 extends AppCompatActivity {


    ListView listView ;
    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_ex1);

        listView = (ListView)findViewById(R.id.listView);
        SensorManager sensorManager = (SensorManager) getSystemService(Context.SENSOR_SERVICE);

        List<Sensor> sensorsList = sensorManager.getSensorList(Sensor.TYPE_ALL);


        List<List<String>> list = new ArrayList<>();

        ArrayList<String> categories = new ArrayList<>();
        categories.add("Nom");
        categories.add("Type");
        categories.add("Version");
        categories.add("Resolution");
        categories.add("Puissance");
        categories.add("Vendeur");
        categories.add("Vmax");
        categories.add("delais");
        list.add(categories);


        for (Sensor sensor : sensorsList) {
            ArrayList<String> line = new ArrayList<>();
            line.add(sensor.getName());
            line.add(sensor.getType()+"");
            line.add(sensor.getVersion()+"");
            line.add(sensor.getResolution()+"");
            line.add(sensor.getPower()+"");
            line.add(sensor.getVendor());
            line.add(sensor.getMaximumRange()+"");
            line.add(sensor.getMinDelay()+"");
            list.add(line);
        }

        Ex1_SensorAdaptator adapter = new Ex1_SensorAdaptator(Ex1.this,list);
        listView.setAdapter(adapter);
    }

    public void back(View view){
        Intent intention= new Intent(Ex1.this, MainActivity.class);
        startActivity(intention);
    }
}