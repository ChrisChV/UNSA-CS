package com.example.xnpio.cameraappv3;

import java.io.ByteArrayOutputStream;

import java.io.FileOutputStream;
import java.io.IOException;
import java.io.OutputStream;
import java.util.Calendar;
import java.util.Date;

import android.Manifest;
import android.app.Activity;
import android.content.Intent;
import android.content.IntentSender;
import android.content.pm.PackageManager;
import android.graphics.Bitmap;
import android.graphics.BitmapFactory;
import android.graphics.Canvas;
import android.graphics.Color;
import android.graphics.Matrix;
import android.graphics.Paint;
import android.graphics.PointF;
import android.hardware.Camera;
import android.media.FaceDetector;
import android.net.Uri;
import android.os.Bundle;
import android.os.Environment;
import android.support.v4.app.ActivityCompat;
import android.support.v4.content.ContextCompat;
import android.util.Log;
import android.view.View;
import android.view.View.OnClickListener;
import android.widget.Button;
import android.widget.ImageView;

import com.google.android.gms.common.ConnectionResult;
import com.google.android.gms.common.api.GoogleApiClient;

import com.google.android.gms.common.api.ResultCallback;
import com.google.android.gms.drive.Drive;
import com.google.android.gms.drive.DriveApi;
import com.google.android.gms.drive.MetadataChangeSet;




import static android.content.ContentValues.TAG;

