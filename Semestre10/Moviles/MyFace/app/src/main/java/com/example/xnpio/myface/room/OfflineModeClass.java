package com.example.xnpio.myface.room;

import android.content.Context;
import android.net.ConnectivityManager;
import android.net.NetworkInfo;
import android.support.v7.app.AppCompatActivity;

import com.example.xnpio.myface.User;
import com.example.xnpio.myface.retrofit.Api;
import com.google.gson.Gson;

import java.util.List;

import retrofit2.Call;
import retrofit2.Callback;
import retrofit2.Response;
import retrofit2.Retrofit;
import retrofit2.converter.gson.GsonConverterFactory;

public class OfflineModeClass {

    public static boolean isOnline(Context context) {
        ConnectivityManager cm = (ConnectivityManager) ((AppCompatActivity)context).getSystemService(Context.CONNECTIVITY_SERVICE);
        NetworkInfo netInfo = cm.getActiveNetworkInfo();
        if (netInfo != null && netInfo.isConnectedOrConnecting()) {
            return true;
        } else {
            return false;
        }
    }

    public static void insertuser(Context context, User user){
        AppDatabase db = AppDatabase.getAppDatabase(context);
        UserEntity tempUser = db.userDao().getByUid(user.getUid());
        Gson gson = new Gson();
        String userJson = gson.toJson(user);
        if(tempUser == null){
            UserEntity nu = new UserEntity();
            nu.setUid(user.getUid());
            nu.setJsonUser(userJson);
            db.userDao().insertAll(nu);
        }
        else{
            tempUser.setJsonUser(userJson);
            db.userDao().updateUser(tempUser);
        }
    }

    public static void verifyUsers(Context context){
        AppDatabase db = AppDatabase.getAppDatabase(context);
        int size = db.userDao().countUsers();
        if(size > 0){
            List<UserEntity> list = db.userDao().getAll();
            Gson gson = new Gson();
            for(UserEntity ue : list){
                User nu = gson.fromJson(ue.getJsonUser(), User.class);
                updateUser(nu);
                db.userDao().delete(ue);
            }
        }
    }

    public static void updateUser(User user){
        Retrofit retrofit = new Retrofit.Builder()
                .baseUrl("https://myface-cf337.firebaseio.com")//url of firebase app
                .addConverterFactory(GsonConverterFactory.create())//use for convert JSON file into object
                .build();

        Api api = retrofit.create(Api.class);
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
