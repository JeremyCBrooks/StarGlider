package com.brooks.jeremy.starglider.platform;

import java.io.IOException;
import java.io.InputStream;
import java.util.ArrayList;
import java.util.List;

import android.content.Context;
import android.content.SharedPreferences;
import android.content.res.AssetManager;
import android.graphics.Bitmap;
import android.graphics.BitmapFactory;
import android.opengl.GLES20;
import android.opengl.GLUtils;

import com.brooks.jeremy.starglider.R;
import com.brooks.jeremy.starglider.util.AppLog;

public class PlatformAndroid implements IPlatformAndroid
{
	private final Context mContext;
	private static AppLog _log = new AppLog(
			PlatformAndroid.class.getSimpleName());

	List<IPlatformAndroidListener> listeners = new ArrayList<IPlatformAndroidListener>();

	public void AddPlatformListener(IPlatformAndroidListener l)
	{
		listeners.add(l);
	}

	public PlatformAndroid(Context context)
	{
		mContext = context;
	}

	@Override
	public String GetUserData(String key)
	{
		// _log.d("getting user data : " + key);
		SharedPreferences sharedPref = mContext.getSharedPreferences(
				mContext.getString(R.string.star_catcher_pref_file),
				Context.MODE_PRIVATE);
		// _log.d("Returning getuserdate: " + result);
		String result = sharedPref.getString(key, "");
		return result;
	}

	@Override
	public void SetUserData(String key, String value)
	{
		SharedPreferences sharedPref = mContext.getSharedPreferences(
				mContext.getString(R.string.star_catcher_pref_file),
				Context.MODE_PRIVATE);
		SharedPreferences.Editor editor = sharedPref.edit();
		editor.putString(key, value);
		editor.commit();
	}

	@Override
	public void GameStarted()
	{
		for (IPlatformAndroidListener l : listeners)
		{
			l.GameStarted();
		}
	}

	@Override
	public void GameEnded()
	{
		for (IPlatformAndroidListener l : listeners)
		{
			l.GameEnded();
		}
	}

	@Override
	public String ReadFileContent(String fileName)
	{
		AssetManager assetManager = mContext.getResources().getAssets();
		String result = "";

		try
		{
			// get input stream for text
			InputStream is = assetManager.open(fileName);
			// check size
			int size = is.available();
			// create buffer for IO
			byte[] buffer = new byte[size];
			// get data to buffer
			is.read(buffer);
			// close stream
			is.close();
			// set result to TextView
			result = new String(buffer);
		}
		catch (IOException ex)
		{
			_log.e(String.format("Failed reading asset %s the error is %s",
					fileName, ex.toString()));
		}

		return result;
	}

	@Override
	public int LoadTexture(String fileName, boolean wrap)
	{
		// load image
		try
		{
			// get input stream
			InputStream ims = mContext.getResources().getAssets()
					.open(fileName);
			Bitmap bitmap = BitmapFactory.decodeStream(ims);
			// set image to ImageView

			final int[] textureHandle = new int[1];

			GLES20.glGenTextures(1, textureHandle, 0);

			if (textureHandle[0] != 0)
			{

				// Bind to the texture in OpenGL
				GLES20.glBindTexture(GLES20.GL_TEXTURE_2D, textureHandle[0]);

				if (wrap)
				{
					GLES20.glTexParameterf(GLES20.GL_TEXTURE_2D,
							GLES20.GL_TEXTURE_WRAP_S, GLES20.GL_REPEAT);
					GLES20.glTexParameterf(GLES20.GL_TEXTURE_2D,
							GLES20.GL_TEXTURE_WRAP_T, GLES20.GL_REPEAT);
				}
				else
				{
					GLES20.glTexParameterf(GLES20.GL_TEXTURE_2D,
							GLES20.GL_TEXTURE_WRAP_S, GLES20.GL_CLAMP_TO_EDGE);
					GLES20.glTexParameterf(GLES20.GL_TEXTURE_2D,
							GLES20.GL_TEXTURE_WRAP_T, GLES20.GL_CLAMP_TO_EDGE);
				}

				// Set filtering
				GLES20.glTexParameteri(GLES20.GL_TEXTURE_2D,
						GLES20.GL_TEXTURE_MIN_FILTER, GLES20.GL_NEAREST);
				GLES20.glTexParameteri(GLES20.GL_TEXTURE_2D,
						GLES20.GL_TEXTURE_MAG_FILTER, GLES20.GL_NEAREST);

				// Load the bitmap into the bound texture.
				GLUtils.texImage2D(GLES20.GL_TEXTURE_2D, 0, bitmap, 0);

			}

			if (textureHandle[0] == 0)
			{
				throw new RuntimeException("Error loading texture.");
			}
			// Recycle the bitmap, since its data has been loaded into OpenGL.
			bitmap.recycle();
			return textureHandle[0];
		}
		catch (IOException ex)
		{
			_log.e(String.format("Failed loading texture %s the error is %s",
					fileName, ex.toString()));
		}

		// TODO Auto-generated method stub
		return 0;
	}

}
