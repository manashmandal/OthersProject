/*
 * Released under MIT License http://opensource.org/licenses/MIT
 * Copyright (c) 2013 Plasty Grove
 * Refer to file LICENSE or URL above for full text 
 */

package com.blueserial;

import java.io.IOException;
import java.io.InputStream;
import java.util.UUID;

import com.blueserial.R;

import android.app.Activity;
import android.app.ProgressDialog;
import android.bluetooth.BluetoothAdapter;
import android.bluetooth.BluetoothDevice;
import android.bluetooth.BluetoothSocket;
import android.content.Intent;
import android.content.pm.ActivityInfo;
import android.hardware.SensorEvent;
import android.os.AsyncTask;
import android.os.Bundle;
import android.text.method.ScrollingMovementMethod;
import android.util.Log;
import android.view.MotionEvent;
import android.view.View;
import android.view.View.OnClickListener;
import android.widget.Button;
import android.widget.CheckBox;
import android.widget.EditText;
import android.widget.RadioButton;
import android.widget.ScrollView;
import android.widget.TextView;
import android.widget.Toast;
import android.hardware.SensorEventListener;
import android.hardware.SensorManager;
import android.hardware.Sensor;

public class MainActivity extends Activity {

	//App name
	public static final String APP_NAME = "Robot Controller";

	public enum ControlMode{
		ACCELEROMETER,
		BUTTON
	}

	public ControlMode controlMode;

	public int x_values;
	public int y_values;

	private static final String TAG = "BlueTest5-MainActivity";
	private int mMaxChars = 50000;//Default
	private UUID mDeviceUUID;
	private BluetoothSocket mBTSocket;
	private ReadInput mReadThread = null;

	private boolean mIsUserInitiatedDisconnect = false;

	// All controls here
	private TextView mTxtReceive;
//	private EditText mEditSend;
	private Button mBtnDisconnect;
	private Button mBtnSend;
	private Button mBtnClear;
	private Button mBtnClearInput;
	private ScrollView scrollView;
	private CheckBox chkScroll;
	private CheckBox chkReceiveText;
	private TextView xValueTextView;
	private TextView yValueTextView;
	private Button left;
	private Button forward;
	private Button right;
	private Button backward;
	private Button stop;

	private boolean mIsBluetoothConnected = false;

	private BluetoothDevice mDevice;

	private ProgressDialog progressDialog;

	Sensor accelerometer;
	SensorManager accelerometerManager;

	@Override
	protected void onCreate(Bundle savedInstanceState) {
		super.onCreate(savedInstanceState);
		setContentView(R.layout.activity_main);
		ActivityHelper.initialize(this);

		Intent intent = getIntent();
		Bundle b = intent.getExtras();
		mDevice = b.getParcelable(Homescreen.DEVICE_EXTRA);
		mDeviceUUID = UUID.fromString(b.getString(Homescreen.DEVICE_UUID));
		mMaxChars = b.getInt(Homescreen.BUFFER_SIZE);

		Log.d(TAG, "Ready");

		mBtnDisconnect = (Button) findViewById(R.id.btnDisconnect);
		scrollView = (ScrollView) findViewById(R.id.viewScroll);
		chkScroll = (CheckBox) findViewById(R.id.chkScroll);
		chkReceiveText = (CheckBox) findViewById(R.id.chkReceiveText);
		mBtnClearInput = (Button) findViewById(R.id.btnClearInput);
		xValueTextView = (TextView) findViewById(R.id.xValue);
		yValueTextView = (TextView) findViewById(R.id.yValue);

		left = (Button) findViewById(R.id.leftButton);
		right = (Button) findViewById(R.id.rightButton);
		forward = (Button) findViewById(R.id.forwardButton);
		backward = (Button) findViewById(R.id.backwardButton);
		stop = (Button) findViewById(R.id.stopButton);


		mBtnDisconnect.setOnClickListener(new OnClickListener() {

			@Override
			public void onClick(View v) {
				mIsUserInitiatedDisconnect = true;
				new DisConnectBT().execute();
			}
		});


		
		mBtnClearInput.setOnClickListener(new OnClickListener() {
			@Override
			public void onClick(View arg0) {
			}
		});

		accelerometerManager = (SensorManager) this.getSystemService(SENSOR_SERVICE);
		accelerometer = accelerometerManager.getDefaultSensor(Sensor.TYPE_ACCELEROMETER);
		accelerometerManager.registerListener(listener, accelerometer, SensorManager.SENSOR_DELAY_NORMAL);


		//Locking orientation
		setRequestedOrientation(ActivityInfo.SCREEN_ORIENTATION_PORTRAIT);

		//Initializing Control mode
		controlMode = ControlMode.ACCELEROMETER;

		//Setting app name
		this.setTitle(APP_NAME);

		//Button touch event
		forward.setOnClickListener(new OnClickListener() {
			@Override
			public void onClick(View view) {
				sendData(view);
			}
		});

		backward.setOnClickListener(new OnClickListener() {
			@Override
			public void onClick(View view) {
				sendData(view);
			}
		});

		left.setOnClickListener(new OnClickListener() {
			@Override
			public void onClick(View view) {
				sendData(view);
			}
		});

		right.setOnClickListener(new OnClickListener() {
			@Override
			public void onClick(View view) {
				sendData(view);
			}
		});

		stop.setOnClickListener(new OnClickListener() {
			@Override
			public void onClick(View view) {
				sendData(view);
			}
		});

	}

