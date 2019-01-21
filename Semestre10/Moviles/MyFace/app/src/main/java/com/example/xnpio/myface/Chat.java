package com.example.xnpio.myface;

import java.util.ArrayList;
import java.util.List;

public class Chat {

    private String userId;
    private List<FirebaseComment> msgs;

    public Chat(){
        this.userId = "";
        this.msgs = new ArrayList<FirebaseComment>();
    }

    public Chat(String userId){
        this.userId = userId;
        this.msgs = new ArrayList<FirebaseComment>();
    }

    public void setUserId(String userID){
        this.userId = userID;
    }

    public String getUserId(){
        return userId;
    }

    public void setMsgs(List<FirebaseComment> msgs){
        this.msgs = msgs;
    }

    public List<FirebaseComment> getMsgs(){
        return msgs;
    }

}