public class MainActivity extends Activity implements GoogleApiClient.ConnectionCallbacks,
        GoogleApiClient.OnConnectionFailedListener {

    private static final int TAKE_PICTURE_REQUEST_B = 100;
    private static final int REQUEST_CODE_CREATOR = 2;
    private static final int REQUEST_CODE_RESOLUTION = 3;

    private static final int MY_PERMISSIONS_REQUEST_CAMERA = 10;
    private static final int MY_PERMISSIONS_REQUEST_INTERNET = 11;
    private static final int MY_PERMISSIONS_REQUEST_WRITE = 12;

    private ImageView mCameraImageView;
    private Bitmap mCameraBitmap;
    private static Bitmap mCameraWithFacesBitmap = null;
    private Button mSaveImageButton;
    private GoogleApiClient mGoogleApiClient;

    private int cameraPermissions;
    private int internetPermissions;
    private int writePermissions;
    private int currentCameraId;

    private String pathPthotoFile;

    private Drive drive;

    private float backCameraDegressRotate = 90;
    private float frontCameraDegressRotate = -90;

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

    private void saveFileToLocal(){
        String path = Environment.getExternalStorageDirectory().toString() + "/" + Environment.DIRECTORY_PICTURES;
        Date currentTime = Calendar.getInstance().getTime();
        String pathPostFix = currentTime.toString();
        pathPostFix = pathPostFix.replace(":","_");
        pathPostFix = pathPostFix.replace(" ","_");
        pathPostFix = pathPostFix.replace("-","_");

        Log.e("PATH", path + pathPostFix);
        pathPthotoFile = "mi_imagen_" + pathPostFix + ".jpg";
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

/*
    private class FileUploadProgressListener implements MediaHttpUploaderProgressListener {

        private String mFileUploadedName;

        public FileUploadProgressListener(String fileName) {
            mFileUploadedName = fileName;
        }

        @Override
        public void progressChanged(MediaHttpUploader mediaHttpUploader) throws IOException {
            if (mediaHttpUploader == null) return;
            switch (mediaHttpUploader.getUploadState()) {
                case INITIATION_STARTED:
                    //System.out.println("Initiation has started!");
                    break;
                case INITIATION_COMPLETE:
                    //System.out.println("Initiation is complete!");
                    break;
                case MEDIA_IN_PROGRESS:
                    double percent = mediaHttpUploader.getProgress() * 100;
                    Log.d(TAG, "Upload to Dropbox: " + mFileUploadedName + " - " + String.valueOf(percent) + "%");

                    //progressBar.setProgress(percent, mFileUploadedName).fire();
                    break;
                case MEDIA_COMPLETE:
                    //System.out.println("Upload is complete!");
            }
        }
    }
*/


    private void saveFileToDrive(){
        final Bitmap image = mCameraBitmap;
        Drive.DriveApi.newDriveContents(mGoogleApiClient)
                .setResultCallback(new ResultCallback<DriveApi.DriveContentsResult>() {
                    @Override
                    public void onResult(DriveApi.DriveContentsResult result) {
                        if (!result.getStatus().isSuccess()) {
                            Log.i("ERROR", "Failed to create new contents.");
                            return;
                        }
                        OutputStream outputStream = result.getDriveContents().getOutputStream();
                        ByteArrayOutputStream bitmapStream = new ByteArrayOutputStream();
                        image.compress(Bitmap.CompressFormat.PNG, 100, bitmapStream);
                        try {
                            outputStream.write(bitmapStream.toByteArray());
                        } catch (IOException e1) {
                            Log.i("ERROR", "Unable to write file contents.");
                        }
                        MetadataChangeSet metadataChangeSet = new MetadataChangeSet.Builder()
                                .setMimeType("image/jpeg").setTitle(pathPthotoFile).build();

                        IntentSender intentSender = Drive.DriveApi
                                .newCreateFileActivityBuilder()
                                .setInitialMetadata(metadataChangeSet)
                                .setInitialDriveContents(result.getDriveContents())
                                .build(mGoogleApiClient);
                        try {
                            startIntentSenderForResult(intentSender, REQUEST_CODE_CREATOR, null, 0, 0, 0);
                        } catch (IntentSender.SendIntentException e) {
                            Log.i("Error", "Failed to launch file chooser.");
                        }
                    }
                });
    }


    /*
    private void saveFileToDrive(){
        class CustomProgressListener implements MediaHttpUploaderProgressListener {
            public void progressChanged(MediaHttpUploader uploader) throws IOException {
                switch (uploader.getUploadState()) {
                    case INITIATION_STARTED:
                        System.out.println("Initiation has started!");
                        break;
                    case INITIATION_COMPLETE:
                        System.out.println("Initiation is complete!");
                        break;
                    case MEDIA_IN_PROGRESS:
                        System.out.println(uploader.getProgress());
                        break;
                    case MEDIA_COMPLETE:
                        System.out.println("Upload is complete!");
                }
            }
        }
        try{

        }

    }
    */

    public void detectFaces(){
        if(null != mCameraBitmap){
            int width = mCameraBitmap.getWidth();
            int height = mCameraBitmap.getHeight();

            int max_faces = 5;

            FaceDetector detector = new FaceDetector(width, height, max_faces);
            FaceDetector.Face[] faces = new FaceDetector.Face[max_faces];

            Bitmap bitmap565 = Bitmap.createBitmap(width, height, Bitmap.Config.RGB_565);
            Paint ditherPaint = new Paint();
            Paint drawPaint = new Paint();

            ditherPaint.setDither(true);
            drawPaint.setColor(Color.RED);
            drawPaint.setStyle(Paint.Style.STROKE);
            drawPaint.setStrokeWidth(2);

            Canvas canvas = new Canvas();
            canvas.setBitmap(bitmap565);
            canvas.drawBitmap(mCameraBitmap, 0, 0, ditherPaint);

            int facesFound = detector.findFaces(bitmap565, faces);

            Log.e("FACEDETECTOR", Integer.toString(facesFound));

            PointF midPoint = new PointF();
            float eyeDistance = 0.0f;
            float confidence = 0.0f;

            if(facesFound > 0){
                for(int index = 0; index < facesFound; ++index){
                    faces[index].getMidPoint(midPoint);
                    eyeDistance = faces[index].eyesDistance();
                    confidence = faces[index].confidence();

                    canvas.drawRect((int)midPoint.x - eyeDistance,
                            (int) midPoint.y - eyeDistance,
                            (int) midPoint.x + eyeDistance,
                            (int) midPoint.y + eyeDistance, drawPaint);
                }

            }

            mCameraWithFacesBitmap = bitmap565;
        }

    }

    private OnClickListener mCaptureImageButtonClickListener = new OnClickListener() {
        @Override
        public void onClick(View v) {
            startImageCapture();
        }
    };

    private OnClickListener mSaveImageButtonClickListener = new OnClickListener() {
        @Override
        public void onClick(View v) {
            saveFileToDrive();
        }
    };

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);

        getPermissions();

        setContentView(R.layout.activity_main);

        mCameraImageView = (ImageView) findViewById(R.id.camera_image_view);

        findViewById(R.id.capture_image_button).setOnClickListener(mCaptureImageButtonClickListener);

        mSaveImageButton = (Button) findViewById(R.id.save_image_button);
        mSaveImageButton.setOnClickListener(mSaveImageButtonClickListener);
        mSaveImageButton.setEnabled(false);

        if (mGoogleApiClient == null) {
            // Create the API client and bind it to an instance variable.
            // We use this instance as the callback for connection and connection
            // failures.
            // Since no account name is passed, the user is prompted to choose.
            mGoogleApiClient = new GoogleApiClient.Builder(this)
                    .addApi(Drive.API)
                    .addScope(Drive.SCOPE_FILE)
                    .addConnectionCallbacks(this)
                    .addOnConnectionFailedListener(this)
                    .build();
        }

        if(mCameraWithFacesBitmap != null){
            mCameraImageView.setImageBitmap(mCameraWithFacesBitmap);
            mSaveImageButton.setEnabled(true);
        }

        // Connect the client. Once connected, the camera is launched.
        mGoogleApiClient.connect();
    }

    @Override
    protected void onResume() {
        super.onResume();
        if (mGoogleApiClient == null) {
            mGoogleApiClient = new GoogleApiClient.Builder(this)
                    .addApi(Drive.API)
                    .addScope(Drive.SCOPE_FILE)
                    .addConnectionCallbacks(this)
                    .addOnConnectionFailedListener(this)
                    .build();
        }
        // Connect the client. Once connected, the camera is launched.
        mGoogleApiClient.connect();
    }

    @Override
    public void onConnectionFailed(ConnectionResult result) {
        // Called whenever the API client fails to connect.
        Log.i(TAG, "GoogleApiClient connection failed: " + result.toString());
        if (!result.hasResolution()) {
            // show the localized error dialog.
            return;
        }
        // The failure has a resolution. Resolve it.
        // Called typically when the app is not yet authorized, and an
        // authorization
        // dialog is displayed to the user.
        try {
            result.startResolutionForResult(this, REQUEST_CODE_RESOLUTION);
        } catch (IntentSender.SendIntentException e) {
            Log.e(TAG, "Exception while starting resolution activity", e);
        }
    }

    @Override
    protected void onActivityResult(int requestCode, int resultCode, Intent data) {
        if (requestCode == TAKE_PICTURE_REQUEST_B) {
            if (resultCode == RESULT_OK) {
                // Recycle the previous bitmap.
                if (mCameraBitmap != null) {
                    mCameraBitmap.recycle();
                    mCameraBitmap = null;
                }
                Bundle extras = data.getExtras();
                byte[] cameraData = extras.getByteArray(CameraActivity.EXTRA_CAMERA_DATA);
                currentCameraId = extras.getInt("CameraId");
                if (cameraData != null) {
                    mCameraBitmap = BitmapFactory.decodeByteArray(cameraData, 0, cameraData.length);
                    mCameraBitmap = rotateBitmap(mCameraBitmap);
                    detectFaces();
                    mCameraImageView.setImageBitmap(mCameraWithFacesBitmap);
                    mSaveImageButton.setEnabled(true);
                    saveFileToLocal();
                }
            } else {
                mCameraBitmap = null;
                mSaveImageButton.setEnabled(false);
            }
        }
        else if(requestCode == REQUEST_CODE_CREATOR){
            if (resultCode == RESULT_OK) {
                Log.i("SAVED", "Image successfully saved.");
                // Just start the camera again for another photo.
            }
        }
    }

    private void startImageCapture() {
        startActivityForResult(new Intent(MainActivity.this, CameraActivity.class), TAKE_PICTURE_REQUEST_B);
    }

    @Override
    public void onConnected(Bundle connectionHint) {
        Log.i(TAG, "API client connected.");
        if (mCameraBitmap == null) {
            // This activity has no UI of its own. Just start the camera.
            //startImageCapture();
            return;
        }
        //saveFileToDrive();
    }

    @Override
    public void onConnectionSuspended(int cause) {
        Log.i(TAG, "GoogleApiClient connection suspended");
    }

}
