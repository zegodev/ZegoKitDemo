package com.zego.zegosdkdemo;

import android.os.Bundle;
import android.annotation.SuppressLint;
import android.app.Activity;
import android.app.AlertDialog;
import android.app.Dialog;
import android.content.Context;
import android.content.Intent;
import android.view.View;
import android.widget.AdapterView;
import android.widget.ArrayAdapter;
import android.widget.Button;
import android.widget.EditText;
import android.widget.Spinner;
import android.widget.TextView;
import android.widget.Toast;
import android.widget.AdapterView.OnItemSelectedListener;


public class MainActivity extends Activity {
	
	private String[] szItem = {"房间1:900000","房间2:900001","房间3:900002","房间4:900003","自定义房间"}; 
    private Spinner spinner;
    private ArrayAdapter<String> adapter;
    private String mStrTestIP = "";
    private int mNTestPort = 0;
	int mNEnv = 0;
	int mNEnvAPP = 0;
	int mNAPPID = 0;
	byte[] mSzTestSignkey = {};
	boolean mBCustomRoom = false;
	
	
	@SuppressLint({ "HandlerLeak", "NewApi" })
	protected void onCreate(Bundle savedInstanceState) {
		super.onCreate(savedInstanceState);
		setContentView(R.layout.activity_main);
			
		//RoomUserID
		long lt = System.currentTimeMillis();
		EditText edUin = (EditText)findViewById(R.id.editUID);
		edUin.setText(Long.toString(lt));
		
		Button btnJoin = (Button)findViewById(R.id.btnJoin);
		btnJoin.setOnClickListener(new Button.OnClickListener(){

			public void onClick(View arg0) {
				openUiChatRoom();
			}	
		});	
		
		//room list
		spinner = (Spinner) findViewById(R.id.spinnerRoom); 
        adapter = new ArrayAdapter<String>(this,android.R.layout.simple_spinner_item,szItem); 	        
        adapter.setDropDownViewResource(android.R.layout.simple_spinner_dropdown_item);  	          
        spinner.setAdapter(adapter);
        spinner.setOnItemSelectedListener(new SpinnerSelectedListener());
        spinner.setVisibility(View.VISIBLE);
        
        //about
        TextView tvAbout = (TextView)findViewById(R.id.textAbout);
        tvAbout.setClickable(true);
        tvAbout.setTextColor(0xff031AC8);
        tvAbout.setOnClickListener(new TextView.OnClickListener(){
    	@Override
    	public void onClick(View arg0) {
    		String strMsg = "深圳市即构科技有限公司\r\n官网: http://www.zego.im\r\n版本:V1.2.9";
			Context context = arg0.getContext();
			Dialog alertDialog = new AlertDialog.Builder(context). 
            setTitle("关于我们"). 
            setMessage(strMsg). 
            setIcon(R.drawable.ic_launcher). 
            create(); 
	        alertDialog.show();
    	}
    	});
        
      //设置
        TextView tvConfig = (TextView)findViewById(R.id.textConfig);
        tvConfig.setClickable(true);
        tvConfig.setTextColor(0xff031AC8);
        tvConfig.setOnClickListener(new TextView.OnClickListener(){
    	@Override
    	public void onClick(View arg0) {
    		openConfigActivity();
    	}
    	});
	}
	
	public void openConfigActivity()
	{
		Intent intent = new Intent(MainActivity.this, Config.class);
		intent.putExtra("Env", mNEnv);
		intent.putExtra("IP", mStrTestIP);
		intent.putExtra("Port", mNTestPort);
		
		intent.putExtra("EnvAPP", mNEnvAPP);
		intent.putExtra("AppID", mNAPPID);
		intent.putExtra("Sign", mSzTestSignkey);
		startActivityForResult(intent, 2);
	}
	 
	static public final int DEMO_ROOMKEY_BASE = 900000;
	static private final int POS_CUSTOM_ROOMKEY = 4;
	
    class SpinnerSelectedListener implements OnItemSelectedListener{
  
        public void onItemSelected(AdapterView<?> arg0, View arg1, int arg2,  
                long arg3) {  
			if(arg2 == POS_CUSTOM_ROOMKEY)
			{
				EditText edCustomRoomKey = (EditText)findViewById(R.id.editCustomRoomKey);
				edCustomRoomKey.setVisibility(View.VISIBLE);
				edCustomRoomKey.setFocusable(true);
				edCustomRoomKey.setFocusableInTouchMode(true);
				edCustomRoomKey.requestFocus();
			}
			else
			{
				EditText edCustomRoomKey = (EditText)findViewById(R.id.editCustomRoomKey);
				edCustomRoomKey.setVisibility(View.INVISIBLE);
			}
        }  
  
        public void onNothingSelected(AdapterView<?> arg0) {  
        }  
    }
    
	@SuppressLint("NewApi") 
	public void openUiChatRoom()
	{
		Button btnJoinRoom = (Button)findViewById(R.id.btnJoin);
        btnJoinRoom.setEnabled(false);
        
        int nPos = spinner.getSelectedItemPosition();
		EditText edUin = (EditText)findViewById(R.id.editUID);
		EditText edName = (EditText)findViewById(R.id.editName);
		String strID = edUin.getText().toString();
		String strName = edName.getText().toString();
		int nRoomKey = DEMO_ROOMKEY_BASE + nPos;
		mBCustomRoom = false;
		if(nPos == POS_CUSTOM_ROOMKEY)
		{
			EditText et = (EditText)findViewById(R.id.editCustomRoomKey);
			String strText = et.getText().toString();
			if(strText.isEmpty())
			{
				Toast.makeText(getApplicationContext(), "请输入房间号", Toast.LENGTH_SHORT).show();
				return;
			}
			nRoomKey = Integer.parseInt(strText);
			if(nRoomKey == 0)
			{
				Toast.makeText(getApplicationContext(), "房间号不能为0", Toast.LENGTH_SHORT).show();
				return;
			}
			mBCustomRoom = true;
		}
		Intent intent = new Intent(MainActivity.this, ChatRoom.class);
		intent.putExtra("ID", strID);
		intent.putExtra("Name", strName);
		intent.putExtra("Roomkey", nRoomKey);
		intent.putExtra("IP", mStrTestIP);
		intent.putExtra("Port", mNTestPort);
		intent.putExtra("AppID", mNAPPID);
		intent.putExtra("Sign", mSzTestSignkey);
		intent.putExtra("CustomRoom", mBCustomRoom);
		startActivityForResult(intent, 1); 
	}
	
	protected void onActivityResult(int requestCode, int resultCode, Intent data) {
		super.onActivityResult(requestCode, resultCode, data);
		if(requestCode == 1)
		{
			Button btnJoinRoom = (Button)findViewById(R.id.btnJoin);
	        btnJoinRoom.setEnabled(true);
		}
		else if(requestCode == 2 && resultCode == RESULT_OK)
		{
			mNEnv = data.getIntExtra("Env", 0);
			if(mNEnv == Config.TEST_ENV)
			{
				mStrTestIP = data.getStringExtra("IP");
				mNTestPort = data.getIntExtra("Port", 0);
			}
			else
			{
				mStrTestIP = "";
			    mNTestPort = 0;
			}
			mNEnvAPP = data.getIntExtra("EnvAPP", 0);
			if(mNEnvAPP == Config.CUSTOM_APP)
			{
				mNAPPID = data.getIntExtra("AppID", 0);
				mSzTestSignkey = data.getByteArrayExtra("Sign");
			}
			else
			{
				mNAPPID = 0;
				mSzTestSignkey = null;
			}
		}
	}
}
