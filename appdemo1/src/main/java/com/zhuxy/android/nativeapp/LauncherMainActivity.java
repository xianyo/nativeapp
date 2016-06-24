package com.zhuxy.android.nativeapp;

import android.annotation.SuppressLint;
import android.app.Activity;
import android.content.Intent;
import android.os.Bundle;
import android.support.v7.widget.LinearLayoutManager;
import android.support.v7.widget.RecyclerView;
import android.support.v7.widget.StaggeredGridLayoutManager;
import android.view.LayoutInflater;
import android.view.View;
import android.view.ViewGroup;
import android.widget.Button;
import android.widget.TextView;

import java.util.ArrayList;

/**
 * Created by zhuxy on 16-6-11.
 */
public class LauncherMainActivity extends Activity {
    private MenuAdapter menuAdapter;
    private Activity mActivity;
    private ArrayList<String> mDatas =new ArrayList<>();

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        RecyclerView mRecyclerView;
        // Hide toolbar
        int SDK_INT = android.os.Build.VERSION.SDK_INT;
        if (SDK_INT >= 19) {
            setImmersiveSticky();

            View decorView = getWindow().getDecorView();
            decorView
                    .setOnSystemUiVisibilityChangeListener(new View.OnSystemUiVisibilityChangeListener() {
                        @Override
                        public void onSystemUiVisibilityChange(int visibility) {
                            setImmersiveSticky();
                        }
                    });
        }

        setContentView(R.layout.activity_main);

        Button btn = (Button)findViewById(R.id.dummy_button);
        btn.setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View view) {
                Intent intentx = new Intent(LauncherMainActivity.this, LauncherNativeActivity.class);
                intentx.putExtra("apppath", "/data/lib/libnativeappdemo1.so");
                startActivity(intentx);
            }
        });

        Button btn2 = (Button)findViewById(R.id.dummy_button2);
        btn2.setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View view) {
                Intent intentx = new Intent(LauncherMainActivity.this,LauncherNativeActivity.class);
                intentx.putExtra("apppath","/data/lib/libnativeappdemo2.so");
                startActivity(intentx);
            }
        });

        mRecyclerView = (RecyclerView)findViewById(R.id.fullscreen_content);

        mDatas.add("test1");
        mDatas.add("test2");
        mDatas.add("test3");
        mDatas.add("test4");
        mDatas.add("test5");
        mDatas.add("test6");

        menuAdapter = new MenuAdapter(this,mDatas);
        if(true)
            mRecyclerView.setLayoutManager(new LinearLayoutManager(this));
        else
            mRecyclerView.setLayoutManager(new StaggeredGridLayoutManager(4, StaggeredGridLayoutManager.VERTICAL));
        mRecyclerView.setAdapter(menuAdapter);
    }


    @SuppressLint("InlinedApi")
    @SuppressWarnings("deprecation")
    protected void onResume() {
        super.onResume();

        // Hide toolbar
        int SDK_INT = android.os.Build.VERSION.SDK_INT;
        if (SDK_INT >= 11 && SDK_INT < 14) {
            getWindow().getDecorView().setSystemUiVisibility(
                    View.STATUS_BAR_HIDDEN);
        } else if (SDK_INT >= 14 && SDK_INT < 19) {
            getWindow().getDecorView().setSystemUiVisibility(
                    View.SYSTEM_UI_FLAG_FULLSCREEN
                            | View.SYSTEM_UI_FLAG_LOW_PROFILE);
        } else if (SDK_INT >= 19) {
            setImmersiveSticky();
        }

    }

    @SuppressLint("InlinedApi")
    void setImmersiveSticky() {
        View decorView = getWindow().getDecorView();
        decorView.setSystemUiVisibility(View.SYSTEM_UI_FLAG_FULLSCREEN
                | View.SYSTEM_UI_FLAG_HIDE_NAVIGATION
                | View.SYSTEM_UI_FLAG_IMMERSIVE_STICKY
                | View.SYSTEM_UI_FLAG_LAYOUT_FULLSCREEN
                | View.SYSTEM_UI_FLAG_LAYOUT_HIDE_NAVIGATION
                | View.SYSTEM_UI_FLAG_LAYOUT_STABLE);
    }

    protected void onPause() {
        super.onPause();

    }


}


class MenuAdapter extends RecyclerView.Adapter<MenuAdapter.MyViewHolder>
{
    Activity mActivity;
    private ArrayList<String> mDatas;

    public MenuAdapter(final Activity activity, ArrayList<String> data) {
        mActivity = activity;
        mDatas = data;
    }

    @Override
    public MyViewHolder onCreateViewHolder(ViewGroup parent, int viewType)
    {
        MyViewHolder holder = new MyViewHolder(LayoutInflater.from(
                mActivity).inflate(android.R.layout.simple_list_item_1, parent,
                false));
        return holder;
    }

    @Override
    public void onBindViewHolder(MyViewHolder holder, int position)
    {
        holder.tv.setText(mDatas.get(position));
    }

    @Override
    public int getItemCount()
    {
        return mDatas.size();
    }

    class MyViewHolder extends RecyclerView.ViewHolder
    {

        TextView tv;

        public MyViewHolder(View view)
        {
            super(view);
            tv = (TextView) view.findViewById(android.R.id.text1);
        }
    }
}
