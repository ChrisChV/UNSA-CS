package com.example.xnpio.myface;

import android.app.ProgressDialog;
import android.content.Intent;
import android.os.Bundle;
import android.support.annotation.NonNull;
import android.support.v7.app.AppCompatActivity;
import android.util.Log;
import android.view.View;
import android.widget.EditText;
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
import com.google.android.gms.tasks.OnCompleteListener;
import com.google.android.gms.tasks.Task;
import com.google.firebase.auth.AuthCredential;
import com.google.firebase.auth.AuthResult;
import com.google.firebase.auth.FacebookAuthProvider;
import com.google.firebase.auth.FirebaseAuth;
import com.google.firebase.auth.FirebaseAuthInvalidCredentialsException;
import com.google.firebase.auth.FirebaseAuthUserCollisionException;
import com.google.firebase.auth.FirebaseAuthWeakPasswordException;
import com.google.firebase.auth.FirebaseUser;
import com.google.firebase.auth.UserProfileChangeRequest;
import com.google.gson.Gson;

import org.json.JSONObject;
import org.w3c.dom.Text;

import java.util.Arrays;

import retrofit2.Call;
import retrofit2.Callback;
import retrofit2.Response;
import retrofit2.Retrofit;
import retrofit2.converter.gson.GsonConverterFactory;

public class SingUpPage extends AppCompatActivity {

    private TextView loginText;
    private TextView createButton;
    private FirebaseAuth mAuth;
    private LoginButton loginButton;
    private CallbackManager callbackManager;
    private ProgressDialog mDialog;
    private Retrofit retrofit;
    private Api api;

    @Override
    protected void onCreate(Bundle savedInstanceState){
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_signup);
        loginText = (TextView) findViewById(R.id.sin);
        createButton = (TextView) findViewById(R.id.createButton);
        mAuth = FirebaseAuth.getInstance();

        retrofit = new Retrofit.Builder()
                .baseUrl("https://myface-cf337.firebaseio.com")//url of firebase app
                .addConverterFactory(GsonConverterFactory.create())//use for convert JSON file into object
                .build();

        api = retrofit.create(Api.class);

        /*
        loginButton = (LoginButton) findViewById(R.id.signup_fac_button);
        loginButton.setReadPermissions(Arrays.asList("public_profile","email","user_birthday","user_friends"));
        callbackManager = CallbackManager.Factory.create();
        */

