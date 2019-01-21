package com.example.xnpio.myface.fragments;

import android.os.Bundle;
import android.support.v4.app.Fragment;
import android.support.v7.widget.LinearLayoutManager;
import android.support.v7.widget.RecyclerView;
import android.util.Log;
import android.view.LayoutInflater;
import android.view.View;
import android.view.ViewGroup;
import android.widget.Button;
import android.widget.EditText;

import com.example.xnpio.myface.R;
import com.example.xnpio.myface.User;
import com.example.xnpio.myface.adapter.FriendsAdapter;
import com.google.firebase.database.DataSnapshot;
import com.google.firebase.database.DatabaseError;
import com.google.firebase.database.DatabaseReference;
import com.google.firebase.database.FirebaseDatabase;
import com.google.firebase.database.Query;
import com.google.firebase.database.ValueEventListener;
import com.google.gson.Gson;

import java.util.ArrayList;
import java.util.List;

public class SearchFragment extends Fragment {

    private User user;
    private EditText searchText;
    private View view;
    private List<User> users;
    private RecyclerView rvFriends;
    private Button searchBtn;

    private static final String USER_TAG = "userTag";

    @Override
    public View onCreateView(LayoutInflater inflater, ViewGroup container,
                             Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);

        view = inflater.inflate(R.layout.search_fargment, container,false);
        searchText = (EditText) view.findViewById(R.id.searchText);
        users = new ArrayList<User>();
        rvFriends = (RecyclerView) view.findViewById(R.id.rvSearch);
        searchBtn = (Button) view.findViewById(R.id.searchBtn);

        Gson gson = new Gson();
        this.user = gson.fromJson(getArguments().getSerializable(USER_TAG).toString(), User.class);

        searchBtn.setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View view) {
                DatabaseReference rootRef = FirebaseDatabase.getInstance().getReference();
                final DatabaseReference userRef = rootRef.child("users");
                ValueEventListener eventListener = new ValueEventListener() {
                    @Override
                    public void onDataChange(DataSnapshot dataSnapshot) {
                        Boolean found;
                        String search = searchText.getText().toString();
                        users = new ArrayList<User>();
                        for(DataSnapshot ds : dataSnapshot.getChildren()) {
                            String userName = ds.child("userName").getValue(String.class);
                            found = userName.contains(search);
                            if(found){
                                users.add(ds.getValue(User.class));
                            }
                        }
                        FriendsAdapter adapter = new FriendsAdapter(users, getContext(), user);
                        rvFriends.setAdapter(adapter);
                        rvFriends.setLayoutManager(new LinearLayoutManager(getContext()));
                    }

                    @Override
                    public void onCancelled(DatabaseError databaseError) {}
                };
                userRef.addListenerForSingleValueEvent(eventListener);
            }
        });

        return view;
    }


    public static SearchFragment newInstance(User user){
        SearchFragment fragment = new SearchFragment();
        Gson gson = new Gson();
        String userInJson = gson.toJson(user);
        Bundle bundle = new Bundle();
        bundle.putSerializable(USER_TAG, userInJson);
        fragment.setArguments(bundle);
        return fragment;
    }
}
