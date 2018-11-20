package com.example.xnpio.myface;

import android.app.ProgressDialog;
import android.content.Intent;
import android.graphics.Bitmap;
import android.graphics.drawable.Drawable;
import android.os.Bundle;
import android.support.annotation.NonNull;
import android.support.v7.app.AppCompatActivity;
import android.util.Log;
import android.view.View;
import android.widget.Button;
import android.widget.EditText;
import android.widget.ImageView;
import android.widget.TextView;
import android.widget.Toast;

import com.example.xnpio.myface.retrofit.Api;
import com.facebook.AccessToken;
import com.facebook.CallbackManager;
import com.facebook.FacebookCallback;
import com.facebook.FacebookException;
import com.facebook.GraphRequest;
import com.facebook.GraphResponse;
import com.facebook.login.LoginResult;
import com.facebook.login.widget.LoginButton;
import com.facebook.share.widget.ShareDialog;
import com.google.android.gms.tasks.OnCompleteListener;
import com.google.android.gms.tasks.OnFailureListener;
import com.google.android.gms.tasks.OnSuccessListener;
import com.google.android.gms.tasks.Task;
import com.google.firebase.auth.AuthCredential;
import com.google.firebase.auth.AuthResult;
import com.google.firebase.auth.FacebookAuthProvider;
import com.google.firebase.auth.FirebaseAuth;
import com.google.firebase.auth.FirebaseAuthInvalidCredentialsException;
import com.google.firebase.auth.FirebaseAuthUserCollisionException;
import com.google.firebase.auth.FirebaseAuthWeakPasswordException;
import com.google.firebase.auth.FirebaseUser;
import com.google.firebase.storage.FirebaseStorage;
import com.google.firebase.storage.OnProgressListener;
import com.google.firebase.storage.StorageReference;
import com.google.firebase.storage.UploadTask;
import com.google.gson.Gson;
import com.squareup.picasso.Picasso;
import com.squareup.picasso.Target;

import org.json.JSONException;
import org.json.JSONObject;
import org.w3c.dom.Text;

import java.io.ByteArrayOutputStream;
import java.net.MalformedURLException;
import java.net.URL;
import java.util.Arrays;
import java.util.UUID;
import java.util.concurrent.locks.ReentrantLock;

import retrofit2.Call;
import retrofit2.Callback;
import retrofit2.Response;
import retrofit2.Retrofit;
import retrofit2.converter.gson.GsonConverterFactory;

public class LoginPage extends AppCompatActivity {

    private TextView singUpText;
    private TextView singInButton;
    private LoginButton loginButton;
    private CallbackManager callbackManager;
    private ProgressDialog mDialog;
    private FirebaseAuth mAuth;
    private Retrofit retrofit;
    private Api api;
    private AccessToken facebookToken;
    private JSONObject facebookJson;
    private FirebaseStorage storage;
    private StorageReference storageReference;
    private String firebaseUid;

    private ProgressDialog progressDialogIng;

    private Target target = new Target() {
        @Override
        public void onBitmapLoaded(Bitmap bitmap, Picasso.LoadedFrom from) {
            uploadImageToFirebase(bitmap);
        }

        @Override
        public void onBitmapFailed(Drawable errorDrawable) {
        }

        @Override
        public void onPrepareLoad(Drawable placeHolderDrawable) {
        }
    };

    @Override
    protected void onCreate(Bundle savedInstanceState){
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_login);
        singUpText = (TextView) findViewById(R.id.sup);
        singInButton = (TextView) findViewById(R.id.sininbutoon);

        loginButton = (LoginButton) findViewById(R.id.login_fac_button);
        loginButton.setReadPermissions(Arrays.asList("public_profile","email","user_birthday","user_friends"));
        callbackManager = CallbackManager.Factory.create();
        mAuth = FirebaseAuth.getInstance();

        retrofit = new Retrofit.Builder()
                .baseUrl("https://myface-cf337.firebaseio.com")//url of firebase app
                .addConverterFactory(GsonConverterFactory.create())//use for convert JSON file into object
                .build();

        api = retrofit.create(Api.class);

        storage = FirebaseStorage.getInstance();
        storageReference = storage.getReference();

