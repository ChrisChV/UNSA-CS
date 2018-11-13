package com.example.xnpio.myface;

public class User {

    private String uid;

    private String userName;

    private String email;

    private int actualImageId;

    public User(String uid, String userName, String email){
        this.uid = uid;
        this.userName = userName;
        this.email = email;
        actualImageId = 0;
    }

    public User(String uid, String userName, String email, int actualImageId){
        this.uid = uid;
        this.userName = userName;
        this.email = email;
        this.actualImageId = actualImageId;
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

    public void setActualImageId(int actualImageId){
        this.actualImageId = actualImageId;
    }

    public int getActualImageId(){
        return actualImageId;
    }

}
