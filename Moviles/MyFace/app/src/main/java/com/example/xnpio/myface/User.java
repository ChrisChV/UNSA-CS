package com.example.xnpio.myface;

public class User {

    private String uid;

    private String userName;

    private String email;

    public User(String uid, String userName, String email){
        this.uid = uid;
        this.userName = userName;
        this.email = email;
    }

    public void setUid(String uid){
        this.uid = uid;
    }

    public String getUid(){
        return uid;
    }

    public void setUserName(String userName){
        this.userName = userName;
    }

    public String getUserName(){
        return userName;
    }

    public void setEmail(String email){
        this.email = email;
    }

    public String getEmail(){
        return email;
    }

}
