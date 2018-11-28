package com.example.xnpio.myface.fragments;

import android.app.ProgressDialog;
import android.graphics.Bitmap;
import android.graphics.BitmapFactory;
import android.os.Bundle;
import android.support.v4.app.Fragment;
import android.support.v7.widget.LinearLayoutManager;
import android.support.v7.widget.RecyclerView;
import android.util.Log;
import android.view.LayoutInflater;
import android.view.View;
import android.view.ViewGroup;
import android.widget.ImageView;
import android.widget.TextView;

import com.example.xnpio.myface.Comment;
import com.example.xnpio.myface.FirebasePublication;
import com.example.xnpio.myface.Publication;
import com.example.xnpio.myface.R;
import com.example.xnpio.myface.User;
import com.example.xnpio.myface.adapter.CommentAdapter;
import com.example.xnpio.myface.adapter.PublicationsAdapter;
import com.example.xnpio.myface.grid.ImageItem;
import com.example.xnpio.myface.retrofit.Api;
import com.google.gson.Gson;

import java.io.ByteArrayOutputStream;
import java.util.ArrayList;
import java.util.List;

import retrofit2.Call;
import retrofit2.Callback;
import retrofit2.Response;
import retrofit2.Retrofit;
import retrofit2.converter.gson.GsonConverterFactory;

public class DetailsFragment extends Fragment {

    private View view;
    private ImageView image;
    private Publication publication;
    private FirebasePublication fbPublication;
    private RecyclerView rvComments;
    private Retrofit retrofit;
    private Api api;
    private int actualI;
    private List<Comment> comments;
    private User actualUser;
    private ProgressDialog progressDialog;

    private static final String USER_TAG = "userTag";
    private static final String USER2_TAG = "user2Tag";

    @Override
    public View onCreateView(LayoutInflater inflater, ViewGroup container,
                             Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);

        view = inflater.inflate(R.layout.details_fragment, container,false);

        Gson gson = new Gson();


        this.publication = gson.fromJson(getArguments().getSerializable(USER_TAG).toString(), Publication.class);
        this.actualUser = gson.fromJson(getArguments().getSerializable(USER2_TAG).toString(), User.class);
        int index = publication.getUser().findPublication(publication.getImageId());
        this.fbPublication = publication.getUser().getPublications().get(index);

        image = (ImageView) view.findViewById(R.id.pblImage);
        image.setImageBitmap(publication.getPublicationImg());

        rvComments = (RecyclerView) view.findViewById(R.id.rvComments);
        retrofit = new Retrofit.Builder()
                .baseUrl("https://myface-cf337.firebaseio.com")//url of firebase app
                .addConverterFactory(GsonConverterFactory.create())//use for convert JSON file into object
                .build();

        api = retrofit.create(Api.class);
        actualI = 0;

        progressDialog = new ProgressDialog(getContext());
        progressDialog.setTitle("Actualizando...");
        progressDialog.show();

        comments = new ArrayList<Comment>();

        getComments();


        return view;
    }

    public static DetailsFragment newInstance(Publication publication, User actualuser){
        DetailsFragment fragment = new DetailsFragment();
        Gson gson = new Gson();
        String userInJson = gson.toJson(publication);
        String user2InJson = gson.toJson(actualuser);
        Bundle bundle = new Bundle();
        bundle.putSerializable(USER_TAG, userInJson);
        bundle.putSerializable(USER2_TAG, user2InJson);
        fragment.setArguments(bundle);
        return fragment;
    }


    public void getComments(){

        if(actualI == fbPublication.getFirebaseComments().size()){
            CommentAdapter adapter = new CommentAdapter(comments, getContext(), actualUser);
            rvComments.setAdapter(adapter);
            rvComments.setLayoutManager(new LinearLayoutManager(getContext()));
            progressDialog.dismiss();
        }
        else{
            Call<User> call2 = api.getData(fbPublication.getFirebaseComments().get(actualI).getUserId());
            call2.enqueue(new Callback<User>() {
                @Override
                public void onResponse(Call<User> call, Response<User> response) {
                    Log.e("GG1GG", Integer.toString(actualI));
                    Log.e("GG1GG", fbPublication.getFirebaseComments().get(actualI).getComment());
                    comments.add(new Comment(response.body(), fbPublication.getFirebaseComments().get(actualI).getComment()));
                    actualI += 1;
                    getComments();
                }

                @Override
                public void onFailure(Call<User> call, Throwable t) {

                }
            });
        }
    }

}
