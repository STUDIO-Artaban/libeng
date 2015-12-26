package com.studio.artaban.apptest.social;

import com.studio.artaban.apptest.EngActivity;

//// Remove import below when no Google social network is used:
// -> Then comment code below and remove from comment the code that follows
import com.studio.artaban.apptest.EngLibrary;
import com.google.android.gms.common.ConnectionResult;
import com.google.android.gms.common.GooglePlayServicesUtil;
import com.google.android.gms.common.api.GoogleApiClient;
import com.google.android.gms.common.api.ResultCallback;
import com.google.android.gms.common.api.Status;
import com.google.android.gms.plus.Plus;
import com.google.android.gms.plus.PlusShare;
import com.google.android.gms.plus.model.people.Person;

import java.util.Date;

import android.app.Activity;
import android.content.IntentSender.SendIntentException;
import android.net.Uri;
import android.os.Bundle;
import android.util.Log;

// WARNING: Add that follows into your project
// * '<meta-data android:name="com.google.android.gms.version" android:value="@integer/google_play_services_version" />'
//   into the <application> tag of the manifest file 

import android.widget.Toast;
import android.content.Intent;

//public class EngGoogle extends EngSocial {

public class EngGoogle extends EngSocial implements GoogleApiClient.ConnectionCallbacks,
														GoogleApiClient.OnConnectionFailedListener {

    private GoogleApiClient mPlusClient;
    private ConnectionResult mConnectionResult;

    private boolean mLoginReq;
    private boolean mIntentReq;
    private boolean mDisconnected; // Only used for a Stop/Start operation

	public EngGoogle(EngActivity activity, boolean displayError) {

		mNetworkID = NETWORK_ID_GOOGLE;
		mDisplayError = displayError;
		mActivity = activity;
		mUserGender = GENDER_NONE;

		//
		mLoginReq = false;
		mIntentReq = false;
		mDisconnected = false;

	    mPlusClient = new GoogleApiClient.Builder(activity)
	        .addApi(Plus.API)
	        .addScope(Plus.SCOPE_PLUS_LOGIN)
	        .addConnectionCallbacks(this)
	        .addOnConnectionFailedListener(this)
	        .build(); 
	}

	////// Additional login result(s)
	static private final short LOGIN_RESULT_SERVICE_INVALID = 6; // EngSocial.LOGGED_RESULT_FAILED + 1
	static private final short LOGIN_RESULT_SERVICE_DISABLED = 7;
	static private final short LOGIN_RESULT_SERVICE_MISSING = 8;
	static private final short LOGIN_RESULT_UPDATE_REQUIRED = 9;
	static private final short LOGIN_RESULT_DATE_INVALID = 10;
	static private final short LOGIN_RESULT_INVALID_ACCOUNT = 11;
	static private final short LOGIN_RESULT_UNLICENSED = 12;

	static private final int SIGN_IN_REQUEST = 0;
	static private final int SHARE_LINK_REQUEST = 1;
	static private final int SHARE_VIDEO_REQUEST = 2;
	static private final int SHARE_PICTURE_REQUEST = 3;

	private void resolveSignInError() {

        if (mConnectionResult.hasResolution()) {
            try {
                mIntentReq = true;
                mConnectionResult.startResolutionForResult(mActivity, SIGN_IN_REQUEST);
            }
            catch (SendIntentException e) {
                mIntentReq = false;
                mPlusClient.connect();
            }
        }
    }

	//
	@Override public void onConnected(Bundle arg0) {

		mLoginReq = false;
		EngLibrary.loadSocial(NETWORK_ID_GOOGLE, REQUEST_LOGIN, REQ_RESULT_SUCCEEDED, (short)0, (short)0, null);
	}
	@Override public void onConnectionSuspended(int arg0) { mPlusClient.connect(); }
	@Override public void onConnectionFailed(ConnectionResult result) {

        if (!result.hasResolution()) {

            GooglePlayServicesUtil.getErrorDialog(result.getErrorCode(), mActivity, 0).show();
            return;
        }
        if (!mIntentReq) {

            mConnectionResult = result;
            if (mLoginReq) {

                resolveSignInError();
                return;
            }
        }
		switch (result.getErrorCode()) {

			case ConnectionResult.CANCELED: {
				EngLibrary.loadSocial(NETWORK_ID_GOOGLE, REQUEST_LOGIN, REQ_RESULT_CANCELED, (short)0, (short)0, null);
				break;
			}
			case ConnectionResult.DATE_INVALID: {

				Log.w("EngGoogle", "The device date is likely set incorrectly");
				if (mDisplayError)
					alert("ERROR: Invalid device date!");
				EngLibrary.loadSocial(NETWORK_ID_GOOGLE, REQUEST_LOGIN, LOGIN_RESULT_DATE_INVALID, (short)0, (short)0, null);
				break;
			}
			case ConnectionResult.INVALID_ACCOUNT: {

				Log.w("EngGoogle", "The account is invalid");
				if (mDisplayError)
					alert("ERROR: Invalid account!");
				EngLibrary.loadSocial(NETWORK_ID_GOOGLE, REQUEST_LOGIN, LOGIN_RESULT_INVALID_ACCOUNT, (short)0, (short)0, null);
				break;
			}
			case ConnectionResult.LICENSE_CHECK_FAILED: {

				Log.w("EngGoogle", "The application is not licensed to the user");
				if (mDisplayError)
					alert("ERROR: Application not licensed!");
				EngLibrary.loadSocial(NETWORK_ID_GOOGLE, REQUEST_LOGIN, LOGIN_RESULT_UNLICENSED, (short)0, (short)0, null);
				break;
			}
			case ConnectionResult.SERVICE_DISABLED: {

				Log.w("EngGoogle", "Google Play services has been disabled on this device");
				if (mDisplayError)
					alert("ERROR: Google Play services disabled!");
				EngLibrary.loadSocial(NETWORK_ID_GOOGLE, REQUEST_LOGIN, LOGIN_RESULT_SERVICE_DISABLED, (short)0, (short)0, null);
				break;
			}
			case ConnectionResult.SERVICE_INVALID: {

				Log.w("EngGoogle", "Google Play services installed on this device is not authentic");
				if (mDisplayError)
					alert("ERROR: Google Play services is not authentic!");
				EngLibrary.loadSocial(NETWORK_ID_GOOGLE, REQUEST_LOGIN, LOGIN_RESULT_SERVICE_INVALID, (short)0, (short)0, null);
				break;
			}
			case ConnectionResult.SERVICE_MISSING: {

				Log.w("EngGoogle", "Google Play services is missing on this device");
				if (mDisplayError)
					alert("ERROR: Missing Google Play services!");
				EngLibrary.loadSocial(NETWORK_ID_GOOGLE, REQUEST_LOGIN, LOGIN_RESULT_SERVICE_MISSING, (short)0, (short)0, null);
				break;
			}
			case ConnectionResult.SERVICE_VERSION_UPDATE_REQUIRED: {

				Log.w("EngGoogle", "Google Play services is out of date");
				if (mDisplayError)
					alert("ERROR: Google Play services is out of date!");
				EngLibrary.loadSocial(NETWORK_ID_GOOGLE, REQUEST_LOGIN, LOGIN_RESULT_UPDATE_REQUIRED, (short)0, (short)0, null);
				break;
			}
			//case ConnectionResult.API_UNAVAILABLE:
			//case ConnectionResult.DEVELOPER_ERROR:
			//case ConnectionResult.DRIVE_EXTERNAL_STORAGE_REQUIRED:
			//case ConnectionResult.INTERNAL_ERROR:
			//case ConnectionResult.INTERRUPTED:
			//case ConnectionResult.NETWORK_ERROR:
			//case ConnectionResult.RESOLUTION_REQUIRED:
			//case ConnectionResult.SIGN_IN_REQUIRED:
			//case ConnectionResult.SUCCESS:
			//case ConnectionResult.TIMEOUT: {
			default: {

				Log.w("EngGoogle", "Failed to login: " + result.toString());
                if (mDisplayError)
                	alert("ERROR: Failed to login!");
				EngLibrary.loadSocial(NETWORK_ID_GOOGLE, REQUEST_LOGIN, REQ_RESULT_FAILED, (short)0, (short)0, null);
				break;
			}
		}
	}

	////// Activity
	public void start(Intent data) {
		if (mDisconnected) {

			mDisconnected = false;
			mPlusClient.connect();
		}
	}
	public void launch(int req, int res, Intent data) {
		switch (req) {
			case SIGN_IN_REQUEST: {

	            if (res != Activity.RESULT_OK)
	                mLoginReq = false;

	            mIntentReq = false;
	            if (!mPlusClient.isConnecting())
	                mPlusClient.connect();
				break;
			}
			case SHARE_LINK_REQUEST: {

	        	if (res != Activity.RESULT_OK) {
	                if (mDisplayError)
						Toast.makeText(mActivity, "ERROR: Failed to share link!", Toast.LENGTH_SHORT).show();
					EngLibrary.loadSocial(NETWORK_ID_GOOGLE, REQUEST_SHARE_LINK, REQ_RESULT_FAILED, (short)0, (short)0, null);
	        	}
	        	else {
	                if (mDisplayError)
						Toast.makeText(mActivity, "INFO: Link shared on Google+!", Toast.LENGTH_SHORT).show();
					EngLibrary.loadSocial(NETWORK_ID_GOOGLE, REQUEST_SHARE_LINK, REQ_RESULT_SUCCEEDED, (short)0, (short)0, null);
	        	}
				break;
			}
			case SHARE_VIDEO_REQUEST: {

	        	if (res != Activity.RESULT_OK) {
	                if (mDisplayError)
						Toast.makeText(mActivity, "ERROR: Failed to share video!", Toast.LENGTH_SHORT).show();
					EngLibrary.loadSocial(NETWORK_ID_GOOGLE, REQUEST_SHARE_VIDEO, REQ_RESULT_FAILED, (short)0, (short)0, null);
	        	}
	        	else {
	                if (mDisplayError)
						Toast.makeText(mActivity, "INFO: Video shared on Google+!", Toast.LENGTH_SHORT).show();
					EngLibrary.loadSocial(NETWORK_ID_GOOGLE, REQUEST_SHARE_VIDEO, REQ_RESULT_SUCCEEDED, (short)0, (short)0, null);
	        	}
				break;
			}
		}
	}
	public void stop() {
		if (mPlusClient.isConnected()) {
			mPlusClient.disconnect();
			mDisconnected = true;
		}
	}

	////// login
	@Override public boolean login() {

		if (!EngActivity.isOnline(200)) {

    		Log.w("EngGoogle", "Device not connected");
            if (mDisplayError)
        		alert("ERROR: Device not connected!");
			return false;
		}
		if (GooglePlayServicesUtil.isGooglePlayServicesAvailable(mActivity) != ConnectionResult.SUCCESS) {

    		Log.w("EngGoogle", "Google Play services not installed/updated");
            if (mDisplayError)
        		alert("ERROR: Google Play services not installed/updated!");
			return false;
		}
		mLoginReq = true;
		mActivity.runOnUiThread(new Runnable() {
			@Override public void run() { mPlusClient.connect(); }
		});
		return true;
	}
	@Override public void logout() {

        if (mPlusClient.isConnected()) {

            Plus.AccountApi.clearDefaultAccount(mPlusClient);
            Plus.AccountApi.revokeAccessAndDisconnect(mPlusClient).setResultCallback(new ResultCallback<Status>() {
            	@Override public void onResult(Status arg0) { }
            });
            mPlusClient.disconnect();
        }
	}
	@Override public boolean isLogged() { return mPlusClient.isConnected(); }

	////// Info
	@Override public boolean getUserInfo() {

        if (!mPlusClient.isConnected()) {

    		Log.w("EngGoogle", "No user connected");
            if (mDisplayError)
            	alert("ERROR: Failed to get user info!");
        	return false;
        }
		try {

			Person currentUser = Plus.PeopleApi.getCurrentPerson(mPlusClient);
			if (currentUser == null) {

	    		Log.w("EngGoogle", "Failed to get user info");
	            if (mDisplayError)
	        		alert("ERROR: Failed to get user info!");
				return false;
			}
			if (!currentUser.hasId()) {

                Log.e("EngGoogle", "Info error: User ID empty");
                if (mDisplayError)
                	alert("ERROR: Failed to get user info!");
				return false;
			}
			mUserID = currentUser.getId();
			if (currentUser.hasDisplayName())
				mUserName = currentUser.getDisplayName();
			if (currentUser.hasGender()) {
				if (currentUser.getGender() == 1)
		    		mUserGender = GENDER_FEMALE;
		    	else
		    		mUserGender = GENDER_MALE;
			}
			if (currentUser.hasBirthday()) {
				try { // 'currentUser.getBirthday()' date format: YYYY-MM-DD
		    		mUserBirthday = new Date(Integer.parseInt(currentUser.getBirthday().substring(0, 4)) - 1900, // Year
		    				Integer.parseInt(currentUser.getBirthday().substring(5, 7)) - 1, // Month
		    				Integer.parseInt(currentUser.getBirthday().substring(8))); // Day
				}
				catch (IndexOutOfBoundsException e) {
					mUserBirthday = null;
				}
			}
			if (currentUser.hasCurrentLocation())
				mUserLocation = currentUser.getCurrentLocation();

			EngLibrary.loadSocial(NETWORK_ID_GOOGLE, REQUEST_INFO, REQ_RESULT_SUCCEEDED, (short)0, (short)0, null);
			return true;
		}
		catch (Exception e) {

    		Log.w("EngGoogle", "Failed to get user info: " + e.getMessage());
            if (mDisplayError)
        		alert("ERROR: Failed to get user info!");
			return false;
		}
	}

	////// Picture
	static private final short PICTURE_SIZE = 256;

	@Override public boolean getUserPicture() {

        if (!mPlusClient.isConnected()) {
    		Log.w("EngGoogle", "No user connected");
            return false;
        }
		try {

			Person currentUser = Plus.PeopleApi.getCurrentPerson(mPlusClient);
			if ((currentUser == null) || (!currentUser.hasImage())) {
	    		Log.w("EngGoogle", "Failed to get user picture URL");
				return false;
			}
			// https://lh3.googleusercontent.com/-XdUIqdMkCWA/AAAAAAAAAAI/AAAAAAAAAAA/4252rscbv5M/photo.jpg?sz=50
			String picURL = currentUser.getImage().getUrl();

			// Replace "sz=50" by "sz=256" to request expected picture size
            picURL = picURL.substring(0, picURL.length() - 2) + PICTURE_SIZE;

        	mPicTask = new PictureTask(picURL, PICTURE_SIZE);
        	mPicTask.execute();
    		return true;
		}
		catch (Exception e) {

    		Log.w("EngGoogle", "Failed to get user picture URL: " + e.getMessage());
			return false;
		}
	}

	////// Share
	static private final short SHARE_FIELD_URL = 0; // Link URL (REQUEST_SHARE_LINK) & Video/Picture file name (REQUEST_SHARE_VIDEO/PICTURE)
	static private final short SHARE_FIELD_TYPE = 1; // Video/Picture mime type (REQUEST_SHARE_VIDEO/PICTURE)
	static private final short SHARE_FIELD_TITLE = 2; // Video/Picture title (REQUEST_SHARE_VIDEO/PICTURE)

	@Override public boolean shareLink(String[] data) {

        if (data == null) {

    		Log.w("EngGoogle", "No link data to share");
            if (mDisplayError)
            	alert("ERROR: Failed to share link!");
        	return false;
        }

        //
        if (!mPlusClient.isConnected()) {
    		Log.w("EngGoogle", "No user connected");
            return false;
        }
		Intent shareIntent = new PlusShare.Builder(mActivity)
			.addCallToAction("VIEW", Uri.parse(data[SHARE_FIELD_URL]), "/libeng/view")
			.setContentUrl(Uri.parse(data[SHARE_FIELD_URL]))
			.setContentDeepLinkId("/libeng/", null, null, null)
			.getIntent();

		mActivity.startActivityForResult(shareIntent, SHARE_LINK_REQUEST);
		return true;
	}
	@Override public boolean sharePicture(String[] data) {

        if ((data == null) || (data[SHARE_FIELD_URL] == null) || (data[SHARE_FIELD_URL].length() == 0)) {

    		Log.w("EngGoogle", "No picture file to share");
            if (mDisplayError)
            	alert("ERROR: Failed to share picture!");
        	return false;
        }

        //
        if (!mPlusClient.isConnected()) {
    		Log.w("EngGoogle", "No user connected");
            return false;
        }
        Uri pictureURI = mActivity.saveMedia(data[SHARE_FIELD_URL], data[SHARE_FIELD_TYPE], data[SHARE_FIELD_TITLE]);
        if (pictureURI == null) {

    		Log.w("EngGoogle", "No '" + data[SHARE_FIELD_URL] + "' picture file into 'android.MediaStore'");
            if (mDisplayError)
            	alert("ERROR: Failed to share picture!");
        	return false;
        }
		Intent shareIntent = new PlusShare.Builder(mActivity)
			.setStream(pictureURI)
			.setType(data[SHARE_FIELD_TYPE])
			.getIntent();

		mActivity.startActivityForResult(shareIntent, SHARE_PICTURE_REQUEST);
        return true;
	}
	@Override public boolean shareVideo(String[] data) {

        if ((data == null) || (data[SHARE_FIELD_URL] == null) || (data[SHARE_FIELD_URL].length() == 0)) {

    		Log.w("EngGoogle", "No video file to share");
            if (mDisplayError)
            	alert("ERROR: Failed to share video!");
        	return false;
        }

        //
        if (!mPlusClient.isConnected()) {
    		Log.w("EngGoogle", "No user connected");
            return false;
        }
        Uri videoURI = mActivity.saveMedia(data[SHARE_FIELD_URL], data[SHARE_FIELD_TYPE], data[SHARE_FIELD_TITLE]);
        if (videoURI == null) {

    		Log.w("EngGoogle", "No '" + data[SHARE_FIELD_URL] + "' video file into 'android.MediaStore'");
            if (mDisplayError)
            	alert("ERROR: Failed to share video!");
        	return false;
        }
		Intent shareIntent = new PlusShare.Builder(mActivity)
			.setStream(videoURI)
			.setType(data[SHARE_FIELD_TYPE])
			.getIntent();

		mActivity.startActivityForResult(shareIntent, SHARE_VIDEO_REQUEST);
        return true;
	}

    /*
	public EngGoogle(EngActivity activity, boolean displayError) { }

	public void start() { }
	public void launch(int req, int res, Intent data) { }
	public void stop() { }

	@Override public boolean login() { return false; }
	@Override public void logout() { }
	@Override public boolean isLogged() { return false; }
	@Override public boolean getUserPicture() { return false; }
	@Override public boolean getUserInfo() { return false; }
	@Override public boolean shareLink(String[] data) { return false; }
	@Override public boolean sharePicture(String[] data) { return false; }
	@Override public boolean shareVideo(String[] data) { return false; }
	*/

    ////// Texture ID
    static public final short TEXTURE_ID = 251; // Reserved

}
