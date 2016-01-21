package com.brooks.jeremy.starglider.util;

import android.util.Log;

public class AppLog
{
	private String	mIdentifier;
	
	public AppLog(String identifier)
	{
		if (identifier != null && !identifier.isEmpty())
			mIdentifier = identifier;
		else
			throw new IllegalArgumentException("Can not initialize AppLog with empty string or null");
	}
	
	public void debug(String message)
	{
		Log.d(mIdentifier, message);
	}
	
	public void warn(String message)
	{
		Log.w(mIdentifier, message);
	}
	
	public void error(String message)
	{
		Log.e(mIdentifier, message);
	}
	
	public void d(String message)
	{
		Log.d(mIdentifier, message);
	}
	
	public void w(String message)
	{
		Log.w(mIdentifier, message);
	}
	
	public void e(String message)
	{
		Log.e(mIdentifier, message);
	}
	
}
