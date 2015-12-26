package com.studio.artaban.apptest.data;

import com.studio.artaban.apptest.EngActivity;
import com.studio.artaban.apptest.EngAdvertising;
import com.studio.artaban.apptest.EngLibrary;
import com.studio.artaban.apptest.EngResources;

import android.util.DisplayMetrics;
import android.util.Log;
import android.view.View;
import android.view.animation.Animation;
import android.view.animation.Animation.AnimationListener;
import android.view.animation.AnimationUtils;
import android.widget.RelativeLayout;

import com.studio.artaban.apptest.R;

public class EngData {

    ////// JNI
    static public final String PROJECT_NAME_LIB = "AppTest";

    ////// Languages
    @SuppressWarnings("unused") static private final short LANG_EN = 0;
    static private final short LANG_FR = 1;
    @SuppressWarnings("unused") static private final short LANG_DE = 2;
    @SuppressWarnings("unused") static private final short LANG_ES = 3;
    @SuppressWarnings("unused") static private final short LANG_IT = 4;
    @SuppressWarnings("unused") static private final short LANG_PT = 5;

    @SuppressWarnings("unused") static private final short mLanguage = LANG_FR;

    ////// Permissions
    static public final boolean USES_PERMISSION_CAMERA = true; // android.permission.CAMERA
    static public final boolean USES_PERMISSION_MIC = true; // android.permission.RECORD_AUDIO
    static public final boolean USES_PERMISSION_INTERNET = true; // android.permission.INTERNET & ACCESS_NETWORK_STATE
    static public final boolean USES_PERMISSION_STORAGE = false; // android.permission.WRITE_INTERNAL_STORAGE & WRITE_EXTERNAL_STORAGE
    static public final boolean USES_PERMISSION_BLUETOOTH = false; // android.permission.BLUETOOTH & BLUETOOTH_ADMIN

    ////// Font
    static public final boolean FONT_TEXTURE_GRAYSCALE = true;

    ////// Textures
    static private final short TEXTURE_ID_SKY = 2; // EngActivity.TEXTURE_ID_FONT + 1
    static private final short TEXTURE_ID_MOUNTAIN = 3; 
    static private final short TEXTURE_ID_FLOOR = 4; 
    static private final short TEXTURE_ID_JOYPAD = 5; 
    static private final short TEXTURE_ID_MEGAMAN = 6;

    static private final short TEXTURE_ID_BACK = 7; 
    static private final short TEXTURE_ID_ANTIALIASING = 8; 
    static private final short TEXTURE_ID_CONTROLS = 9; 
    static private final short TEXTURE_ID_TOUCH = 10; 
    static private final short TEXTURE_ID_SPHERE = 11;

    static private final short TEXTURE_ID_CHECK1 = 12; 
    static private final short TEXTURE_ID_CHECK2 = 13; 
    static private final short TEXTURE_ID_CHECK3 = 14; 
    static private final short TEXTURE_ID_CHECK4 = 15; 
    static private final short TEXTURE_ID_CHECK5 = 16; 
    static private final short TEXTURE_ID_CHECK6 = 17; 
    static private final short TEXTURE_ID_CHECK7 = 18; 
    static private final short TEXTURE_ID_CHECK8 = 19; 
    static private final short TEXTURE_ID_CHECK9 = 20; 
    static private final short TEXTURE_ID_CLAYMAN = 21; 

    static private final short TEXTURE_ID_WALKPAD = 22;
    static private final short TEXTURE_ID_WALKMAN = 23;
    static private final short TEXTURE_ID_WALKFLOOR = 24; 

