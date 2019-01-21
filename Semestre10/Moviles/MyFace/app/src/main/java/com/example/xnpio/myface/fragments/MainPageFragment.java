package com.example.xnpio.myface.fragments;

import android.app.ProgressDialog;
import android.graphics.Bitmap;
import android.graphics.drawable.Drawable;
import android.net.Uri;
import android.os.Bundle;
import android.support.annotation.NonNull;
import android.support.v4.app.Fragment;
import android.support.v7.widget.LinearLayoutManager;
import android.support.v7.widget.RecyclerView;
import android.view.LayoutInflater;
import android.view.View;
import android.view.ViewGroup;

import com.example.xnpio.myface.Publication;
import com.example.xnpio.myface.R;
import com.example.xnpio.myface.User;
import com.example.xnpio.myface.adapter.FriendsAdapter;
import com.example.xnpio.myface.adapter.PublicationsAdapter;
import com.example.xnpio.myface.grid.ImageItem;
import com.example.xnpio.myface.retrofit.Api;
import com.google.android.gms.tasks.OnFailureListener;
import com.google.android.gms.tasks.OnSuccessListener;
import com.google.firebase.storage.FirebaseStorage;
import com.google.firebase.storage.StorageReference;
import com.google.gson.Gson;
import com.squareup.picasso.Picasso;
import com.squareup.picasso.Target;

import java.util.ArrayList;
import java.util.List;

import retrofit2.Call;
import retrofit2.Callback;
import retrofit2.Response;
import retrofit2.Retrofit;
import retrofit2.converter.gson.GsonConverterFactory;

public class MainPageFragment extends Fragment {


    private View view;
    private RecyclerView rvFriends;
    private User user;
    private List<Publication> publicationList;
    private FirebaseStorage storage;
    private StorageReference storageReference;
    private Retrofit retrofit;
    private Api api;
    private int actualI;
    private int actualU;
    private User tempUser;
    private ProgressDialog progressDialog;

    private static final String USER_TAG = "userTag";

    private Target target = new Target() {
        @Override
        public void onBitmapLoaded(Bitmap bitmap, Picasso.LoadedFrom from) {
            publicationList.add(new Publication(tempUser, bitmap, tempUser.getUid() + ":" + Integer.toString(actualI)));
            actualI += 1;
            getImages();

        }

        @Override
        public void onBitmapFailed(Drawable errorDrawable) {
        }

        @Override
        public void onPrepareLoad(Drawable placeHolderDrawable) {
        }
    };

    @Override
    public View onCreateView(LayoutInflater inflater, ViewGroup container,
                             Bundle savedInstanceState) {
        // Inflate the layout for this fragment

        view = inflater.inflate(R.layout.content_main_page, container, false);
        rvFriends = (RecyclerView) view.findViewById(R.id.rvImages);
        retrofit = new Retrofit.Builder()
                .baseUrl("https://myface-cf337.firebaseio.com")//url of firebase app
                .addConverterFactory(GsonConverterFactory.create())//use for convert JSON file into object
                .build();

        api = retrofit.create(Api.class);
        storage = FirebaseStorage.getInstance();
        storageReference = storage.getReference();

        progressDialog = new ProgressDialog(getContext());
        progressDialog.setTitle("Actualizando...");
        progressDialog.show();

        Gson gson = new Gson();
        this.user = gson.fromJson(getArguments().getSerializable(USER_TAG).toString(), User.class);
        actualU = 0;
        publicationList = new ArrayList<Publication>();
        getPublications();

        return view;
    }

    public static MainPageFragment newInstance(User user){
        MainPageFragment fragment = new MainPageFragment();
        Gson gson = new Gson();
        String userInJson = gson.toJson(user);
        Bundle bundle = new Bundle();
        bundle.putSerializable(USER_TAG, userInJson);
        fragment.setArguments(bundle);
        return fragment;
    }

    private void getPublications(){
        if(actualU == user.getFriends().size()){
            PublicationsAdapter adapter = new PublicationsAdapter(publicationList, getContext(), user);
            rvFriends.setAdapter(adapter);
            rvFriends.setLayoutManager(new LinearLayoutManager(getContext()));
            progressDialog.dismiss();
        }
        else{
            Call<User> call2=api.getData(user.getFriends().get(actualU));
            call2.enqueue(new Callback<User>() {
                @Override
                public void onResponse(Call<User> call, Response<User> response) {
                    tempUser = response.body();
                    actualI = 0;
                    getImages();
                }

                @Override
                public void onFailure(Call<User> call, Throwable t) {

                }
            });
        }
    }

    public void getImages(){
        if(actualI == tempUser.getActualImageId()){
            actualU += 1;
            getPublications();
        }
        else{
            storageReference.child(tempUser.getUid() + "/images/" + Integer.toString(actualI)).getDownloadUrl().addOnSuccessListener(new OnSuccessListener<Uri>() {
                @Override
                public void onSuccess(Uri uri) {
                    Picasso.with(getContext()).load(uri).into(target);

                    //Toast.makeText(MainPage.this, "Cover downloaded", Toast.LENGTH_SHORT).show();

                }
            }).addOnFailureListener(new OnFailureListener() {
                @Override
                public void onFailure(@NonNull Exception exception) {
                    //Toast.makeText(MainPage.this, "Fail Cover" + exception.toString(), Toast.LENGTH_SHORT).show();
                }
            });
        }
    }




}