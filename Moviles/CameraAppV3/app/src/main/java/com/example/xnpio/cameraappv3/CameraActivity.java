package com.example.xnpio.cameraappv3;

import java.io.ByteArrayOutputStream;
import java.io.IOException;

import android.Manifest;
import android.app.Activity;
import android.content.Intent;
import android.content.pm.PackageManager;
import android.graphics.Bitmap;
import android.graphics.BitmapFactory;
import android.graphics.Canvas;
import android.graphics.Matrix;
import android.graphics.drawable.Drawable;
import android.hardware.Camera;
import android.hardware.Camera.PictureCallback;
import android.os.Bundle;
import android.support.v4.app.ActivityCompat;
import android.support.v4.content.ContextCompat;
import android.util.Log;
import android.view.Surface;
import android.view.SurfaceHolder;
import android.view.SurfaceView;
import android.view.View;
import android.view.View.OnClickListener;
import android.widget.Button;
import android.widget.ImageView;
import android.widget.Toast;

public class CameraActivity extends Activity implements PictureCallback, SurfaceHolder.Callback {

    public static final String EXTRA_CAMERA_DATA = "camera_data";

    private static final String KEY_IS_CAPTURING = "is_capturing";

    private Camera mCamera;
    private ImageView mCameraImage;
    private SurfaceView mCameraPreview;
    private Button mCaptureImageButton;
    private Button mDoneButton;
    private Button mRetakeButton;
    private static int currentCameraId = 0;
    private byte[] mCameraData;
    private boolean mIsCapturing;

    private int cameraPermissions;

    private float backCameraDegressRotate = 90;
    private float frontCameraDegressRotate = -90;

    private int cameraResume = 0;


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

    public void changeCamera(){
        if (mIsCapturing) {
            mCamera.stopPreview();
        }
        mCamera.release();
        if(currentCameraId == Camera.CameraInfo.CAMERA_FACING_BACK){
            currentCameraId = Camera.CameraInfo.CAMERA_FACING_FRONT;
        }
        else {
            currentCameraId = Camera.CameraInfo.CAMERA_FACING_BACK;
        }
        mCamera = Camera.open(currentCameraId);
        setCameraDisplayOrientation(CameraActivity.this, currentCameraId, mCamera);
        try {
            mCamera.setPreviewDisplay(mCameraPreview.getHolder());
        } catch (IOException e) {
            e.printStackTrace();
        }
        mCamera.startPreview();
    }

    public static void setCameraDisplayOrientation(Activity activity,
                                                   int cameraId, android.hardware.Camera camera) {
        android.hardware.Camera.CameraInfo info =
                new android.hardware.Camera.CameraInfo();
        android.hardware.Camera.getCameraInfo(cameraId, info);
        int rotation = activity.getWindowManager().getDefaultDisplay()
                .getRotation();
        Log.e("ROTATION", Integer.toString(rotation));
        int degrees = 0;
        switch (rotation) {
            case Surface.ROTATION_0: degrees = 0; break;
            case Surface.ROTATION_90: degrees = 90; break;
            case Surface.ROTATION_180: degrees = 180; break;
            case Surface.ROTATION_270: degrees = 270; break;
        }
        //degrees = 0;

        int result;
        if (info.facing == Camera.CameraInfo.CAMERA_FACING_FRONT) {
            result = (info.orientation + degrees) % 360;
            result = (360 - result) % 360;  // compensate the mirror
        } else {  // back-facing
            result = (info.orientation - degrees + 360) % 360;
        }
        camera.setDisplayOrientation(result);
    }

    private OnClickListener mCaptureImageButtonClickListener = new OnClickListener() {
        @Override
        public void onClick(View v) {
            captureImage();
        }
    };

    private OnClickListener mRecaptureImageButtonClickListener = new OnClickListener() {
        @Override
        public void onClick(View v) {
            setupImageCapture();
        }
    };

    private OnClickListener mDoneButtonClickListener = new OnClickListener() {
        @Override
        public void onClick(View v) {
            if (mCameraData != null) {
                Bitmap bmp = BitmapFactory.decodeByteArray(mCameraData, 0, mCameraData.length);
                ByteArrayOutputStream stream = new ByteArrayOutputStream();
                bmp.compress(Bitmap.CompressFormat.JPEG, 25, stream);
                byte[] bytes = stream.toByteArray();
                Intent intent = new Intent();
                intent.putExtra(EXTRA_CAMERA_DATA, bytes);
                intent.putExtra("CameraId", currentCameraId);
                setResult(RESULT_OK, intent);
            } else {
                setResult(RESULT_CANCELED);
            }
            Log.e("TEsT", "entrooo");
            if (mIsCapturing) {
                mCamera.stopPreview();
            }
            mCamera.release();
            cameraResume = 1;
            finish();
        }
    };

    private OnClickListener changeCameraButtonClickListener = new OnClickListener() {
        @Override
        public void onClick(View view) {
            changeCamera();
        }
    };

