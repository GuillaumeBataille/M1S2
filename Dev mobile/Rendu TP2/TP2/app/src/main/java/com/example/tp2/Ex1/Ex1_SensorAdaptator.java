package com.example.tp2.Ex1;

import android.content.Context;
import android.view.LayoutInflater;
import android.view.View;
import android.view.ViewGroup;
import android.widget.BaseAdapter;
import android.widget.TextView;

import com.example.tp2.R;

import java.util.List;

public class Ex1_SensorAdaptator extends BaseAdapter {

    private Context context;
    private List<List<String>> list;
    private LayoutInflater inflater;

    public Ex1_SensorAdaptator(Context context, List<List<String>> list) {
        this.context = context;
        this.list = list;
        this.inflater = LayoutInflater.from(context);
    }

    @Override
    public int getCount() {
        return list.size();
    }

    @Override
    public List<String> getItem(int i) {
        return list.get(i);
    }

    @Override
    public long getItemId(int i) {
        return 0;
    }

    @Override
    public View getView(int i, View view, ViewGroup viewGroup) {

        view = inflater.inflate(R.layout.ex1_adaptator,null);
        TextView Text1 = view.findViewById(R.id.TextView1);
        TextView Text2 = view.findViewById(R.id.TextView2);
        TextView Text3 = view.findViewById(R.id.TextView3);
        TextView Text4 = view.findViewById(R.id.TextView4);
        TextView Text5 = view.findViewById(R.id.TextView5);
        TextView Text6 = view.findViewById(R.id.TextView6);
        TextView Text7 = view.findViewById(R.id.TextView7);
        TextView Text8 = view.findViewById(R.id.TextView8);

        List<String> s = getItem(i);
            Text1.setText(s.get(0));
            Text2.setText(s.get(1));
            Text3.setText(s.get(2));
            Text4.setText(s.get(3));
            Text5.setText(s.get(4));
            Text6.setText(s.get(5));
            Text7.setText(s.get(6));
            Text8.setText(s.get(7));

        return view;
    }
}
