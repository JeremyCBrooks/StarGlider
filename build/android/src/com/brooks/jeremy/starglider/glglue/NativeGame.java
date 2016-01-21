package com.brooks.jeremy.starglider.glglue;

import android.content.Context;
import android.content.pm.ApplicationInfo;
import android.content.pm.PackageManager;
import android.content.pm.PackageManager.NameNotFoundException;
import android.util.Log;

import com.brooks.jeremy.starglider.GameState;
import com.brooks.jeremy.starglider.MainActivity;
import com.brooks.jeremy.starglider.platform.IPlatformAndroidListener;
import com.brooks.jeremy.starglider.platform.PlatformAndroid;
import com.brooks.jeremy.starglider.util.AppLog;

public class NativeGame implements IPlatformAndroidListener
{

	private Context context = null;
	private boolean _disposed = false;
	private final PlatformAndroid platform;
	private GameState mState = GameState.InMenu;
	private static AppLog _log = new AppLog(NativeGame.class.getSimpleName());

	/*
	 * JNI #include "SoundTouch.h" using namespace soundtouch;
	 */

	/**
	 * Creates a new SoundTouch object. Needs to be disposed via
	 * {@link #dispose()}.
	 */
	public NativeGame(Context context, int screenw, int screenh)
	{
		System.loadLibrary("StarGliderNative");
		this.context = context;

		// return apk file path (or null on error)
		String apkFilePath = null;
		ApplicationInfo appInfo = null;
		PackageManager packMgmr = context.getPackageManager();

		for (ApplicationInfo app : packMgmr.getInstalledApplications(0))
		{
			Log.d("PackageList", "package: " + app.packageName
					+ ", sourceDir: " + app.sourceDir);
		}

		try
		{
			appInfo = packMgmr.getApplicationInfo(
					"com.brooks.jeremy.starglider", 0);
		}
		catch (NameNotFoundException e)
		{
			e.printStackTrace();
			throw new RuntimeException("Unable to locate assets, aborting...");
		}
		apkFilePath = appInfo.sourceDir;
		platform = new PlatformAndroid(context);
		platform.AddPlatformListener(this);
		createEngineJni(apkFilePath, platform, screenw, screenh);
	}

	private void verifyDispose()
	{
		if (_disposed)
		{
			String stacktrace = "";
			StackTraceElement[] stes = Thread.currentThread().getStackTrace();
			for (StackTraceElement element : stes)
			{
				stacktrace += element.toString() + "\r\n";
			}
			_log.e("Trying to perform operation on disposed NativeGame printing StackTrace: \r\n"
					+ stacktrace);
		}
	}

	public void update()
	{
		verifyDispose();
		updateJni();
	}

	public void mouseDown(float x, float y)
	{
		verifyDispose();
		mouseDownJni(x, y);
	}

	public void mouseUp(float x, float y)
	{
		verifyDispose();
		mouseUpJni(x, y);
	}

	public void mouseMotion(float x, float y)
	{
		verifyDispose();
		mouseMotionJni(x, y);
	}

	public void keyUp()
	{
		verifyDispose();
		keyUpJni();
	}

	public void keyDown()
	{
		verifyDispose();
		keyDownJni();
	}

	public void dispose()
	{
		_log.debug("In dispose of gameengine");
		verifyDispose();
		deleteEngineJni();
		_disposed = true;
		_log.debug("After dispose of gameengine");
	}

	private native void deleteEngineJni();

	private native void createEngineJni(String apkPath,
			PlatformAndroid platform, int screenw, int screenh);

	private native void updateJni();

	private native void mouseDownJni(float x, float y);

	private native void mouseUpJni(float x, float y);

	private native void mouseMotionJni(float x, float y);

	private native void keyDownJni();

	private native void keyUpJni();

	@Override
	public void GameStarted()
	{
		setGameState(GameState.InGame);
		if (MainActivity.GameStateListener != null)
		{
			MainActivity.GameStateListener.GameStarted();
		}
	}

	@Override
	public void GameEnded()
	{
		setGameState(GameState.InMenu);
		if (MainActivity.GameStateListener != null)
		{
			MainActivity.GameStateListener.GameEnded();
		}
	}

	public GameState getGameState()
	{
		return mState;
	}

	public void setGameState(GameState mState)
	{
		this.mState = mState;
	}

}
