package com.brooks.jeremy.starglider.glglue;

import javax.microedition.khronos.opengles.GL10;
import android.opengl.GLSurfaceView.Renderer;
import com.brooks.jeremy.starglider.IReleaseListener;
//import com.brooks.jeremy.starglider.util.AppLog;

public class StarCatcherRenderer implements Renderer
{
//	private static AppLog _log = new AppLog(StarCatcherRenderer.class.getSimpleName());
	NativeGame mGame;
	boolean _inited = false;
	StarCatcherSurfaceView surfaceView;
	private boolean released = false;
	private IReleaseListener releasedWaiter;

	public void setSurface(StarCatcherSurfaceView surfaceView)
	{
		this.surfaceView = surfaceView;
	}

	@Override
	public void onSurfaceChanged(GL10 gl, int width, int height)
	{
		// No-op
	}

	@Override
	public void onDrawFrame(GL10 gl)
	{
		if (released)
		{
			mGame.dispose();
			releasedWaiter.released();
		}
		else
		{
			mGame.update();
		}
	}

	@Override
	public void onSurfaceCreated(GL10 gl,
			javax.microedition.khronos.egl.EGLConfig config)
	{
		int width = surfaceView.getWidth();
		int height = surfaceView.getHeight();
		mGame = new NativeGame(surfaceView.getContext(), width, height);
		_inited = true;
	}

	public NativeGame getGame()
	{
		return mGame;
	}

	public void release(IReleaseListener releasedObj)
	{
		releasedWaiter = releasedObj;
		released = true;
	}
}