    static public short loadTexture(EngResources resources, short id) {

        switch (id) {
	        case TEXTURE_ID_SKY: {
	            EngResources.BmpInfo bmpInfo = resources.getBufferPNG("sky.png");
	            return EngLibrary.loadTexture(TEXTURE_ID_SKY, bmpInfo.width, bmpInfo.height, bmpInfo.buffer, false);
	        }
	        case TEXTURE_ID_MOUNTAIN: {
	            EngResources.BmpInfo bmpInfo = resources.getBufferPNG("montains.png");
	            return EngLibrary.loadTexture(TEXTURE_ID_MOUNTAIN, bmpInfo.width, bmpInfo.height, bmpInfo.buffer, false);
	        }
	        case TEXTURE_ID_FLOOR: {
	            EngResources.BmpInfo bmpInfo = resources.getBufferPNG("floor.png");
	            return EngLibrary.loadTexture(TEXTURE_ID_FLOOR, bmpInfo.width, bmpInfo.height, bmpInfo.buffer, false);
	        }
	        case TEXTURE_ID_JOYPAD: {
	            EngResources.BmpInfo bmpInfo = resources.getBufferPNG("joypad.png");
	            return EngLibrary.loadTexture(TEXTURE_ID_JOYPAD, bmpInfo.width, bmpInfo.height, bmpInfo.buffer, true);
	        }
	        case TEXTURE_ID_MEGAMAN: {
	            EngResources.BmpInfo bmpInfo = resources.getBufferPNG("megaman.png");
	            return EngLibrary.loadTexture(TEXTURE_ID_MEGAMAN, bmpInfo.width, bmpInfo.height, bmpInfo.buffer, false);
	        }
	        case TEXTURE_ID_BACK: {
	            EngResources.BmpInfo bmpInfo = resources.getBufferPNG("back.png");
	            return EngLibrary.loadTexture(TEXTURE_ID_BACK, bmpInfo.width, bmpInfo.height, bmpInfo.buffer, false);
	        }
	        case TEXTURE_ID_ANTIALIASING: {
	            EngResources.BmpInfo bmpInfo = resources.getBufferPNG("antialiasing.png");
	            return EngLibrary.loadTexture(TEXTURE_ID_ANTIALIASING, bmpInfo.width, bmpInfo.height, bmpInfo.buffer, false);
	        }
	        case TEXTURE_ID_CONTROLS: {
	            EngResources.BmpInfo bmpInfo = resources.getBufferPNG("controls.png");
	            return EngLibrary.loadTexture(TEXTURE_ID_CONTROLS, bmpInfo.width, bmpInfo.height, bmpInfo.buffer, false);
	        }
	        case TEXTURE_ID_TOUCH: {
	            EngResources.BmpInfo bmpInfo = resources.getBufferPNG("touch.png");
	            return EngLibrary.loadTexture(TEXTURE_ID_TOUCH, bmpInfo.width, bmpInfo.height, bmpInfo.buffer, false);
	        }
	        case TEXTURE_ID_SPHERE: {
	            EngResources.BmpInfo bmpInfo = resources.getBufferPNG("sphere.png");
	            return EngLibrary.loadTexture(TEXTURE_ID_SPHERE, bmpInfo.width, bmpInfo.height, bmpInfo.buffer, false);
	        }
	        case TEXTURE_ID_CHECK1:
	        case TEXTURE_ID_CHECK2:
	        case TEXTURE_ID_CHECK3:
	        case TEXTURE_ID_CHECK4:
	        case TEXTURE_ID_CHECK5:
	        case TEXTURE_ID_CHECK6:
	        case TEXTURE_ID_CHECK7:
	        case TEXTURE_ID_CHECK8:
	        case TEXTURE_ID_CHECK9: {
	            EngResources.BmpInfo bmpInfo = resources.getBufferPNG("check" + Integer.toString(id - TEXTURE_ID_SPHERE) + ".png");
	            return EngLibrary.loadTexture(id, bmpInfo.width, bmpInfo.height, bmpInfo.buffer, false);
	        }
	        case TEXTURE_ID_CLAYMAN: {
	            EngResources.BmpInfo bmpInfo = resources.getBufferPNG("clayman.png");
	            return EngLibrary.loadTexture(TEXTURE_ID_CLAYMAN, bmpInfo.width, bmpInfo.height, bmpInfo.buffer, false);
	        }
	        case TEXTURE_ID_WALKPAD: {
	            EngResources.BmpInfo bmpInfo = resources.getBufferPNG("walkpad.png");
	            return EngLibrary.loadTexture(TEXTURE_ID_WALKPAD, bmpInfo.width, bmpInfo.height, bmpInfo.buffer, true);
	        }
	        case TEXTURE_ID_WALKMAN: {
	            EngResources.BmpInfo bmpInfo = resources.getBufferPNG("megaman.png");
	            return EngLibrary.loadTexture(TEXTURE_ID_WALKMAN, bmpInfo.width, bmpInfo.height, bmpInfo.buffer, true);
	        }
	        case TEXTURE_ID_WALKFLOOR: {
	            EngResources.BmpInfo bmpInfo = resources.getBufferPNG("floor.png");
	            return EngLibrary.loadTexture(TEXTURE_ID_WALKFLOOR, bmpInfo.width, bmpInfo.height, bmpInfo.buffer, true);
	        }

            default: {
                Log.e("EngData", "Failed to load PNG ID: " + Integer.toString(id));
                return EngActivity.NO_TEXTURE_LOADED;
            }
        }
    }

    ////// Sounds
    static private final short SOUND_ID_JUMP = 1; // EngActivity.SOUND_ID_LOGO + 1
    static private final short SOUND_ID_PIANO = 2;
    static private final short SOUND_ID_ROBOT = 3;
    static private final short SOUND_ID_CHIP = 4;

