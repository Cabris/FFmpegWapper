package com.myapp.h264streamingviwer;

import android.R.string;
import android.app.Activity;
import android.app.Fragment;
import android.content.Intent;
import android.content.pm.PackageManager;
import android.os.Bundle;
import android.util.Log;
import android.view.LayoutInflater;
import android.view.View;
import android.view.ViewGroup;
import android.view.View.OnClickListener;
import android.widget.Button;
import android.widget.EditText;
import android.widget.Toast;

import com.example.h264streamingviwer.R;
import com.myapp.h264streamingviwer.funcs.SensorClient;
import com.simpleMessage.sender.MessageSender;

class ConnectionFragment extends Fragment implements OnClickListener {
	SensorClient sClient;
	MessageSender sender;
	EditText ipAddressText;
	EditText portText;
	IOnConnectedListener connectedListener;

	public ConnectionFragment() {
	}

	@Override
	public View onCreateView(LayoutInflater inflater, ViewGroup container, Bundle savedInstanceState) {
		View view = inflater.inflate(R.layout.connection_main, container, false);
		return view;
	}

	@Override
	public void onActivityCreated(final Bundle savedInstanceState) {
		super.onActivityCreated(savedInstanceState);
		ipAddressText = (EditText) getView().findViewById(R.id.ip_editText);
		portText = (EditText) getView().findViewById(R.id.port_editText);
		Button connectButton = (Button) getView().findViewById(R.id.connect_button);
		Button scanButton = (Button) getView().findViewById(R.id.scan);
		connectButton.setOnClickListener(this);
		scanButton.setOnClickListener(new OnClickListener() {
			@Override
			public void onClick(View v) {
				Intent intent = new Intent("com.google.zxing.client.android.SCAN");
			    if(getActivity().getPackageManager().queryIntentActivities(intent, PackageManager.MATCH_DEFAULT_ONLY).size()==0)
			        {
			            // 未安裝
			            Toast.makeText(getActivity(), "請至 Play 商店安裝 ZXing 條碼掃描器", Toast.LENGTH_LONG).show();
			        }
			    else
			        {
			            // SCAN_MODE, 可判別所有支援的條碼
			            // QR_CODE_MODE, 只判別 QRCode
			            // PRODUCT_MODE, UPC and EAN 碼
			            // ONE_D_MODE, 1 維條碼
			            intent.putExtra("SCAN_MODE", "SCAN_MODE");
			     
			            // 呼叫ZXing Scanner，完成動作後回傳 1 給 onActivityResult 的 requestCode 參數
			            startActivityForResult(intent, 1);
			        }
			}
		});
	}

	// 接收 ZXing 掃描後回傳來的結果
	public void onActivityResult(int requestCode, int resultCode, Intent intent) 
	{
	    if(requestCode==1) 
	    {
	        if(resultCode==Activity.RESULT_OK) 
	            {
	                // ZXing回傳的內容
	                String contents = intent.getStringExtra("SCAN_RESULT");
	                //TextView textView1 = (TextView) findViewById(R.id.textView1);
	                String[] data= contents.split(":");
	                if(data.length==2){
	                	ipAddressText.setText(data[0]);
	                	portText.setText(data[1]);
	                }
	            }
	        else
	            if(resultCode==Activity.RESULT_CANCELED) 
	            {
	                Toast.makeText(getActivity(), "取消掃描", Toast.LENGTH_LONG).show();
	            }
	    }
	}
	
	@Override
	public void onClick(View v) {
		String adrs = ipAddressText.getText().toString();
		//adrs = "192.168.1.47";
		int port = Integer.parseInt(portText.getText().toString());
		port = 8888;

		sender = new MessageSender(adrs, 8887);
		sClient = new SensorClient(getActivity(), sender);
		sClient.onStart();
		sender.connect();
		if(connectedListener!=null)
			connectedListener.onConnected(adrs, port);
	}

	public void setConnectedListener(IOnConnectedListener connectedListener) {
		this.connectedListener = connectedListener;
	}
	
	@Override
	public void onDestroy() {
		Log.d("ConnectionFragment", "onDestroy");
		super.onDestroy();
		if (sClient != null)
			sClient.onStop();
		if (sender != null)
			sender.onStop();
	}

}
