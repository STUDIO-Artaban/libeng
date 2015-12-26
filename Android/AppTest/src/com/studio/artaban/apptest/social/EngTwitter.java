package com.studio.artaban.apptest.social;

import java.io.File;

import com.studio.artaban.apptest.EngActivity;
import com.studio.artaban.apptest.EngLibrary;

////Remove import below when no Twitter social network is used:
//-> Then comment code below and remove from comment the code that follows
import twitter4j.Twitter;
import twitter4j.TwitterException;
import twitter4j.TwitterFactory;
import twitter4j.auth.AccessToken;
import twitter4j.auth.RequestToken;
import twitter4j.conf.Configuration;
import twitter4j.conf.ConfigurationBuilder;

import android.content.Intent;
import android.net.Uri;
import android.os.AsyncTask;
import android.util.Log;

// WARNING: Add that follows into your project
// <intent-filter>
//     <action android:name="android.intent.action.VIEW" />
//     <category android:name="android.intent.category.DEFAULT" />
//     <category android:name="android.intent.category.BROWSABLE" />
//     <data android:scheme="oauth" android:host="t4jsample"/>
// </intent-filter>
// into the <application><activity> tag of the manifest file 

public class EngTwitter extends EngSocial {

    private static final String CONSUMER_KEY = "jcHAb671bWKKVaFpXS4HhICKU"; 
    private static final String CONSUMER_SECRET = "t7G2yjXLvUiYC2RXOVpMoI4nFtOtaQ8HfXr3vjkJIctRkdsJ71";
    private static final String CALLBACK_URL = "oauth://t4jsample";

    private Twitter mTwitter;
    private RequestToken mReqToken;
    private String mAuthVerif;

    private boolean mLoginReq;
    private boolean mLogoutReq;
    private boolean mLogged;

	public EngTwitter(EngActivity activity, boolean displayError) {

		mNetworkID = NETWORK_ID_TWITTER;
		mDisplayError = displayError;
		mActivity = activity;
		mUserGender = GENDER_NONE;

		//
		mLoginReq = false;
		mLogoutReq = false;
		mLogged = false;

		ConfigurationBuilder builder = new ConfigurationBuilder();
        builder.setOAuthConsumerKey(CONSUMER_KEY);
        builder.setOAuthConsumerSecret(CONSUMER_SECRET);

        TwitterFactory factory = new TwitterFactory(builder.build());
        mTwitter = factory.getInstance();
	}

	////// Task
    private static final short TASK_LOGIN = 0;
    private static final short TASK_TWEET = 1;
    private static final short TASK_PICTURE = 2;
    private static final short TASK_VIDEO = 3;

	class twitterTask extends AsyncTask<Void, Void, Void> {

		private short mTaskID;
		private String mTweet;
		private String mMediaPath;

