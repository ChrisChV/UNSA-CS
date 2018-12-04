package com.example.xnpio.myface.room;

import android.arch.persistence.room.Dao;
import android.arch.persistence.room.Delete;
import android.arch.persistence.room.Insert;
import android.arch.persistence.room.Query;
import android.arch.persistence.room.Update;

import java.util.List;

@Dao
public interface UserDao {

    @Query("SELECT * FROM user")
    List<UserEntity> getAll();

    @Query("SELECT * FROM user WHERE user.user_id LIKE :uid")
    UserEntity getByUid(String uid);

    @Insert
    void insertAll(UserEntity ... users);

    @Update
    void updateUser(UserEntity userEntity);

    @Delete
    void delete(UserEntity user);

    @Query("SELECT COUNT(*) from user")
    int countUsers();
}
