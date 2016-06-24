/*
 * Copyright 2015 The Android Open Source Project
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *      http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */
package com.sample.helper;

import android.app.NativeActivity;
import android.support.v7.widget.RecyclerView;
import android.support.v7.widget.RecyclerView.ViewHolder;
import android.view.LayoutInflater;
import android.view.View;
import android.view.ViewGroup;
import android.widget.TextView;
import android.support.v7.widget.GridLayoutManager;
import android.support.v7.widget.RecyclerView;
import android.support.v7.widget.RecyclerView.LayoutManager;
import android.support.v7.widget.RecyclerView.State;
import android.support.v7.widget.StaggeredGridLayoutManager;
import android.support.v7.widget.LinearLayoutManager;
import android.util.Log;
import java.lang.String;
import java.util.ArrayList;


/*
 * Java UI RecyclerView implementation
 */
public class JUIRecyclerView extends RecyclerView {
    private MenuAdapter menuAdapter;
    private NativeActivity mActivity;
    private ArrayList<String> mDatas =new ArrayList<>();

    public JUIForwardingPopupWindow getDummyWindow() {
        return null;
    }

    public JUIRecyclerView(final NativeActivity activity) {
        super(activity);
        mActivity = activity;
        //initMenu(false);
        Log.d("test","tew......");
    }

    public void initMenu(boolean isList){
        mDatas.add("test1");
        mDatas.add("test2");
        mDatas.add("test3");
        mDatas.add("test4");
        mDatas.add("test5");
        mDatas.add("test6");
        Log.d("test", "initMenu");
        menuAdapter = new MenuAdapter(mActivity,mDatas);
        if(isList)
            setLayoutManager(new LinearLayoutManager(mActivity));
        else
            setLayoutManager(new StaggeredGridLayoutManager(4, StaggeredGridLayoutManager.VERTICAL));
        setAdapter(menuAdapter);
    }
}


class MenuAdapter extends RecyclerView.Adapter<MenuAdapter.MyViewHolder>
{
    NativeActivity mActivity;
    private ArrayList<String> mDatas;

    public MenuAdapter(final NativeActivity activity, ArrayList<String> data) {
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

    class MyViewHolder extends ViewHolder
    {

        TextView tv;

        public MyViewHolder(View view)
        {
            super(view);
            tv = (TextView) view.findViewById(android.R.id.text1);
        }
    }
}