	//Writes string to arduino port
	public void sendStringToArduino(String input){
		if (mBTSocket.isConnected()) {
			try {

				mBTSocket.getOutputStream().write(input.getBytes());
				//mBTSocket.getOutputStream().write(String.valueOf(y_values).getBytes());
				Thread.sleep(2);
			} catch (IOException e) {
				e.printStackTrace();
			} catch (InterruptedException e) {
				e.printStackTrace();
			}
		}
	}


	protected SensorEventListener listener = new SensorEventListener() {
		@Override
		public void onSensorChanged(SensorEvent sensorEvent) {
			calculateAcceleration(sensorEvent);
		}

		@Override
		public void onAccuracyChanged(Sensor sensor, int i) {

		}
	};

	private void calculateAcceleration(SensorEvent sensorEvent){
		final float alpha = (float) 0.8;

		float gravity = (float) 9.8;

		float linearAcceleration = (float) 0;

		gravity = alpha * gravity + (1 - alpha) * sensorEvent.values[0];

		linearAcceleration = sensorEvent.values[0] - gravity;

		x_values = (int) sensorEvent.values[0];
		y_values = (int) sensorEvent.values[1];

		xValueTextView.setText(String.valueOf(x_values));
		yValueTextView.setText(String.valueOf(y_values));

		if (mBTSocket.isConnected() && controlMode == ControlMode.ACCELEROMETER){
			try {

				String accelerometerData = "(" + String.valueOf(x_values) + "," + String.valueOf(y_values) + ")\n";

				mBTSocket.getOutputStream().write(accelerometerData.getBytes());
				//mBTSocket.getOutputStream().write(String.valueOf(y_values).getBytes());
				Thread.sleep(2);
			} catch (IOException e){
				e.printStackTrace();
			} catch (InterruptedException e) {
				e.printStackTrace();
			}
		}
	}

	//Check control mode
	public void onRadioButtonClicked(View view){

		sendStringToArduino("~STOP");

		boolean checked = ((RadioButton) view).isChecked();

		//Check the mode
		switch (view.getId()){
			case R.id.accelerometerRadioButton:
				if (checked){
					controlMode = ControlMode.ACCELEROMETER;
					Log.i(TAG, "Accelerometer was chosen");
					Toast.makeText(getApplicationContext(), "Accelerometer Control Activated!",
							Toast.LENGTH_LONG).show();
				}
				break;

			case R.id.buttonRadioButton:
				if (checked){
					controlMode = ControlMode.BUTTON;
					Log.i(TAG, "Button Control was chosen");
					Toast.makeText(getApplicationContext(), "Button Control Activated!", Toast
							.LENGTH_LONG).show();
				}
				break;

		}
	}

	//Send data
	public void sendData(View view){

		if (controlMode == ControlMode.BUTTON) {


			switch (view.getId()) {
				case R.id.forwardButton:
					Log.i(TAG, "FORWARD");
					sendStringToArduino("~FORWARD\n");
					break;

				case R.id.backwardButton:
					Log.i(TAG, "BACKWARD");
					sendStringToArduino("~BACKWARD\n");
					break;

				case R.id.leftButton:
					Log.i(TAG, "LEFT");
					sendStringToArduino("~LEFT\n");
					break;

				case R.id.rightButton:
					Log.i(TAG, "RIGHT");
					sendStringToArduino("~RIGHT\n");
					break;

				case R.id.stopButton:
					Log.i(TAG, "STOP");
					sendStringToArduino("~STOP\n");
					break;

				default:
					sendStringToArduino("~STOP\n");
					break;
			}
		} else {
			Toast.makeText(getApplicationContext(), "Select Button Control Mode first", Toast
					.LENGTH_SHORT).show();
		}

	}



