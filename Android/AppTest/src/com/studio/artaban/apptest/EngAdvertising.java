package com.studio.artaban.apptest;

import java.security.MessageDigest;
import java.security.NoSuchAlgorithmException;
import java.util.Date;

import android.content.ContentResolver;
import android.content.Context;
import android.util.Log;
import android.view.View;

//// Remove import below when no advertising are used:
// -> Then comment code below and remove from comment the code that follows
import android.graphics.Color;
import android.location.Location;
import android.provider.Settings;

import com.google.android.gms.ads.AdRequest;
import com.google.android.gms.ads.AdListener;
import com.google.android.gms.ads.AdSize;
import com.google.android.gms.ads.AdView;
import com.google.android.gms.ads.InterstitialAd;
////

import com.studio.artaban.apptest.social.EngSocial;

public class EngAdvertising {

    static private AdView mAdView;
    static private InterstitialAd mFullAd;

    static public View getView() { return mAdView; }
    static public void initialize(Context context, boolean interstitial) {

    	mStatus = AD_STATUS_READY;
    	if (!interstitial) {

	    	mAdView = new AdView(context);
	    	mAdView.setVisibility(View.INVISIBLE);
	        mAdView.setBackgroundColor(Color.BLACK);
	        mAdView.setAdListener(new AdListener() {
	
	            @Override public void onAdClosed() { }
	            @Override public void onAdFailedToLoad(int error) {
	
	            	switch (error) {
						case AdRequest.ERROR_CODE_INTERNAL_ERROR: Log.e("EngAdvertising", "Failed to load: Internal error"); break;
						case AdRequest.ERROR_CODE_INVALID_REQUEST: Log.e("EngAdvertising", "Failed to load: Invalid request"); break;
						case AdRequest.ERROR_CODE_NETWORK_ERROR: Log.e("EngAdvertising", "Failed to load: Network Error"); break;
						case AdRequest.ERROR_CODE_NO_FILL: Log.e("EngAdvertising", "Failed to load: No fill"); break;
	            	}
	            	mStatus = AD_STATUS_FAILED;
	            }
	
	            @Override public void onAdLeftApplication() { }
	            @Override public void onAdOpened() { }
	            @Override public void onAdLoaded() {
	            	if (mStatus == AD_STATUS_LOADING) {
	
	            		if (mAdView.getVisibility() == View.INVISIBLE)
	            			mStatus = AD_STATUS_LOADED;
	            		else
	            			mStatus = AD_STATUS_DISPLAYED;
	            	}
	            }
	
	        });
    	}
    	else {

    		mFullAd = new InterstitialAd(context);
    		mFullAd.setAdListener(new AdListener() {

	            @Override public void onAdClosed() { }
	            @Override public void onAdFailedToLoad(int error) {

	            	switch (error) {
						case AdRequest.ERROR_CODE_INTERNAL_ERROR: Log.e("EngAdvertising", "Failed to load: Internal error"); break;
						case AdRequest.ERROR_CODE_INVALID_REQUEST: Log.e("EngAdvertising", "Failed to load: Invalid request"); break;
						case AdRequest.ERROR_CODE_NETWORK_ERROR: Log.e("EngAdvertising", "Failed to load: Network Error"); break;
						case AdRequest.ERROR_CODE_NO_FILL: Log.e("EngAdvertising", "Failed to load: No fill"); break;
	            	}
	            	mStatus = AD_STATUS_FAILED;
	            }

	            @Override public void onAdLeftApplication() { }
	            @Override public void onAdOpened() { }
	            @Override public void onAdLoaded() { mStatus = AD_STATUS_LOADED; }
	
	        });
    	}
    }
    static public void setBanner(short bannerID) {

    	if (mAdView == null) {
    		Log.wtf("EngAdvertising", "Illegal 'setBanner' method call: Interstitial advertising");
    		return;
    	}
    	if (mAdView.getAdSize() != null)
    		return; // The ad size can be set only once!

    	switch (bannerID) {
    		case BANNER_ID_BANNER: mAdView.setAdSize(AdSize.BANNER); break;
    		case BANNER_ID_FULL_BANNER: mAdView.setAdSize(AdSize.FULL_BANNER); break;
    		case BANNER_ID_LARGE_BANNER: mAdView.setAdSize(AdSize.LARGE_BANNER); break;
    		case BANNER_ID_LEADERBOARD: mAdView.setAdSize(AdSize.LEADERBOARD); break;
    		case BANNER_ID_MEDIUM_RECTANGLE: mAdView.setAdSize(AdSize.MEDIUM_RECTANGLE); break;
    		case BANNER_ID_WIDE_SKYSCRAPER: mAdView.setAdSize(AdSize.WIDE_SKYSCRAPER); break;
    		case BANNER_ID_SMART_BANNER: mAdView.setAdSize(AdSize.SMART_BANNER); break;
    	}
    }
    static public void setUnitID(String unitID) {

    	if (mAdView != null) {
        	if (mAdView.getAdUnitId() != null)
        		return; // The ad unit ID can be set only once!

        	mAdView.setAdUnitId(unitID);
    	}
    	else {
        	if (mFullAd.getAdUnitId() != null)
        		return; // The ad unit ID can be set only once!

    		mFullAd.setAdUnitId(unitID);
    	}
    }
    static public void load(ContentResolver contentResolver) {

		AdRequest.Builder reqBuilder = new AdRequest.Builder();
		if (contentResolver != null) // Test
	    	reqBuilder.addTestDevice(MD5(Settings.Secure.getString(contentResolver, Settings.Secure.ANDROID_ID)));

		switch (mAdReqGender) {
			case EngSocial.GENDER_MALE: reqBuilder.setGender(AdRequest.GENDER_MALE); break;
			case EngSocial.GENDER_FEMALE: reqBuilder.setGender(AdRequest.GENDER_FEMALE); break;
		}
		if (mAdReqBirthday != null)
			reqBuilder.setBirthday(mAdReqBirthday);
		if (mAdReqLocation != null)
			reqBuilder.setLocation(new Location(mAdReqLocation));

        mStatus = AD_STATUS_LOADING;
		if (mAdView != null) {

	    	try { mAdView.loadAd(reqBuilder.build()); }
	    	catch (IllegalStateException e) {
	    		Log.e("EngAdvertising", "Illegal state (size:" + mAdView.getAdSize() + ";unit:" + mAdView.getAdUnitId() + ")");
	    		mStatus = AD_STATUS_FAILED;
	    	}
		}
		else
			mFullAd.loadAd(reqBuilder.build());
    }
    static public boolean display() {

    	if (mAdView != null) {
    		Log.wtf("EngAdvertising", "Illegal 'display' method call: View advertising");
    		return false;
    	}
        if (!mFullAd.isLoaded())
        	return false;

      	mFullAd.show();
      	mStatus = AD_STATUS_DISPLAYING;
      	return true;
    }

