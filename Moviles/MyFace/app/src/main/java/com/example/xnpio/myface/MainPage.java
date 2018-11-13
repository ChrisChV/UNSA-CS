package com.example.xnpio.myface;

import android.Manifest;
import android.app.ProgressDialog;
import android.content.Intent;
import android.content.pm.PackageManager;
import android.graphics.Bitmap;
import android.graphics.BitmapFactory;
import android.graphics.Matrix;
import android.hardware.Camera;
import android.net.Uri;
import android.os.Bundle;
import android.os.Environment;
import android.provider.SyncStateContract;
import android.support.annotation.NonNull;
import android.support.design.widget.FloatingActionButton;
import android.support.design.widget.Snackbar;
import android.support.v4.app.ActivityCompat;
import android.support.v4.app.Fragment;
import android.support.v4.app.FragmentManager;
import android.support.v4.app.FragmentTransaction;
import android.support.v4.content.ContextCompat;
import android.util.Log;
import android.view.View;
import android.support.design.widget.NavigationView;
import android.support.v4.view.GravityCompat;
import android.support.v4.widget.DrawerLayout;
import android.support.v7.app.ActionBarDrawerToggle;
import android.support.v7.app.AppCompatActivity;
import android.support.v7.widget.Toolbar;
import android.view.Menu;
import android.view.MenuItem;
import android.widget.GridView;
import android.widget.ImageView;
import android.widget.TextView;
import android.widget.Toast;

import com.bumptech.glide.Glide;
import com.example.xnpio.myface.fragments.MainPageFragment;
import com.example.xnpio.myface.fragments.WallFragment;
import com.example.xnpio.myface.grid.GridViewAdapter;
import com.example.xnpio.myface.grid.ImageItem;
import com.example.xnpio.myface.retrofit.Api;
import com.facebook.login.LoginManager;
import com.google.android.gms.tasks.OnFailureListener;
import com.google.android.gms.tasks.OnSuccessListener;
import com.google.firebase.auth.FirebaseAuth;
import com.google.firebase.auth.FirebaseUser;
import com.google.firebase.storage.FirebaseStorage;
import com.google.firebase.storage.OnProgressListener;
import com.google.firebase.storage.StorageReference;
import com.google.firebase.storage.UploadTask;
import com.google.gson.Gson;
import com.squareup.picasso.Picasso;

import java.io.ByteArrayOutputStream;
import java.io.FileOutputStream;
import java.io.IOException;
import java.io.OutputStream;
import java.util.ArrayList;
import java.util.Calendar;
import java.util.Date;

import retrofit2.Call;
import retrofit2.Callback;
import retrofit2.Response;
import retrofit2.Retrofit;
import retrofit2.converter.gson.GsonConverterFactory;

