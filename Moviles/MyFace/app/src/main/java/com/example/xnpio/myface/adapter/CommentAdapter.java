package com.example.xnpio.myface.adapter;

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

import com.example.xnpio.myface.Comment;
import com.example.xnpio.myface.FirebaseComment;
import com.example.xnpio.myface.R;
import com.example.xnpio.myface.User;
import com.example.xnpio.myface.fragments.WallFragment;
import com.google.android.gms.tasks.OnFailureListener;
import com.google.android.gms.tasks.OnSuccessListener;
import com.google.firebase.storage.FirebaseStorage;
import com.google.firebase.storage.StorageReference;
import com.squareup.picasso.Picasso;


import org.w3c.dom.Text;

import java.util.List;




public class CommentAdapter extends RecyclerView.Adapter<CommentAdapter.ViewHolder> {

    public Context context;
    public User actualUser;
    private FirebaseStorage storage;
    private StorageReference storageReference;

    public static class ViewHolder extends RecyclerView.ViewHolder {
        // en este ejemplo cada elemento consta solo de un título

        public ImageView coverImage;
        public TextView friendName;
        public TextView commentText;


        public ViewHolder(View view) {
            super(view);
            coverImage = (ImageView) view.findViewById(R.id.friend_img);
            friendName = (TextView) view.findViewById(R.id.friend_name);
            commentText = (TextView) view.findViewById(R.id.commentText);
        }
    }

    public List<Comment> comments;

    public CommentAdapter(List<Comment> comments, Context context, User actualUser) {
        this.comments = comments;
        this.context = context;
        this.actualUser = actualUser;
    }

    @Override
    public CommentAdapter.ViewHolder onCreateViewHolder(ViewGroup parent, int viewType) {
        Context context = parent.getContext();
        LayoutInflater inflater = LayoutInflater.from(context);

        // Inflate the custom layout
        View contactView = inflater.inflate(R.layout.comment_item, parent, false);

        // Return a new holder instance
        CommentAdapter.ViewHolder viewHolder = new CommentAdapter.ViewHolder(contactView);
        return viewHolder;
    }

    @Override
    public void onBindViewHolder(final CommentAdapter.ViewHolder viewHolder, final int position) {

        storage = FirebaseStorage.getInstance();
        storageReference = storage.getReference();
        Comment comment = comments.get(position);
        Log.e("GG3GG", comment.getCommentText());

        ImageView img = (ImageView) viewHolder.coverImage;
        //Seteat imagen
        storageReference.child(comment.getUser().getUid() + "/coverImage50x50").getDownloadUrl().addOnSuccessListener(new OnSuccessListener<Uri>() {
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
                FragmentManager fragmentManager = ((AppCompatActivity)context).getSupportFragmentManager();
                fragmentManager.beginTransaction().replace(R.id.frame, WallFragment.newInstance(comments.get(position).getUser(), actualUser.verifyFiendship(comments.get(position).getUser().getUid()), actualUser), "Wall").commit();
            }
        });

        TextView name = (TextView) viewHolder.friendName;
        name.setText(comment.getUser().getUserName());

        TextView commentText = (TextView) viewHolder.commentText;
        commentText.setText(comment.getCommentText());


    }

    @Override
    public int getItemCount() {
        return comments.size();
    }




}