    static public void pause() { if (mAdView != null) mAdView.pause(); }
    static public void resume() {

    	if (mAdView != null) // AdView
    		mAdView.resume();
    	else if (mStatus == AD_STATUS_DISPLAYING) // InterstitialAd
    		mStatus = AD_STATUS_READY;
    }
    static public void destroy() {
    	if (mAdView != null) {

	    	mAdView.destroy();
	    	mAdView = null;
    	}
    }

    /*
    static public View getView() { return null; }
    static public void initialize(Context context, boolean interstitial) {

    	mStatus = AD_STATUS_NONE;
    	mAdReqGender = EngSocial.GENDER_NONE;
	}
    static public void setBanner(short bannerID) { }
    static public void setUnitID(String unitID) { }
    static public void load() { }
    static public void load(ContentResolver contentResolver) { }
    static public void pause() { }
    static public void resume() { }
    static public void destroy() { }
    */

    ////// Banner ID
    static public final short BANNER_ID_BANNER = 0;
    static public final short BANNER_ID_FULL_BANNER = 1;
    static public final short BANNER_ID_LARGE_BANNER = 2;
    static public final short BANNER_ID_LEADERBOARD = 3;
    static public final short BANNER_ID_MEDIUM_RECTANGLE = 4;
    static public final short BANNER_ID_WIDE_SKYSCRAPER = 5;
    static public final short BANNER_ID_SMART_BANNER = 6;

    ////// Status
    static public final short AD_STATUS_NONE = 0;
    static public final short AD_STATUS_READY = 1;
    static public final short AD_STATUS_LOADING = 2;
    static public final short AD_STATUS_LOADED = 3;
    static public final short AD_STATUS_FAILED = 4;
    static public final short AD_STATUS_DISPLAYING = 5;
    static public final short AD_STATUS_DISPLAYED = 6;

    static public short mStatus;

    ////// Request info
    static public short mAdReqGender;
    static public Date mAdReqBirthday;
    static public String mAdReqLocation;

	//@SuppressWarnings("unused")
	private static String MD5(String deviceID) {

    	MessageDigest digest;
  	    try {
  	    	digest = java.security.MessageDigest.getInstance("MD5");
  	    }
  	    catch (NoSuchAlgorithmException e) {

  	    	Log.e("EngAdvertising", "MD5: " + e.getMessage());
  	  	    return "";
  	    }
        digest.update(deviceID.getBytes());
        byte messageDigest[] = digest.digest();

        StringBuffer hexString = new StringBuffer();
        for (int i = 0; i < messageDigest.length; ++i) {

            String h = Integer.toHexString(0xFF & messageDigest[i]);
            while (h.length() < 2)
                h = "0" + h;
            hexString.append(h);
        }
        return hexString.toString().toUpperCase();
  	}
}
