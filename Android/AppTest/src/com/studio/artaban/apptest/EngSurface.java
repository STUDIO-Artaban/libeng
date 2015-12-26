package com.studio.artaban.apptest;

import com.studio.artaban.apptest.EngLibrary;

import android.content.Context;
import android.opengl.GLSurfaceView;
import android.view.MotionEvent;
import android.view.SurfaceHolder;

public class EngSurface extends GLSurfaceView {

    public EngSurface(Context context) {

        super(context);
        this.setZOrderMediaOverlay(true);
    }

    @Override public boolean onTouchEvent(final MotionEvent event) {

        if (event.getActionMasked() != MotionEvent.ACTION_MOVE) {

            int index = event.getActionIndex();
            EngLibrary.touch(event.getPointerId(index), (short)event.getActionMasked(), event.getX(index), event.getY(index));
        }
        else {
            for (int i = 0; i < event.getPointerCount(); ++i) {
                EngLibrary.touch(event.getPointerId(i), (short)event.getActionMasked(), event.getX(i), event.getY(i));
            }
        }
        return true;
    }

    @Override public void surfaceCreated(SurfaceHolder holder) {
        EngLibrary.create();
    }

    @Override public void surfaceChanged(SurfaceHolder holder, int format, int width, int height) {
        EngLibrary.change(holder.getSurface());
    }

    @Override public void surfaceDestroyed(SurfaceHolder holder) {
        EngLibrary.destroy();
    }
}
