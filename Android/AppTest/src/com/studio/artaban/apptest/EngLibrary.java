package com.studio.artaban.apptest;

import com.studio.artaban.apptest.data.EngData;

import android.content.Intent;
import android.view.Surface;

public class EngLibrary {

    static {
        System.loadLibrary("ogg");
        System.loadLibrary("vorbis");
        System.loadLibrary("openal");
        System.loadLibrary("eng");

        System.loadLibrary(EngData.PROJECT_NAME_LIB); // JNI library name
    }

    public static native void init(EngActivity activity, int millis, float accelRange, float xDpi, float yDpi); // onCreate
    public static native void create(); // surfaceCreated
    public static native void start(Intent data); // onStart
    public static native void change(Surface surface); // surfaceChanged
    public static native void resume(int millis); // onResume
    public static native void result(int req, int res, Intent data); // onActivityResult
    public static native void pause(boolean finishing, boolean lockScreen); // onPause
    public static native void stop(); // onStop
    public static native void destroy(); // surfaceDestroyed
    public static native void free(); // onDestroy

    public static native short loadTexture(short id, short width, short height, byte[] data, boolean grayscale);
    public static native void loadOgg(short id, byte[] data, boolean queued);
    public static native boolean loadFile(String file, String content);
    public static native void loadCamera(byte[] data);
    public static native void loadMic(int len, short[] data);
    public static native void loadSocial(short id, short request, short result, short width, short height, byte[] data);
    public static native void loadStore(short result);

    public static native void touch(int id, short type, float x, float y);
    public static native void accelerometer(float xRate, float yRate, float zRate);

}
