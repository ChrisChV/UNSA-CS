package com.example.xnpio.myface;

public class FirebaseComment {

    private String userId;
    private String comment;

    public FirebaseComment(){
        this.userId = "";
        this.comment = "";
    }

    public FirebaseComment(String userId, String comment){
        this.userId = userId;
        this.comment = comment;
    }

    public void setUserId(String userId){
        this.userId = userId;
    }

    public String getUserId(){
        return userId;
    }

    public void setComment(String comment){
        this.comment = comment;
    }

    public String getComment(){
        return comment;
    }

}
