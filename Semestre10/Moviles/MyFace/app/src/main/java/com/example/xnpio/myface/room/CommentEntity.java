package com.example.xnpio.myface.room;


import android.arch.persistence.room.ColumnInfo;
import android.arch.persistence.room.Entity;
import android.arch.persistence.room.PrimaryKey;

@Entity
public class CommentEntity {
    @PrimaryKey
    public int id;

    @ColumnInfo(name = "publication_id")
    public String publicationId;

    @ColumnInfo(name = "user_id")
    public String userId;

    @ColumnInfo(name = "comment")
    public String comment;

}