		public twitterTask(short task, String tweet, String media) {

			mTaskID = task;
			mTweet = tweet;
			mMediaPath = media;
		}
		@Override protected Void doInBackground(Void... arg0) {
			switch (mTaskID) {

				case TASK_LOGIN: {
					try { 

						AccessToken token = mTwitter.getOAuthAccessToken(mReqToken, mAuthVerif);
						ConfigurationBuilder builder = new ConfigurationBuilder();
						Configuration config = builder
											   .setOAuthConsumerKey(CONSUMER_KEY)
											   .setOAuthConsumerSecret(CONSUMER_SECRET)
											   .setOAuthAccessToken(token.getToken())
											   .setOAuthAccessTokenSecret(token.getTokenSecret())
											   .build();
						mTwitter = new TwitterFactory(config).getInstance();
						mLogged = true;
						EngLibrary.loadSocial(NETWORK_ID_TWITTER, REQUEST_LOGIN, REQ_RESULT_SUCCEEDED, (short)0, (short)0, null);				
			        }
					catch (TwitterException e) {

						Log.e("EngTwitter", "Access token exception: " + e.getMessage());
			            if (mDisplayError)
			        		alert("ERROR: Wrong Twitter access token!");
						EngLibrary.loadSocial(NETWORK_ID_TWITTER, REQUEST_LOGIN, REQ_RESULT_FAILED, (short)0, (short)0, null);
					}
					break;
				}
				case TASK_TWEET: {
					try {

						mTwitter.updateStatus(mTweet);
		                if (mDisplayError)
		                	alert("INFO: Link shared on Twitter!");
						EngLibrary.loadSocial(NETWORK_ID_TWITTER, REQUEST_SHARE_LINK, REQ_RESULT_SUCCEEDED, (short)0, (short)0, null);
					}
					catch (TwitterException e) {

						Log.e("EngTwitter", "Twitter exception (TASK_TWEET): " + e.getMessage());
						if (mDisplayError)
		                	alert("ERROR: Failed to share link!");
						EngLibrary.loadSocial(NETWORK_ID_TWITTER, REQUEST_SHARE_LINK, REQ_RESULT_FAILED, (short)0, (short)0, null);
					}
					break;
				}
				case TASK_PICTURE:
				case TASK_VIDEO: {

					twitter4j.StatusUpdate status = new twitter4j.StatusUpdate(mTweet);
			        status.setMedia(new File(mMediaPath));
			        try {
						mTwitter.updateStatus(status);
		                if (mDisplayError)
		                	alert("INFO: Media shared on Twitter!");
	                	if (mTaskID == TASK_PICTURE)
	                		EngLibrary.loadSocial(NETWORK_ID_TWITTER, REQUEST_SHARE_PICTURE, REQ_RESULT_SUCCEEDED, (short)0, (short)0, null);
	                	else
	                		EngLibrary.loadSocial(NETWORK_ID_TWITTER, REQUEST_SHARE_VIDEO, REQ_RESULT_SUCCEEDED, (short)0, (short)0, null);
					}
			        catch (TwitterException e) {

						Log.e("EngTwitter", "Twitter exception (TASK_MEDIA): " + e.getMessage());
						if (mDisplayError)
		                	alert("ERROR: Failed to share media!");
	                	if (mTaskID == TASK_PICTURE)
	                		EngLibrary.loadSocial(NETWORK_ID_TWITTER, REQUEST_SHARE_PICTURE, REQ_RESULT_FAILED, (short)0, (short)0, null);
	                	else
	                		EngLibrary.loadSocial(NETWORK_ID_TWITTER, REQUEST_SHARE_VIDEO, REQ_RESULT_FAILED, (short)0, (short)0, null);
			        }
					break;
				}
			}
			return null;
		}
	}

	//////
	public void start(Intent data) {
		if (mLoginReq) {

			mLoginReq = false;
			Uri uri = data.getData();
			if ((uri != null) && (uri.toString().startsWith(CALLBACK_URL))) {

				mAuthVerif = uri.getQueryParameter("oauth_verifier");
				if (mAuthVerif == null) {

					// oauth://t4jsample?denied=znrY8trGa7tYpuUuIYQIfVZ19gZrdl7G
					EngLibrary.loadSocial(NETWORK_ID_TWITTER, REQUEST_LOGIN, REQ_RESULT_CANCELED, (short)0, (short)0, null);
					mLogoutReq = false;
				}
				//else // oauth://t4jsample?oauth_token=7maDx4fwvrbjuXKZUro4aC9h0caSR70s&oauth_verifier=6PIfO0fuN0E6lnXCche3E4Kyzt80FhNi
				new twitterTask(TASK_LOGIN, null, null).execute();
	        }
			else {

				Log.e("EngTwitter", "Unexpected intent: " + data.toString()); // Should not happen!
	            if (mDisplayError)
	        		alert("ERROR: Wrong Twitter login URL!");
				EngLibrary.loadSocial(NETWORK_ID_TWITTER, REQUEST_LOGIN, REQ_RESULT_FAILED, (short)0, (short)0, null);
			}
		}
	}
	public void launch(int req, int res, Intent data) {	}
	public void stop() { }

	//////
	@Override public boolean login() {

		if (!EngActivity.isOnline(200)) {

    		Log.w("EngTwitter", "Device not connected");
            if (mDisplayError)
        		alert("ERROR: Device not connected!");
			return false;
		}
		if ((mLoginReq) || (mLogged)) {
    		Log.e("EngTwitter", "Already logged or login in progress");
			return false;
		}

		//
        try {
        	mReqToken = mTwitter.getOAuthRequestToken(CALLBACK_URL);
			mLoginReq = true;
        	mActivity.runOnUiThread(new Runnable() {
        		@Override public void run() {

		        	String forceLogin = "";
		        	if (mLogoutReq) {

		        		forceLogin = "&force_login=true";
		        		mLogoutReq = false;
		        	}
		        	mActivity.startActivity(new Intent(Intent.ACTION_VIEW, Uri.parse(mReqToken.getAuthenticationURL() +
		        			forceLogin)));
				}
        	});
        }
        catch (TwitterException e) {
        	Log.e("EngTwitter", "Login exception: " + e.getMessage());
            if (mDisplayError)
        		alert("ERROR: Twitter login failed!");
    		return false;
        }
        return true;
	}
	@Override public void logout() {
		if (mLogged) {

			mLogged = false;
			mLogoutReq = true;
		}
	}
	@Override public boolean isLogged() { return mLogged; }