        loginText.setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View v) {
                Intent it = new Intent(SingUpPage.this, LoginPage.class);
                startActivity(it);
            }
        });
        createButton.setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View view) {
                createUser();
            }
        });

        /*
        loginButton.registerCallback(callbackManager, new FacebookCallback<LoginResult>() {
            @Override
            public void onSuccess(final LoginResult loginResult) {
                mDialog = new ProgressDialog(SingUpPage.this);
                mDialog.setMessage("Retrieving data...");
                mDialog.show();

                String accesstoken = loginResult.getAccessToken().getToken();
                GraphRequest request = GraphRequest.newMeRequest(loginResult.getAccessToken(), new GraphRequest.GraphJSONObjectCallback() {
                    @Override
                    public void onCompleted(JSONObject object, GraphResponse response) {
                        mDialog.dismiss();

                        Log.d("response",response.toString());

                        handleFacebookAccessToken(loginResult.getAccessToken());

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
        */

    }

    private void createUser(){
        EditText emailET = (EditText) findViewById(R.id.sup_mail);
        EditText passET = (EditText) findViewById(R.id.sup_pass);
        String email = emailET.getText().toString();
        String password = passET.getText().toString();

        mAuth.createUserWithEmailAndPassword(email, password)
                .addOnCompleteListener(this, new OnCompleteListener<AuthResult>() {
                    @Override
                    public void onComplete(@NonNull Task<AuthResult> task) {
                        if (task.isSuccessful()) {
                            // Sign in success, update UI with the signed-in user's information
                            Log.d("CreateUser", "createUserWithEmail:success");
                            //FirebaseUser user = mAuth.getCurrentUser();
                            //UserProfileChangeRequest profileUpdates = new UserProfileChangeRequest.Builder()
                             //       .setDisplayName().build();

                            FirebaseUser f_user = mAuth.getCurrentUser();
                            if(f_user != null){
                                Intent it = new Intent(SingUpPage.this, MainPage.class);
                                //User user = new User(f_user.getDisplayName(), f_user.getEmail());
                                EditText userET = (EditText) findViewById(R.id.sup_user);
                                String userName = userET.getText().toString();
                                User user = new User(f_user.getUid(), userName, f_user.getEmail());
                                Call<User> call1=api.setDataWithoutRandomness(user.getUid(), user);
                                call1.enqueue(new Callback<User>() {
                                    @Override
                                    public void onResponse(Call<User> call, Response<User> response) {
                                        Toast.makeText(SingUpPage.this, "Cuenta creada con éxito",
                                                Toast.LENGTH_SHORT).show();
                                        //t1.setText("Success "+response.body().getName());
                                    }

                                    @Override
                                    public void onFailure(Call<User> call, Throwable t) {
                                        //t1.setText("fail");
                                        Toast.makeText(SingUpPage.this, "Cuenta creada sin EXITO",
                                                Toast.LENGTH_SHORT).show();
                                    }
                                });
                                Gson gson = new Gson();
                                String userInJson = gson.toJson(user);
                                it.putExtra("userString", userInJson);
                                startActivity(it);
                            }
                            //updateUI(user);
                        } else {
                            // If sign in fails, display a message to the user.
                            Log.w("CreateUser", "createUserWithEmail:failure", task.getException());

                            try
                            {
                                throw task.getException();
                            }
                            // if user enters wrong email.
                            catch (FirebaseAuthWeakPasswordException weakPassword)
                            {
                                Log.d("Auth", "onComplete: weak_password");
                                Toast.makeText(SingUpPage.this, "La password no es valida",
                                        Toast.LENGTH_SHORT).show();

                            }
                            // if user enters wrong password.
                            catch (FirebaseAuthInvalidCredentialsException malformedEmail)
                            {
                                Log.d("Auth", "onComplete: malformed_email");
                                Toast.makeText(SingUpPage.this, "El correo no es valido",
                                        Toast.LENGTH_SHORT).show();


                            }
                            catch (FirebaseAuthUserCollisionException existEmail)
                            {
                                Log.d("Auth", "onComplete: exist_email");
                                Toast.makeText(SingUpPage.this, "El correo ya existe",
                                        Toast.LENGTH_SHORT).show();

                            }
                            catch (Exception e)
                            {
                                Log.d("Auth", "onComplete: " + e.getMessage());
                                Toast.makeText(SingUpPage.this, e.getMessage(),
                                        Toast.LENGTH_SHORT).show();
                            }


                            //updateUI(null);
                        }


                    }
                });


    }

    private void handleFacebookAccessToken(AccessToken token) {
        Log.d("Token", "handleFacebookAccessToken:" + token);

        AuthCredential credential = FacebookAuthProvider.getCredential(token.getToken());
        mAuth.signInWithCredential(credential)
                .addOnCompleteListener(this, new OnCompleteListener<AuthResult>() {
                    @Override
                    public void onComplete(@NonNull Task<AuthResult> task) {
                        if (task.isSuccessful()) {
                            // Sign in success, update UI with the signed-in user's information
                            Log.d("FaceLogin", "signInWithCredential:success");
                            FirebaseUser f_user = mAuth.getCurrentUser();
                            Intent it = new Intent(SingUpPage.this, MainPage.class);
                            User user = new User(f_user.getUid(), f_user.getDisplayName(), f_user.getEmail());
                            Gson gson = new Gson();
                            String userInJson = gson.toJson(user);
                            it.putExtra("userString", userInJson);
                            startActivity(it);
                        } else {
                            // If sign in fails, display a message to the user.
                            Log.w("FaceLogin", "signInWithCredential:failure", task.getException());
                            Toast.makeText(SingUpPage.this, "Authentication failed.",
                                    Toast.LENGTH_SHORT).show();

                        }

                        // ...
                    }
                });
    }

}