    @Override
    protected void onResume(){
        super.onResume();
    }

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);

        cameraPermissions = ContextCompat.checkSelfPermission(this,
                Manifest.permission.CAMERA);
        setContentView(R.layout.activity_camera);

        mCameraImage = (ImageView) findViewById(R.id.camera_image_view);
        mCameraImage.setVisibility(View.INVISIBLE);

        mCameraPreview = (SurfaceView) findViewById(R.id.preview_view);
        final SurfaceHolder surfaceHolder = mCameraPreview.getHolder();
        surfaceHolder.addCallback(this);
        surfaceHolder.setType(SurfaceHolder.SURFACE_TYPE_PUSH_BUFFERS);

        mCaptureImageButton = (Button) findViewById(R.id.capture_image_button);
        mCaptureImageButton.setOnClickListener(mCaptureImageButtonClickListener);

        mDoneButton = (Button) findViewById(R.id.done_button);
        mRetakeButton = (Button) findViewById(R.id.retake_button);
        mRetakeButton.setEnabled(false);

        final Button doneButton = (Button) findViewById(R.id.done_button);
        //doneButton.setOnClickListener(mDoneButtonClickListener);
        doneButton.setOnClickListener(changeCameraButtonClickListener);

        mIsCapturing = true;

        Log.e("KEY", Integer.toString(currentCameraId));

        if (mCamera == null) {
            if(cameraPermissions == PackageManager.PERMISSION_DENIED || cameraPermissions != PackageManager.PERMISSION_GRANTED){
                Toast.makeText(CameraActivity.this, "Permisos de camara denegados", Toast.LENGTH_LONG)
                        .show();
            }
            else{
                try {
                    //currentCameraId = Camera.CameraInfo.CAMERA_FACING_BACK;
                    mCamera = Camera.open(currentCameraId);
                    setCameraDisplayOrientation(CameraActivity.this, currentCameraId, mCamera);
                    mCamera.setPreviewDisplay(mCameraPreview.getHolder());

                    if (mIsCapturing) {
                        mCamera.startPreview();
                    }
                } catch (Exception e) {
                    Toast.makeText(CameraActivity.this, "Unable to open camera.", Toast.LENGTH_LONG)
                            .show();
                }
            }
        }
    }

    @Override
    public void onPictureTaken(byte[] data, Camera camera) {
        mCameraData = data;
        setupImageDisplay();
    }

    @Override
    public void surfaceChanged(SurfaceHolder holder, int format, int width, int height) {
        if (mCamera != null) {
            Log.e("SURFACECHANGED", "Surface Changed");
            try {
                setCameraDisplayOrientation(CameraActivity.this, currentCameraId, mCamera);
                mCamera.setPreviewDisplay(holder);
                if (mIsCapturing) {
                    mCamera.startPreview();
                }
            } catch (IOException e) {
                Toast.makeText(CameraActivity.this, "Unable to start camera preview.", Toast.LENGTH_LONG).show();
            }

        }
    }

    @Override
    public void surfaceCreated(SurfaceHolder holder) {
    }

    @Override
    public void surfaceDestroyed(SurfaceHolder holder) {
    }

    private void captureImage() {
        mCamera.takePicture(null, null, this);
        mRetakeButton.setEnabled(false);
    }

    private void setupImageCapture() {
        mCameraImage.setVisibility(View.INVISIBLE);
        mCameraPreview.setVisibility(View.VISIBLE);
        mCaptureImageButton.setVisibility(View.VISIBLE);
        mCaptureImageButton.setEnabled(true);
        mRetakeButton.setEnabled(false);

        mCamera.startPreview();
        Drawable drawable = ContextCompat.getDrawable(getApplicationContext(), R.drawable.change);
        mRetakeButton.setCompoundDrawablesWithIntrinsicBounds(drawable, null, null, null);
        mDoneButton.setCompoundDrawablesWithIntrinsicBounds(drawable, null, null, null);
        mDoneButton.setOnClickListener(changeCameraButtonClickListener);

    }

    private void setupImageDisplay() {
        Bitmap bitmap = BitmapFactory.decodeByteArray(mCameraData, 0, mCameraData.length);
        bitmap = rotateBitmap(bitmap);
        mCameraImage.setImageBitmap(bitmap);
        mCamera.stopPreview();
        mCameraPreview.setVisibility(View.INVISIBLE);
        mCameraImage.setVisibility(View.VISIBLE);
        mCaptureImageButton.setVisibility(View.INVISIBLE);
        mRetakeButton.setOnClickListener(mRecaptureImageButtonClickListener);
        mRetakeButton.setEnabled(true);
        Drawable drawable = ContextCompat.getDrawable(getApplicationContext(), R.drawable.reload);
        mRetakeButton.setCompoundDrawablesWithIntrinsicBounds(drawable, null, null, null);
        drawable = ContextCompat.getDrawable(getApplicationContext(), R.drawable.done);
        mDoneButton.setCompoundDrawablesWithIntrinsicBounds(drawable, null, null, null);
        mDoneButton.setOnClickListener(mDoneButtonClickListener);
    }
}
