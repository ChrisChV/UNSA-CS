package com.example.xnpio.myface;

import android.graphics.Bitmap;

public class Publication {

    private User user;
    private Bitmap publicationImg;
    private String imageId;


    public Publication(){
        this.user = null;
        this.publicationImg = null;
        this.imageId = "";
    }

    public Publication(User user, Bitmap publicationImg){
        this.user = user;
        this.publicationImg = publicationImg;
        this.imageId = "";
    }

    public Publication(User user, Bitmap publicationImg, String imageId){
        this.user = user;
        this.publicationImg = publicationImg;
        this.imageId = imageId;
    }



    public void setUser(User user){
        this.user = user;
    }

    public User getUser(){
        return user;
    }

    public void setPublicationImg(Bitmap publicationImg){
        this.publicationImg = publicationImg;
    }

    public Bitmap getPublicationImg(){
        return publicationImg;
    }

    public void setImageId(String imageId){
        this.imageId = imageId;
    }

    public String getImageId(){
        return imageId;
    }

}