public class MainPage extends AppCompatActivity
        implements NavigationView.OnNavigationItemSelectedListener {

    //private FirebaseUser f_user;
    private User user;
    private TextView userName;
    private TextView email;
    private FirebaseStorage storage;
    private StorageReference storageReference;
    private ImageView coverImage;
    private FragmentManager fragmentManager;
    private FloatingActionButton cameraButton;

    private Retrofit retrofit;
    private Api api;
    //private FirebaseAuth mAuth;

    private int TAG_TAKE_PICTURE_REQUEST = 17;
    private static final int MY_PERMISSIONS_REQUEST_CAMERA = 10;
    private static final int MY_PERMISSIONS_REQUEST_INTERNET = 11;
    private static final int MY_PERMISSIONS_REQUEST_WRITE = 12;

    private float backCameraDegressRotate = 90;
    private float frontCameraDegressRotate = -90;

    private int cameraPermissions;
    private int internetPermissions;
    private int writePermissions;
    private int currentCameraId;

    private String actualFragment;

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_main_page);

        getPermissions();

        //mAuth = FirebaseAuth.getInstance();
        //f_user = mAuth.getCurrentUser();

        fragmentManager = getSupportFragmentManager();
        fragmentManager.beginTransaction().add(R.id.frame, new MainPageFragment()).commit();

        retrofit = new Retrofit.Builder()
                .baseUrl("https://myface-cf337.firebaseio.com")//url of firebase app
                .addConverterFactory(GsonConverterFactory.create())//use for convert JSON file into object
                .build();

        api = retrofit.create(Api.class);

        storage = FirebaseStorage.getInstance();
        storageReference = storage.getReference();

        cameraButton = findViewById(R.id.fab);
        cameraButton.setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View view) {
                startActivityForResult(new Intent(MainPage.this, CameraActivity.class), TAG_TAKE_PICTURE_REQUEST);
            }
        });

        actualFragment = "Home";

        Toolbar toolbar = (Toolbar) findViewById(R.id.toolbar);
        setSupportActionBar(toolbar);


        DrawerLayout drawer = (DrawerLayout) findViewById(R.id.drawer_layout);
        ActionBarDrawerToggle toggle = new ActionBarDrawerToggle(
                this, drawer, toolbar, R.string.navigation_drawer_open, R.string.navigation_drawer_close);
        drawer.addDrawerListener(toggle);
        toggle.syncState();

        NavigationView navigationView = (NavigationView) findViewById(R.id.nav_view);
        navigationView.setNavigationItemSelectedListener(this);

        View navHeader = navigationView.getHeaderView(0);

        userName = (TextView) navHeader.findViewById(R.id.menu_username);
        email = (TextView) navHeader.findViewById(R.id.menu_email);

        Gson gson = new Gson();
        String userInJson = getIntent().getStringExtra("userString");
        user = gson.fromJson(userInJson, User.class);

        userName.setText(user.getUserName());
        email.setText(user.getEmail());

        coverImage = (ImageView) navHeader.findViewById(R.id.imageView);

        Log.d("TTest1", "TTest1");



        storageReference.child(user.getUid() + "/coverImage50x50").getDownloadUrl().addOnSuccessListener(new OnSuccessListener<Uri>() {
            @Override
            public void onSuccess(Uri uri) {
                Picasso.with(MainPage.this).load(uri).into(coverImage);
                //Toast.makeText(MainPage.this, "Cover downloaded", Toast.LENGTH_SHORT).show();

            }
        }).addOnFailureListener(new OnFailureListener() {
            @Override
            public void onFailure(@NonNull Exception exception) {
                //Toast.makeText(MainPage.this, "Fail Cover" + exception.toString(), Toast.LENGTH_SHORT).show();
            }
        });
    }

    @Override
    public void onBackPressed() {
        DrawerLayout drawer = (DrawerLayout) findViewById(R.id.drawer_layout);
        if (drawer.isDrawerOpen(GravityCompat.START)) {
            drawer.closeDrawer(GravityCompat.START);
        } else {
            super.onBackPressed();
        }
    }



    @Override
    public boolean onCreateOptionsMenu(Menu menu) {
        // Inflate the menu; this adds items to the action bar if it is present.
        getMenuInflater().inflate(R.menu.main_page, menu);
        return true;
    }

    @Override
    public boolean onOptionsItemSelected(MenuItem item) {
        // Handle action bar item clicks here. The action bar will
        // automatically handle clicks on the Home/Up button, so long
        // as you specify a parent activity in AndroidManifest.xml.
        int id = item.getItemId();

        //noinspection SimplifiableIfStatement
        if (id == R.id.action_settings) {
            return true;
        }

        return super.onOptionsItemSelected(item);
    }

    @SuppressWarnings("StatementWithEmptyBody")
    @Override
    public boolean onNavigationItemSelected(MenuItem item) {
        // Handle navigation view item clicks here.
        int id = item.getItemId();

        if (id == R.id.nav_camera) {
            actualFragment = "Wall";
            fragmentManager.beginTransaction().replace(R.id.frame, WallFragment.newInstance(user), actualFragment).commit();


        } else if (id == R.id.nav_gallery) {

        } else if (id == R.id.nav_slideshow) {

        } else if (id == R.id.nav_manage) {

        } else if (id == R.id.nav_share) {

        } else if (id == R.id.nav_send) {

        } else if (id == R.id.nav_logout){
            FirebaseAuth.getInstance().signOut();
            LoginManager.getInstance().logOut();
            Intent it = new Intent(MainPage.this, LoginPage.class);
            startActivity(it);
        }

        DrawerLayout drawer = (DrawerLayout) findViewById(R.id.drawer_layout);
        drawer.closeDrawer(GravityCompat.START);
        return true;
    }

    public Bitmap rotateBitmap(Bitmap bitmap){
        int width = bitmap.getWidth();
        int height = bitmap.getHeight();

        Matrix matrix = new Matrix();

        if(currentCameraId == Camera.CameraInfo.CAMERA_FACING_BACK) {
            matrix.preRotate(backCameraDegressRotate);
        }
        else{
            matrix.preRotate(frontCameraDegressRotate);
        }
        Bitmap rotatedBitmap = Bitmap.createBitmap(bitmap, 0, 0, width, height, matrix, true);
        return rotatedBitmap;
    }

    @Override
    protected void onActivityResult(int requestCode, int resultCode, Intent data) {
        if(requestCode == TAG_TAKE_PICTURE_REQUEST){
            Log.d("TAKEN", "TAKEN");
            Bundle extras = data.getExtras();
            byte[] cameraData = extras.getByteArray(CameraActivity.EXTRA_CAMERA_DATA);
            currentCameraId = extras.getInt("CameraId");
            Bitmap mCameraBitmap;
            if (cameraData != null) {
                mCameraBitmap = BitmapFactory.decodeByteArray(cameraData, 0, cameraData.length);
                mCameraBitmap = rotateBitmap(mCameraBitmap);
                uploadImageToFirebase(mCameraBitmap);
                saveFileToLocal(mCameraBitmap);
            }
        }
        //FAcebook Share
        //else{
          //  super.onActivityResult(requestCode,resultCode,data);

            //callbackManager.onActivityResult(requestCode,resultCode,data);
        //}
    }

    public void getPermissions(){
        cameraPermissions = ContextCompat.checkSelfPermission(this,
                Manifest.permission.CAMERA);
        internetPermissions = ContextCompat.checkSelfPermission(this, Manifest.permission.INTERNET);
        writePermissions = ContextCompat.checkSelfPermission(this, Manifest.permission.WRITE_EXTERNAL_STORAGE);
        if(cameraPermissions != PackageManager.PERMISSION_GRANTED){
            ActivityCompat.requestPermissions(this, new String[]{
                    Manifest.permission.CAMERA
            }, MY_PERMISSIONS_REQUEST_CAMERA);
        }
        if(internetPermissions != PackageManager.PERMISSION_GRANTED){
            ActivityCompat.requestPermissions(this, new String[]{
                    Manifest.permission.INTERNET
            }, MY_PERMISSIONS_REQUEST_INTERNET);
        }
        if(writePermissions != PackageManager.PERMISSION_GRANTED){
            Log.e("LOGLOG", "ENtro");
            ActivityCompat.requestPermissions(this, new String[]{
                    Manifest.permission.WRITE_EXTERNAL_STORAGE
            }, MY_PERMISSIONS_REQUEST_WRITE);
        }
    }

    private void uploadImageToFirebase(Bitmap image) {

        if(image != null)
        {
            final ProgressDialog progressDialog = new ProgressDialog(MainPage.this);
            progressDialog.setTitle("Subiendo Foto...");
            progressDialog.show();
            Date currentTime = Calendar.getInstance().getTime();
            String pathPostFix = currentTime.toString();
            pathPostFix = pathPostFix.replace(":","_");
            pathPostFix = pathPostFix.replace(" ","_");
            pathPostFix = pathPostFix.replace("-","_");
            StorageReference ref = storageReference.child(user.getUid() + "/images/" + Integer.toString(user.getActualImageId()));
            ByteArrayOutputStream baos = new ByteArrayOutputStream();
            image.compress(Bitmap.CompressFormat.JPEG, 100, baos);
            byte[] data = baos.toByteArray();

            ref.putBytes(data)
                    .addOnSuccessListener(new OnSuccessListener<UploadTask.TaskSnapshot>() {
                        @Override
                        public void onSuccess(UploadTask.TaskSnapshot taskSnapshot) {
                            progressDialog.dismiss();
                            user.setActualImageId(user.getActualImageId() + 1);
                            updateUser();
                            if(actualFragment == "Wall"){
                                fragmentManager.beginTransaction().replace(R.id.frame, WallFragment.newInstance(user), actualFragment).commit();
                            }
                            //Toast.makeText(LoginPage.this, "Uploaded File", Toast.LENGTH_SHORT).show();
                        }
                    })
                    .addOnFailureListener(new OnFailureListener() {
                        @Override
                        public void onFailure(@NonNull Exception e) {
                            progressDialog.dismiss();
                            //Toast.makeText(LoginPage.this, "Failed "+e.getMessage(), Toast.LENGTH_SHORT).show();
                        }
                    })
                    .addOnProgressListener(new OnProgressListener<UploadTask.TaskSnapshot>() {
                        @Override
                        public void onProgress(UploadTask.TaskSnapshot taskSnapshot) {
                            double progress = (100.0*taskSnapshot.getBytesTransferred()/taskSnapshot
                                    .getTotalByteCount());
                            progressDialog.setMessage("Uploaded "+(int)progress+"%");

                        }
                    });
        }
        else{
            Toast.makeText(MainPage.this, "Failed bitmap null", Toast.LENGTH_SHORT).show();
        }
    }

    private void saveFileToLocal(Bitmap mCameraBitmap){
        String path = Environment.getExternalStorageDirectory().toString() + "/" + Environment.DIRECTORY_PICTURES;
        Date currentTime = Calendar.getInstance().getTime();
        String pathPostFix = currentTime.toString();
        pathPostFix = pathPostFix.replace(":","_");
        pathPostFix = pathPostFix.replace(" ","_");
        pathPostFix = pathPostFix.replace("-","_");

        Log.e("PATH", path + pathPostFix);
        String pathPthotoFile = "mi_imagen_" + pathPostFix + ".jpg";
        java.io.File photoFile = new java.io.File(path, pathPthotoFile);
        try {
            OutputStream fOut = new FileOutputStream(photoFile);
            mCameraBitmap.compress(Bitmap.CompressFormat.JPEG, 50, fOut);
            Intent MediaStorageUpdateIntent = new Intent(Intent.ACTION_MEDIA_SCANNER_SCAN_FILE);
            MediaStorageUpdateIntent.setData(Uri.fromFile(photoFile));
            sendBroadcast(MediaStorageUpdateIntent);
        } catch (IOException ex){
            Log.e("ERROR", ex.getMessage());
            return;
        }
    }

    private void updateUser(){
        Call<User> call1=api.setDataWithoutRandomness(user.getUid(), user);
        call1.enqueue(new Callback<User>() {
            @Override
            public void onResponse(Call<User> call, Response<User> response) {
                //Toast.makeText(.this, "Cuenta creada con éxito",
                        //Toast.LENGTH_SHORT).show();
                //t1.setText("Success "+response.body().getName());
            }

            @Override
            public void onFailure(Call<User> call, Throwable t) {
                //t1.setText("fail");
                //Toast.makeText(SingUpPage.this, "Cuenta creada sin EXITO",
                        //Toast.LENGTH_SHORT).show();
            }
        });
    }

}
