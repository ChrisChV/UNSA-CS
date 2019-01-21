package com.example.xnpio.myface;

import java.util.ArrayList;
import java.util.List;

public class FirebasePublication {

    private String publicationID;
    private int likes;
    private List<FirebaseComment> comments;
    private List<String> userLikes;


    public FirebasePublication(){
        this.publicationID = "";
        this.likes = 0;
        this.comments = new ArrayList<FirebaseComment>();
        this.userLikes = new ArrayList<String>();
    }

    public FirebasePublication(String publicationID){
        this.publicationID = publicationID;
        this.likes = 0;
        this.comments = new ArrayList<FirebaseComment>();
        this.userLikes = new ArrayList<String>();
    }

    public FirebasePublication(String publicationID, int likes, List<FirebaseComment> firebaseComments){
        this.publicationID = publicationID;
        this.likes = likes;
        this.comments = firebaseComments;
        this.userLikes = new ArrayList<String>();
    }


    public void setPublicationID(String publicationID){
        this.publicationID = publicationID;
    }

    public String getPublicationID(){
        return publicationID;
    }

    public void setLikes(int likes){
        this.likes = likes;
    }

    public int getLikes(){
        return likes;
    }

    public void setFirebaseComments(List<FirebaseComment> firebaseComments){
        this.comments = firebaseComments;
    }

    public List<FirebaseComment> getFirebaseComments(){
        return comments;
    }

    public void setUserLikes(List<String> userLikes){
        this.userLikes = userLikes;
    }

    public List<String> getUserLikes(){
        return userLikes;
    }

}
