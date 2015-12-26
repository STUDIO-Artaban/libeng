package com.studio.artaban.apptest.social;

import java.io.FilterInputStream;
import java.io.IOException;
import java.io.InputStream;
import java.text.SimpleDateFormat;
import java.util.Date;
import java.net.MalformedURLException;
import java.net.URL;

import com.studio.artaban.apptest.EngActivity;
import com.studio.artaban.apptest.EngLibrary;
import com.studio.artaban.apptest.EngResources;

import android.content.Intent;
import android.graphics.Bitmap;
import android.graphics.BitmapFactory;
import android.os.AsyncTask;
import android.util.Log;

public abstract class EngSocial {

	////// Network ID
	static protected final short NETWORK_ID_FACEBOOK = 1; // Facebook
	static protected final short NETWORK_ID_TWITTER = 2; // Twitter
	static protected final short NETWORK_ID_GOOGLE = 3; // Google+

	protected short mNetworkID;

	public abstract void start(Intent data);
	public abstract void launch(int req, int res, Intent data);
	public abstract void stop();

	////// Request
	static protected final short REQUEST_LOGIN = 1;
	static protected final short REQUEST_INFO = 2;
	static protected final short REQUEST_PICTURE = 3;
	static protected final short REQUEST_SHARE_LINK = 4;
	static protected final short REQUEST_SHARE_VIDEO = 5;
	static protected final short REQUEST_SHARE_PICTURE = 6;

	////// Result
	static protected final short REQ_RESULT_SUCCEEDED = 2;
	static protected final short REQ_RESULT_CANCELED = 3;
	static protected final short REQ_RESULT_EXPIRED = 4;
	static protected final short REQ_RESULT_FAILED = 5;
	// Check 'Request' struct in 'libeng/Social/Network.h' file to see why results start with 2

	public abstract boolean login();
	public abstract void logout();
	public abstract boolean isLogged();

	//
	protected boolean mDisplayError; // Display a Toast message in error case
	protected EngActivity mActivity;

    protected void alert(String message) {
    	mActivity.runOnUiThread(new EngResources.AlertRunnable(mActivity, message));
    }

	////// Gender
	static public final short GENDER_NONE = 0;
	static public final short GENDER_MALE = 1;
	static public final short GENDER_FEMALE = 2;

	//
	protected String mUserID; // Social network user ID (unique ID + REQUEST_INFO failed if empty)
	protected String mUserName; // UTF-16 encoded string
	protected short mUserGender; // See definitions above
	protected Date mUserBirthday;
	protected String mUserLocation;

	public abstract boolean getUserInfo();
	public String getUserID() { return mUserID; } // Cannot be null
	public String getUserName() { return (mUserName != null)? mUserName:EngActivity.nullString; }
	public short getUserGender() { return mUserGender; }
	public String getUserBirthday() {

		if (mUserBirthday == null)
			return EngActivity.nullString;

		SimpleDateFormat strDate = new SimpleDateFormat("MM/dd/yyyy");
		return strDate.format(mUserBirthday);
	}
	public String getUserLocation() { return (mUserLocation != null)? mUserLocation:EngActivity.nullString; }

	private class PictureStream extends FilterInputStream {

		public PictureStream(InputStream in) { super(in); }
		@Override public long skip(long n) throws IOException {

		    long totalBytesSkipped = 0L;
		    while (totalBytesSkipped < n) {

		        long bytesSkipped = in.skip(n - totalBytesSkipped);
		        if (bytesSkipped == 0L) {

		              int onebyte = read();
		              if (onebyte < 0)
		                  break;  // EOF reached
		              else
		                  bytesSkipped = 1; // One byte has been read
		       }
		       totalBytesSkipped += bytesSkipped;
		    }
		    return totalBytesSkipped;
		}
	};
	protected AsyncTask<Void, Void, Void> mPicTask;
	protected class PictureTask extends AsyncTask<Void, Void, Void> {

		private String mPicURL;
		private short mPicSize;

		public PictureTask(String url, short size) {

			mPicURL = url;
			mPicSize = size;
		}
		@Override protected Void doInBackground(Void... params) {
            try {

            	URL reqURL = new URL(mPicURL);
            	PictureStream inStream = new PictureStream(reqURL.openConnection().getInputStream());
		        Bitmap bmpPIC = BitmapFactory.decodeStream(inStream);

		        EngResources resources = new EngResources(mActivity);			        
		        EngResources.BmpInfo bmpInfo = resources.getBufferBMP(bmpPIC);
		        if ((bmpInfo.width == mPicSize) && (bmpInfo.height == mPicSize))
		            EngLibrary.loadSocial(mNetworkID, REQUEST_PICTURE, REQ_RESULT_SUCCEEDED, bmpInfo.width,
		            		bmpInfo.height, bmpInfo.buffer);
		        else
		            EngLibrary.loadSocial(mNetworkID, REQUEST_PICTURE, REQ_RESULT_FAILED, (short)0, (short)0,
		            		null);
            }
            catch (MalformedURLException e) {
	            Log.e("EngSocial", "Wrong picture URL: " + e.getMessage());
	            EngLibrary.loadSocial(mNetworkID, REQUEST_PICTURE, REQ_RESULT_FAILED, (short)0, (short)0,
	            		null);
            }
            catch (Exception e) {
	            Log.e("EngSocial", "Failed to get user picture: " + e.getMessage());
	            EngLibrary.loadSocial(mNetworkID, REQUEST_PICTURE, REQ_RESULT_FAILED, (short)0, (short)0,
	            		null);
            }
			return null;
		}
	};
	public abstract boolean getUserPicture();

	//
	public abstract boolean shareLink(String[] data); // Failed if 'data' == null + Display alert message if requested
	public abstract boolean sharePicture(String[] data); // ...
	public abstract boolean shareVideo(String[] data); // ...

}
