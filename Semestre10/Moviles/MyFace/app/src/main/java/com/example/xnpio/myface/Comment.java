package com.example.xnpio.myface;

public class Comment {

    private User user;
    private String commentText;

    public Comment(){
        this.user = null;
        this.commentText = "";
    }

    public Comment(User user, String commentText){
        this.user = user;
        this.commentText = commentText;
    }

    public void setUser(User user){
        this.user = user;
    }

    public User getUser(){
        return user;
    }

    public void setCommentText(String commentText){
        this.commentText = commentText;
    }

    public String getCommentText(){
        return commentText;
    }

}
