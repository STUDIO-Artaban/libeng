package com.studio.artaban.apptest;

import java.io.File;
import java.io.FileInputStream;
import java.io.FileNotFoundException;
import java.io.FileOutputStream;
import java.io.IOException;
import java.io.InputStream;
import java.nio.ByteBuffer;
import java.nio.ByteOrder;
import java.nio.IntBuffer;

import android.content.Context;
import android.graphics.Bitmap;
import android.graphics.BitmapFactory;
import android.util.Log;
import android.widget.Toast;

public class EngResources {

    private Context mContext;

    public EngResources(Context content) {
        mContext = content;
    }

    public class BmpInfo {
        public byte[] buffer;
        public short width;
        public short height;
    };

    ////// Texture
    public BmpInfo getBufferBMP(Bitmap bmpPNG) {

        ByteBuffer bbPNG = ByteBuffer.allocateDirect(bmpPNG.getHeight() * bmpPNG.getWidth() * 4);
        bbPNG.order(ByteOrder.BIG_ENDIAN);
        IntBuffer ibPNG = bbPNG.asIntBuffer();

        // Convert ARGB -> RGBA
        for (int y = bmpPNG.getHeight() - 1; y > -1; --y) {
            for (int x = 0; x < bmpPNG.getWidth(); ++x) {

                int pix = bmpPNG.getPixel(x, bmpPNG.getHeight() - y - 1);

                int alpha = (pix >> 24) & 0xff;
                int red = (pix >> 16) & 0xff;
                int green = (pix >> 8) & 0xff;
                int blue = (pix) & 0xff;

                // Make up alpha for interesting effect
                ibPNG.put(red << 24 | green << 16 | blue << 8 | alpha);
            }
        }
        bbPNG.position(0);

        BmpInfo bmpRes = new BmpInfo();
        bmpRes.buffer = bbPNG.array();
        bmpRes.width = (short)bmpPNG.getWidth();
        bmpRes.height = (short)bmpPNG.getHeight();

        return bmpRes;
    }
    public BmpInfo getBufferPNG(String png) {
        try {
            InputStream inBmp = mContext.getAssets().open(png);
            return getBufferBMP(BitmapFactory.decodeStream(inBmp));
        }
        catch (IOException e) {
            return null;
        }
    }

    ////// Sound
    public byte[] getBufferOGG(String ogg) {
        try  {
            InputStream inOGG = mContext.getAssets().open(ogg);
            byte[] bbOGG = new byte[inOGG.available()];
            inOGG.read(bbOGG);
            inOGG.close();
            
            return bbOGG;
        }
        catch (java.io.IOException e) {
            return null;
        }
    }

    ////// Storage
    static private final String FILE_NOT_FOUND = "#FNF#";

    public boolean setFileBuffer(String file, byte[] content, int len) {
        try {
            FileOutputStream fos = mContext.openFileOutput(file, Context.MODE_PRIVATE);

            // If already exists: Overwrite!
            try {

                fos.write(content, 0, len);
                fos.close();
                return true;
            }
            catch (IOException e) {
                return false;
            }
        }
        catch (FileNotFoundException e) {
            return false; // Should create the file if not exists!
        }
    }
    public String getFileBuffer(String file) {
        try {
            FileInputStream fis = mContext.openFileInput(file);
            StringBuilder strBuffer = new StringBuilder();
            try {

                int chr;
                while ((chr = fis.read()) != -1)
                    strBuffer.append((char)chr);

                fis.close();
                return strBuffer.toString();
            }
            catch (IOException e) {
                return null;
            }
        }
        catch (FileNotFoundException e) {
            return FILE_NOT_FOUND;
        }
    }
    public boolean saveBMP(File bmpFile, short width, short height, byte[] rgba) {

    	Bitmap.CompressFormat fileFormat = Bitmap.CompressFormat.JPEG;
    	if (bmpFile.getAbsolutePath().substring(bmpFile.getAbsolutePath().length() - 3).toUpperCase().equals("PNG"))
    		fileFormat = Bitmap.CompressFormat.PNG;

    	if (rgba.length != (width * height * 4)) {
            Log.e("EngResources", "Wrong RGBA buffer lenght");
    		return false;
    	}

    	Bitmap bmpToSave = null;
    	try { bmpToSave = Bitmap.createBitmap(width, height, Bitmap.Config.ARGB_8888); }
        catch (OutOfMemoryError e) {

            Log.e("EngResources", "Failed to save BMP: " + e.getMessage());
            return false;
        }
    	if (bmpToSave == null) {
            Log.e("EngResources", "Failed to create BMP");
    		return false;
    	}

        try { bmpToSave.copyPixelsFromBuffer(ByteBuffer.wrap(rgba)); }
        catch (OutOfMemoryError e) {

            Log.e("EngResources", "Failed to save BMP: " + e.getMessage());
            return false;
        }

        try {

            FileOutputStream fos = new FileOutputStream(bmpFile);
            bmpToSave.compress(fileFormat, 90, fos); // Picture file precision: 90
            fos.flush();
            fos.close();
        }
        catch (FileNotFoundException e) {
            Log.e("EngResources", "Failed to save BMP: " + e.getMessage());
            return false;
        }
        catch (IOException e) {
            Log.e("EngResources", "Failed to save BMP: " + e.getMessage());
            return false;
        }
        return true;
    }

    ////// Alert
    static public class AlertRunnable implements Runnable {

    	private String mMessage;
    	private Context mContext;

    	public AlertRunnable(Context context, String message) {

    		mMessage = message;
    		mContext = context;
    	}
		@Override public void run() {
        	Toast.makeText(mContext, mMessage, Toast.LENGTH_LONG).show();
		}
    };

}
