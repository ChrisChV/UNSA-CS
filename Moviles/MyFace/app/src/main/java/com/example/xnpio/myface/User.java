package com.example.xnpio.myface;

import com.example.xnpio.myface.fragments.WallFragment;

import java.util.ArrayList;
import java.util.List;

public class User {

    private String uid;

    private String userName;

    private String email;

    private int actualImageId;

    private List<String> friends;

    private List<String> pendings;

    private List<String> pendingsResponse;

    private List<FirebasePublication> publications;

    public User(){
        uid = "";
        userName = "";
        email = "";
        actualImageId = 0;
        friends = new ArrayList<String>();
        pendings = new ArrayList<String>();
        pendingsResponse = new ArrayList<String>();
        publications = new ArrayList<FirebasePublication>();
    }

    public User(String uid, String userName, String email){
        this.uid = uid;
        this.userName = userName;
        this.email = email;
        actualImageId = 0;
        friends = new ArrayList<String>();
        //friends.add("5su0NSJCa1cT7WXh2zx2cC3g7oB3");
        pendings = new ArrayList<String>();
        pendingsResponse = new ArrayList<String>();
        publications = new ArrayList<FirebasePublication>();
    }

    public User(String uid, String userName, String email, int actualImageId){
        this.uid = uid;
        this.userName = userName;
        this.email = email;
        this.actualImageId = actualImageId;
        friends = new ArrayList<String>();
        pendings = new ArrayList<String>();
        pendingsResponse = new ArrayList<String>();
        publications = new ArrayList<FirebasePublication>();
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

    public void setFriends(List<String> friends){
        this.friends = friends;
    }

    public List<String> getFriends(){
        return friends;
    }

    public void setPendings(List<String> pendings){
        this.pendings = pendings;
    }

    public List<String> getPendings(){
        return pendings;
    }

    public void setPendingsResponse(List<String> pendingsResponse){
        this.pendingsResponse = pendingsResponse;
    }

    public List<String> getPendingsResponse(){
        return pendingsResponse;
    }

    public void setPublications(List<FirebasePublication> publications){
        this.publications = publications;
    }

    public List<FirebasePublication> getPublications(){
        return publications;
    }

    public int verifyFiendship(String uid){
        if(this.uid.equals(uid)) return WallFragment.MY_WALL;
        if(friends.contains(uid)) return WallFragment.FRIEND_WALL;
        if(pendings.contains(uid)) return WallFragment.PENDING_FRIEND_WALL;
        if(pendingsResponse.contains(uid)) return WallFragment.PENDING_RESPONSE_FRIEND_WALL;
        return WallFragment.NO_FRIEND_WALL;
    }

    public int findPublication(String publicationID){
        for(int i = 0; i < publications.size(); i++){
            if(publications.get(i).getPublicationID().equals(publicationID)) return i;
        }
        return -1;
    }


}