	private class ReadInput implements Runnable {

		private boolean bStop = false;
		private Thread t;

		public ReadInput() {
			t = new Thread(this, "Input Thread");
			t.start();
		}

		public boolean isRunning() {
			return t.isAlive();
		}

		@Override
		public void run() {
			InputStream inputStream;

			try {
				inputStream = mBTSocket.getInputStream();
				while (!bStop) {
					byte[] buffer = new byte[256];
					if (inputStream.available() > 0) {
						inputStream.read(buffer);
						int i = 0;
						/*
						 * This is needed because new String(buffer) is taking the entire buffer i.e. 256 chars on Android 2.3.4 http://stackoverflow.com/a/8843462/1287554
						 */
						for (i = 0; i < buffer.length && buffer[i] != 0; i++) {
						}
						final String strInput = new String(buffer, 0, i);

						/*
						 * If checked then receive text, better design would probably be to stop thread if unchecked and free resources, but this is a quick fix
						 */

						if (chkReceiveText.isChecked()) {

						}

					}
					Thread.sleep(500);
				}
			} catch (IOException e) {
				// TODO Auto-generated catch block
				e.printStackTrace();
			} catch (InterruptedException e) {
				// TODO Auto-generated catch block
				e.printStackTrace();
			}

		}

		public void stop() {
			bStop = true;
		}

	}

	private class DisConnectBT extends AsyncTask<Void, Void, Void> {

		@Override
		protected void onPreExecute() {
		}

		@Override
		protected Void doInBackground(Void... params) {

			if (mReadThread != null) {
				mReadThread.stop();
				while (mReadThread.isRunning())
					; // Wait until it stops
				mReadThread = null;

			}

			try {
				mBTSocket.close();
			} catch (IOException e) {
				// TODO Auto-generated catch block
				e.printStackTrace();
			}

			return null;
		}

		@Override
		protected void onPostExecute(Void result) {
			super.onPostExecute(result);
			mIsBluetoothConnected = false;
			if (mIsUserInitiatedDisconnect) {
				finish();
			}
		}

	}

	private void msg(String s) {
		Toast.makeText(getApplicationContext(), s, Toast.LENGTH_SHORT).show();
	}

	@Override
	protected void onPause() {
		if (mBTSocket != null && mIsBluetoothConnected) {
			new DisConnectBT().execute();
		}
		Log.d(TAG, "Paused");
		super.onPause();
	}

	@Override
	protected void onResume() {
		if (mBTSocket == null || !mIsBluetoothConnected) {
			new ConnectBT().execute();
		}
		Log.d(TAG, "Resumed");
		super.onResume();
	}

	@Override
	protected void onStop() {
		Log.d(TAG, "Stopped");
		super.onStop();
	}

	@Override
	protected void onSaveInstanceState(Bundle outState) {
		// TODO Auto-generated method stub
		super.onSaveInstanceState(outState);
	}

	private class ConnectBT extends AsyncTask<Void, Void, Void> {
		private boolean mConnectSuccessful = true;

		@Override
		protected void onPreExecute() {
			progressDialog = ProgressDialog.show(MainActivity.this, "Hold on", "Connecting");// http://stackoverflow.com/a/11130220/1287554
		}

		@Override
		protected Void doInBackground(Void... devices) {

			try {
				if (mBTSocket == null || !mIsBluetoothConnected) {
					mBTSocket = mDevice.createInsecureRfcommSocketToServiceRecord(mDeviceUUID);
					BluetoothAdapter.getDefaultAdapter().cancelDiscovery();
					mBTSocket.connect();
				}
			} catch (IOException e) {
				// Unable to connect to device
				e.printStackTrace();
				mConnectSuccessful = false;
			}
			return null;
		}

		@Override
		protected void onPostExecute(Void result) {
			super.onPostExecute(result);

			if (!mConnectSuccessful) {
				Toast.makeText(getApplicationContext(), "Could not connect to device. Is it a Serial device? Also check if the UUID is correct in the settings", Toast.LENGTH_LONG).show();
				finish();
			} else {
				msg("Connected to device");
				mIsBluetoothConnected = true;
				mReadThread = new ReadInput(); // Kick off input reader
			}

			progressDialog.dismiss();
		}

	}

}
