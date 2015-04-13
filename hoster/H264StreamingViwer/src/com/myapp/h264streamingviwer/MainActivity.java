package com.myapp.h264streamingviwer;

import com.example.h264streamingviwer.R;

import android.app.Activity;
import android.app.Fragment;
import android.content.res.Resources;
import android.os.Bundle;
import android.view.Window;
import android.widget.Button;

public class MainActivity extends Activity implements IOnConnectedListener {

	ConnectionFragment connectionFragment;
	VideoFragment videoFragment;

	// CameraVideoFragment cameraVideoFragment;

	@Override
	protected void onCreate(final Bundle savedInstanceState) {
		super.onCreate(savedInstanceState);
		requestWindowFeature(Window.FEATURE_NO_TITLE);// must 1sf
		setContentView(R.layout.activity_main);
		connectionFragment = new ConnectionFragment();
		connectionFragment.setConnectedListener(this);
		getFragmentManager().beginTransaction()
				.add(R.id.container, connectionFragment).commit();
	}

	protected void onStop() {
		super.onStop();
		// connectionFragment.onDestroy();
		// videoFragment.onDestroy();
	}

	@Override
	public void onConnected(String ip, int port) {
		// TODO Auto-generated method stub
		Resources res = getResources();
		String[] types = res.getStringArray(R.array.video_types);
		if (connectionFragment.getLayoutType().equals(types[0]))
			videoFragment = new CameraVideoFragment(ip, port);
		else if (connectionFragment.getLayoutType().equals(types[1]))
			videoFragment = new VideoFragment(ip, port);
		if (videoFragment != null)
			getFragmentManager().beginTransaction()
					.add(R.id.container, videoFragment).commit();

	}

}
