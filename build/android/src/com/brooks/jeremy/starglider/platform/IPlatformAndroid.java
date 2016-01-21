package com.brooks.jeremy.starglider.platform;

// /Provides services for native game
public interface IPlatformAndroid
{
	String GetUserData(String key);

	void SetUserData(String key, String value);

	void GameStarted();

	void GameEnded();

	String ReadFileContent(String fileName);

	int LoadTexture(String fileName, boolean wrap);
}
