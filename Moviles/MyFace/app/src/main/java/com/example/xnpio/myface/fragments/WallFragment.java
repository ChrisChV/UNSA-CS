package com.example.xnpio.myface.fragments;


import android.app.ProgressDialog;
import android.graphics.Bitmap;
import android.graphics.drawable.Drawable;
import android.media.Image;
import android.net.Uri;
import android.os.Bundle;
import android.support.annotation.NonNull;
import android.support.v4.app.Fragment;
import android.util.Log;
import android.view.LayoutInflater;
import android.view.View;
import android.view.ViewGroup;
import android.widget.AdapterView;
import android.widget.Button;
import android.widget.GridView;
import android.widget.ImageView;
import android.widget.TextView;

import com.example.xnpio.myface.MainPage;
import com.example.xnpio.myface.R;
import com.example.xnpio.myface.User;
import com.example.xnpio.myface.grid.GridViewAdapter;
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

public class WallFragment extends Fragment {

    private GridView gridView;
    private GridViewAdapter gridAdapter;
    private User user;
    private User actualUser;
    private FirebaseStorage storage;
    private StorageReference storageReference;
    private ArrayList<ImageItem> data;
    private int actualI;
    private int state;
    private View view;
    private ProgressDialog progressDialog;
    private Button friendsBtn;
    private TextView nameText;
    private ImageView coverImage;
    private Retrofit retrofit;
    private Api api;

    private static final String USER_TAG = "userTag";
    private static final String ACTUAL_USER_TAG = "actualUserTag";
    private static final String STATE_TAG = "stateTag";

    public static final int MY_WALL = 0;
    public static final int FRIEND_WALL = 1;
    public static final int NO_FRIEND_WALL = 2;
    public static final int PENDING_FRIEND_WALL = 3;
    public static final int PENDING_RESPONSE_FRIEND_WALL = 4;

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

    public static WallFragment newInstance(User user, int state, User actualUser){
        WallFragment fragment = new WallFragment();
        Gson gson = new Gson();
        String userInJson = gson.toJson(user);
        String actualUserInJson = gson.toJson(actualUser);
        Bundle bundle = new Bundle();
        bundle.putSerializable(USER_TAG, userInJson);
        bundle.putSerializable(ACTUAL_USER_TAG, actualUserInJson);
        bundle.putInt(STATE_TAG, state);
        fragment.setArguments(bundle);
        return fragment;
    }

    @Override
    public View onCreateView(LayoutInflater inflater, ViewGroup container,
                             Bundle savedInstanceState) {
        // Inflate the layout for this fragment
        actualI = 0;
        view = inflater.inflate(R.layout.wall_fragment,container,false);
        storage = FirebaseStorage.getInstance();
        storageReference = storage.getReference();
        Gson gson = new Gson();
        this.user = gson.fromJson(getArguments().getSerializable(USER_TAG).toString(), User.class);
        this.actualUser = gson.fromJson(getArguments().getSerializable(ACTUAL_USER_TAG).toString(), User.class);
        this.state = getArguments().getInt(STATE_TAG);

        retrofit = new Retrofit.Builder()
                .baseUrl("https://myface-cf337.firebaseio.com")//url of firebase app
                .addConverterFactory(GsonConverterFactory.create())//use for convert JSON file into object
                .build();

        api = retrofit.create(Api.class);

        progressDialog = new ProgressDialog(getContext());
        progressDialog.setTitle("Actualizando...");
        progressDialog.show();
        getData();
        nameText = (TextView) view.findViewById(R.id.nameText);
        nameText.setText(user.getUserName());
        friendsBtn = (Button) view.findViewById(R.id.friendBtn);
        coverImage = (ImageView) view.findViewById(R.id.cover);

        storageReference.child(user.getUid() + "/coverImage50x50").getDownloadUrl().addOnSuccessListener(new OnSuccessListener<Uri>() {
            @Override
            public void onSuccess(Uri uri) {
                Picasso.with(getContext()).load(uri).into(coverImage);
                //Toast.makeText(MainPage.this, "Cover downloaded", Toast.LENGTH_SHORT).show();

            }
        }).addOnFailureListener(new OnFailureListener() {
            @Override
            public void onFailure(@NonNull Exception exception) {
                //Toast.makeText(MainPage.this, "Fail Cover" + exception.toString(), Toast.LENGTH_SHORT).show();
            }
        });

        if(state == MY_WALL){
            friendsBtn.setVisibility(View.INVISIBLE);
            friendsBtn.setEnabled(false);
        }
        else if(state == FRIEND_WALL){
            friendsBtn.setEnabled(false);
            friendsBtn.setText("Ya son amigos!!");
        }
        else if(state == PENDING_FRIEND_WALL){
            friendsBtn.setEnabled(false);
            friendsBtn.setText("Esperando respuesta");
        }
        else if(state == PENDING_RESPONSE_FRIEND_WALL){
            friendsBtn.setText("Responder Amistad");
            friendsBtn.setOnClickListener(new View.OnClickListener() {
                @Override
                public void onClick(View view) {
                    List<String> temp1 = user.getPendings();
                    temp1.remove(actualUser.getUid());
                    user.setPendings(temp1);
                    List<String> temp2 = actualUser.getPendingsResponse();
                    temp2.remove(user.getUid());
                    actualUser.setPendingsResponse(temp2);
                    List<String> temp3 = actualUser.getFriends();
                    temp3.add(user.getUid());
                    actualUser.setFriends(temp3);
                    List<String> temp4 = user.getFriends();
                    temp4.add(actualUser.getUid());
                    user.setFriends(temp4);
                    updateUser(actualUser);
                    updateUser(user);
                    friendsBtn.setEnabled(false);
                    friendsBtn.setText("Ya son amigos!!");
                    MainPage.setUser(actualUser);
                }
            });
        }
        else{
            friendsBtn.setOnClickListener(new View.OnClickListener() {
                @Override
                public void onClick(View view) {
                    List<String> temp1 = actualUser.getPendings();
                    temp1.add(user.getUid());
                    actualUser.setPendings(temp1);
                    List<String> temp2 = user.getPendingsResponse();
                    temp2.add(actualUser.getUid());
                    user.setPendingsResponse(temp2);
                    updateUser(actualUser);
                    updateUser(user);
                    friendsBtn.setEnabled(false);
                    friendsBtn.setText("Esperando respuesta");
                    MainPage.setUser(actualUser);
                }
            });
        }
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

    private void updateUser(User nUser){
        Call<User> call1 = api.setDataWithoutRandomness(nUser.getUid(), nUser);
        call1.enqueue(new Callback<User>() {
            @Override
            public void onResponse(Call<User> call, Response<User> response) {
                //Toast.makeText(.this, "Cuenta creada con éxito",
                //Toast.LENGTH_SHORT).show();
                //t1.setText("Success "+response.body().getName());
            }

            @Override
            public void onFailure(Call<User> call, Throwable t) {
                //t1.setText("fail");
                //Toast.makeText(SingUpPage.this, "Cuenta creada sin EXITO",
                //Toast.LENGTH_SHORT).show();
            }
        });
    }

}
