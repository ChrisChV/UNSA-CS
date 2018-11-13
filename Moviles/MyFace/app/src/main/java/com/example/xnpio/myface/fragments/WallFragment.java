package com.example.xnpio.myface.fragments;


import android.app.ProgressDialog;
import android.graphics.Bitmap;
import android.graphics.drawable.Drawable;
import android.net.Uri;
import android.os.Bundle;
import android.support.annotation.NonNull;
import android.support.v4.app.Fragment;
import android.util.Log;
import android.view.LayoutInflater;
import android.view.View;
import android.view.ViewGroup;
import android.widget.AdapterView;
import android.widget.GridView;

import com.example.xnpio.myface.MainPage;
import com.example.xnpio.myface.R;
import com.example.xnpio.myface.User;
import com.example.xnpio.myface.grid.GridViewAdapter;
import com.example.xnpio.myface.grid.ImageItem;
import com.google.android.gms.tasks.OnFailureListener;
import com.google.android.gms.tasks.OnSuccessListener;
import com.google.firebase.storage.FirebaseStorage;
import com.google.firebase.storage.StorageReference;
import com.google.gson.Gson;
import com.squareup.picasso.Picasso;
import com.squareup.picasso.Target;

import java.util.ArrayList;

public class WallFragment extends Fragment {

    private GridView gridView;
    private GridViewAdapter gridAdapter;
    private User user;
    private FirebaseStorage storage;
    private StorageReference storageReference;
    private ArrayList<ImageItem> data;
    private int actualI;
    private View view;
    private ProgressDialog progressDialog;

    private static final String USER_TAG = "userTag";

    private Target target = new Target() {
        @Override
        public void onBitmapLoaded(Bitmap bitmap, Picasso.LoadedFrom from) {
            data.add(new ImageItem(bitmap, "Image"));
            getBitmapFromFirebase();
        }

        @Override
        public void onBitmapFailed(Drawable errorDrawable) {
        }

        @Override
        public void onPrepareLoad(Drawable placeHolderDrawable) {
        }
    };

    public static WallFragment newInstance(User user){
        WallFragment fragment = new WallFragment();
        Gson gson = new Gson();
        String userInJson = gson.toJson(user);
        Bundle bundle = new Bundle();
        bundle.putSerializable(USER_TAG, userInJson);
        fragment.setArguments(bundle);
        return fragment;
    }

    @Override
    public View onCreateView(LayoutInflater inflater, ViewGroup container,
                             Bundle savedInstanceState) {
        // Inflate the layout for this fragment
        actualI = 0;
        storage = FirebaseStorage.getInstance();
        storageReference = storage.getReference();
        Gson gson = new Gson();
        this.user = gson.fromJson(getArguments().getSerializable(USER_TAG).toString(), User.class);
        progressDialog = new ProgressDialog(getContext());
        progressDialog.setTitle("Actualizando...");
        progressDialog.show();
        getData();
        view = inflater.inflate(R.layout.wall_fragment,container,false);
        return view;
    }

    private void getData() {
        data = new ArrayList<>();
        getBitmapFromFirebase();
    }

    private void getBitmapFromFirebase(){
        Log.d("TT1", Integer.toString(actualI));
        if(actualI == user.getActualImageId()){
            gridView = (GridView) view.findViewById(R.id.gridView);
            gridAdapter = new GridViewAdapter(getContext(), R.layout.grid_item , data);
            gridView.setAdapter(gridAdapter);
            progressDialog.dismiss();
            /*gridView.setOnItemClickListener(new AdapterView.OnItemClickListener() {
                public void onItemClick(AdapterView<?> parent, View v, int position, long id) {
                    ImageItem item = (ImageItem) parent.getItemAtPosition(position);

                    }
                    */
        }
        storageReference.child(user.getUid() + "/images/" + Integer.toString(actualI)).getDownloadUrl().addOnSuccessListener(new OnSuccessListener<Uri>() {
            @Override
            public void onSuccess(Uri uri) {
                actualI += 1;
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