	//////
	static private final short PICTURE_SIZE = 48;

	@Override public boolean getUserInfo() {

        if (!mLogged) {

    		Log.w("EngTwitter", "Failed to get user info: Not logged");
            if (mDisplayError)
            	alert("ERROR: Failed to get user info!");
        	return false;
        }
		try {
			twitter4j.User info = mTwitter.showUser(mTwitter.getId());

			mUserID = info.getScreenName();
			mUserName = info.getName();
			mUserLocation = info.getLocation();
			//mUserGender
			//mUserBirthday

			EngLibrary.loadSocial(NETWORK_ID_TWITTER, REQUEST_INFO, REQ_RESULT_SUCCEEDED, (short)0, (short)0, null);
			return true;
		}
		catch (IllegalStateException e) {
    		Log.e("EngTwitter", "Illegal state exception (getUserInfo): " + e.getMessage());
		}
		catch (TwitterException e) {
    		Log.e("EngTwitter", "Twitter exception (getUserInfo): " + e.getMessage());
		}
        if (mDisplayError)
        	alert("ERROR: Failed to get user info!");
		return false;
	}
	@Override public boolean getUserPicture() {

        if (!mLogged) {
    		Log.w("EngTwitter", "Failed to get user picture: Not logged");
        	return false;
        }
		try {
			twitter4j.User info = mTwitter.showUser(mTwitter.getId());

        	mPicTask = new PictureTask(info.getProfileImageURL(), PICTURE_SIZE);
        	mPicTask.execute();
    		return true;
		}
		catch (IllegalStateException e) {
    		Log.w("EngTwitter", "Illegal state exception (getUserPicture): " + e.getMessage());
		}
		catch (TwitterException e) {
    		Log.w("EngTwitter", "Twitter exception (getUserPicture): " + e.getMessage());
		}
		return false;
	}

	////// Share
	private static final short SHARE_FIELD_TWEET = 0;
	private static final short SHARE_FIELD_MEDIA = 1; // Video/Picture file path (REQUEST_SHARE_VIDEO/PICTURE)

	@Override public boolean shareLink(String[] data) {

        if (data == null) {

    		Log.w("EngTwitter", "No link data to share");
            if (mDisplayError)
            	alert("ERROR: Failed to share link!");
        	return false;
        }

        //
        if (!mLogged) {
    		Log.w("EngTwitter", "Failed to share link: Not logged");
        	return false;
        }
        new twitterTask(TASK_TWEET, data[SHARE_FIELD_TWEET], null).execute();
		return true;
	}
	@Override public boolean sharePicture(String[] data) {

        if (data == null) {

    		Log.w("EngTwitter", "No picture data to share");
            if (mDisplayError)
            	alert("ERROR: Failed to share picture!");
        	return false;
        }

        //
        if (!mLogged) {
    		Log.w("EngTwitter", "Failed to share picture: Not logged");
        	return false;
        }
        new twitterTask(TASK_PICTURE, data[SHARE_FIELD_TWEET], data[SHARE_FIELD_MEDIA]).execute();
		return true;
	}
	@Override public boolean shareVideo(String[] data) {

        if (data == null) {

    		Log.w("EngTwitter", "No video data to share");
            if (mDisplayError)
            	alert("ERROR: Failed to share video!");
        	return false;
        }

        //
        if (!mLogged) {
    		Log.w("EngTwitter", "Failed to share video: Not logged");
        	return false;
        }
        new twitterTask(TASK_VIDEO, data[SHARE_FIELD_TWEET], data[SHARE_FIELD_MEDIA]).execute();
		return true;
	}

	/*
	public EngTwitter(EngActivity activity, boolean displayError) {

		mNetworkID = NETWORK_ID_TWITTER;
		mDisplayError = displayError;
		mActivity = activity;
		mUserGender = GENDER_NONE;
	}

	public void start(Intent data) { }
	public void launch(int req, int res, Intent data) { }
	public void stop() { }

	@Override public boolean login() {

        if (mDisplayError)
    		alert("ERROR: Not implemented yet!");
		return false;
	}
	@Override public void logout() { }
	@Override public boolean isLogged() { return false; }
	@Override public boolean getUserPicture() { return false; }
	@Override public boolean getUserInfo() { return false; }
	@Override public boolean shareLink(String[] data) { return false; }
	@Override public boolean sharePicture(String[] data) { return false; }
	@Override public boolean shareVideo(String[] data) { return false; }
	*/

    ////// Texture ID
    static public final short TEXTURE_ID = 252; // Reserved

}