        singUpText.setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View v) {
                Intent it = new Intent(LoginPage.this, SingUpPage.class);
                startActivity(it);
            }
        });
        singInButton.setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View view) {
                progressDialogIng = new ProgressDialog(LoginPage.this);
                progressDialogIng.setTitle("Ingresando...");
                progressDialogIng.show();
                autenticate();
            }
        });

        loginButton.registerCallback(callbackManager, new FacebookCallback<LoginResult>() {
            @Override
            public void onSuccess(final LoginResult loginResult) {
                mDialog = new ProgressDialog(LoginPage.this);
                mDialog.setMessage("Retrieving data...");
                mDialog.show();

                String accesstoken = loginResult.getAccessToken().getToken();
                GraphRequest request = GraphRequest.newMeRequest(loginResult.getAccessToken(), new GraphRequest.GraphJSONObjectCallback() {
                    @Override
                    public void onCompleted(JSONObject object, GraphResponse response) {
                        mDialog.dismiss();

                        Log.d("response",response.toString());

                        facebookToken = loginResult.getAccessToken();
                        facebookJson = object;
                        handleFacebookAccessToken();

                        //getData(object);
                        //Bundle facebookData = getFacebookData(object);
                    }
                });

                //Request Graph API

                Bundle parameters = new Bundle();
                parameters.putString("fields","id,email,birthday,friends,cover");
                request.setParameters(parameters);
                request.executeAsync();

            }

            @Override
            public void onCancel() {

            }

            @Override
            public void onError(FacebookException error) {

            }
        });


    }

    @Override
    protected void onActivityResult(int requestCode, int resultCode, Intent data) {
        super.onActivityResult(requestCode,resultCode, data);
        callbackManager.onActivityResult(requestCode,resultCode, data);
    }

    @Override
    public void onStart() {
        super.onStart();
        FirebaseUser f_user = mAuth.getCurrentUser();
        if(f_user != null){
            Call<User> call2=api.getData(f_user.getUid());
            call2.enqueue(new Callback<User>() {
                @Override
                public void onResponse(Call<User> call, Response<User> response) {
                    Intent it = new Intent(LoginPage.this, MainPage.class);
                    //User user = new User(response.body().getUid(), response.body().getUserName(), response.body().getEmail());
                    User user = response.body();
                    Gson gson = new Gson();
                    String userInJson = gson.toJson(user);
                    it.putExtra("userString", userInJson);
                    startActivity(it);
                }

                @Override
                public void onFailure(Call<User> call, Throwable t) {

                }
            });

        }
    }

    private void autenticate(){
        EditText emailET = (EditText) findViewById(R.id.login_mail);
        EditText passET = (EditText) findViewById(R.id.login_pass);
        String email = emailET.getText().toString();
        String password = passET.getText().toString();
        mAuth.signInWithEmailAndPassword(email, password)
                .addOnCompleteListener(this, new OnCompleteListener<AuthResult>() {
                    @Override
                    public void onComplete(@NonNull Task<AuthResult> task) {
                        if (task.isSuccessful()) {
                            // Sign in success, update UI with the signed-in user's information
                            Log.d("Login", "signInWithEmail:success");

                            FirebaseUser f_user = mAuth.getCurrentUser();
                            Call<User> call2=api.getData(f_user.getUid());
                            call2.enqueue(new Callback<User>() {
                                @Override
                                public void onResponse(Call<User> call, Response<User> response) {
                                    Intent it = new Intent(LoginPage.this, MainPage.class);
                                    //User user = new User(response.body().getUid(), response.body().getUserName(), response.body().getEmail(), response.body().getActualImageId());
                                    User user = response.body();
                                    Gson gson = new Gson();
                                    String userInJson = gson.toJson(user);
                                    it.putExtra("userString", userInJson);
                                    startActivity(it);
                                }

                                @Override
                                public void onFailure(Call<User> call, Throwable t) {

                                }
                            });
                            //updateUI(user);
                        } else {
                            // If sign in fails, display a message to the user.
                            Log.w("Login", "signInWithEmail:failure", task.getException());
                            Toast.makeText(LoginPage.this, "Authentication failed.",
                                    Toast.LENGTH_SHORT).show();
                            progressDialogIng.dismiss();
                            //updateUI(null);
                        }


                    }
                });
    }

    private void handleFacebookAccessToken() {
        final ProgressDialog progressDialog = new ProgressDialog(this);
        progressDialog.setTitle("Ingresando...");
        progressDialog.show();
        Log.d("Token", "handleFacebookAccessToken:" + facebookToken);

        AuthCredential credential = FacebookAuthProvider.getCredential(facebookToken.getToken());
        mAuth.signInWithCredential(credential)
                .addOnCompleteListener(this, new OnCompleteListener<AuthResult>() {
                    @Override
                    public void onComplete(@NonNull Task<AuthResult> task) {
                        if (task.isSuccessful()) {
                            // Sign in success, update UI with the signed-in user's information
                            Log.d("FaceLogin", "signInWithCredential:success");
                            FirebaseUser f_user = mAuth.getCurrentUser();
                            User user = new User(f_user.getUid(), f_user.getDisplayName(), f_user.getEmail());
                            Call<User> call1=api.setDataWithoutRandomness(user.getUid(), user);
                            try{
                                URL profile_picture = new URL("https://graph.facebook.com/"+ facebookJson.getString("id") +"/picture?width=50&height=50");
                                Log.d("URL_Token", profile_picture.toString());
                                firebaseUid = f_user.getUid();
                                Picasso.with(LoginPage.this).load(profile_picture.toString()).into(target);
                            }
                            catch (MalformedURLException e){
                                Log.d("error","MAL PARSE");
                            }
                            catch (JSONException e) {
                                Log.d("error","MAL PARSE2");
                                //e.printStackTrace();
                            }


                            call1.enqueue(new Callback<User>() {
                                @Override
                                public void onResponse(Call<User> call, Response<User> response) {
                                    //Toast.makeText(LoginPage.this, "Cuenta creada con éxito",
                                     //       Toast.LENGTH_SHORT).show();
                                    //t1.setText("Success "+response.body().getName());
                                }

                                @Override
                                public void onFailure(Call<User> call, Throwable t) {
                                    //t1.setText("fail");
                                    Toast.makeText(LoginPage.this, "Cuenta creada sin EXITO",
                                            Toast.LENGTH_SHORT).show();
                                }
                            });
                        } else {
                            // If sign in fails, display a message to the user.
                            Log.w("FaceLogin", "signInWithCredential:failure", task.getException());
                            Toast.makeText(LoginPage.this, "Authentication failed.",
                                    Toast.LENGTH_SHORT).show();

                        }

                        // ...
                    }
                });
    }

    private void uploadImageToFirebase(Bitmap image) {

        if(image != null)
        {

            StorageReference ref = storageReference.child(firebaseUid + "/coverImage50x50");
            ByteArrayOutputStream baos = new ByteArrayOutputStream();
            image.compress(Bitmap.CompressFormat.JPEG, 100, baos);
            byte[] data = baos.toByteArray();

            ref.putBytes(data)
                    .addOnSuccessListener(new OnSuccessListener<UploadTask.TaskSnapshot>() {
                        @Override
                        public void onSuccess(UploadTask.TaskSnapshot taskSnapshot) {
                            FirebaseUser f_user = mAuth.getCurrentUser();
                            User user = new User(f_user.getUid(), f_user.getDisplayName(), f_user.getEmail());
                            Intent it = new Intent(LoginPage.this, MainPage.class);
                            Gson gson = new Gson();
                            String userInJson = gson.toJson(user);
                            it.putExtra("userString", userInJson);
                            startActivity(it);
                            //progressDialog.dismiss();
                            //Toast.makeText(LoginPage.this, "Uploaded File", Toast.LENGTH_SHORT).show();
                        }
                    })
                    .addOnFailureListener(new OnFailureListener() {
                        @Override
                        public void onFailure(@NonNull Exception e) {
                            //progressDialog.dismiss();
                            //Toast.makeText(LoginPage.this, "Failed "+e.getMessage(), Toast.LENGTH_SHORT).show();
                        }
                    })
                    .addOnProgressListener(new OnProgressListener<UploadTask.TaskSnapshot>() {
                        @Override
                        public void onProgress(UploadTask.TaskSnapshot taskSnapshot) {
                            double progress = (100.0*taskSnapshot.getBytesTransferred()/taskSnapshot
                                    .getTotalByteCount());
                            //Toast.makeText(LoginPage.this, "Progreess", Toast.LENGTH_SHORT).show();
                        }
                    });
        }
        else{
            Toast.makeText(LoginPage.this, "Failed bitmap null", Toast.LENGTH_SHORT).show();
        }
    }

}
