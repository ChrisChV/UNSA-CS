package com.example.xnpio.myface;

import android.app.ProgressDialog;
import android.content.Intent;
import android.os.Bundle;
import android.support.annotation.NonNull;
import android.support.v7.app.AppCompatActivity;
import android.util.Log;
import android.view.View;
import android.widget.Button;
import android.widget.EditText;
import android.widget.TextView;
import android.widget.Toast;

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
import com.google.android.gms.tasks.Task;
import com.google.firebase.auth.AuthCredential;
import com.google.firebase.auth.AuthResult;
import com.google.firebase.auth.FacebookAuthProvider;
import com.google.firebase.auth.FirebaseAuth;
import com.google.firebase.auth.FirebaseAuthInvalidCredentialsException;
import com.google.firebase.auth.FirebaseAuthUserCollisionException;
import com.google.firebase.auth.FirebaseAuthWeakPasswordException;
import com.google.firebase.auth.FirebaseUser;
import com.google.gson.Gson;

import org.json.JSONException;
import org.json.JSONObject;
import org.w3c.dom.Text;

import java.net.MalformedURLException;
import java.util.Arrays;

public class LoginPage extends AppCompatActivity {

    private TextView singUpText;
    private TextView singInButton;
    private LoginButton loginButton;
    private CallbackManager callbackManager;
    private ProgressDialog mDialog;
    private FirebaseAuth mAuth;


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
            Intent it = new Intent(LoginPage.this, MainPage.class);
            User user = new User(f_user.getUid(), f_user.getDisplayName(), f_user.getEmail());
            Gson gson = new Gson();
            String userInJson = gson.toJson(user);
            it.putExtra("userString", userInJson);
            startActivity(it);
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
                            Intent it = new Intent(LoginPage.this, MainPage.class);
                            User user = new User(f_user.getUid(), f_user.getDisplayName(), f_user.getEmail());
                            Gson gson = new Gson();
                            String userInJson = gson.toJson(user);
                            it.putExtra("userString", userInJson);
                            startActivity(it);
                            //updateUI(user);
                        } else {
                            // If sign in fails, display a message to the user.
                            Log.w("Login", "signInWithEmail:failure", task.getException());
                            Toast.makeText(LoginPage.this, "Authentication failed.",
                                    Toast.LENGTH_SHORT).show();
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
                            Intent it = new Intent(LoginPage.this, MainPage.class);
                            User user = new User(f_user.getUid(), f_user.getDisplayName(), f_user.getEmail());
                            Gson gson = new Gson();
                            String userInJson = gson.toJson(user);
                            it.putExtra("userString", userInJson);
                            startActivity(it);
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

}
