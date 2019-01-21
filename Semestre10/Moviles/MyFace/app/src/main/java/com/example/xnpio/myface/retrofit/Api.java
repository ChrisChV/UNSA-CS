package com.example.xnpio.myface.retrofit;

import com.example.xnpio.myface.User;

import retrofit2.Call;
import retrofit2.http.Body;
import retrofit2.http.GET;
import retrofit2.http.POST;
import retrofit2.http.PUT;
import retrofit2.http.Path;

public interface Api {
    @POST("/users/{new}.json")
    Call<User> setData(@Path("new") String s1, @Body User user);

    @GET("/users/{userUid}.json")
    Call<User> getData(@Path("userUid") String uid);

    @PUT("/users/{new}.json")
    Call<User> setDataWithoutRandomness(@Path("new") String s1, @Body User user);

}
