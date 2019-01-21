package com.example.xnpio.myface.adapter;

import android.content.Context;
import android.net.ConnectivityManager;
import android.net.NetworkInfo;
import android.net.Uri;
import android.support.annotation.NonNull;
import android.support.v4.app.FragmentManager;
import android.support.v7.app.AppCompatActivity;
import android.support.v7.widget.RecyclerView;
import android.util.Log;
import android.view.LayoutInflater;
import android.view.View;
import android.view.ViewGroup;
import android.widget.Button;
import android.widget.EditText;
import android.widget.ImageView;
import android.widget.TextView;
import android.widget.Toast;

import com.example.xnpio.myface.FirebaseComment;
import com.example.xnpio.myface.FirebasePublication;
import com.example.xnpio.myface.Publication;
import com.example.xnpio.myface.R;
import com.example.xnpio.myface.User;
import com.example.xnpio.myface.fragments.DetailsFragment;
import com.example.xnpio.myface.fragments.WallFragment;
import com.example.xnpio.myface.retrofit.Api;
import com.example.xnpio.myface.room.OfflineModeClass;
import com.google.android.gms.tasks.OnFailureListener;
import com.google.android.gms.tasks.OnSuccessListener;
import com.google.firebase.storage.FirebaseStorage;
import com.google.firebase.storage.StorageReference;
import com.squareup.picasso.Picasso;

import java.util.ArrayList;
import java.util.List;

import retrofit2.Call;
import retrofit2.Callback;
import retrofit2.Response;
import retrofit2.Retrofit;
import retrofit2.converter.gson.GsonConverterFactory;

public class PublicationsAdapter extends RecyclerView.Adapter<PublicationsAdapter.ViewHolder> {

    private User actualUser;
    private Context context;
    private FirebaseStorage storage;
    private StorageReference storageReference;
    private Retrofit retrofit;
    private Api api;
    private List<Button> btns;


    public static class ViewHolder extends RecyclerView.ViewHolder {
        // en este ejemplo cada elemento consta solo de un título

        public ImageView coverImage;
        public ImageView publicationImage;
        public TextView friendName;
        public Button likeBtn;
        public Button commentBtn;
        public EditText commentText;


        public ViewHolder(View view) {
            super(view);
            coverImage = (ImageView) view.findViewById(R.id.friend_img);
            friendName = (TextView) view.findViewById(R.id.friend_name);
            publicationImage = (ImageView) view.findViewById(R.id.pblImg);
            likeBtn = (Button) view.findViewById(R.id.like);
            commentBtn = (Button) view.findViewById(R.id.comentar);
            commentText = (EditText) view.findViewById(R.id.commentText);
        }
    }

    private List<Publication> publications;

    public PublicationsAdapter(List<Publication> publications, Context context, User actualUser) {
        this.publications = publications;
        this.context = context;
        this.actualUser = actualUser;
        this.btns = new ArrayList<Button>();
        for(int i = 0; i < publications.size(); i++){
            btns.add(null);
        }
        retrofit = new Retrofit.Builder()
                .baseUrl("https://myface-cf337.firebaseio.com")//url of firebase app
                .addConverterFactory(GsonConverterFactory.create())//use for convert JSON file into object
                .build();

        api = retrofit.create(Api.class);
    }

    @Override
    public PublicationsAdapter.ViewHolder onCreateViewHolder(ViewGroup parent, int viewType) {
        Context context = parent.getContext();
        LayoutInflater inflater = LayoutInflater.from(context);

        // Inflate the custom layout
        View contactView = inflater.inflate(R.layout.publication_item, parent, false);

        // Return a new holder instance
        PublicationsAdapter.ViewHolder viewHolder = new PublicationsAdapter.ViewHolder(contactView);
        return viewHolder;
    }

    @Override
    public void onBindViewHolder(final PublicationsAdapter.ViewHolder viewHolder, final int position) {
        // Get the data model based on position


        storage = FirebaseStorage.getInstance();
        storageReference = storage.getReference();
        Publication publication = publications.get(position);

        // Set item views based on your views and data model
        ImageView img = (ImageView) viewHolder.coverImage;
        //Seteat imagen
        storageReference.child(publication.getUser().getUid() + "/coverImage50x50").getDownloadUrl().addOnSuccessListener(new OnSuccessListener<Uri>() {
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
                fragmentManager.beginTransaction().replace(R.id.frame, WallFragment.newInstance(publications.get(position).getUser(), actualUser.verifyFiendship(publications.get(position).getUser().getUid()), actualUser), "Wall").commit();
            }
        });

        TextView name = (TextView) viewHolder.friendName;
        name.setText(publication.getUser().getUserName());

        ImageView pblImage = (ImageView) viewHolder.publicationImage;
        pblImage.setImageBitmap(publication.getPublicationImg());

        pblImage.setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View view) {
                FragmentManager fragmentManager = ((AppCompatActivity)context).getSupportFragmentManager();
                fragmentManager.beginTransaction().replace(R.id.frame, DetailsFragment.newInstance(publications.get(position), actualUser), "Details").commit();
            }
        });


        final Button likeBtn = (Button) viewHolder.likeBtn;

        int fbIndex = publications.get(position).getUser().findPublication(publications.get(position).getImageId());
        FirebasePublication fb = publications.get(position).getUser().getPublications().get(fbIndex);

        if(fb.getUserLikes().contains(actualUser.getUid())){
            likeBtn.setEnabled(false);
        }

        likeBtn.setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View view) {
                view.setEnabled(false);
               // likeBtn.setText("Unlike");
                User tempUser = publications.get(position).getUser();
                int index = tempUser.findPublication(publications.get(position).getImageId());
                FirebasePublication tempP = tempUser.getPublications().get(index);
                tempP.setLikes(tempP.getLikes() + 1);
                List<String> temp = tempP.getUserLikes();
                temp.add(actualUser.getUid());
                tempP.setUserLikes(temp);
                List<FirebasePublication>  tempL = tempUser.getPublications();
                tempL.set(index,tempP);
                tempUser.setPublications(tempL);
                updateUser(tempUser);
            }
        });

        final EditText commentText = (EditText) viewHolder.commentText;
        Button commentBtn = (Button) viewHolder.commentBtn;

        commentBtn.setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View view) {
                if(!commentText.getText().toString().equals("")){
                    User tempUser = publications.get(position).getUser();
                    int index = tempUser.findPublication(publications.get(position).getImageId());
                    FirebasePublication tempP = tempUser.getPublications().get(index);
                    List<FirebaseComment> tempC = tempP.getFirebaseComments();
                    tempC.add(new FirebaseComment(actualUser.getUid(), commentText.getText().toString()));
                    tempP.setFirebaseComments(tempC);
                    List<FirebasePublication>  tempL = tempUser.getPublications();
                    tempL.set(index,tempP);
                    tempUser.setPublications(tempL);
                    if(OfflineModeClass.isOnline(context)){
                        updateUser(tempUser);
                    }
                    else{
                        Log.e("OFFLINE", "OFFLINE");
                        OfflineModeClass.insertuser(context, tempUser);
                    }


                    Toast.makeText(context, "Comentario realizado con exito",
                            Toast.LENGTH_SHORT).show();
                    commentText.setText("");
                }
            }
        });


    }

    // Returns the total count of items in the list
    @Override
    public int getItemCount() {
        return publications.size();
    }

    private void updateUser(User user){
        Call<User> call1=api.setDataWithoutRandomness(user.getUid(), user);
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
