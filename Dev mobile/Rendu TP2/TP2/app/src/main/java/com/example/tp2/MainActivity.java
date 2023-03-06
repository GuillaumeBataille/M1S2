package com.example.tp2;

import android.content.Intent;
import android.os.Bundle;
import android.view.View;
import android.widget.Button;
import android.widget.LinearLayout;

import androidx.appcompat.app.AppCompatActivity;

import com.example.tp2.Ex1.Ex1;
import com.example.tp2.Ex2.Ex2;
import com.example.tp2.Ex3.Ex3;
import com.example.tp2.Ex4.Ex4;
import com.example.tp2.Ex5.Ex5;
import com.example.tp2.Ex6.Ex6;


public class MainActivity extends AppCompatActivity {

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);

        LinearLayout linearLayout = new LinearLayout(this);
        linearLayout.setOrientation(LinearLayout.VERTICAL);


        Button btEx1 = new Button(this);
        btEx1.setText("Ex1");

        Button btEx2 = new Button(this);
        btEx2.setText("Ex2");

        Button btEx3 = new Button(this);
        btEx3.setText("Ex3");

        Button btEx4 = new Button(this);
        btEx4.setText("Ex4");

        Button btEx5 = new Button(this);
        btEx5.setText("Ex5");

        Button btEx6 = new Button(this);
        btEx6.setText("Ex6");

        btEx1.setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View v) {
                Intent intention = new Intent(MainActivity.this, Ex1.class);
                startActivity(intention);
            }
        });

        btEx2.setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View v) {
                Intent intention = new Intent(MainActivity.this, Ex2.class);
                startActivity(intention);
            }
        });

        btEx3.setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View v) {
                Intent intention = new Intent(MainActivity.this, Ex3.class);
                startActivity(intention);
            }
        });

        btEx4.setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View v) {
                Intent intention = new Intent(MainActivity.this, Ex4.class);
                startActivity(intention);
            }
        });

        btEx5.setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View v) {
                Intent intention = new Intent(MainActivity.this, Ex5.class);
                startActivity(intention);
            }
        });

        btEx6.setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View v) {
                Intent intention = new Intent(MainActivity.this, Ex6.class);
                startActivity(intention);
            }
        });

        linearLayout.addView(btEx1);
        linearLayout.addView(btEx2);
        linearLayout.addView(btEx3);
        linearLayout.addView(btEx4);
        linearLayout.addView(btEx5);
        linearLayout.addView(btEx6);

        setContentView(linearLayout);
    }
}