/*
The MIT License (MIT)

Copyright (c) 2014 Jeremy C Brooks
http://vertexarray.com

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in all
copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
SOFTWARE.
*/
package com.brooks.jeremy.starglider;

import android.app.Activity;
import android.app.AlertDialog;
import android.content.DialogInterface;
import android.content.pm.ActivityInfo;
import android.os.Bundle;
import android.view.KeyEvent;
import android.view.Window;
import android.view.WindowManager;
import android.widget.LinearLayout;

import com.brooks.jeremy.starglider.glglue.StarCatcherSurfaceView;
import com.brooks.jeremy.starglider.platform.IPlatformAndroidListener;
import com.brooks.jeremy.starglider.util.SystemUiHider;
import com.google.android.gms.ads.AdListener;
import com.google.android.gms.ads.AdRequest;
import com.google.android.gms.ads.AdView;

/**
 * An example full-screen activity that shows and hides the system UI (i.e.
 * status bar and navigation/system bar) with user interaction.
 * 
 * @see SystemUiHider
 */
public class MainActivity extends Activity implements IReleaseListener,
		IPlatformAndroidListener
{

	public static IPlatformAndroidListener GameStateListener = null;
	private StarCatcherSurfaceView mSurfaceView;
	private AdView adView;
	private boolean canShowAds = false;

	@Override
	protected void onCreate(Bundle savedInstanceState)
	{
		super.onCreate(savedInstanceState);
		//
		// Remove title bar and setup requests
		//
		getWindow().addFlags(WindowManager.LayoutParams.FLAG_KEEP_SCREEN_ON);
		this.requestWindowFeature(Window.FEATURE_NO_TITLE);

		// Remove notification bar
		this.getWindow().setFlags(WindowManager.LayoutParams.FLAG_FULLSCREEN,
				WindowManager.LayoutParams.FLAG_FULLSCREEN);

		GameStateListener = this;

		setRequestedOrientation(ActivityInfo.SCREEN_ORIENTATION_PORTRAIT);

		setContentView(R.layout.activity_main);
		
		mSurfaceView = (StarCatcherSurfaceView) findViewById(R.id.fullscreen_content);

		canShowAds = true;
		adView = (AdView) this.findViewById(R.id.adView);
		adView.setAdListener(new AdListener() {
			
			@Override
			public void onAdLoaded(){
				showAds();
			}
			
			@Override
			public void onAdFailedToLoad(int errorCode){
				hideAds();
			}
			
			@Override
			public void onAdOpened(){}
			
			@Override
			public void onAdClosed(){}
			
			@Override
			public void onAdLeftApplication(){}
		});

		loadAds();
	}
	
	private void hideAds()
	{
		LinearLayout adsLayout = (LinearLayout) findViewById(R.id.layoutAds);
		adsLayout.setVisibility(LinearLayout.GONE);
		adsLayout.setEnabled(false);
	}
	
	private void showAds()
	{
		if(canShowAds){
			LinearLayout adsLayout = (LinearLayout) findViewById(R.id.layoutAds);
			adsLayout.setVisibility(LinearLayout.VISIBLE);
			adsLayout.setEnabled(true);
		}else{
			hideAds();
		}
	}

	private void loadAds()
	{
		hideAds();
		// Create an ad request. Check logcat output for the hashed device ID to
		// get test ads on a physical device.
		AdRequest adRequest = new AdRequest.Builder()
				.addTestDevice(AdRequest.DEVICE_ID_EMULATOR).build();

		// Start loading the ad in the background.
		adView.loadAd(adRequest);
		adView.invalidate();
	}

	public void Exit()
	{
		if (mSurfaceView.getGameState() == GameState.InMenu)
		{
			new AlertDialog.Builder(this)
					.setIcon(android.R.drawable.ic_dialog_alert)
					.setTitle(R.string.quit)
					.setMessage(R.string.really_quit)
					.setPositiveButton(R.string.yes,
							new DialogInterface.OnClickListener()
							{
								@Override
								public void onClick(DialogInterface dialog,
										int which)
								{

									// Stop the activity
									mSurfaceView.release(MainActivity.this);
									MainActivity.this.finish();
								}

							}).setNegativeButton(R.string.no, null).show();
		}
	}

	@Override
	public boolean onKeyDown(int keyCode, KeyEvent event)
	{
		switch (keyCode)
		{

		case KeyEvent.KEYCODE_BACK:
			Exit();
			return true;

		}
		return true;
	}

	@Override
	protected void onPause()
	{
		super.onPause();

		if ((mSurfaceView != null) && mSurfaceView.GetRendererSet())
		{
			mSurfaceView.onPause();
		}
	}

	@Override
	protected void onResume()
	{
		super.onResume();

		if ((mSurfaceView != null) && mSurfaceView.GetRendererSet())
		{
			mSurfaceView.onResume();
		}
	}

	@Override
	public void released()
	{
		this.finish();
	}

	@Override
	public void GameStarted()
	{
		runOnUiThread(new Runnable()
		{
			@Override
			public void run()
			{
				canShowAds = false;
				hideAds();
			}
		});
	}

	@Override
	public void GameEnded()
	{
		runOnUiThread(new Runnable()
		{
			@Override
			public void run()
			{
				canShowAds = true;
				loadAds();
			}
		});
	}
	
	@Override
    protected void onDestroy() {
		if(adView != null){
	    	adView.destroy();
	    	adView = null;
		}
    	super.onDestroy();
    }
}
