package com.brooks.jeremy.starglider.glglue;

import android.app.ActivityManager;
import android.content.Context;
import android.content.pm.ConfigurationInfo;
import android.opengl.GLSurfaceView;
import android.support.v4.view.MotionEventCompat;
import android.util.AttributeSet;
import android.view.KeyEvent;
import android.view.MotionEvent;

import com.brooks.jeremy.starglider.GameState;
import com.brooks.jeremy.starglider.IReleaseListener;

public class StarCatcherSurfaceView extends GLSurfaceView
{
	private boolean mRendererSet = false;
	StarCatcherRenderer mRenderer = null;

	public StarCatcherSurfaceView(Context context)
	{
		super(context);
		init(context);
	}

	private void init(Context context)
	{
		ActivityManager activityManager = (ActivityManager) context
				.getSystemService(Context.ACTIVITY_SERVICE);

		ConfigurationInfo configurationInfo = activityManager
				.getDeviceConfigurationInfo();

		final boolean supportsEs2 = (configurationInfo.reqGlEsVersion >= 0x20000);

		if (supportsEs2)
		{
			// Create an OpenGL ES 2.0 context.
			setEGLContextClientVersion(2);
			mRenderer = new StarCatcherRenderer();
			mRenderer.setSurface(this);
			setRenderer(mRenderer);
			mRendererSet = true;
		}

	}

	public void release(IReleaseListener released)
	{
		this.onPause();
		mRenderer.release(released);
	}

	public StarCatcherSurfaceView(Context context, AttributeSet attrs)
	{
		super(context, attrs);
		init(context);
	}

	public StarCatcherSurfaceView(Context context, AttributeSet attrs,
			int defStyle)
	{
		super(context, attrs);
		init(context);
	}

	public boolean GetRendererSet()
	{
		return mRendererSet;
	}

	public GameState getGameState()
	{
		return mRenderer.mGame.getGameState();
	}

	@Override
	public boolean onTouchEvent(MotionEvent ev)
	{

		// Let the ScaleGestureDetector inspect all events.

		final int action = MotionEventCompat.getActionMasked(ev);

		switch (action)
		{
		case MotionEvent.ACTION_DOWN:
		{
			final int pointerIndex = MotionEventCompat.getActionIndex(ev);
			final float x = MotionEventCompat.getX(ev, pointerIndex);
			final float y = MotionEventCompat.getY(ev, pointerIndex);
			mRenderer.getGame().mouseDown(x, y);
			invalidate();

			break;
		}

		case MotionEvent.ACTION_MOVE:
		{
			// Find the index of the active pointer and fetch its position
			final int pointerIndex = MotionEventCompat.getActionIndex(ev);

			final float x = MotionEventCompat.getX(ev, pointerIndex);
			final float y = MotionEventCompat.getY(ev, pointerIndex);

			mRenderer.getGame().mouseMotion(x, y);

			invalidate();

			break;
		}

		case MotionEvent.ACTION_UP:
		{
			final int pointerIndex = MotionEventCompat.getActionIndex(ev);
			final float x = MotionEventCompat.getX(ev, pointerIndex);
			final float y = MotionEventCompat.getY(ev, pointerIndex);
			mRenderer.getGame().mouseUp(x, y);
			invalidate();

			break;

		}

		case MotionEvent.ACTION_POINTER_UP:
		{

			final int pointerIndex = MotionEventCompat.getActionIndex(ev);
			final float x = MotionEventCompat.getX(ev, pointerIndex);
			final float y = MotionEventCompat.getY(ev, pointerIndex);
			invalidate();
			mRenderer.getGame().mouseUp(x, y);
			break;

		}
		}
		return true;

	}

	@Override
	public boolean onKeyDown(int keyCode, KeyEvent event)
	{
		mRenderer.getGame().keyDown();
		return super.onKeyDown(keyCode, event);
	}

	@Override
	public boolean onKeyUp(int keyCode, KeyEvent event)
	{
		mRenderer.getGame().keyUp();
		return super.onKeyUp(keyCode, event);
	}
}
