package com.example.xnpio.myface.room;

import android.arch.persistence.room.Dao;
import android.arch.persistence.room.Delete;
import android.arch.persistence.room.Insert;
import android.arch.persistence.room.Query;

import java.util.List;

@Dao
public interface CommentDao {

    //@Query("SELECT * FROM comments")
    //List<CommentEntity> getAll();

    @Insert
    void insertAll(CommentEntity ... comments);

    @Delete
    void delete(CommentEntity comment);

}
