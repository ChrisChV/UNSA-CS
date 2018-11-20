package com.example.xnpio.myface.fragments;

import android.support.v4.app.Fragment;
import android.content.Intent;
import android.os.Bundle;
import android.support.v4.app.FragmentManager;
import android.support.v7.widget.LinearLayoutManager;
import android.support.v7.widget.RecyclerView;
import android.util.Log;
import android.view.LayoutInflater;
import android.view.View;
import android.view.ViewGroup;

import com.example.xnpio.myface.LoginPage;
import com.example.xnpio.myface.MainPage;
import com.example.xnpio.myface.R;
import com.example.xnpio.myface.User;
import com.example.xnpio.myface.adapter.FriendsAdapter;
import com.example.xnpio.myface.retrofit.Api;
import com.google.gson.Gson;

import java.util.ArrayList;
import java.util.List;

import retrofit2.Call;
import retrofit2.Callback;
import retrofit2.Response;
import retrofit2.Retrofit;
import retrofit2.converter.gson.GsonConverterFactory;

public class FriendListFragment extends Fragment {

    private RecyclerView rvFriends;
    private List<User> friends;
    private List<String> actualList;
    private User user;
    private View view;
    private Retrofit retrofit;
    private Api api;
    private int var;

    private static final String USER_TAG = "userTag";
    private static final String VAR_TAG = "varTag";

    public static final int FRIEND_VAR = 0;
    public static final int PENDING_VAR = 1;
    public static final int PENDING_RESPONSE = 2;


    @Override
    public View onCreateView(LayoutInflater inflater, ViewGroup container,
                             Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);

        retrofit = new Retrofit.Builder()
                .baseUrl("https://myface-cf337.firebaseio.com")//url of firebase app
                .addConverterFactory(GsonConverterFactory.create())//use for convert JSON file into object
                .build();

        api = retrofit.create(Api.class);
        friends = new ArrayList<User>();
        view = inflater.inflate(R.layout.friend_list_fragment, container,false);
        rvFriends = (RecyclerView) view.findViewById(R.id.rvFriends);
        Gson gson = new Gson();
        this.user = gson.fromJson(getArguments().getSerializable(USER_TAG).toString(), User.class);
        this.var = getArguments().getInt(VAR_TAG);
        if(var == FRIEND_VAR) actualList = user.getFriends();
        if(var == PENDING_VAR) actualList = user.getPendings();
        if(var == PENDING_RESPONSE) actualList = user.getPendingsResponse();
        getFriends();

        return view;
    }

    public static FriendListFragment newInstance(User user, int var){
        FriendListFragment fragment = new FriendListFragment();
        Gson gson = new Gson();
        String userInJson = gson.toJson(user);
        Bundle bundle = new Bundle();
        bundle.putSerializable(USER_TAG, userInJson);
        bundle.putInt(VAR_TAG, var);
        fragment.setArguments(bundle);
        return fragment;
    }

    private void getFriends(){
        for(int i = 0; i < actualList.size(); i++){
            if(i == actualList.size() - 1){
                Call<User> call2=api.getData(actualList.get(i));
                call2.enqueue(new Callback<User>() {
                    @Override
                    public void onResponse(Call<User> call, Response<User> response) {
                        User nUser = response.body();
                        friends.add(nUser);
                        FriendsAdapter adapter = new FriendsAdapter(friends, getContext(), user);
                        rvFriends.setAdapter(adapter);
                        rvFriends.setLayoutManager(new LinearLayoutManager(getContext()));
                    }

                    @Override
                    public void onFailure(Call<User> call, Throwable t) {

                    }
                });
            }
            else{
                Call<User> call2=api.getData(actualList.get(i));
                call2.enqueue(new Callback<User>() {
                    @Override
                    public void onResponse(Call<User> call, Response<User> response) {
                        User nUser = response.body();
                        friends.add(nUser);
                    }

                    @Override
                    public void onFailure(Call<User> call, Throwable t) {

                    }
                });
            }

        }

    }

}
