package com.example.xnpio.myface.fragments;

import android.app.ProgressDialog;
import android.os.Bundle;
import android.support.v4.app.Fragment;
import android.support.v4.app.FragmentManager;
import android.support.v7.app.AppCompatActivity;
import android.support.v7.widget.LinearLayoutManager;
import android.support.v7.widget.RecyclerView;
import android.util.Log;
import android.view.LayoutInflater;
import android.view.View;
import android.view.ViewGroup;
import android.widget.Button;
import android.widget.EditText;
import android.widget.ImageView;
import android.widget.Toast;

import com.example.xnpio.myface.Chat;
import com.example.xnpio.myface.Comment;
import com.example.xnpio.myface.FirebaseComment;
import com.example.xnpio.myface.FirebasePublication;
import com.example.xnpio.myface.Publication;
import com.example.xnpio.myface.R;
import com.example.xnpio.myface.User;
import com.example.xnpio.myface.adapter.CommentAdapter;
import com.example.xnpio.myface.retrofit.Api;
import com.google.gson.Gson;

import java.util.ArrayList;
import java.util.List;

import retrofit2.Call;
import retrofit2.Callback;
import retrofit2.Response;
import retrofit2.Retrofit;
import retrofit2.converter.gson.GsonConverterFactory;

public class CommentsFragment extends Fragment {

    private View view;
    private RecyclerView rvComments;
    private Retrofit retrofit;
    private Api api;
    private int actualI;
    private List<Comment> comments;
    private User actualUser;
    private User friend;
    private ProgressDialog progressDialog;
    private Chat chatFriend;
    private Chat chatActualUser;
    private int indexF;
    private int indexAU;
    private EditText msg;
    private Button msgBtn;

    private static final String USER_TAG = "userTag";
    private static final String USER2_TAG = "user2Tag";

    @Override
    public View onCreateView(LayoutInflater inflater, ViewGroup container,
                             Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);

        view = inflater.inflate(R.layout.chat_fragment, container,false);

        Gson gson = new Gson();


        this.friend = gson.fromJson(getArguments().getSerializable(USER_TAG).toString(), User.class);
        this.actualUser = gson.fromJson(getArguments().getSerializable(USER2_TAG).toString(), User.class);

        indexAU = actualUser.findChat(friend.getUid());
        if(indexAU == -1){
            chatFriend = new Chat(actualUser.getUid());
            chatActualUser = new Chat(friend.getUid());
            indexF = -1;
        }
        else{
            Log.e("GGGG","EEEE");
            chatActualUser = actualUser.getChats().get(indexAU);
            indexF = friend.findChat(actualUser.getUid());
            chatFriend = friend.getChats().get(indexF);
        }

        rvComments = (RecyclerView) view.findViewById(R.id.rvChat);
        retrofit = new Retrofit.Builder()
                .baseUrl("https://myface-cf337.firebaseio.com")//url of firebase app
                .addConverterFactory(GsonConverterFactory.create())//use for convert JSON file into object
                .build();

        api = retrofit.create(Api.class);
        actualI = 0;

        progressDialog = new ProgressDialog(getContext());
        progressDialog.setTitle("Actualizando...");
        progressDialog.show();

        msg = (EditText) view.findViewById(R.id.msg);
        msgBtn = (Button) view.findViewById(R.id.msgBtn);


        msgBtn.setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View view) {
                if(!msg.getText().toString().equals("")){

                    List<FirebaseComment> fclTemp1 = chatFriend.getMsgs();
                    fclTemp1.add(new FirebaseComment(friend.getUid(), msg.getText().toString()));
                    List<FirebaseComment> fclTemp2 = chatActualUser.getMsgs();
                    fclTemp2.add(new FirebaseComment(actualUser.getUid(), msg.getText().toString()));
                    chatFriend.setMsgs(fclTemp1);
                    chatActualUser.setMsgs(fclTemp2);
                    List<Chat> clTemp1 = friend.getChats();
                    List<Chat> clTemp2 = actualUser.getChats();
                    if(indexAU == -1 || indexF == -1){
                        clTemp1.add(chatFriend);
                        clTemp2.add(chatActualUser);
                    }
                    else{
                        clTemp1.set(indexF, chatFriend);
                        clTemp2.set(indexAU, chatActualUser);
                    }
                    Log.e("GGGG", Integer.toString(actualUser.getChats().size()));
                    friend.setChats(clTemp1);
                    actualUser.setChats(clTemp2);
                    updateUser(friend);
                    updateUser(actualUser);
                    FragmentManager fragmentManager = ((AppCompatActivity)getContext()).getSupportFragmentManager();
                    fragmentManager.beginTransaction().replace(R.id.frame, CommentsFragment.newInstance(friend,actualUser)).commit();
                }
            }
        });

        comments = new ArrayList<Comment>();

        getComments();


        return view;
    }

    public static CommentsFragment newInstance(User friend, User actualuser){
        CommentsFragment fragment = new CommentsFragment();
        Gson gson = new Gson();
        String userInJson = gson.toJson(friend);
        String user2InJson = gson.toJson(actualuser);
        Bundle bundle = new Bundle();
        bundle.putSerializable(USER_TAG, userInJson);
        bundle.putSerializable(USER2_TAG, user2InJson);
        fragment.setArguments(bundle);
        return fragment;
    }


    public void getComments(){
        for(FirebaseComment  fbComment : chatActualUser.getMsgs()){
            if(fbComment.getUserId().equals(actualUser.getUid())){
                comments.add(new Comment(actualUser, fbComment.getComment()));
            }
            else{
                comments.add(new Comment(friend, fbComment.getComment()));
            }
        }
        Log.e("GGGG", Integer.toString(comments.size()));
        CommentAdapter adapter = new CommentAdapter(comments, getContext(), actualUser);
        rvComments.setAdapter(adapter);
        rvComments.setLayoutManager(new LinearLayoutManager(getContext()));
        progressDialog.dismiss();
    }

    private void updateUser(User nUser){
        Call<User> call1 = api.setDataWithoutRandomness(nUser.getUid(), nUser);
        call1.enqueue(new Callback<User>() {
            @Override
            public void onResponse(Call<User> call, Response<User> response) {
                //Toast.makeText(getContext(), "Cuenta creada con éxito",
                //Toast.LENGTH_SHORT).show();
                //t1.setText("Success "+response.body().getName());
            }

            @Override
            public void onFailure(Call<User> call, Throwable t) {
                //t1.setText("fail");
                //Toast.makeText(getContext(), "Cuenta creada sin EXITO",
                //Toast.LENGTH_SHORT).show();
            }
        });
    }

}
