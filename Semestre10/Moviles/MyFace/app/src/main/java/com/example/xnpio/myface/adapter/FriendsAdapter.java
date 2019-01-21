package com.example.xnpio.myface.adapter;

import android.app.Activity;
import android.app.ProgressDialog;
import android.content.Context;
import android.net.Uri;
import android.support.annotation.NonNull;
import android.support.v4.app.FragmentManager;
import android.support.v7.app.AppCompatActivity;
import android.support.v7.widget.RecyclerView;
import android.util.Log;
import android.view.LayoutInflater;
import android.view.View;
import android.view.ViewGroup;
import android.widget.ImageView;
import android.widget.TextView;

import com.example.xnpio.myface.MainPage;
import com.example.xnpio.myface.R;
import com.example.xnpio.myface.User;
import com.example.xnpio.myface.fragments.WallFragment;
import com.google.android.gms.tasks.OnFailureListener;
import com.google.android.gms.tasks.OnSuccessListener;
import com.google.firebase.storage.FirebaseStorage;
import com.google.firebase.storage.StorageReference;
import com.squareup.picasso.Picasso;

import java.util.List;

public class FriendsAdapter extends RecyclerView.Adapter<FriendsAdapter.ViewHolder> {

    private FirebaseStorage storage;
    private StorageReference storageReference;
    private Context context;
    //private User user;
    private User actualUser;



    public static class ViewHolder extends RecyclerView.ViewHolder {
        // en este ejemplo cada elemento consta solo de un título

        public ImageView coverImage;
        public TextView friendName;

        public ViewHolder(View view) {
            super(view);
            coverImage = (ImageView) view.findViewById(R.id.friend_img);
            friendName = (TextView) view.findViewById(R.id.friend_name);
        }
    }

    private List<User> friends;

    public FriendsAdapter(List<User> friend, Context context, User actualUser) {
        friends = friend;
        Log.d("SIZE2AAA", Integer.toString(friends.size()));
        this.context = context;
        this.actualUser = actualUser;

    }

    @Override
    public FriendsAdapter.ViewHolder onCreateViewHolder(ViewGroup parent, int viewType) {
        Context context = parent.getContext();
        LayoutInflater inflater = LayoutInflater.from(context);

        // Inflate the custom layout
        View contactView = inflater.inflate(R.layout.friend_item, parent, false);

        // Return a new holder instance
        ViewHolder viewHolder = new ViewHolder(contactView);
        return viewHolder;
    }

    @Override
    public void onBindViewHolder(final FriendsAdapter.ViewHolder viewHolder, final int position) {
        // Get the data model based on position


        storage = FirebaseStorage.getInstance();
        storageReference = storage.getReference();
        User user = friends.get(position);

        // Set item views based on your views and data model
        ImageView img = (ImageView) viewHolder.coverImage;
        //Seteat imagen
        storageReference.child(user.getUid() + "/coverImage50x50").getDownloadUrl().addOnSuccessListener(new OnSuccessListener<Uri>() {
            @Override
            public void onSuccess(Uri uri) {
                Picasso.with(context).load(uri).into(viewHolder.coverImage);
                //Toast.makeText(MainPage.this, "Cover downloaded", Toast.LENGTH_SHORT).show();

            }
        }).addOnFailureListener(new OnFailureListener() {
            @Override
            public void onFailure(@NonNull Exception exception) {
                //Toast.makeText(MainPage.this, "Fail Cover" + exception.toString(), Toast.LENGTH_SHORT).show();
            }
        });

        img.setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View view) {
                Log.d("GGGG", "onClick: ");
                FragmentManager fragmentManager = ((AppCompatActivity)context).getSupportFragmentManager();
                fragmentManager.beginTransaction().replace(R.id.frame, WallFragment.newInstance(friends.get(position), actualUser.verifyFiendship(friends.get(position).getUid()), actualUser), "Wall").commit();
            }
        });

        TextView name = (TextView) viewHolder.friendName;
        name.setText(user.getUserName());

    }

    // Returns the total count of items in the list
    @Override
    public int getItemCount() {
        return friends.size();
    }


}
