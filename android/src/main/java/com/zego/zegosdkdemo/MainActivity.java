package com.zego.zegosdkdemo;

//import java.util.Random;

//import com.zego.AVRoom;
//import com.zego.RoomUser;

import android.os.Bundle;
import android.annotation.SuppressLint;
import android.app.Activity;
import android.app.AlertDialog;
import android.app.Dialog;
import android.content.Context;
import android.content.Intent;
import android.view.View;
import android.widget.ArrayAdapter;
import android.widget.Button;
import android.widget.EditText;
import android.widget.Spinner;
import android.widget.TextView;


public class MainActivity extends Activity {
	
	private String[] szItem = {"房间1","房间2","房间3","房间4"}; 
    private Spinner spinner;
    private ArrayAdapter<String> adapter;
    private String mStrTestIP = "";
    private int mNTestPort = 0;
	int mNEnv = 0;
	int mNEnvAPP = 0;
	int mNAPPID = 0;
	byte[] mSzTestSignkey = {};
	boolean bHasInit = false;
	
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
        
        ///testcode
        /*
        Button btnTest = (Button)findViewById(R.id.btnTest);
        btnTest.setOnClickListener(new Button.OnClickListener(){

			public void onClick(View arg0) {
				AVRoom avRoom = new AVRoom();
				Random rand = new Random();
				
				String strApiSeq = "";
				TextView tvTest = (TextView)findViewById(R.id.tvTest);
				for(int n=0;n<10;n++)
				{
					int nOrder = rand.nextInt(14);
					strApiSeq = strApiSeq + Integer.toString(nOrder) + ";";
					tvTest.setText(strApiSeq);
					switch (nOrder)
					{
						case 0:
							if(bHasInit)
								break;
							byte[] signkey = {(byte)0x91,(byte)0x93,(byte)0xcc,(byte)0x66,(byte)0x2a,(byte)0x1c,(byte)0xe,(byte)0xc1,
									(byte)0x35,(byte)0xec,(byte)0x71,(byte)0xfb,(byte)0x7,(byte)0x19,(byte)0x4b,(byte)0x38,
									(byte)0x15,(byte)0xf1,(byte)0x43,(byte)0xf5,(byte)0x7c,(byte)0xd2,(byte)0xb5,(byte)0x9a,
									(byte)0xe3,(byte)0xdd,(byte)0xdb,(byte)0xe0,(byte)0xf1,(byte)0x74,(byte)0x36,(byte)0xd};
							avRoom.Init(1, signkey, getApplicationContext());
							bHasInit = true;
							break;
						case 1:
							//avRoom.SetLogLevel(getApplicationContext(), rand.nextInt(5), null);
							break;
						case 2:
							RoomUser roomUser = new RoomUser();
							roomUser.strID = Long.toString(rand.nextLong());
							roomUser.strName = Integer.toString(rand.nextInt());;
							avRoom.GetInRoom(rand.nextInt(), roomUser);
							break;
						case 3:
							int nRoomKey = avRoom.GetRoomKey();
							Toast.makeText(getApplicationContext(), "RoomKey="+Integer.toString(nRoomKey), Toast.LENGTH_SHORT).show();
							break;
						case 4:
							RoomUser[] userList = avRoom.GetRoomUsers();
							if(userList == null)
								break;
							int nLen = userList.length;
							if(nLen == 0)
							{
								Toast.makeText(getApplicationContext(), "userlist=0", Toast.LENGTH_SHORT).show();
								break;
							}
							String strUserList = "";
							for(int i = 0; i < nLen; i++)
							{
								strUserList = strUserList + "ID:" + userList[i].strID + ", Name:" + userList[i].strName + "\n"; 
							}
							Toast.makeText(getApplicationContext(), strUserList, Toast.LENGTH_SHORT).show();
							break;
						case 5:
							{
								boolean b = (rand.nextInt(2) != 0);
								avRoom.EnableMic(b);
							}
							break;
						case 6:
							{
								if(avRoom.GetMicState())
									Toast.makeText(getApplicationContext(), "MicOn", Toast.LENGTH_SHORT).show();
								else
									Toast.makeText(getApplicationContext(), "MicOff", Toast.LENGTH_SHORT).show();
							}
							break;
						case 7:
							{
								boolean b = (rand.nextInt(2) != 0);
								avRoom.EnableSpeaker(b);
							}
							break;
						case 8:
							{
								if(avRoom.GetSpeakerState())
									Toast.makeText(getApplicationContext(), "SpeakerOn", Toast.LENGTH_SHORT).show();
								else
									Toast.makeText(getApplicationContext(), "SpeakerOff", Toast.LENGTH_SHORT).show();
							}
							break;
						case 9:
							{
								int nState = avRoom.GetAudioState();
								Toast.makeText(getApplicationContext(), "AudioState=" + Integer.toString(nState), Toast.LENGTH_SHORT).show();
							}
							break;
						case 10:
							avRoom.PauseAudio();
							break;
						case 11:
							avRoom.ResumeAudio();
							break;
						case 12:
							avRoom.ReGetInRoom();
							break;
						case 13:
							if(avRoom.GetRoomKey() != 0)
								bHasInit = false;
							avRoom.LeaveRoom();
							break;
					}
				}
			}
			
		});	
		*/
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
		Intent intent = new Intent(MainActivity.this, ChatRoom.class);
		intent.putExtra("ID", strID);
		intent.putExtra("Name", strName);
		intent.putExtra("Roomkey", nRoomKey);
		intent.putExtra("IP", mStrTestIP);
		intent.putExtra("Port", mNTestPort);
		intent.putExtra("AppID", mNAPPID);
		intent.putExtra("Sign", mSzTestSignkey);
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