    static public void loadOgg(EngResources resources, short id) {

        switch (id) {
	        case SOUND_ID_JUMP: {
	            EngLibrary.loadOgg(SOUND_ID_JUMP, resources.getBufferOGG("LRBlast passing 01 by Lionel Allorge.ogg"), false);
	            break;
	        }
	        case SOUND_ID_PIANO: {
	            EngLibrary.loadOgg(SOUND_ID_PIANO, resources.getBufferOGG("Katatonia - Deadhouse_(piano version).ogg"), true);
	            break;
	        }
	        case SOUND_ID_ROBOT: {
	            EngLibrary.loadOgg(SOUND_ID_ROBOT, resources.getBufferOGG("trash80_-_three-four-robot-slojam.ogg"), true);
	            break;
	        }
	        case SOUND_ID_CHIP: {
	            EngLibrary.loadOgg(SOUND_ID_CHIP, resources.getBufferOGG("LRWeird 2 by Lionel Allorge.ogg"), false);
	            break;
	        }

            default: {
                Log.e("EngData", "Failed to load OGG ID: " + Integer.toString(id));
                break;
            }
        }
    }

    ////// Advertising
    static public final boolean INTERSTITIAL_AD = false; // TRUE: InterstitialAd; FALSE: AdView
    static private final boolean TEST_ADVERTISING = true; // Set to 'false' in release mode
    static private final String ADV_UNIT_ID = "ca-app-pub-1474300545363558/9895582627"; // Enter your AdMob unit ID here

    static private class AdAnimListener implements AnimationListener {

		public void onAnimationEnd(Animation animation) { EngAdvertising.mStatus = EngAdvertising.AD_STATUS_DISPLAYED; }
		public void onAnimationRepeat(Animation animation) { }
		public void onAnimationStart(Animation animation) { }
    };

    static public void loadAd(EngActivity activity) {

    	if (!USES_PERMISSION_INTERNET)
    		Log.e("EngData", "Missing INTERNET & ACCESS_NETWORK_STATE permissions");

    	//
    	DisplayMetrics metrics = new DisplayMetrics();
    	activity.getWindowManager().getDefaultDisplay().getMetrics(metrics);
        if (((activity.getWindowManager().getDefaultDisplay().getWidth() * 160) / metrics.xdpi) > 468)
        	EngAdvertising.setBanner(EngAdvertising.BANNER_ID_FULL_BANNER);
        else
    		EngAdvertising.setBanner(EngAdvertising.BANNER_ID_BANNER);

		EngAdvertising.setUnitID(ADV_UNIT_ID);
		if (TEST_ADVERTISING)
			EngAdvertising.load(activity.getContentResolver());
		else
			EngAdvertising.load(null);
    }
    static public void displayAd(short id, EngActivity activity) {

		if (activity.mSurfaceLayout.getChildCount() == 2)
			activity.mSurfaceLayout.removeView(EngAdvertising.getView());
		EngAdvertising.getView().setVisibility(View.VISIBLE);

        RelativeLayout.LayoutParams adParams = new RelativeLayout.LayoutParams(RelativeLayout.LayoutParams.WRAP_CONTENT,
        																	RelativeLayout.LayoutParams.WRAP_CONTENT);
        adParams.addRule(RelativeLayout.ALIGN_PARENT_TOP);
        adParams.addRule(RelativeLayout.CENTER_HORIZONTAL);

        activity.mSurfaceLayout.addView(EngAdvertising.getView(), adParams);

        Animation animAds = (Animation)AnimationUtils.loadAnimation(activity, R.anim.disp_ad);
        animAds.setAnimationListener(new AdAnimListener());
        EngAdvertising.getView().startAnimation(animAds);

        EngAdvertising.mStatus = EngAdvertising.AD_STATUS_DISPLAYING;
    }
    static public void hideAd(short id, EngActivity activity) {

		EngAdvertising.getView().clearAnimation();

        EngAdvertising.mStatus = EngAdvertising.AD_STATUS_LOADED;
		EngAdvertising.getView().setVisibility(View.INVISIBLE);
		// -> Always set invisible when hidden (see 'onAdLoaded' method in 'EngAdvertising')
    }

    ////// Social
    static public final boolean INFO_FIELD_FACEBOOK_BIRTHDAY = true;
    static public final boolean INFO_FIELD_FACEBOOK_LOCATION = true;
    // NB: INFO_FIELD_FACEBOOK_NAME & INFO_FIELD_FACEBOOK_GENDER are always 'true'

}
