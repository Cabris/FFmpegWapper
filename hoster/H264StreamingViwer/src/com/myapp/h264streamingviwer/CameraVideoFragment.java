package com.myapp.h264streamingviwer;

import java.io.IOException;

import com.example.h264streamingviwer.R;
import com.myapp.h264streamingviwer.funcs.Decoder;
import com.stream.source.StreamReceiver;

import android.hardware.Camera;
import android.os.Bundle;
import android.view.LayoutInflater;
import android.view.SurfaceHolder;
import android.view.SurfaceView;
import android.view.View;
import android.view.ViewGroup;

public class CameraVideoFragment extends VideoFragment {

	SurfaceHolder cameraHolder;
	Camera myCamera;
	boolean previewing = false;

	public CameraVideoFragment(String ip, int port) {
		super(ip, port);
	}
	
	@Override
	public View onCreateView(LayoutInflater inflater, ViewGroup container, Bundle savedInstanceState) {
		View view = inflater.inflate(R.layout.video_camera, container, false);
		return view;
	}

	@Override
	protected void createVideoHolder() {
		SurfaceView surfaceView = (SurfaceView) getView().findViewById(
				R.id.videoSurface);
		holder = surfaceView.getHolder();
		holder.addCallback(this);
		SurfaceView cameraSurfaceView = (SurfaceView) getView().findViewById(
				R.id.cameraSurface);
		cameraHolder = cameraSurfaceView.getHolder();
		cameraHolder.addCallback(new CameraHolder());
	}

	class CameraHolder implements SurfaceHolder.Callback {

		@Override
		public void surfaceCreated(SurfaceHolder holder) {
			myCamera = Camera.open();
		}

		@Override
		public void surfaceChanged(SurfaceHolder holder, int format, int width,
				int height) {
			if (previewing) {
				myCamera.stopPreview();
				previewing = false;
			}

			try {
				myCamera.setPreviewDisplay(holder);
				myCamera.startPreview();
				previewing = true;
			} catch (IOException e) {
				// TODO Auto-generated catch block
				e.printStackTrace();
			}
		}

		@Override
		public void surfaceDestroyed(SurfaceHolder holder) {
			myCamera.stopPreview();
			myCamera.release();
			myCamera = null;
			previewing = false;
		}
	}

}
