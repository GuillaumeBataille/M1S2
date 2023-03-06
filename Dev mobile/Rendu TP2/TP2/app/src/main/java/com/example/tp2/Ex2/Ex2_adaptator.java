package com.example.tp2.Ex2;

import android.content.Context;
import android.view.LayoutInflater;
import android.view.View;
import android.view.ViewGroup;
import android.widget.BaseAdapter;
import android.widget.TextView;

import com.example.tp2.R;

import java.util.List;

public class Ex2_adaptator extends BaseAdapter {

    private Context context;
    private List<String> list;
    private LayoutInflater inflater;

    public Ex2_adaptator(Context context, List<String> list) {
        this.context = context;
        this.list = list;
        this.inflater = LayoutInflater.from(context);
    }

    /*
    Getter
     */
    @Override
    public int getCount() {
        return list.size();
    }

    @Override
    public String getItem(int i) {
        return list.get(i);
    }

    @Override
    public long getItemId(int i) {
        return 0;
    }

    @Override
    public View getView(int i, View view, ViewGroup viewGroup) {

        view = inflater.inflate(R.layout.ex2_adaptator,null);
        TextView Text1 = view.findViewById(R.id.textexo2);



        String s = getItem(i);
        Text1.setText(s);



        return view;
    }
}